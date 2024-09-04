#include "Image.h"

#include <stb_image.h>
#include <stdexcept>

Image::Image(const std::string& filepath) : width(0), height(0), channels(0), data(nullptr) {
    data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image: " + filepath);
    }
}

Image::~Image() {
    stbi_image_free(data);
}

int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }
int Image::getChannels() const { return channels; }
const unsigned char* Image::getData() const { return data; }
