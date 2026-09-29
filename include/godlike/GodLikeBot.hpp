#pragma once

#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace DotaGSIData;

class GodLikeBot {
public:
    using Handler = std::function<void(const json& snapshot, const json& prev)>;

    void on(const std::string& event, Handler h) {
        handlers_[event].push_back(std::move(h));
    }

    void update(const json& snap) {
        int64_t ts = Provider::get_timestamp(snap);
        if (ts == last_timestamp_) return;
        last_timestamp_ = ts;

        const json& prev = at(snap, "previously");
        if (prev.empty()) return;

        check_events(snap, prev);

        previous_snapshot_ = snap;
    }

private:
    std::unordered_map<std::string, std::vector<Handler>> handlers_;
    int64_t last_timestamp_ = 0;
    json previous_snapshot_;

    void fire(const std::string& event, const json& snap, const json& prev) {
        auto it = handlers_.find(event);
        if (it == handlers_.end()) return;
        for (auto& h : it->second) h(snap, prev);
    }

    void check_events(const json& s, const json& prev) {
        if (prev.contains("map")) {
            const auto& pm = prev["map"];

            if (pm.contains("game_state")) {
                auto old_state = pm["game_state"].get<std::string>();
                auto new_state = Map::get_game_state(s);
                if (old_state != new_state) {
                    if (new_state == "DOTA_GAMERULES_STATE_GAME_IN_PROGRESS")
                        fire("game_started", s, prev);
                    else if (new_state == "DOTA_GAMERULES_STATE_POST_GAME")
                        fire("game_ended", s, prev);
                    else if (new_state == "DOTA_GAMERULES_STATE_HERO_SELECTION")
                        fire("hero_selection_started", s, prev);
                }
            }

            if (pm.contains("daytime")) {
                bool old_day = pm["daytime"].get<bool>();
                bool new_day = Map::is_daytime(s);
                if (old_day != new_day) {
                    fire(new_day ? "day_started" : "night_started", s, prev);
                }
            }

            if (pm.contains("paused")) {
                bool old_p = pm["paused"].get<bool>();
                bool new_p = Map::is_paused(s);
                if (old_p != new_p)
                    fire(new_p ? "game_paused" : "game_resumed", s, prev);
            }

        }

        if (prev.contains("hero")) {
            const auto& ph = prev["hero"];

            if (ph.contains("alive")) {
                bool was_alive = ph["alive"].get<bool>();
                bool now_alive = Hero::is_alive(s);
                if (was_alive && !now_alive) fire("hero_died", s, prev);
                if (!was_alive && now_alive) fire("hero_respawned", s, prev);
            }

            if (ph.contains("level")) {
                int old_l = ph["level"].get<int>();
                int new_l = Hero::get_level(s);
                if (new_l > old_l) fire("hero_level_up", s, prev);
            }

            if (ph.contains("health_percent")) {
                int old_hp = ph["health_percent"].get<int>();
                int new_hp = Hero::get_health_percent(s);
                if (old_hp > 30 && new_hp <= 30) fire("hero_low_hp", s, prev);
                if (old_hp <= 30 && new_hp > 30) fire("hero_hp_recovered", s, prev);
            }

            if (ph.contains("mana_percent")) {
                int old_mp = ph["mana_percent"].get<int>();
                int new_mp = Hero::get_mana_percent(s);
                if (old_mp > 5 && new_mp <= 5) fire("hero_out_of_mana", s, prev);
            }

            if (ph.contains("aghanims_scepter")) {
                if (!ph["aghanims_scepter"].get<bool>() && Hero::has_aghanims_scepter(s))
                    fire("aghanims_scepter_bought", s, prev);
            }

            if (ph.contains("stunned") && !ph["stunned"].get<bool>() && Hero::is_stunned(s))
                fire("hero_stunned", s, prev);
            if (ph.contains("silenced") && !ph["silenced"].get<bool>() && Hero::is_silenced(s))
                fire("hero_silenced", s, prev);
            if (ph.contains("smoked") && !ph["smoked"].get<bool>() && Hero::is_smoked(s))
                fire("hero_smoked", s, prev);
        }

        if (prev.contains("player")) {
            const auto& pp = prev["player"];

            if (pp.contains("kills")) {
                int old_k = pp["kills"].get<int>();
                int new_k = Player::get_kills(s);
                if (new_k > old_k) fire("kill", s, prev);
            }

            if (pp.contains("deaths")) {
                int old_d = pp["deaths"].get<int>();
                int new_d = Player::get_deaths(s);
                if (new_d > old_d) fire("death", s, prev);
            }

            if (pp.contains("assists")) {
                int old_a = pp["assists"].get<int>();
                int new_a = Player::get_assists(s);
                if (new_a > old_a) fire("assist", s, prev);
            }

            if (pp.contains("gold")) {
                int old_g = pp["gold"].get<int>();
                int new_g = Player::get_gold(s);
                if (new_g != old_g) fire("gold_changed", s, prev);
            }

            if (pp.contains("gpm")) fire("gpm_changed", s, prev);
            if (pp.contains("xpm")) fire("xpm_changed", s, prev);
        }

        if (prev.contains("abilities")) {
            const auto& pa = prev["abilities"];
            const auto& cur = at(s, "abilities");
            for (auto& [slot, val] : cur.items()) {
                if (slot.rfind("ability", 0) != 0) continue;
                if (!pa.contains(slot)) continue;
                int old_cd = pa[slot].value("cooldown", 0);
                int new_cd = val.value("cooldown", 0);
                if (old_cd > 0 && new_cd == 0) {
                    fire("ability_ready", s, prev);
                }
            }
        }

        if (prev.contains("items")) {
            const auto& pi = prev["items"];
            const auto& cur = at(s, "items");

            for (auto& [slot, val] : cur.items()) {
                std::string new_name = val.value("name", "empty");
                std::string old_name = pi.contains(slot)
                    ? pi[slot].value("name", "empty")
                    : "empty";

                if (old_name != new_name) {
                    if (new_name == "empty" && old_name != "empty")
                        fire("item_removed", s, prev);
                    else if (new_name != "empty" && old_name == "empty")
                        fire("item_added", s, prev);
                }

                int old_cd = pi.contains(slot) ? pi[slot].value("cooldown", 0) : 0;
                int new_cd = val.value("cooldown", 0);
                if (old_cd > 0 && new_cd == 0)
                    fire("item_ready", s, prev);
            }
        }

        if (prev.contains("buildings")) {
            const auto& pb = prev["buildings"];
            const auto& cb = at(s, "buildings");

            for (auto& [team, list] : pb.items()) {
                for (auto& [name, val] : list.items()) {
                    bool still_there = cb.contains(team) && cb[team].contains(name);
                    if (!still_there) fire("building_destroyed", s, prev);
                }
            }
        }
    }
};