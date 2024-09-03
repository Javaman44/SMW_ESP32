#include "DrawioLibGenerator.hpp"

DrawioLibGenerator::DrawioLibGenerator(const std::string& imagePath, const std::string& jsonMappingFile)
    : image(imagePath) {
    TileConfig config = readTileConfig(jsonMappingFile);
    tileWidth = config.tileWidth;
    tileHeight = config.tileHeight;
    tiles = config.tiles;
}

void DrawioLibGenerator::generate(const std::string& outputDrawioFile) {
    DrawioExporter exporter(image, tileWidth, tileHeight);
    exporter.exportTiles(tiles, outputDrawioFile);
}
