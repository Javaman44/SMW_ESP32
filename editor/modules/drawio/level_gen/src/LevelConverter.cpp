#include "LevelConverter.hpp"
#include "JsonWriter.hpp"

LevelConverter::LevelConverter(const std::string& xmlPath) : xmlPath(xmlPath) {}

void LevelConverter::convertToJson(const std::string& outputJsonFile) const {
    XmlExtractor extractor(xmlPath);
    std::vector<Component> components = extractor.extractComponents();
    JsonWriter::toFile(components, outputJsonFile);
}
