cmake_minimum_required(VERSION 3.15)
project(SDL2Project)

# Spécifiez les standards de C++
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclure FetchContent pour gérer les dépendances
include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.26.0
)

FetchContent_GetProperties(SDL2)
if(NOT SDL2_POPULATED)
  set(BUILD_SHARED_LIBS OFF)  # Désactiver les bibliothèques partagées pour SDL2
  FetchContent_MakeAvailable(SDL2)
endif()

# Définir la macro SDL_MAIN_HANDLED pour gérer la fonction main par vous-même
#add_definitions(-DSDL_MAIN_HANDLED)

# LovyanGFX
set(LOVYANGFX_DIR "lib/LovyanGFX/")
add_definitions(-DLGFX_SDL)

# Ajouter les fichiers sources de LovyanGFX nécessaires
file(GLOB LOVYANGFX_SOURCES
  ${LOVYANGFX_DIR}/src/lgfx/Fonts/efont/*.c
  ${LOVYANGFX_DIR}/src/lgfx/Fonts/IPA/*.c
  ${LOVYANGFX_DIR}/src/lgfx/utility/*.c
  ${LOVYANGFX_DIR}/src/lgfx/v1/*.cpp
  ${LOVYANGFX_DIR}/src/lgfx/v1/misc/*.cpp
  ${LOVYANGFX_DIR}/src/lgfx/v1/panel/Panel_Device.cpp
  ${LOVYANGFX_DIR}/src/lgfx/v1/panel/Panel_FrameBufferBase.cpp
  ${LOVYANGFX_DIR}/src/lgfx/v1/platforms/sdl/*.cpp
)

# Récupérer automatiquement tous les fichiers .cpp dans le répertoire src
file(GLOB_RECURSE PROJECT_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Définir l'exécutable
add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCES}
  ${LOVYANGFX_SOURCES}
)

# Spécifier les répertoires des includes (hpp) pour LovyanGFX
target_include_directories(${PROJECT_NAME} PUBLIC 
  ${LOVYANGFX_DIR}/src
  ${CMAKE_SOURCE_DIR}/src
)

# Lier SDL2 et LovyanGFX à l'exécutable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2) #SDL2::SDL2main)
