#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <nlohmann/json.hpp>

struct TileConfig {
    int tileWidth;
    int tileHeight;
    std::string tilesetPath;
};

// Déclarations des fonctions
TileConfig readTileConfig(const std::string& jsonMappingFile);
void printTileConfig(const TileConfig& config);

#endif // UTILS_HPP
