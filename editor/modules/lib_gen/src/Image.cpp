#include "Image.hpp"

#include <stb_image.h>


Image::Image(const std::string& filepath) : width(0), height(0), channels(0), data(nullptr) {
    data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw loading_image_failed(filepath);
    }
}

Image::~Image() {
    stbi_image_free(data);
}

int Image::getWidth() const { return width; }
int Image::getHeight() const { return height; }
int Image::getChannels() const { return channels; }
const unsigned char* Image::getData() const { return data; }
