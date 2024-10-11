#ifndef TILE_EXTRACTOR_H
#define TILE_EXTRACTOR_H

#include "Image.h"
#include "utils.hpp"  // Inclusion de utils.hpp pour accéder à TileConfig

#include <string>
#include <vector>
#include <utility>


// Déclaration de la fonction printTileConfig
void printTileConfig(const TileConfig& config);

class TileExtractor {
public:
    TileExtractor(const Image& image, int tileWidth, int tileHeight);
    void extractTiles(const std::vector<std::pair<std::string, std::pair<int, int>>>& tileInfo);
    
private:
    const Image& image;
    int tileWidth;
    int tileHeight;
    
    void saveTile(const std::string& filename, int x, int y);
};

#endif // TILE_EXTRACTOR_H
