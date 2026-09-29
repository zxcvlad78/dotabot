#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

#include <nlohmann/json.hpp>
#include <httplib.h>

using json = nlohmann::json;

namespace DotaGSIData {
    inline const json& at(const json& j, const std::string& key) {
        static const json empty = json::object();
        auto it = j.find(key);
        return it != j.end() ? *it : empty;
    }

    namespace Provider {
        inline std::string get_name(const json& s) { return at(s, "provider").value("name", ""); }
        inline int get_appid(const json& s) { return at(s, "provider").value("appid", 0); }
        inline int get_version(const json& s) { return at(s, "provider").value("version", 0); }
        inline int64_t get_timestamp(const json& s) { return at(s, "provider").value("timestamp", (int64_t)0); }
    }

    namespace Map {
        inline std::string get_name(const json& s) { return at(s, "map").value("name", ""); }
        inline std::string get_matchid(const json& s) { return at(s, "map").value("matchid", ""); }
        inline std::string get_game_state(const json& s) { return at(s, "map").value("game_state", ""); }
        inline std::string get_win_team(const json& s) { return at(s, "map").value("win_team", "none"); }
        inline std::string get_customgamename(const json& s) { return at(s, "map").value("customgamename", ""); }

        inline int  get_game_time(const json& s) { return at(s, "map").value("game_time", 0); }
        inline int  get_clock_time(const json& s) { return at(s, "map").value("clock_time", 0); }
        inline int  get_radiant_score(const json& s) { return at(s, "map").value("radiant_score", 0); }
        inline int  get_dire_score(const json& s) { return at(s, "map").value("dire_score", 0); }
        inline int  get_ward_purchase_cooldown(const json& s)  { return at(s, "map").value("ward_purchase_cooldown", 0); }

        inline bool is_daytime(const json& s) { return at(s, "map").value("daytime", true); }
        inline bool is_nightstalker_night(const json& s)   { return at(s, "map").value("nightstalker_night", false); }
        inline bool is_paused(const json& s) { return at(s, "map").value("paused", false); }

        inline bool is_in_progress(const json& s) {
            return get_game_state(s) == "DOTA_GAMERULES_STATE_GAME_IN_PROGRESS";
        }
        inline bool is_hero_selection(const json& s) {
            return get_game_state(s) == "DOTA_GAMERULES_STATE_HERO_SELECTION";
        }
        inline bool is_pre_game(const json& s) {
            return get_game_state(s) == "DOTA_GAMERULES_STATE_PRE_GAME";
        }
        inline bool is_post_game(const json& s) {
            return get_game_state(s) == "DOTA_GAMERULES_STATE_POST_GAME";
        }
    }

    namespace Hero {
        inline std::string get_name(const json& s) { return at(s, "hero").value("name", ""); }
        inline int get_id(const json& s) { return at(s, "hero").value("id", 0); }
        inline int get_level(const json& s) { return at(s, "hero").value("level", 0); }
        inline int get_xp(const json& s) { return at(s, "hero").value("xp", 0); }

        inline int get_health(const json& s) { return at(s, "hero").value("health", 0); }
        inline int get_max_health(const json& s) { return at(s, "hero").value("max_health", 0); }
        inline int get_health_percent(const json& s) { return at(s, "hero").value("health_percent", 0); }

        inline int get_mana(const json& s) { return at(s, "hero").value("mana", 0); }
        inline int get_max_mana(const json& s) { return at(s, "hero").value("max_mana", 0); }
        inline int get_mana_percent(const json& s) { return at(s, "hero").value("mana_percent", 0); }

        inline int get_respawn_seconds(const json& s) { return at(s, "hero").value("respawn_seconds", 0); }
        inline int get_buyback_cost(const json& s) { return at(s, "hero").value("buyback_cost", 0); }
        inline int get_buyback_cooldown(const json& s) { return at(s, "hero").value("buyback_cooldown", 0); }
        inline int get_attributes_level(const json& s) { return at(s, "hero").value("attributes_level", 0); }
        inline int get_facet(const json& s) { return at(s, "hero").value("facet", 0); }

        inline float get_xpos(const json& s) { return at(s, "hero").value("xpos", 0.f); }
        inline float get_ypos(const json& s) { return at(s, "hero").value("ypos", 0.f); }

