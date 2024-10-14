#include "LibGenerator.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include <base64.h>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <cstring>

// Prototypes of utility functions
std::string escapeXmlWithLibXml2(const std::string& data);

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

    // Créer un nouveau document XML
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST "mxlibrary");
    xmlDocSetRootElement(doc, rootNode);

    int tileSize = 16;  // Taille de chaque tuile
    int tileSpacing = 1;  // Espacement entre les tuiles si applicable
    int numColumns = image.cols / (tileSize + tileSpacing);
    int numRows = image.rows / (tileSize + tileSpacing);
    
    // Parcours des tuiles et traitement
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numColumns; ++col) {
            processTile(row, col, image, tileSize, tileSpacing, rootNode, doc);
        }
    }

    // Sauvegarder le document XML généré dans un fichier
    xmlSaveFormatFileEnc(outputDrawioFile.c_str(), doc, "UTF-8", 1);

    // Libérer les ressources
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

// Nouvelle méthode pour traiter chaque tuile
void LibGenerator::processTile(int row, int col, const cv::Mat& image, int tileSize, int tileSpacing, xmlNodePtr rootNode, xmlDocPtr doc) {
    cv::Rect roi(col * (tileSize + tileSpacing), row * (tileSize + tileSpacing), tileSize, tileSize);
    cv::Mat tileImage = image(roi);

    // Vérifier si l'image de la tuile est non vide
    if (isTileNonEmpty(tileImage)) {
        std::string title = "Tile (" + std::to_string(col) + "," + std::to_string(row) + ")";
        std::string id = "tile_" + std::to_string(col) + "_" + std::to_string(row);

        // Traiter la tuile : redimensionnement, encodage, etc.
        cv::Mat resizedTileImage = resizeTile(tileImage);
        std::string base64Image = encodeTileToBase64(resizedTileImage);

        // Générer le XML échappé
        std::string escapedXml = generateEscapedXmlForTile(id, base64Image, title);

        // Générer le JSON pour la tuile
        std::string jsonString = generateJsonForTile(escapedXml, title);

        // Ajouter le JSON dans le document XML
        xmlNodePtr jsonElement = xmlNewChild(rootNode, NULL, BAD_CAST "data", BAD_CAST jsonString.c_str());
    }
}

// Utiliser libxml2 pour échapper le XML
std::string LibGenerator::generateEscapedXmlForTile(const std::string& id, const std::string& base64Image, const std::string& title) {
    xmlDocPtr innerDoc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST "mxGraphModel");
    xmlDocSetRootElement(innerDoc, rootNode);

    generateXmlForTile(innerDoc, rootNode, id, base64Image, title);

    xmlChar* xmlBuff;
    int buffersize;
    xmlDocDumpFormatMemory(innerDoc, &xmlBuff, &buffersize, 1);

    std::string rawXml((char*)xmlBuff);
    std::string escapedXml = escapeXmlWithLibXml2(rawXml);
    
    xmlFree(xmlBuff);
    xmlFreeDoc(innerDoc);

    return escapedXml;
}

// Nouvelle méthode pour échapper le XML en utilisant libxml2
std::string escapeXmlWithLibXml2(const std::string& data) {
    xmlChar* escapedXml = xmlEncodeEntitiesReentrant(NULL, BAD_CAST data.c_str());
    std::string escapedStr((const char*)escapedXml);  // Convertir en std::string
    xmlFree(escapedXml);  // Libérer la mémoire allouée par libxml2
    return escapedStr;
}

// Nouvelle méthode pour générer le JSON pour une tuile
std::string LibGenerator::generateJsonForTile(const std::string& escapedXml, const std::string& title) {
    nlohmann::json jsonData;
    jsonData["xml"] = escapedXml;
    jsonData["w"] = 40;
    jsonData["h"] = 40;
    jsonData["aspect"] = "fixed";
    jsonData["title"] = title;

    return jsonData.dump(4);  // Convertir le contenu JSON en chaîne avec indentation
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
void LibGenerator::generateXmlForTile(xmlDocPtr doc, xmlNodePtr rootNode, const std::string& id, const std::string& base64Image, const std::string& title) {
    // Création des éléments XML en utilisant libxml2
    xmlNodePtr mxGraphModel = xmlNewChild(rootNode, NULL, BAD_CAST "mxGraphModel", NULL);

    xmlNodePtr root = xmlNewChild(mxGraphModel, NULL, BAD_CAST "root", NULL);

    xmlNodePtr cell0 = xmlNewChild(root, NULL, BAD_CAST "mxCell", NULL);
    xmlNewProp(cell0, BAD_CAST "id", BAD_CAST "0");

    xmlNodePtr cell1 = xmlNewChild(root, NULL, BAD_CAST "mxCell", NULL);
    xmlNewProp(cell1, BAD_CAST "id", BAD_CAST "1");
    xmlNewProp(cell1, BAD_CAST "parent", BAD_CAST "0");

    xmlNodePtr object = xmlNewChild(root, NULL, BAD_CAST "object", NULL);
    xmlNewProp(object, BAD_CAST "label", BAD_CAST "");
    xmlNewProp(object, BAD_CAST "Component", BAD_CAST id.c_str());
    xmlNewProp(object, BAD_CAST "id", BAD_CAST "2");

    xmlNodePtr mxCell = xmlNewChild(object, NULL, BAD_CAST "mxCell", NULL);
    std::string styleValue = "shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=1;aspect=fixed;image=data:image/png," + base64Image;
    xmlNewProp(mxCell, BAD_CAST "style", BAD_CAST styleValue.c_str());
    xmlNewProp(mxCell, BAD_CAST "vertex", BAD_CAST "1");
    xmlNewProp(mxCell, BAD_CAST "parent", BAD_CAST "1");

    xmlNodePtr mxGeometry = xmlNewChild(mxCell, NULL, BAD_CAST "mxGeometry", NULL);
    xmlNewProp(mxGeometry, BAD_CAST "width", BAD_CAST "40");
    xmlNewProp(mxGeometry, BAD_CAST "height", BAD_CAST "40");
    xmlNewProp(mxGeometry, BAD_CAST "as", BAD_CAST "geometry");
}
