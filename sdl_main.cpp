#include <lgfx/v1/platforms/sdl/Panel_sdl.hpp>
#include "user_code.hpp"
#if defined(SDL_h_)

int userFunc(bool *running)
{
    setup();
    do {
      loop();
    } while (*running);
    return 0;
}

int main(int argc, char *argv[]) { 
  return lgfx::Panel_sdl::main(userFunc); 
}

#endif