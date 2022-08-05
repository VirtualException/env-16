#include "dpy.h"

SDL_Window*     win;
SDL_Renderer*   rdr;
SDL_Texture*    txr;

SDL_Event   event;

uint32_t*   pixels;
void*       vmem;
int         pitch;

void
initDisplay(int w, int h, void* vidmem) {

    if (!SDL_Init(SDL_INIT_EVERYTHING)) {

        win  = SDL_CreateWindow("Env16Dpy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
        rdr  = SDL_CreateRenderer(win, 1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

        SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);

    }
    else {
        fprintf(stderr, "Couldn't init SDL\n");

        exit(EXIT_FAILURE);
    }

    vmem = vidmem;
    pixels = malloc(w * h * sizeof(uint32_t));
    pitch = w * sizeof(uint32_t);

    txr = SDL_CreateTexture(rdr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    SDL_SetRenderTarget(rdr, txr);

}

void
render() {

    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    SDL_RenderClear(rdr);

    SDL_LockTexture(txr, NULL, (void**) &pixels, &pitch);
    memcpy(pixels, vmem, ENV16_VIDMEM_SIZE);
    SDL_UnlockTexture(txr);

    SDL_RenderCopy(rdr, txr, NULL, NULL);

    SDL_RenderPresent(rdr);

}

void
handleEvents() {

    while(SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_WINDOWEVENT:

                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    winrunning = false;
                    running = false;
                }
                break;

            default:
                break;
        }
    }
}