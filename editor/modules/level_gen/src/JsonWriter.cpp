#include "JsonWriter.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

// Using directive for convenience
using nlohmann::json;

json JsonWriter::toJson(const std::vector<Component>& components) {
    nlohmann::json level_data;
    level_data["level"] = "Level-1";  // This could be parameterized if needed
    for (const auto& comp : components) {
        nlohmann::json component_json;
        component_json["id"] = comp.id;
        component_json["component"] = comp.component;
        component_json["x"] = comp.x;
        component_json["y"] = comp.y;
        component_json["width"] = comp.width;
        component_json["height"] = comp.height;
        level_data["components"].push_back(component_json);
    }
    return level_data;
}

void JsonWriter::toFile(const std::vector<Component>& components, const std::string& outputPath) {
    nlohmann::json jsonData = toJson(components);  // Convert components to JSON internally
    std::ofstream outFile(outputPath);
    if (!outFile) {
        throw std::runtime_error("Failed to open output file");
    }
    outFile << jsonData.dump(4);  // Pretty print with 4 spaces
}
