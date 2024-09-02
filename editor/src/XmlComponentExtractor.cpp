#include "XmlComponentExtractor.hpp"
#include <stdexcept>

using namespace tinyxml2;

XmlComponentExtractor::XmlComponentExtractor(const std::string& xmlPath) {
    if (doc.LoadFile(xmlPath.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("Failed to load XML file");
    }
}

std::vector<Component> XmlComponentExtractor::extractComponents() const {
    std::vector<Component> components;
    XMLElement* mxfile = doc.RootElement();
    if (!mxfile) {
        throw std::runtime_error("No <mxfile> element found in the XML");
    }

    XMLElement* diagram = mxfile->FirstChildElement("diagram");
    if (!diagram) {
        throw std::runtime_error("No <diagram> element found in the XML");
    }

    XMLElement* graphModel = diagram->FirstChildElement("mxGraphModel");
    if (!graphModel) {
        throw std::runtime_error("No <mxGraphModel> element found in the XML");
    }

    XMLElement* root = graphModel->FirstChildElement("root");
    if (!root) {
        throw std::runtime_error("No <root> element found in the XML");
    }

    for (XMLElement* obj = root->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
        const char* id = obj->Attribute("id");
        const char* componentType = obj->Attribute("Component");

        XMLElement* geometry = obj->FirstChildElement("mxCell")->FirstChildElement("mxGeometry");
        if (geometry) {
            double x = geometry->DoubleAttribute("x");
            double y = geometry->DoubleAttribute("y");
            double width = geometry->DoubleAttribute("width");
            double height = geometry->DoubleAttribute("height");

            Component comp = {id ? id : "", componentType ? componentType : "", x, y, width, height};
            components.push_back(comp);
        }
    }

    return components;
}
