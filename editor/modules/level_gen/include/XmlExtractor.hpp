#ifndef XML_COMPONENT_EXTRACTOR_HPP
#define XML_COMPONENT_EXTRACTOR_HPP

#include <vector>
#include <string>
#include <pugixml.hpp>  // Include pugixml instead of tinyxml2

// Structure pour stocker les données de chaque composant
struct Component {
    std::string id;
    std::string component;
    double x;
    double y;
    double width;
    double height;
};

class XmlExtractor {
public:
    explicit XmlExtractor(const std::string& xmlPath);
    std::vector<Component> extractComponents() const;

private:
    pugi::xml_document doc;  // Use pugi::xml_document instead of tinyxml2::XMLDocument
};

#endif // XML_COMPONENT_EXTRACTOR_HPP
