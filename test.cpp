#define SDL_MAIN_HANDLED
#include <string>
#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_image/SDL_image.h"

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
        std::cerr << "IMG_LoadTexture(" << path << "): " << SDL_GetError() << "\n";
    return tex;
}

// ── Helper: load audio via SDL_mixer ─────────────────────────────────────────
MIX_Audio* loadAudio(MIX_Mixer* mixer, const std::string& path)
{
    MIX_Audio* audio = MIX_LoadAudio(mixer, path.c_str(), false);
    if (!audio)
        std::cerr << "MIX_LoadAudio(" << path << "): " << SDL_GetError() << "\n";
    return audio;
}

// ── Helper: render text centred at (cx, cy) via SDL_ttf ─────────────────────
void drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy)
{
    SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FRect dst = {
        static_cast<float>(cx - surf->w / 2),
        static_cast<float>(cy - surf->h / 2),
        static_cast<float>(surf->w),
        static_cast<float>(surf->h)
    };
    SDL_DestroySurface(surf);
    if (tex)
    {
        SDL_RenderTexture(ren, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}

// ── Main ─────────────────────────────────────────────────────────────────────
int main(void)
{
    // 1. Init core SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << "\n";
        return (1);
    }

    // 2. SDL_ttf
    if (!TTF_Init())
    {
        std::cerr << "TTF_Init: " << SDL_GetError() << "\n";
        SDL_Quit();
        return (1);
    }

    // 3. SDL_mixer
    if (!MIX_Init())
    {
        std::cerr << "MIX_Init: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return (1);
    }

    // 4. SDL_image works without explicit init in this dependency setup.

    // 5. Window + hardware-accelerated renderer
    SDL_Window* window = SDL_CreateWindow(
        "Hello, SDL2!",
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!window || !renderer)
    {
        std::cerr << "Window/Renderer: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return (1);
    }
    if (!SDL_SetRenderVSync(renderer, 1))
        std::cerr << "SDL_SetRenderVSync: " << SDL_GetError() << "\n";

    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
    {
        std::cerr << "MIX_CreateMixerDevice: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        MIX_Quit();
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
        std::cerr << "TTF_OpenFont: " << SDL_GetError() << "\n";
        return (1);
    }

    // SDL_image: load PNG sprite
    SDL_Texture* iconTex = loadTexture(renderer, "resources/textures/icon.png");

    // SDL_mixer: load WAV sound effect
    MIX_Audio* beep = loadAudio(mixer, "resources/sounds/fahh.wav");
    if (beep && !MIX_PlayAudio(mixer, beep))
        std::cerr << "MIX_PlayAudio: " << SDL_GetError() << "\n";

    // 7. Event loop
    bool running   = true;
    SDL_Event event;
    Uint32 lastFrame = SDL_GetTicks();

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                switch (event.key.key)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_SPACE:
                        if (beep && !MIX_PlayAudio(mixer, beep))
                            std::cerr << "MIX_PlayAudio: " << SDL_GetError() << "\n";
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
            SDL_FRect iconDst = {
                static_cast<float>(WINDOW_W/2 - 32),
                static_cast<float>(WINDOW_H/2 - 130),
                64.0f,
                64.0f
            };
            SDL_RenderTexture(renderer, iconTex, nullptr, &iconDst);
        }

        // Accent rule
        SDL_SetRenderDrawColor(renderer, COL_ACCENT.r, COL_ACCENT.g, COL_ACCENT.b, 180);
        SDL_FRect rule = {
            static_cast<float>(WINDOW_W/4),
            static_cast<float>(WINDOW_H/2 - 50),
            static_cast<float>(WINDOW_W/2),
            3.0f
        };
        SDL_RenderFillRect(renderer, &rule);

        // Text via SDL_ttf
        drawText(renderer, fontLarge, "Hello, World!", COL_TEXT, WINDOW_W/2, WINDOW_H/2 + 10);
        drawText(renderer, fontSmall, "SPACE: play sound  |  ESC: quit", COL_SUB, WINDOW_W/2, WINDOW_H/2 + 75);

        SDL_RenderPresent(renderer);
    }

    // 8. Cleanup (reverse init order)
    if (iconTex)
        SDL_DestroyTexture(iconTex);
    if (beep)
        MIX_DestroyAudio(beep);
    MIX_DestroyMixer(mixer);
    TTF_CloseFont(fontSmall);
    TTF_CloseFont(fontLarge);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    MIX_Quit();
    TTF_Quit();
    SDL_Quit();

    std::cout << "Goodbye!\n";
    return (0);
}