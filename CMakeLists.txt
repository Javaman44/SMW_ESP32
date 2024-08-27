cmake_minimum_required(VERSION 3.15)
project(SDL2Project)

set(CMAKE_CXX_STANDARD 17)

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

# LovyanGFX
add_definitions(-DLGFX_SDL)

# Spécifier les fichiers sources à compiler
file(GLOB Target_Files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS 
    main.cpp
    lib/LovyanGFX/src/lgfx/Fonts/efont/*.c
    lib/LovyanGFX/src/lgfx/Fonts/IPA/*.c
    lib/LovyanGFX/src/lgfx/utility/*.c
    lib/LovyanGFX/src/lgfx/v1/*.cpp
    lib/LovyanGFX/src/lgfx/v1/misc/*.cpp
    lib/LovyanGFX/src/lgfx/v1/panel/Panel_Device.cpp
    lib/LovyanGFX/src/lgfx/v1/panel/Panel_FrameBufferBase.cpp
    lib/LovyanGFX/src/lgfx/v1/platforms/sdl/*.cpp
)



# Créer l'exécutable avec les fichiers sources listés
add_executable(${PROJECT_NAME} main.cpp ${Target_Files})

# Spécifier les répertoires d'inclusion pour LovyanGFX
#target_include_directories(${PROJECT_NAME} PUBLIC "lib/LovyanGFX/src")

# Lier SDL2 et LovyanGFX à l'exécutable
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)
