#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

// Structure pour stocker la configuration des tuiles
struct TileConfig {
    int tileWidth;
    int tileHeight;
    std::vector<std::pair<std::string, std::pair<int, int>>> tiles;
};

// Fonction utilitaire pour lire le fichier JSON d'association de tiles
TileConfig readTileConfig(const std::string& jsonMappingFile) {
    using json = nlohmann::json;
    
    std::ifstream jsonFile(jsonMappingFile);
    if (!jsonFile.is_open()) {
        throw std::runtime_error("Failed to open JSON mapping file: " + jsonMappingFile);
    }

    json configJson;
    jsonFile >> configJson;

    TileConfig config;
    config.tileWidth = configJson["tileWidth"];
    config.tileHeight = configJson["tileHeight"];
    
    for (const auto& tile : configJson["tiles"]) {
        std::string id = tile["id"];
        int x = tile["x"];
        int y = tile["y"];
        config.tiles.push_back({id, {x, y}});
    }

    return config;
}

#endif // UTILS_HPP
