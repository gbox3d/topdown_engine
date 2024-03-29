#include <SDL.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);

                {
                    SDL_Rect rt = { 128 + 32,128 - 32,64,64 };
                    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
                    SDL_RenderFillRect(renderer, &rt);
                }

                {


                    //반투명 모드 
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                    SDL_Rect rt = { 128,128,64,64 };
                    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x80);
                    SDL_RenderFillRect(renderer, &rt);
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                }

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}