#include "LibGenerator.hpp"
#include "TileConfig.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <tinyxml2.h>
#include <base64.h>
#include <nlohmann/json.hpp>

// Define stb_image and stb_image_write implementations
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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

    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        throw ImageLoadException(imagePath);
    }

    std::ofstream outputFile(outputDrawioFile);
    if (!outputFile.is_open()) {
        throw XmlSaveException(outputDrawioFile);
    }

    // Add the <mxlibrary> tag at the beginning of the file
    outputFile << "<mxlibrary>[\n";

    for (const auto& tile : config.tiles) {
        std::string title = tile.title;
        std::string id = tile.id;
        int x = tile.x;
        int y = tile.y;

        // Extract tile data from the BMP image
        unsigned char* tileData = extractTileData(data, width, height, channels, x, y, config.tileWidth, config.tileHeight);
        if (!tileData) {
            stbi_image_free(data);
            throw TileExtractionException();
        }

        // Encode the image to base64 in BMP format, without adding ";base64"
        std::string base64Image = encodeImageToBase64(tileData, config.tileWidth, config.tileHeight, channels);
        delete[] tileData;

        // Create the XML string with proper image encoding format (without ";base64")
        std::string xmlData = R"(<mxGraphModel><root><mxCell id="0"/><mxCell id="1" parent="0"/><object label="" Component=")" + id + R"(" id="2"><mxCell style="shape=image;html=1;verticalLabelPosition=bottom;verticalAlign=top;imageAspect=1;aspect=fixed;image=data:image/bmp,)" + base64Image + R"(" vertex="1" parent="1"><mxGeometry width=")" + std::to_string(config.tileWidth) + R"(" height=")" + std::to_string(config.tileHeight) + R"(" as="geometry"/></mxCell></object></root></mxGraphModel>)";

        // Escape the XML string
        std::string escapedXml = escapeXml(xmlData);

        // Write the JSON content with escaped XML into the output file
        outputFile << "  {\n";
        outputFile << "    \"xml\": \"" << escapedXml << "\",\n";
        outputFile << "    \"w\": " << config.tileWidth << ",\n";
        outputFile << "    \"h\": " << config.tileHeight << ",\n";
        outputFile << "    \"aspect\": \"fixed\",\n";
        outputFile << "    \"title\": \"" << title << "\"\n";
        outputFile << "  }";

        if (&tile != &config.tiles.back()) {
            outputFile << ",";
        }
        outputFile << "\n";
    }

    // Close the <mxlibrary> tag
    outputFile << "]</mxlibrary>";
    outputFile.close();

    stbi_image_free(data);
}

unsigned char* LibGenerator::extractTileData(unsigned char* data, int imgWidth, int imgHeight, int channels, int x, int y, int tileWidth, int tileHeight) {
    // Extract a specific tile's data from the image
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
    // Use stb_image_write to encode BMP and directly convert to base64
    std::ostringstream bmpStream;

    // Encode BMP using stb_image_write
    stbi_write_bmp_to_func([](void* context, void* data, int size) {
        std::ostringstream* stream = static_cast<std::ostringstream*>(context);
        stream->write(reinterpret_cast<const char*>(data), size);
    }, &bmpStream, width, height, channels, data);

    // Convert the BMP data from the stream to a vector
    std::string bmpStr = bmpStream.str();
    std::vector<unsigned char> bmpData(bmpStr.begin(), bmpStr.end());

    // Encode the BMP data to base64
    return base64_encode(bmpData.data(), bmpData.size());
}


std::string escapeXml(const std::string& data) {
    std::string escaped;
    for (char c : data) {
        switch (c) {
            case '&':  escaped.append("&amp;"); break;
            case '<':  escaped.append("&lt;"); break;
            case '>':  escaped.append("&gt;"); break;
            case '"':  escaped.append("\\\""); break;  // Escape quotes with a backslash
            default:   escaped.push_back(c); break;
        }
    }
    return escaped;
}
