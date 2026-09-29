#pragma once

#include <entt/resource/resource.hpp>
#include <entt/core/hashed_string.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <utility>

namespace meatengine {
    class ResourceLoader {
    private:
        template<typename T>
        struct Cache {
            std::unordered_map<entt::id_type, std::shared_ptr<T>> map;
        };

        template<typename T>
        static Cache<T>& get_cache() {
            static Cache<T> cache;
            return cache;
        }

    public:
        template<typename T, typename... Args>
        static entt::resource<T> load(Args&&... args) {
            auto& cache = get_cache<T>();

            std::string id_str;
            ((id_str += std::string{args} + "|"), ...);
            if (!id_str.empty()) id_str.pop_back();

            if (id_str.empty()) return entt::resource<T>{nullptr};
            
            auto id = entt::hashed_string{id_str.c_str()};

            auto it = cache.map.find(id);
            if (it != cache.map.end()) {
                return entt::resource<T>{it->second};
            }

            auto resource = T{}(std::forward<Args>(args)...);
            if (!resource) {
                return entt::resource<T>{nullptr};
            }

            cache.map[id] = resource;
            return entt::resource<T>{resource};
        }

        template<typename T, typename... Args>
        static entt::resource<T> get(Args&&... args) {
            auto& cache = get_cache<T>();

            std::string id_str;
            ((id_str += std::string{std::forward<Args>(args)} + "|"), ...);
            if (!id_str.empty()) id_str.pop_back();

            if (id_str.empty()) return entt::resource<T>{nullptr};

            auto id = entt::hashed_string{id_str.c_str()};

            auto it = cache.map.find(id);
            if (it != cache.map.end()) {
                return entt::resource<T>{it->second};
            }
            return entt::resource<T>{nullptr};
        }
    };
}