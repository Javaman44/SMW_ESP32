#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>

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

#endif // IMAGE_H
