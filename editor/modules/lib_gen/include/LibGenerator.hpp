#ifndef DRAWIO_LIBGENERATOR_HPP
#define DRAWIO_LIBGENERATOR_HPP

#include <string>
#include <vector>

class LibGenerator {
public:
    // Constructeur prenant le fichier de configuration JSON en paramètre
    explicit LibGenerator(const std::string& jsonMappingFile);

    // Méthode pour générer le fichier de bibliothèque Draw.io
    void build(const std::string& outputDrawioFile);

private:
    // Chemin de l'image tileset
    std::string imagePath;

    // Chemin du fichier de configuration JSON
    std::string jsonMappingFile;

    // Méthode pour extraire les données d'une tuile de l'image
    unsigned char* extractTileData(unsigned char* data, int imgWidth, int imgHeight, int channels, int x, int y, int tileWidth, int tileHeight);

    // Méthode pour encoder une image en base64
    std::string encodeImageToBase64(unsigned char* data, int width, int height, int channels);

    // Méthode pour encoder des données en PNG (utilise une bibliothèque PNG comme lodepng)
    void encodePNG(unsigned char* data, int width, int height, int channels, std::vector<unsigned char>& outPng);
};

#endif // DRAWIO_LIBGENERATOR_HPP
