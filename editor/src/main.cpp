#include "LevelConverter.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input XML file> <output JSON file>" << std::endl;
        return 1;
    }

    std::string inputXmlFile = argv[1];
    std::string outputJsonFile = argv[2];

    try {
        LevelConverter converter(inputXmlFile);
        converter.convertToJson(outputJsonFile);

        std::cout << "JSON data written to " << outputJsonFile << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
