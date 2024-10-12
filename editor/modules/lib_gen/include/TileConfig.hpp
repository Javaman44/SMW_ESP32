#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Structure pour stocker la configuration des tuiles
struct Tile {
    std::string title;
    std::string id;
    int x;
    int y;
};

struct TileConfig {
    int tileWidth;
    int tileHeight;
    std::string tilesetPath;
    std::vector<Tile> tiles;
};

// Déclarations des fonctions
TileConfig readTileConfig(const std::string& jsonMappingFile);
void printTileConfig(const TileConfig& config);

#endif // UTILS_HPP
