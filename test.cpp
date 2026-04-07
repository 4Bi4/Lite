#define SDL_MAIN_HANDLED
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

// ── Constants ────────────────────────────────────────────────────────────────
const int   WINDOW_W  = 800;
const int   WINDOW_H  = 600;
const int   FPS       = 60;
const float FRAME_MS  = 1000.0f / FPS;

const SDL_Color COL_BG     = { 30,  30,  46, 255};
const SDL_Color COL_TEXT   = {205, 214, 244, 255};
const SDL_Color COL_ACCENT = {137, 180, 250, 255};
const SDL_Color COL_SUB    = {166, 173, 200, 255};

// ── Helper: load texture via SDL_image ───────────────────────────────────────
SDL_Texture* loadTexture(SDL_Renderer* ren, const std::string& path)
{
    SDL_Texture* tex = IMG_LoadTexture(ren, path.c_str());
    if (!tex)
        std::cerr << "IMG_LoadTexture(" << path << "): " << IMG_GetError() << "\n";
    return tex;
}

// ── Helper: render text centred at (cx, cy) via SDL_ttf ─────────────────────
void drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy)
{
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { cx - surf->w / 2, cy - surf->h / 2, surf->w, surf->h };
    SDL_FreeSurface(surf);
    if (tex)
    {
        SDL_RenderCopy(ren, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// ── Main ─────────────────────────────────────────────────────────────────────
int main(void)
{
    // 1. Init core SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << "\n";
        return (1);
    }

    // 2. SDL_ttf
    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << "\n";
        SDL_Quit();
        return (1);
    }

    // 3. SDL_image (PNG + JPG)
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
    {
        std::cerr << "IMG_Init: " << IMG_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return (1);
    }

    // 4. SDL_mixer (44100 Hz, stereo, 2048-sample chunks)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        std::cerr << "Mix_OpenAudio: " << Mix_GetError() << "\n";
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return (1);
    }

    // 5. Window + hardware-accelerated renderer
    SDL_Window* window = SDL_CreateWindow(
        "Hello, SDL2!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!window || !renderer)
    {
        std::cerr << "Window/Renderer: " << SDL_GetError() << "\n";
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return (1);
    }

    // 6. Load assets
    const std::string fontPath = "./resources/fonts/TlwgTypo/TlwgTypo-Bold.ttf";
    TTF_Font* fontLarge = TTF_OpenFont(fontPath.c_str(), 72);
    TTF_Font* fontSmall = TTF_OpenFont(fontPath.c_str(), 26);
    if (!fontLarge || !fontSmall)
    {
        std::cerr << "TTF_OpenFont: " << TTF_GetError() << "\n";
        return (1);
    }

    // SDL_image: load PNG sprite
    SDL_Texture* iconTex = loadTexture(renderer, "resources/textures/icon.png");

    // SDL_mixer: load WAV sound effect
    Mix_Chunk* beep = Mix_LoadWAV("resources/sounds/fahh.wav");
    if (!beep)
        std::cerr << "Mix_LoadWAV: " << Mix_GetError() << "\n";

    // Play beep once at startup
    if (beep)
        Mix_PlayChannel(-1, beep, 0);

    // 7. Event loop
    bool running   = true;
    SDL_Event event;
    Uint32 lastFrame = SDL_GetTicks();

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_SPACE:
                        // Removed image toggle behavior
                        if (beep)
                            Mix_PlayChannel(-1, beep, 0);
                        break;
                    default:
                        break;
                }
            }
        }

        // Frame cap
        Uint32 now = SDL_GetTicks();
        if ((now - lastFrame) < (Uint32)FRAME_MS)
            SDL_Delay((Uint32)FRAME_MS - (now - lastFrame));
        lastFrame = SDL_GetTicks();

        // Draw
        SDL_SetRenderDrawColor(renderer, COL_BG.r, COL_BG.g, COL_BG.b, 255);
        SDL_RenderClear(renderer);

        // PNG icon via SDL_image (Now always renders if texture exists)
        if (iconTex)
        {
            SDL_Rect iconDst = { WINDOW_W/2 - 32, WINDOW_H/2 - 130, 64, 64 };
            SDL_RenderCopy(renderer, iconTex, nullptr, &iconDst);
        }

        // Accent rule
        SDL_SetRenderDrawColor(renderer, COL_ACCENT.r, COL_ACCENT.g, COL_ACCENT.b, 180);
        SDL_Rect rule = { WINDOW_W/4, WINDOW_H/2 - 50, WINDOW_W/2, 3 };
        SDL_RenderFillRect(renderer, &rule);

        // Text via SDL_ttf
        drawText(renderer, fontLarge, "Hello, World!", COL_TEXT, WINDOW_W/2, WINDOW_H/2 + 10);
        drawText(renderer, fontSmall, "SPACE: play sound  |  ESC: quit", COL_SUB, WINDOW_W/2, WINDOW_H/2 + 75);

        SDL_RenderPresent(renderer);
    }

    // 8. Cleanup (reverse init order)
    if (beep)
        Mix_FreeChunk(beep);
    if (iconTex)
        SDL_DestroyTexture(iconTex);
    TTF_CloseFont(fontSmall);
    TTF_CloseFont(fontLarge);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    std::cout << "Goodbye!\n";
    return (0);
}