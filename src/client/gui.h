#include "sdl-imgui/imgui_impl_sdl.h"

class GUImanager
{
public:
    void init(SDL_Window* sdl_window_ptr);
    void view();
    void showdebuginfo();

private:
    SDL_Window* sdl_window;
};