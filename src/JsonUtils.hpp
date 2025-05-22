#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
namespace JsonUtils {
    inline nlohmann::json load(const std::string& relPath) {

        std::ifstream ifs(relPath, std::ios::binary);
        if (!ifs.is_open()) {
            throw std::runtime_error("Не удалось открыть JSON: " + relPath);
        }

        std::vector<char> buf((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        std::string content(buf.begin(), buf.end());

        if (content.rfind("\xEF\xBB\xBF", 0) == 0) {
            content.erase(0, 3);
        }

        if (!nlohmann::json::accept(content)) {
            throw std::runtime_error("JSON невалиден (accept=false): " + relPath);
        }

        return nlohmann::json::parse(content);
    }
}
