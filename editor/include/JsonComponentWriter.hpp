#ifndef JSON_COMPONENT_WRITER_HPP
#define JSON_COMPONENT_WRITER_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "XmlComponentExtractor.hpp"

class JsonComponentWriter {
public:
    static nlohmann::json toJson(const std::vector<Component>& components);
    static void toFile(const std::vector<Component>& components, const std::string& outputPath);
};

#endif // JSON_COMPONENT_WRITER_HPP
