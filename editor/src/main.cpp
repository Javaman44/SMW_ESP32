#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "tinyxml2.h"

using namespace tinyxml2;
using json = nlohmann::json;

// Structure pour stocker les données de chaque composant
struct Component {
    std::string id;
    std::string component;
    double x;
    double y;
    double width;
    double height;
};

// Fonction pour extraire les données XML et les stocker dans un vecteur de composants
std::vector<Component> extractComponents(XMLElement* root) {
    std::vector<Component> components;

    // Parcourir tous les éléments <object>
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

int main() {
    XMLDocument doc;
    if (doc.LoadFile("../../editor/level.drawio") != XML_SUCCESS) {
        std::cerr << "Failed to load Drawio file" << std::endl;
        return -1;
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found" << std::endl;
        return -1;
    }

    // Extraire les composants
    std::vector<Component> components = extractComponents(root);

    // Convertir les composants en JSON
    json level_data;
    level_data["level"] = "Level-1";
    for (const auto& comp : components) {
        json component_json;
        component_json["id"] = comp.id;
        component_json["component"] = comp.component;
        component_json["x"] = comp.x;
        component_json["y"] = comp.y;
        component_json["width"] = comp.width;
        component_json["height"] = comp.height;
        level_data["components"].push_back(component_json);
    }

    // Convertir le JSON en une chaîne et l'imprimer
    std::cout << level_data.dump(4) << std::endl;

    return 0;
}
