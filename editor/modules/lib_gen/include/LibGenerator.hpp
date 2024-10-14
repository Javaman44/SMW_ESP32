#ifndef DRAWIO_LIBGENERATOR_HPP
#define DRAWIO_LIBGENERATOR_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <opencv2/opencv.hpp>  // Inclusion de la bibliothèque OpenCV
#include <libxml/parser.h>     // Inclusion pour libxml2
#include <libxml/tree.h>       // Inclusion pour libxml2
#include <b64/encode.h>       // Inclusion pour l'encodage base64 avec libb64

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

    // Méthode pour charger l'image à partir du chemin tileset
    cv::Mat loadImage();

    // Méthode pour traiter chaque tuile
    void processTile(int row, int col, const cv::Mat& image, int tileSize, int tileSpacing, xmlNodePtr rootNode, xmlDocPtr doc);

    // Méthode pour vérifier si une tuile est non vide
    bool isTileNonEmpty(const cv::Mat& tile);

    // Méthode pour redimensionner une tuile à 40x40 pixels
    cv::Mat resizeTile(cv::Mat& tileImage);

    // Méthode pour encoder une image redimensionnée en base64 avec libb64
    std::string encodeTileToBase64(cv::Mat& resizedTileImage);

    // Méthode pour générer le XML pour une tuile donnée
    void generateXmlForTile(xmlDocPtr doc, xmlNodePtr rootNode, const std::string& id, const std::string& base64Image, const std::string& title);

    // Méthode pour échapper le contenu XML en utilisant libxml2
    std::string generateEscapedXmlForTile(const std::string& id, const std::string& base64Image, const std::string& title);

    // Méthode pour échapper le XML avec libxml2
    std::string escapeXmlWithLibXml2(const std::string& data);

    // Méthode pour générer le JSON pour une tuile donnée
    std::string generateJsonForTile(const std::string& escapedXml, const std::string& title);
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
