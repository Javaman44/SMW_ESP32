#include "XmlExtractor.hpp"
#include <stdexcept>
#include <pugixml.hpp>

XmlExtractor::XmlExtractor(const std::string& xmlPath) {
    pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());
    if (!result) {
        throw std::runtime_error("Failed to load XML file");
    }
}

std::vector<Component> XmlExtractor::extractComponents() const {
    std::vector<Component> components;
    
    // Get the root element (equivalent to <mxfile>)
    pugi::xml_node mxfile = doc.child("mxfile");
    if (!mxfile) {
        throw std::runtime_error("No <mxfile> element found in the XML");
    }

    // Get the first <diagram> element
    pugi::xml_node diagram = mxfile.child("diagram");
    if (!diagram) {
        throw std::runtime_error("No <diagram> element found in the XML");
    }

    // Get the first <mxGraphModel> element
    pugi::xml_node graphModel = diagram.child("mxGraphModel");
    if (!graphModel) {
        throw std::runtime_error("No <mxGraphModel> element found in the XML");
    }

    // Get the first <root> element
    pugi::xml_node root = graphModel.child("root");
    if (!root) {
        throw std::runtime_error("No <root> element found in the XML");
    }

    // Iterate over each <object> element
    for (pugi::xml_node obj = root.child("object"); obj; obj = obj.next_sibling("object")) {
        const char* id = obj.attribute("id").value();
        const char* componentType = obj.attribute("Component").value();

        pugi::xml_node mxCell = obj.child("mxCell");
        pugi::xml_node geometry = mxCell.child("mxGeometry");
        if (geometry) {
            double x = geometry.attribute("x").as_double();
            double y = geometry.attribute("y").as_double();
            double width = geometry.attribute("width").as_double();
            double height = geometry.attribute("height").as_double();

            Component comp = {id ? id : "", componentType ? componentType : "", x, y, width, height};
            components.push_back(comp);
        }
    }

    return components;
}