        inline bool is_alive(const json& s) { return at(s, "hero").value("alive", false); }
        inline bool is_smoked(const json& s) { return at(s, "hero").value("smoked", false); }
        inline bool is_stunned(const json& s) { return at(s, "hero").value("stunned", false); }
        inline bool is_silenced(const json& s) { return at(s, "hero").value("silenced", false); }
        inline bool is_muted(const json& s) { return at(s, "hero").value("muted", false); }
        inline bool is_hexed(const json& s) { return at(s, "hero").value("hexed", false); }
        inline bool is_disarmed(const json& s) { return at(s, "hero").value("disarmed", false); }
        inline bool is_magicimmune(const json& s) { return at(s, "hero").value("magicimmune", false); }
        inline bool is_break(const json& s) { return at(s, "hero").value("break", false); }
        inline bool has_debuff(const json& s) { return at(s, "hero").value("has_debuff", false); }
        inline bool has_aghanims_scepter(const json& s) { return at(s, "hero").value("aghanims_scepter", false); }
        inline bool has_aghanims_shard(const json& s) { return at(s, "hero").value("aghanims_shard", false); }

        inline bool has_talent(const json& s, int idx) {
            return at(s, "hero").value("talent_" + std::to_string(idx), false);
        }

        inline bool has_permanent_buff(const json& s, const std::string& name) {
            return at(at(s, "hero"), "permanent_buffs").contains(name);
        }
    }

    namespace Player {
        inline std::string get_name(const json& s) { return at(s, "player").value("name", ""); }
        inline std::string get_steamid(const json& s) { return at(s, "player").value("steamid", ""); }
        inline std::string get_accountid(const json& s) { return at(s, "player").value("accountid", ""); }
        inline std::string get_team_name(const json& s) { return at(s, "player").value("team_name", ""); }
        inline std::string get_activity(const json& s) { return at(s, "player").value("activity", ""); }

        inline int get_player_slot(const json& s) { return at(s, "player").value("player_slot", 0); }
        inline int get_team_slot(const json& s) { return at(s, "player").value("team_slot", 0); }

        inline int get_kills(const json& s) { return at(s, "player").value("kills", 0); }
        inline int get_deaths(const json& s) { return at(s, "player").value("deaths", 0); }
        inline int get_assists(const json& s) { return at(s, "player").value("assists", 0); }
        inline int get_kill_streak(const json& s) { return at(s, "player").value("kill_streak", 0); }
        inline int get_last_hits(const json& s) { return at(s, "player").value("last_hits", 0); }
        inline int get_denies(const json& s) { return at(s, "player").value("denies", 0); }
        inline int get_commands_issued(const json& s){ return at(s, "player").value("commands_issued", 0); }

        inline int get_gold(const json& s) { return at(s, "player").value("gold", 0); }
        inline int get_gold_reliable(const json& s) { return at(s, "player").value("gold_reliable", 0); }
        inline int get_gold_unreliable(const json& s) { return at(s, "player").value("gold_unreliable", 0); }
        inline int get_gold_from_income(const json& s) { return at(s, "player").value("gold_from_income", 0); }
        inline int get_gold_from_creep_kills(const json& s) { return at(s, "player").value("gold_from_creep_kills", 0); }
        inline int get_gold_from_hero_kills(const json& s) { return at(s, "player").value("gold_from_hero_kills", 0); }
        inline int get_gold_from_shared(const json& s) { return at(s, "player").value("gold_from_shared", 0); }
        inline int get_gold_from_summon_kills(const json& s) { return at(s, "player").value("gold_from_summon_kills", 0); }

        inline int get_gpm(const json& s) { return at(s, "player").value("gpm", 0); }
        inline int get_xpm(const json& s) { return at(s, "player").value("xpm", 0); }

        inline json get_kill_list(const json& s) { return at(s, "player").value("kill_list", json::object()); }
    }

    namespace Abilities {
        struct Ability {
            std::string name;
            int  level = 0;
            int  cooldown = 0;
            int  max_cooldown = 0;
            bool can_cast = false;
            bool ability_active = false;
            bool passive = false;
            bool ultimate = false;
        };

        inline Ability get_ability(const json& s, const std::string& slot) {
            Ability a;
            const auto& ab = at(s, "abilities");
            if (ab.contains(slot)) {
                const auto& j = ab[slot];
                a.name = j.value("name", "");
                a.level = j.value("level", 0);
                a.cooldown = j.value("cooldown", 0);
                a.max_cooldown = j.value("max_cooldown", 0);
                a.can_cast = j.value("can_cast", false);
                a.ability_active = j.value("ability_active", false);
                a.passive = j.value("passive", false);
                a.ultimate = j.value("ultimate", false);
            }
            return a;
        }

