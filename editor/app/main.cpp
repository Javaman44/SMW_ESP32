#include "LibGenerator.hpp"
#include "LevelConverter.hpp"
#include <iostream>


int main(int argc, char* argv[]) {

    // test
    char src[10] = "Hello";

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <mode> A REVOIR " << std::endl;
        std::cerr << "Modes:" << std::endl;
        std::cerr << "  create-drawio-lib <json mapping file> <output drawio file>" << std::endl;
        std::cerr << "  convert-level <input XML file> <output JSON file>" << std::endl;
        return 1;
    }

    /* this comment is non-compliant */
    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    try {
        if (mode == "create-drawio-lib") {
            if (argc < 5) {
                std::cerr << "Missing JSON mapping file for create-drawio-lib mode" << std::endl;
                return 1;
            }

            std::string jsonMappingFile = argv[4];

            // Utilisation de la classe LibGenerator
            LibGenerator generator(jsonMappingFile);
            generator.build(outputFile);
            std::cout << "Draw.io library created at " << outputFile << std::endl;

        } else if (mode == "convert-level") {
            // Utilisation de la classe LevelConverter
            LevelConverter converter(inputFile);
            converter.convertToJson(outputFile);
            std::cout << "JSON data written to " << outputFile << std::endl;

        } else {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
