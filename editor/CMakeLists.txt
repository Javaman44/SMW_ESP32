project(SMW_Editor)

# Set the path to the OpenCV 
set(OpenCV_DIR "C:/msys64/ucrt64/lib/opencv4") # A voir si vraiment utile

#set(libb64_INLCUDE_DIR "C:/msys64/ucrt64/include/b64") # A voir si vraiment utile
#set(libxml2_INLCUDE_DIR "C:/msys64/ucrt64/include/libxml2") # renomer libxml2_DIR ???

# Trouver l'emplacement des fichiers de libb64
#find_path(LIBB64_INCLUDE_DIR b64/encode.h PATHS /mingw64/include)
#find_library(LIBB64_LIBRARY NAMES b64 PATHS /mingw64/lib)

# add libraries
find_package(OpenCV REQUIRED)
find_package(pugixml REQUIRED)
find_package(nlohmann_json REQUIRED)

# Utiliser FetchContent pour inclure cpp-base64
FetchContent_Declare(
  base64
  GIT_REPOSITORY https://github.com/ReneNyffenegger/cpp-base64.git
  GIT_TAG master  # Choisissez une version spécifique si nécessaire
)
FetchContent_MakeAvailable(base64)

# Add source files 
set(SRC_FILES
    app/main.cpp
    modules/level_gen/src/JsonWriter.cpp
    modules/level_gen/src/LevelConverter.cpp
    modules/level_gen/src/XmlExtractor.cpp
    modules/lib_gen/src/LibGenerator.cpp
    modules/lib_gen/src/TileConfig.cpp
    ${base64_SOURCE_DIR}/base64.cpp   
)

# Ajouter l'exécutable avec les fichiers sources
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories
include_directories(
  ${OpenCV_INCLUDE_DIRS} 
)

# Inclure les répertoires d'en-tête pour les modules et les dépendances tierces
target_include_directories(${PROJECT_NAME} PRIVATE 
  #${libxml2_INLCUDE_DIR}
  #${LIBB64_INCLUDE_DIR}
  ${base64_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/level_gen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/lib_gen/include
)

# Lier les bibliothèques json et pugixml à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE 
  nlohmann_json::nlohmann_json
  pugixml::pugixml
  ${OpenCV_LIBS}
) 
