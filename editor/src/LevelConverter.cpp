#include "LevelConverter.hpp"
#include "JsonComponentWriter.hpp"

LevelConverter::LevelConverter(const std::string& xmlPath) : xmlPath(xmlPath) {}

void LevelConverter::convertToJson(const std::string& outputJsonFile) const {
    XmlComponentExtractor extractor(xmlPath);
    std::vector<Component> components = extractor.extractComponents();
    JsonComponentWriter::toFile(components, outputJsonFile);
}
