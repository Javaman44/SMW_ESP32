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

# Ajouter l'exécutable
add_executable(${PROJECT_NAME} src/main.cpp)

# Lier les bibliothèques json et tinyxml2 à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json tinyxml2)
