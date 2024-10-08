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

# Add source files 
set(SRC_FILES
    app/main.cpp
    modules/level_gen/src/JsonWriter.cpp
    modules/level_gen/src/LevelConverter.cpp
    modules/level_gen/src/XmlExtractor.cpp
    modules/lib_gen/src/Image.cpp
    modules/lib_gen/src/LibGenerator.cpp
    modules/lib_gen/src/TileExtractor.cpp
    ${lodepng_SOURCE_DIR}/lodepng.cpp  # Explicitly include lodepng.cpp
    ${base64_SOURCE_DIR}/base64.cpp    # Explicitly include base64.cpp
)

# Ajouter l'exécutable avec les fichiers sources
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Inclure les répertoires d'en-tête pour les modules et les dépendances tierces
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${stb_SOURCE_DIR} 
  ${lodepng_SOURCE_DIR}
  ${base64_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/level_gen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/lib_gen/include
)

# Lier les bibliothèques json et tinyxml2 à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json tinyxml2)
