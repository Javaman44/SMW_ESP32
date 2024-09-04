#ifndef TILE_EXTRACTOR_H
#define TILE_EXTRACTOR_H

#include "Image.h"
#include <string>
#include <vector>
#include <utility>

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
