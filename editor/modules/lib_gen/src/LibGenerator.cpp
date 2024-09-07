#include "LibGenerator.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>

#include <tinyxml2.h>
#include <base64.h>
#include <nlohmann/json.hpp>
#include <lodepng.h>

// Définir l'implémentation de stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// Constructeur prenant uniquement le fichier JSON en paramètre
LibGenerator::LibGenerator(const std::string& jsonMappingFile): jsonMappingFile(jsonMappingFile) {
    
    // Lire le fichier JSON pour obtenir le chemin du tileset
    std::ifstream jsonFile(jsonMappingFile);
    if (!jsonFile.is_open()) {
        throw std::runtime_error("Failed to open JSON mapping file: " + jsonMappingFile);
    }

    nlohmann::json configJson;
    jsonFile >> configJson;
    jsonFile.close();

    // Extraire le chemin du tileset du JSON
    if (configJson.contains("tilesetPath")) {
        imagePath = configJson["tilesetPath"];
    } else {
        throw std::runtime_error("JSON mapping file is missing 'tilesetPath'.");
    }
}

void LibGenerator::build(const std::string& outputDrawioFile) {
    // Lire la configuration des tuiles depuis le fichier JSON
    TileConfig config = readTileConfig(jsonMappingFile);

    // Charger l'image avec stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image: " + imagePath);
    }

    // Commencer à créer le fichier XML pour la bibliothèque Draw.io
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLNode* root = doc.NewElement("mxlibrary");
    doc.InsertFirstChild(root);

    // Convertir l'image en base64 et ajouter chaque tuile au fichier XML
    for (const auto& tile : config.tiles) {
        std::string id = tile.first;
        int x = tile.second.first;
        int y = tile.second.second;
        
        // Extraire la sous-image correspondant à la tuile
        unsigned char* tileData = extractTileData(data, width, height, channels, x, y, config.tileWidth, config.tileHeight);
        if (!tileData) {
            stbi_image_free(data);
            throw std::runtime_error("Failed to extract tile data from image.");
        }

        // Convertir la tuile en base64
        std::string base64Image = encodeImageToBase64(tileData, config.tileWidth, config.tileHeight, channels);
        delete[] tileData;

        // Créer un élément XML pour cette tuile
        tinyxml2::XMLElement* element = doc.NewElement("library");
        element->SetAttribute("title", id.c_str());
        element->SetAttribute("w", config.tileWidth);
        element->SetAttribute("h", config.tileHeight);
        element->SetAttribute("aspect", "fixed");

        std::string xmlData = "<mxGraphModel><root><mxCell id=\"0\"/><mxCell id=\"1\" parent=\"0\"/><object label=\"\" Component=\"" + id + "\" id=\"2\"><mxCell style=\"shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=0;aspect=fixed;image=data:image/png;base64," + base64Image + "\" vertex=\"1\" parent=\"1\"><mxGeometry width=\"" + std::to_string(config.tileWidth) + "\" height=\"" + std::to_string(config.tileHeight) + "\" as=\"geometry\"/></mxCell></object></root></mxGraphModel>";

        element->SetText(xmlData.c_str());
        root->InsertEndChild(element);
    }

    // Libérer la mémoire de l'image complète après traitement
    stbi_image_free(data);

    // Sauvegarder le fichier XML final
    tinyxml2::XMLError eResult = doc.SaveFile(outputDrawioFile.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Failed to save Draw.io library file: " + outputDrawioFile);
    }
}

unsigned char* LibGenerator::extractTileData(unsigned char* data, int imgWidth, int imgHeight, int channels, int x, int y, int tileWidth, int tileHeight) {
    // Extraire une région spécifique de l'image (une tuile) et retourner ses données
    int tileSize = tileWidth * tileHeight * channels;
    unsigned char* tileData = new unsigned char[tileSize];

    for (int row = 0; row < tileHeight; ++row) {
        for (int col = 0; col < tileWidth; ++col) {
            for (int c = 0; c < channels; ++c) {
                int srcIndex = ((y + row) * imgWidth + (x + col)) * channels + c;
                int destIndex = (row * tileWidth + col) * channels + c;
                tileData[destIndex] = data[srcIndex];
            }
        }
    }

    return tileData;
}

std::string LibGenerator::encodeImageToBase64(unsigned char* data, int width, int height, int channels) {
    // Encoder les données d'image en PNG puis en base64
    std::vector<unsigned char> pngData;
    encodePNG(data, width, height, channels, pngData);
    return base64_encode(pngData.data(), pngData.size());
}

void LibGenerator::encodePNG(unsigned char* data, int width, int height, int channels, std::vector<unsigned char>& outPng) {
    // Utilisez une bibliothèque comme lodepng pour encoder l'image en PNG
    // Exemple d'utilisation de lodepng :
    lodepng::State state;
    state.info_raw.colortype = (channels == 4) ? LCT_RGBA : LCT_RGB;
    state.info_raw.bitdepth = 8;
    unsigned error = lodepng::encode(outPng, data, width, height, state);
    if (error) {
        throw std::runtime_error("Failed to encode PNG: " + std::string(lodepng_error_text(error)));
    }
}
