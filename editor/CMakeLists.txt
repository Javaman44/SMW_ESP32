project(SMW_Editor)

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
FetchContent_MakeAvailable(base64)

# Témécharge CImg (manipulation d'image)
FetchContent_Declare(
  cimg
  GIT_REPOSITORY https://github.com/dtschump/CImg.git
  GIT_TAG v.3.4.3
)
FetchContent_MakeAvailable(cimg)

# Set the path to the OpenCV installed by MSYS2
set(OpenCV_DIR "C:/msys64/ucrt64/lib/opencv4")

# Rechercher la configuration d'OpenCV
find_package(OpenCV REQUIRED)

# Add source files 
set(SRC_FILES
    app/main.cpp
    modules/level_gen/src/JsonWriter.cpp
    modules/level_gen/src/LevelConverter.cpp
    modules/level_gen/src/XmlExtractor.cpp
    modules/lib_gen/src/LibGenerator.cpp
    modules/lib_gen/src/TileConfig.cpp    
    ${base64_SOURCE_DIR}/base64.cpp    # Explicitly include base64.cpp
)

# Ajouter l'exécutable avec les fichiers sources
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories for OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Inclure les répertoires d'en-tête pour les modules et les dépendances tierces
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${base64_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/level_gen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/lib_gen/include
)

# Lier les bibliothèques json et tinyxml2 à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE 
  nlohmann_json::nlohmann_json 
  tinyxml2
  ${OpenCV_LIBS}
) 
