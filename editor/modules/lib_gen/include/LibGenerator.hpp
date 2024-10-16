#ifndef LIBGENERATOR_HPP
#define LIBGENERATOR_HPP

#include <string>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>

// Custom exceptions
class JsonFileOpenException : public std::runtime_error {
public:
    explicit JsonFileOpenException(const std::string& filePath)
        : std::runtime_error("Failed to open JSON file: " + filePath) {}
};

class JsonParsingException : public std::runtime_error {
public:
    explicit JsonParsingException(const std::string& errorMsg)
        : std::runtime_error("JSON parsing error: " + errorMsg) {}
};

class ImageLoadException : public std::runtime_error {
public:
    explicit ImageLoadException(const std::string& imagePath)
        : std::runtime_error("Failed to load image: " + imagePath) {}
};

// LibGenerator class to handle the generation process
class LibGenerator {
public:
    explicit LibGenerator(const std::string& jsonMappingFile);

    // Build the output XML file for draw.io
    void build(const std::string& outputDrawioFile);

private:
    std::string jsonMappingFile;
    std::string imagePath;

    // Process each tile and add its data to the JSON array
    void processTile(int row, int col, const cv::Mat& image, int tileSize, int tileSpacing, nlohmann::json& jsonArray);

    // Load the image using OpenCV
    cv::Mat loadImage();

    // Check if a tile is non-empty
    bool isTileNonEmpty(const cv::Mat& tile);

    // Resize a tile to 40x40 pixels
    cv::Mat resizeTile(cv::Mat& tileImage);

    // Encode a resized tile in base64
    std::string encodeTileToBase64(cv::Mat& image);

    // Generate the XML structure for a tile using pugixml
    void generateXmlForTile(pugi::xml_document& doc, pugi::xml_node& rootNode, const std::string& id, const std::string& base64Image, const std::string& title);

    // Generate escaped XML for a tile
    std::string generateEscapedXmlForTile(const std::string& id, const std::string& base64Image, const std::string& title);

    // Custom method to escape special characters in XML
    std::string escapeXml(const std::string& data);

    // Generate JSON data for a tile
    std::string generateJsonForTile(const std::string& escapedXml, const std::string& title);
};

#endif  // LIBGENERATOR_HPP
