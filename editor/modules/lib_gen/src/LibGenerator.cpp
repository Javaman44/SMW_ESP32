#include "LibGenerator.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <pugixml.hpp>  // Use pugixml instead of tinyxml2
#include <base64.h>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Constructor to initialize the generator with a JSON mapping file
LibGenerator::LibGenerator(const std::string& jsonMappingFile) : jsonMappingFile(jsonMappingFile) {
    std::ifstream jsonFile(jsonMappingFile);
    if (!jsonFile.is_open()) {
        throw JsonFileOpenException(jsonMappingFile);
    }

    nlohmann::json configJson;
    try {
        jsonFile >> configJson;
    } catch (const std::exception& e) {
        throw JsonParsingException(e.what());
    }
    jsonFile.close();

    if (configJson.contains("tilesetPath")) {
        imagePath = configJson["tilesetPath"];
    } else {
        throw JsonParsingException("Missing 'tilesetPath' in JSON.");
    }
}

// Method to build the output XML for draw.io
void LibGenerator::build(const std::string& outputDrawioFile) {
    cv::Mat image = loadImage();  // Use OpenCV to load the image

    // Create a new pugixml document
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("mxlibrary");

    nlohmann::json jsonArray = nlohmann::json::array();  // Initialize JSON array

    int tileSize = 16;  // Tile size
    int tileSpacing = 1;  // Tile spacing
    int numColumns = image.cols / (tileSize + tileSpacing);
    int numRows = image.rows / (tileSize + tileSpacing);

    // Process each tile and add it to the JSON array
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numColumns; ++col) {
            processTile(row, col, image, tileSize, tileSpacing, jsonArray);
        }
    }

    // Convert the JSON array to a string and add it as text inside the <mxlibrary> element
    std::string jsonString = jsonArray.dump(4);  // JSON string with indentation
    root.append_child(pugi::node_pcdata).set_value(jsonString.c_str());

    // Save the document to a file
    doc.save_file(outputDrawioFile.c_str(), "", pugi::format_raw | pugi::format_no_declaration);    
}

// Method to process each tile and add its data to the JSON array
void LibGenerator::processTile(int row, int col, const cv::Mat& image, int tileSize, int tileSpacing, nlohmann::json& jsonArray) {
    cv::Rect roi(col * (tileSize + tileSpacing), row * (tileSize + tileSpacing), tileSize, tileSize);
    cv::Mat tileImage = image(roi);

    if (isTileNonEmpty(tileImage)) {
        std::string title = "Tile (" + std::to_string(col) + "," + std::to_string(row) + ")";
        std::string id = "tile_" + std::to_string(col) + "_" + std::to_string(row);

        // Resize and encode the tile to base64
        cv::Mat resizedTileImage = resizeTile(tileImage);
        std::string base64Image = encodeTileToBase64(resizedTileImage);

        // Generate the escaped XML
        std::string escapedXml = generateEscapedXmlForTile(id, base64Image, title);

        // Generate the JSON object for the tile
        nlohmann::json tileJson;
        tileJson["xml"] = escapedXml;
        tileJson["w"] = 40;
        tileJson["h"] = 40;
        tileJson["aspect"] = "fixed";
        tileJson["title"] = title;

        // Add the tile JSON object to the JSON array
        jsonArray.push_back(tileJson);
    }
}

// Method to generate escaped XML for a tile
std::string LibGenerator::generateEscapedXmlForTile(const std::string& id, const std::string& base64Image, const std::string& title) {
    pugi::xml_document innerDoc;
    pugi::xml_node root = innerDoc.append_child("mxGraphModel");

    generateXmlForTile(innerDoc, root, id, base64Image, title);

    // Convert the document to a string
    std::ostringstream xmlStream;
    innerDoc.save(xmlStream, "", pugi::format_raw | pugi::format_no_declaration);  

    // Apply manual escape function to ensure proper escaping
    return xmlStream.str();
}

// Custom method to escape special characters in XML
std::string LibGenerator::escapeXml(const std::string& data) {
    std::string escaped;
    for (char c : data) {
        switch (c) {
            case '&':  escaped.append("&amp;"); break;
            case '<':  escaped.append("&lt;"); break;
            case '>':  escaped.append("&gt;"); break;
            case '"':  escaped.append("&quot;"); break;  // Properly escape quotes as &quot;
            case '\'': escaped.append("&apos;"); break;  // Properly escape single quotes
            default:   escaped.push_back(c); break;
        }
    }
    return escaped;
}

// Method to generate the XML structure for a tile using pugixml
void LibGenerator::generateXmlForTile(pugi::xml_document& doc, pugi::xml_node& rootNode, const std::string& id, const std::string& base64Image, const std::string& title) {
    
    pugi::xml_node root = rootNode.append_child("root");

    pugi::xml_node cell0 = root.append_child("mxCell");
    cell0.append_attribute("id") = "0";

    pugi::xml_node cell1 = root.append_child("mxCell");
    cell1.append_attribute("id") = "1";
    cell1.append_attribute("parent") = "0";

    pugi::xml_node object = root.append_child("object");
    object.append_attribute("label") = "";
    object.append_attribute("Component") = id.c_str();
    object.append_attribute("id") = "2";

    pugi::xml_node mxCell = object.append_child("mxCell");
    std::string styleValue = "shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=1;aspect=fixed;image=data:image/png," + base64Image;
    mxCell.append_attribute("style") = styleValue.c_str();
    mxCell.append_attribute("vertex") = "1";
    mxCell.append_attribute("parent") = "1";

    pugi::xml_node mxGeometry = mxCell.append_child("mxGeometry");
    mxGeometry.append_attribute("width") = "40";
    mxGeometry.append_attribute("height") = "40";
    mxGeometry.append_attribute("as") = "geometry";
}

// Méthode pour charger l'image avec OpenCV
cv::Mat LibGenerator::loadImage() {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        throw ImageLoadException(imagePath);
    }
    return image;
}

// Méthode pour vérifier si une tuile est vide (par exemple, entièrement transparente ou noire)
bool LibGenerator::isTileNonEmpty(const cv::Mat& tile) {
    cv::Mat grayTile;
    cv::cvtColor(tile, grayTile, cv::COLOR_BGR2GRAY);  // Convertir en niveaux de gris
    double minVal;
    double maxVal;
    cv::minMaxLoc(grayTile, &minVal, &maxVal);
    return maxVal > 0;  // Si maxVal > 0, la tuile n'est pas vide
}

// Méthode pour redimensionner une tuile à 40x40 pixels
cv::Mat LibGenerator::resizeTile(cv::Mat& tileImage) {
    cv::Mat resizedImage;
    cv::resize(tileImage, resizedImage, cv::Size(40, 40));  // Redimensionner à 40x40 pixels
    return resizedImage;
}

// Méthode pour encoder une tuile redimensionnée en base64 au format PNG
std::string LibGenerator::encodeTileToBase64(cv::Mat& image) {
    std::vector<uchar> buffer;
    // Utilisation de PNG au lieu de BMP pour gérer le canal alpha
    cv::imencode(".png", image, buffer);  // Encoder l'image au format PNG

    return base64_encode(buffer.data(), buffer.size());  // Encoder en base64
}

