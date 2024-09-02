cmake_minimum_required(VERSION 3.30)
project(SMW_Editor)

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
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2 # Remplacez par la version que vous souhaitez utiliser
)
FetchContent_MakeAvailable(json)


# Ajouter l'exécutable
add_executable(${PROJECT_NAME} src/main.cpp)

# Lier la bibliothèque json à votre cible
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Lier tinyxml2 à votre projet
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)