#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <stdexcept>

class Image {
public:
    Image(const std::string& filepath);
    ~Image();

    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    const unsigned char* getData() const;

private:
    int width, height, channels;
    unsigned char* data;
};

struct loading_image_failed : public std::runtime_error {
    explicit loading_image_failed(const std::string& message) 
        : std::runtime_error("Failed to load image: " + message) {}
};

#endif // IMAGE_H
