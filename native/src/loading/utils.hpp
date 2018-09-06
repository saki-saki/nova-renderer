/*!
 * \author ddubois 
 * \date 05-Sep-18.
 */

#ifndef NOVA_RENDERER_UTILS_HPP
#define NOVA_RENDERER_UTILS_HPP

#include <optional>
#include <nlohmann/json.hpp>

namespace nova {
    /*!
     * \brief Retrieves an individual value from the provided JSON structure
     * \tparam ValType The type of the value to retrieve
     * \param json_obj The JSON object where your value might be found
     * \param key The name of the value
     * \return An optional that contains the value, if it can be found, or an empty optional if the value cannot be found
     */
    template <typename ValType>
    std::optional<ValType> get_json_value(const nlohmann::json& json_obj, const std::string& key) {
        const auto& itr = json_obj.find(key);
        if(itr != json_obj.end()) {
            auto& json_node = json_obj.at(key);
            return std::optional<ValType>(json_node.get<ValType>());
        }

        return std::optional<ValType>{};
    }

    /*!
     * \brief Retrieves an individual value from the provided JSON structure
     * \tparam ValType The type of the value to retrieve
     * \param json_obj The JSON object where your value might be found
     * \param key The name of the value
     * \param deserializer A function that deserializes the JSON value
     * \return An optional that contains the value, if it can be found, or an empty optional if the value cannot be found
     */
    template <typename ValType>
    std::optional<ValType> get_json_value(const nlohmann::json& json_obj, const std::string& key, std::function<ValType(const nlohmann::json&)> deserializer) {
        const auto& itr = json_obj.find(key);
        if(itr != json_obj.end()) {
            auto& json_node = json_obj.at(key);
            ValType val = deserializer(json_node);
            return std::optional<ValType>{std::move(val)};
        }

        return std::optional<ValType>{};
    }

    /*!
     * \brief Retrieves an array of values from the provided JSON object
     * \tparam ValType The type fo the values in the array
     * \param json_obj The JSON object where the values might be found
     * \param key The name fo the array that has your values
     * \return An array of values, if the value can be found, or an empty vector if the values cannot be found
     */
    template <typename ValType>
    std::vector<ValType> get_json_array(const nlohmann::json& json_obj, const std::string& key) {
        const auto& itr = json_obj.find(key);
        if(itr != json_obj.end()) {
            auto& json_node = json_obj.at(key);
            std::vector<ValType> vec;
            vec.reserve(json_node.size());

            for(auto& elem : json_node) {
                vec.push_back(elem.get<ValType>());
            }

            return vec;
        }

        return std::vector<ValType>{};
    }

    /*!
     * \brief Retrieves an array of values from the provided JSON object
     * \tparam ValType The type fo the values in the array
     * \param json_obj The JSON object where the values might be found
     * \param key The name fo the array that has your values
     * \param deserializer A function that can deserialize each value from JSON
     * \return An array of values, if the value can be found, or an empty vector if the values cannot be found
     */
    template <typename ValType>
    std::vector<ValType> get_json_array(const nlohmann::json& json_obj, const std::string& key, std::function<ValType(const nlohmann::json&)> deserializer) {
        const auto& itr = json_obj.find(key);
        if(itr != json_obj.end()) {
            auto& json_node = json_obj.at(key);
            std::vector<ValType> vec;
            vec.reserve(json_node.size());

            for(auto& elem : json_node) {
                vec.push_back(deserializer(elem));
            }

            return vec;
        }

        return std::vector<ValType>{};
    }
}

#endif //NOVA_RENDERER_UTILS_HPP