        inline Ability get(const json& s, int idx) {
            return get_ability(s, "ability" + std::to_string(idx));
        }

        inline std::vector<Ability> get_all(const json& s) {
            std::vector<Ability> out;
            const auto& ab = at(s, "abilities");
            for (auto& [key, val] : ab.items()) {
                if (key.rfind("ability", 0) == 0)
                    out.push_back(get_ability(s, key));
            }
            return out;
        }

        inline Ability get_ultimate(const json& s) {
            for (auto& a : get_all(s))
                if (a.ultimate) return a;
            return {};
        }
    }

    namespace Items {
        struct Item {
            std::string name = "empty";
            int cooldown = 0;
            int max_cooldown = 0;
            int charges = 0;
            int item_charges = 0;
            int item_level = 0;
            int purchaser = 0;
            bool can_cast = false;
            bool passive = false;
            bool empty = true;
        };

        inline Item get_item(const json& s, const std::string& slot) {
            Item i;
            const auto& items = at(s, "items");
            if (items.contains(slot)) {
                const auto& j = items[slot];
                i.name = j.value("name", "empty");
                i.empty = (i.name == "empty" || i.name.empty());
                i.can_cast = j.value("can_cast", false);
                i.passive = j.value("passive", false);
                i.charges = j.value("charges", 0);
                i.item_charges = j.value("item_charges", 0);
                i.cooldown = j.value("cooldown", 0);
                i.max_cooldown = j.value("max_cooldown", 0);
                i.item_level = j.value("item_level", 0);
                i.purchaser = j.value("purchaser", 0);
            }
            return i;
        }

        inline Item get_slot(const json& s, int idx) {
			return get_item(s, "slot" + std::to_string(idx));
		}
        inline Item get_stash(const json& s, int idx) {
			return get_item(s, "stash" + std::to_string(idx));
		}
        inline Item get_neutral(const json& s, int idx) {
			return get_item(s, "neutral" + std::to_string(idx));
		}
        inline Item get_teleport(const json& s) {
			return get_item(s, "teleport0");
		}

        inline std::vector<Item> get_inventory(const json& s) {
            std::vector<Item> out;
            out.reserve(9);
            for (int i = 0; i < 9; ++i) out.push_back(get_slot(s, i));
            return out;
        }
    }

    namespace Buildings {
        struct Building {
            std::string team;
            std::string name;
            int health = 0;
            int max_health = 0;
        };

        inline Building get(const json& s, const std::string& team, const std::string& name) {
            Building b;
            b.team = team;
            b.name = name;
            const auto& bld = at(s, "buildings");
            if (bld.contains(team) && bld[team].contains(name)) {
                b.health     = bld[team][name].value("health", 0);
                b.max_health = bld[team][name].value("max_health", 0);
            }
            return b;
        }

        inline std::vector<Building> get_all(const json& s) {
            std::vector<Building> out;
            const auto& bld = at(s, "buildings");
            for (auto& [team, list] : bld.items()) {
                for (auto& [name, val] : list.items()) {
                    Building b;
                    b.team       = team;
                    b.name       = name;
                    b.health     = val.value("health", 0);
                    b.max_health = val.value("max_health", 0);
                    out.push_back(std::move(b));
                }
            }
            return out;
        }

        inline bool is_alive(const json& s, const std::string& team, const std::string& name) {
            Building b = get(s, team, name);
            return b.health > 0;
        }
    }

    namespace Wearables {
        inline std::vector<int> get_all(const json& s) {
            std::vector<int> out;
            const auto& w = at(s, "wearables");
            for (auto& [key, val] : w.items()) {
                if (val.is_number()) out.push_back(val.get<int>());
            }
            return out;
        }
    }


    namespace Previously {
        inline bool has(const json& s, const std::string& category) {
            return at(s, "previously").contains(category);
        }
        inline json get(const json& s, const std::string& category) {
            return at(at(s, "previously"), category);
        }
    }

}

class DotaGSI {
private:
	httplib::Server server;
    std::thread worker;

    std::atomic<bool> running{false};
    std::mutex data_mutex;
    std::function<void()> on_update_cb;

	json snapshot;

public:
	DotaGSI();
	~DotaGSI();
    DotaGSI(const DotaGSI&) = delete;
    DotaGSI& operator=(const DotaGSI&) = delete;

    void start(const std::string& host = "127.0.0.1", int port = 3000);
    void stop();
    bool is_running() const;

    void set_on_update(std::function<void()> cb);

	httplib::Server& get_server();
	json get_snapshot() const;
};