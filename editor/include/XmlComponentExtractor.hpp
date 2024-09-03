#ifndef XML_COMPONENT_EXTRACTOR_HPP
#define XML_COMPONENT_EXTRACTOR_HPP

#include <vector>
#include <string>
#include "tinyxml2.h"

// Structure pour stocker les données de chaque composant
struct Component {
    std::string id;
    std::string component;
    double x;
    double y;
    double width;
    double height;
};

class XmlComponentExtractor {
public:
    explicit XmlComponentExtractor(const std::string& xmlPath);
    std::vector<Component> extractComponents() const;

private:
    tinyxml2::XMLDocument doc;
};

#endif // XML_COMPONENT_EXTRACTOR_HPP
