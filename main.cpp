#include <SDL.h>
#include <iostream>

// SDL redefines the entry point main with SDL_main, but if SDL2main.lib is not present, we define main directly.
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    } else {
        std::cout << "SDL Init ok" << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }




    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set the drawing color to blue (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Clear the screen with the drawing color
    SDL_RenderClear(renderer);

    // Set the drawing color to red (R, G, B, A)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Define a rectangle
    SDL_Rect rect;
    rect.x = 200; // X position
    rect.y = 150; // Y position
    rect.w = 400; // Width
    rect.h = 300; // Height

    // Draw the rectangle
    SDL_RenderFillRect(renderer, &rect);

    // Present the renderer to the window
    SDL_RenderPresent(renderer);








    SDL_Delay(3000); // Display the window for 3 seconds
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
