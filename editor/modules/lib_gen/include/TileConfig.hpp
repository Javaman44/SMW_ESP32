#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Structure pour stocker la configuration des tuiles
struct Tile {
    std::string title;  // Le titre de la tuile
    std::string id;     // L'ID ou identifiant de la tuile
    int x;              // La colonne où se trouve la tuile
    int y;              // La ligne où se trouve la tuile
    int width;          // La largeur de la tuile (par défaut 16 ou 40 après redimensionnement)
    int height;         // La hauteur de la tuile (par défaut 16 ou 40 après redimensionnement)
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
