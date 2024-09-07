#include "TileExtractor.h"

#include <stb_image_write.h>
#include <stdexcept>

TileExtractor::TileExtractor(const Image& image, int tileWidth, int tileHeight)
    : image(image), tileWidth(tileWidth), tileHeight(tileHeight) {}

void TileExtractor::extractTiles(const std::vector<std::pair<std::string, std::pair<int, int>>>& tileInfo) {
    for (const auto& tile : tileInfo) {
        const std::string& filename = tile.first;
        int x = tile.second.first;
        int y = tile.second.second;
        saveTile(filename, x, y);
    }
}

void TileExtractor::saveTile(const std::string& filename, int x, int y) {
    int imageWidth = image.getWidth();
    int imageHeight = image.getHeight();
    int channels = image.getChannels();
    const unsigned char* data = image.getData();

    if (x + tileWidth > imageWidth || y + tileHeight > imageHeight) {
        throw std::out_of_range("Tile coordinates are out of image bounds.");
    }

    std::vector<unsigned char> tileData(tileWidth * tileHeight * channels);
    for (int ty = 0; ty < tileHeight; ++ty) {
        for (int tx = 0; tx < tileWidth; ++tx) {
            for (int c = 0; c < channels; ++c) {
                tileData[(ty * tileWidth + tx) * channels + c] =
                    data[((y + ty) * imageWidth + (x + tx)) * channels + c];
            }
        }
    }

    if (!stbi_write_png(filename.c_str(), tileWidth, tileHeight, channels, tileData.data(), tileWidth * channels)) {
        throw std::runtime_error("Failed to save tile: " + filename);
    }
}
