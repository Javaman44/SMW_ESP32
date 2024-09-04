#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "XmlExtractor.hpp"

class JsonWriter {
public:
    static nlohmann::json toJson(const std::vector<Component>& components);
    static void toFile(const std::vector<Component>& components, const std::string& outputPath);
};

#endif // JSON_COMPONENT_WRITER_HPP
