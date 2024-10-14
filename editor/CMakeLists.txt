project(SMW_Editor)

# Set the path to the OpenCV and libxml2
set(OpenCV_DIR "C:/msys64/ucrt64/lib/opencv4") # A voir si vraiment utile

#set(libb64_INLCUDE_DIR "C:/msys64/ucrt64/include/b64") # A voir si vraiment utile
set(libxml2_INLCUDE_DIR "C:/msys64/ucrt64/include/libxml2") # renomer libxml2_DIR ???

# Trouver l'emplacement des fichiers de libb64
find_path(LIBB64_INCLUDE_DIR b64/cdecode.h PATHS /mingw64/include)
find_library(LIBB64_LIBRARY NAMES b64 PATHS /mingw64/lib)

# add libraries
find_package(OpenCV REQUIRED)
find_package(libxml2 REQUIRED)
find_package(nlohmann_json REQUIRED)
#find_package(b64 REQUIRED)

# Add source files 
set(SRC_FILES
    app/main.cpp
    modules/level_gen/src/JsonWriter.cpp
    modules/level_gen/src/LevelConverter.cpp
    modules/level_gen/src/XmlExtractor.cpp
    modules/lib_gen/src/LibGenerator.cpp
    modules/lib_gen/src/TileConfig.cpp   
)

# Ajouter l'exécutable avec les fichiers sources
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories
include_directories(
  ${OpenCV_INCLUDE_DIRS} 
 # ${libb64_INLCUDE_DIR}
)

# Inclure les répertoires d'en-tête pour les modules et les dépendances tierces
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${libxml2_INLCUDE_DIR}
  ${LIBB64_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/level_gen/include
  ${CMAKE_CURRENT_SOURCE_DIR}/modules/lib_gen/include
)

# Lier les bibliothèques json et tinyxml2 à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE 
  nlohmann_json::nlohmann_json 
  libxml2
  ${OpenCV_LIBS}
  ${LIBB64_LIBRARY}
) 
