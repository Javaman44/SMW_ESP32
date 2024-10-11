#ifndef LEVEL_CONVERTER_HPP
#define LEVEL_CONVERTER_HPP

#include <string>
#include "XmlExtractor.hpp"
#include "JsonWriter.hpp"

class LevelConverter {
public:
    // Constructeur
    explicit LevelConverter(const std::string& xmlPath);

    // Méthode pour directement extraire et écrire dans un fichier JSON
    void convertToJson(const std::string& outputJsonFile) const;

private:
    std::string xmlPath;  // Chemin du fichier XML
};

#endif // LEVEL_CONVERTER_HPP
