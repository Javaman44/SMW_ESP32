#include "LibGenerator.hpp"
#include "LevelConverter.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    
    if (argc < 4) {
        std::cerr << argc << std::endl;
        std::cerr << "Usage: " << argv[0] << " <mode> A REVOIR " << std::endl;
        std::cerr << "Modes:" << std::endl;
        std::cerr << "  generate <config file> <output drawio lib file>" << std::endl;
        std::cerr << "  export <input XML file> <output JSON file>" << std::endl;
        return 1;
    }
    
    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    try {
        if (mode == "generate") {
            if (argc < 4) {                
                std::cerr << "Missing arguments for generate mode" << std::endl;
                return 1;
            }

            std::string jsonMappingFile = argv[2];
            std::cout << "jsonMappingFile at " << jsonMappingFile << std::endl;

            // Utilisation de la classe LibGenerator
            LibGenerator generator(jsonMappingFile);
            generator.build(outputFile);
            std::cout << "Draw.io library created at " << outputFile << std::endl;

        } 
        
        // TODO: need to fix
        /*
        else if (mode == "export") {
            // Utilisation de la classe LevelConverter
            LevelConverter converter(inputFile);
            converter.convertToJson(outputFile);
            std::cout << "JSON data written to " << outputFile << std::endl;

        } else {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return 1;
        }*/

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
