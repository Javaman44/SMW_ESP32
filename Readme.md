# Mémo

Pour les performances:
- utilisation ESP-IDF
- DMA
- FreeRTOS pour les 2 processeurs


# setup
installation msys2 + cmake + ninja + vscode + recommended plugin
https://www.msys2.org/
https://cmake.org/download/
https://ninja-build.org/
https://code.visualstudio.com/docs/cpp/config-mingw#_installing-the-mingww64-toolchain

- créer tache de pretraitement dans le build general pour génrer le fichier json
- trouver un moeyn de faire une compilation multiplateforme (ifdef ou autre)
- gerer le mode dev et release



{
    "image_path": "path/to/your/tileset.png",
    "tile_width": 32,
    "tile_height": 32,
    "tiles": [
        {"filename": "tile1.png", "x": 0, "y": 0},
        {"filename": "tile2.png", "x": 32, "y": 0}
        // Ajouter d'autres tiles ici
    ]
}