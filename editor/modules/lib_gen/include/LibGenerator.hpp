#ifndef DRAWIO_LIBGENERATOR_HPP
#define DRAWIO_LIBGENERATOR_HPP

#include <string>
#include <vector>
#include <stdexcept>

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
};

// Déclaration des exceptions personnalisées
struct JsonFileOpenException : public std::runtime_error {
    explicit JsonFileOpenException(const std::string& filepath)
        : std::runtime_error("Failed to open JSON file: " + filepath) {}
};

struct JsonParsingException : public std::runtime_error {
    explicit JsonParsingException(const std::string& message)
        : std::runtime_error("Failed to parse JSON: " + message) {}
};

struct ImageLoadException : public std::runtime_error {
    explicit ImageLoadException(const std::string& filepath)
        : std::runtime_error("Failed to load image: " + filepath) {}
};

struct TileExtractionException : public std::runtime_error {
    explicit TileExtractionException()
        : std::runtime_error("Failed to extract tile data from image.") {}
};

struct PngEncodingException : public std::runtime_error {
    explicit PngEncodingException(const std::string& errorMessage)
        : std::runtime_error("Failed to encode PNG: " + errorMessage) {}
};

struct XmlSaveException : public std::runtime_error {
    explicit XmlSaveException(const std::string& filepath)
        : std::runtime_error("Failed to save XML file: " + filepath) {}
};

#endif // DRAWIO_LIBGENERATOR_HPP
