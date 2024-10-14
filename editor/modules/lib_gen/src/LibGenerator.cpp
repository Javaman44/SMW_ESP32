#include "LibGenerator.hpp"
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
    cv::Mat image = loadImage();  // Utilisation d'OpenCV pour charger l'image

    std::ofstream outputFile(outputDrawioFile);
    if (!outputFile.is_open()) {
        throw XmlSaveException(outputDrawioFile);
    }

    outputFile << "<mxlibrary>[\n";
    
    int tileSize = 16;  // Taille de chaque tuile
    int tileSpacing = 1;  // Espacement entre les tuiles si applicable
    int numColumns = image.cols / (tileSize + tileSpacing);
    int numRows = image.rows / (tileSize + tileSpacing);

    bool firstTile = true;  // Pour éviter la virgule après la dernière tuile
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numColumns; ++col) {
            cv::Rect roi(col * (tileSize + tileSpacing), row * (tileSize + tileSpacing), tileSize, tileSize);
            cv::Mat tileImage = image(roi);

            // Vérifier si l'image de la tuile est non vide
            if (isTileNonEmpty(tileImage)) {
                std::string title = "Tile (" + std::to_string(col) + "," + std::to_string(row) + ")";
                std::string id = "tile_" + std::to_string(col) + "_" + std::to_string(row);
                
                // Traiter la tuile : redimensionnement, encodage, etc.
                cv::Mat resizedTileImage = resizeTile(tileImage);
                std::string base64Image = encodeTileToBase64(resizedTileImage);
                std::string xmlData = generateXmlForTile(id, base64Image, title);
                std::string escapedXml = escapeXml(xmlData);

                // Ajouter une virgule avant chaque tuile sauf la première
                if (!firstTile) {
                    outputFile << ",\n";
                }
                outputFile << "  {\n";
                outputFile << "    \"xml\": \"" << escapedXml << "\",\n";
                outputFile << "    \"w\": 40,\n";
                outputFile << "    \"h\": 40,\n";
                outputFile << "    \"aspect\": \"fixed\",\n";
                outputFile << "    \"title\": \"" << title << "\"\n";
                outputFile << "  }";

                firstTile = false;  // La première tuile a été écrite
            }
        }
    }

    outputFile << "\n]</mxlibrary>";
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

// Méthode pour générer le XML pour une tuile donnée
std::string LibGenerator::generateXmlForTile(const std::string& id, const std::string& base64Image, const std::string& title) {
    return R"(<mxGraphModel><root><mxCell id="0"/><mxCell id="1" parent="0"/><object label="" Component=")"
           + id + R"(" id="2"><mxCell style="shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=1;aspect=fixed;image=data:image/png,)"
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
