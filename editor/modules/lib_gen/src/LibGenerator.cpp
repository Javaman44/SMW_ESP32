#include "LibGenerator.hpp"
#include "TileConfig.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <tinyxml2.h>
#include <base64.h>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Prototypes of utility functions
std::string escapeXml(const std::string& data);

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

void LibGenerator::build(const std::string& outputDrawioFile) {
    TileConfig config = readTileConfig(jsonMappingFile);
    cv::Mat image = loadImage();  // Utilisation d'OpenCV pour charger l'image

    std::ofstream outputFile(outputDrawioFile);
    if (!outputFile.is_open()) {
        throw XmlSaveException(outputDrawioFile);
    }

    outputFile << "<mxlibrary>[\n";
    
    for (const auto& tile : config.tiles) {
        std::string xmlData = processTile(image, tile);
        std::string escapedXml = escapeXml(xmlData);

        outputFile << "  {\n";
        outputFile << "    \"xml\": \"" << escapedXml << "\",\n";
        outputFile << "    \"w\": 40,\n";
        outputFile << "    \"h\": 40,\n";
        outputFile << "    \"aspect\": \"fixed\",\n";
        outputFile << "    \"title\": \"" << tile.title << "\"\n";
        outputFile << "  }";

        if (&tile != &config.tiles.back()) {
            outputFile << ",";
        }
        outputFile << "\n";
    }

    outputFile << "]</mxlibrary>";
    outputFile.close();
}

// Méthode pour charger l'image avec OpenCV
cv::Mat LibGenerator::loadImage() {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        throw ImageLoadException(imagePath);
    }
    return image;
}

// Méthode pour traiter une tuile (extraction, redimensionnement, génération du XML)
std::string LibGenerator::processTile(cv::Mat& image, const Tile& tile) {
    cv::Mat tileImage = extractTile(image, tile);
    cv::Mat resizedTileImage = resizeTile(tileImage);

    std::string base64Image = encodeTileToBase64(resizedTileImage);
    return generateXmlForTile(tile, base64Image);
}

// Méthode pour extraire une tuile de 16x16 pixels
cv::Mat LibGenerator::extractTile(cv::Mat& image, const Tile& tile) {
    int tileX = tile.x;
    int tileY = tile.y;
    cv::Rect roi(tileX * 17, tileY * 17, 16, 16);  // Définir la région de 16x16 pixels
    return image(roi).clone();  // Extraire et cloner la tuile
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

// Méthode pour générer le XML pour une tuile donnée
std::string LibGenerator::generateXmlForTile(const Tile& tile, const std::string& base64Image) {
    return R"(<mxGraphModel><root><mxCell id="0"/><mxCell id="1" parent="0"/><object label="" Component=")"
           + tile.id + R"(" id="2"><mxCell style="shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=1;aspect=fixed;image=data:image/png,)"
           + base64Image + R"(" vertex="1" parent="1"><mxGeometry width="40" height="40" as="geometry"/></mxCell></object></root></mxGraphModel>)";
}

std::string escapeXml(const std::string& data) {
    std::string escaped;
    for (char c : data) {
        switch (c) {
            case '&':  escaped.append("&amp;"); break;
            case '<':  escaped.append("&lt;"); break;
            case '>':  escaped.append("&gt;"); break;
            case '"':  escaped.append(R"(\")"); break;  // Échapper les guillemets avec un antislash
            default:   escaped.push_back(c); break;
        }
    }
    return escaped;
}
