cmake_minimum_required(VERSION 3.30)
project(SMW_Editor)

# Ajouter le répertoire include/
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Inclure FetchContent
include(FetchContent)

# Télécharger tinyxml2
FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
  GIT_TAG        master  # ou choisissez une version spécifique
)

# Charger tinyxml2
FetchContent_MakeAvailable(tinyxml2)

# Utiliser FetchContent pour inclure nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2 # Remplacez par la version que vous souhaitez utiliser
)
FetchContent_MakeAvailable(json)

# Utiliser FetchContent pour inclure cpp-base64
FetchContent_Declare(
  base64
  GIT_REPOSITORY https://github.com/ReneNyffenegger/cpp-base64.git
  GIT_TAG master  # Choisissez une version spécifique si nécessaire
)

# Charger cpp-base64
FetchContent_MakeAvailable(base64)

# Télécharger et configurer stb_image
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master  # Vous pouvez spécifier une version fixe si nécessaire
)
FetchContent_MakeAvailable(stb)

# Télécharger et configurer lodepng pour le traitement PNG
FetchContent_Declare(
  lodepng
  GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
  GIT_TAG master
)
FetchContent_MakeAvailable(lodepng)

# Rechercher tous les fichiers .cpp dans les sous-modules
file(GLOB_RECURSE MODULE_SOURCES "modules/**/src/*.cpp")

# Ajouter l'exécutable avec le fichier main.cpp et les fichiers sources des modules
add_executable(${PROJECT_NAME} app/main.cpp ${MODULE_SOURCES})

# Rechercher tous les répertoires include dans les modules
file(GLOB_RECURSE INCLUDE_DIRS "modules/**/include")

# Inclure les répertoires d'en-tête pour les modules et les dépendances tierces
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${INCLUDE_DIRS}
  ${stb_SOURCE_DIR} 
  ${lodepng_SOURCE_DIR}
)

# Lier les bibliothèques json et tinyxml2 à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json tinyxml2 base64)
