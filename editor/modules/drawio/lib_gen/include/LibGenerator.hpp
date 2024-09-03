#ifndef DRAWIO_LIB_GENERATOR_HPP
#define DRAWIO_LIB_GENERATOR_HPP

#include <string>
// #include "Image.h"
// #include "DrawioExporter.h"
// #include "utils.hpp"  // Inclure les utilitaires pour lire le JSON

class DrawioLibGenerator {
public:
    DrawioLibGenerator(const std::string& imagePath, const std::string& jsonMappingFile, int tileWidth, int tileHeight);
    void generate(const std::string& outputDrawioFile);

private:
    Image image;
    std::string jsonMappingFile;
    int tileWidth;
    int tileHeight;
};

#endif // DRAWIO_LIB_GENERATOR_HPP
