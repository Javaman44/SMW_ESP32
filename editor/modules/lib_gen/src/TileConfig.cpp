#include "TileConfig.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

// Prototypes des fonctions
void checkJsonKey(const nlohmann::json& jsonObject, const std::string& key, const std::string& type);
void validateTileConfigJson(const nlohmann::json& configJson);

TileConfig readTileConfig(const std::string& jsonMappingFile) {
    using json = nlohmann::json;

    // Ouvrir le fichier JSON
    std::ifstream jsonFile(jsonMappingFile);
    if (!jsonFile.is_open()) {
        throw std::runtime_error("Failed to open JSON mapping file: " + jsonMappingFile);
    }

    // Lire le contenu du fichier JSON
    json configJson;
    jsonFile >> configJson;

    // Valider la structure du JSON
    validateTileConfigJson(configJson);

    // Si la validation est réussie, extraire les données
    TileConfig config;
    config.tileWidth = configJson["tileWidth"];
    config.tileHeight = configJson["tileHeight"];
    config.tilesetPath = configJson["tilesetPath"];

    return config;
}

// Fonction pour afficher la configuration des tuiles
void printTileConfig(const TileConfig& config) {
    std::cout << "Tileset Path: " << config.tilesetPath << std::endl;
    std::cout << "Tile Width: " << config.tileWidth << std::endl;
    std::cout << "Tile Height: " << config.tileHeight << std::endl;    
}

// Fonction utilitaire pour vérifier la présence et le type d'une clé dans un objet JSON
void checkJsonKey(const nlohmann::json& jsonObject, const std::string& key, const std::string& type) {
    if (!jsonObject.contains(key)) {
        throw std::runtime_error("Missing key: '" + key + "' in JSON.");
    }
    if ((type == "number" && !jsonObject[key].is_number()) ||
        (type == "string" && !jsonObject[key].is_string()) ||
        (type == "array" && !jsonObject[key].is_array())) {
        throw std::runtime_error("Invalid type for key: '" + key + "'. Expected " + type + ".");
    }
}

// Fonction pour vérifier la structure globale du fichier JSON
void validateTileConfigJson(const nlohmann::json& configJson) {
    // Vérification des clés principales
    checkJsonKey(configJson, "tilesetPath", "string");
    checkJsonKey(configJson, "tileWidth", "number");
    checkJsonKey(configJson, "tileHeight", "number");
}
