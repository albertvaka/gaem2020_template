#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

inline GPU_Image* loadImage(const std::string& path) {

    GPU_Image* texture = GPU_LoadImage(path.c_str());
    if (!texture) {
        Debug::out << "Unable to load image '" << path.c_str() << "': " << IMG_GetError();
        return nullptr;
    }
    GPU_SetImageFilter(texture, GPU_FILTER_NEAREST);
    GPU_SetSnapMode(texture, GPU_SNAP_NONE);
    return texture;
}

inline TTF_Font* loadFont(const std::string& path, int size) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font) {
        Debug::out << "Unable to load font '" << path.c_str() << "': " << TTF_GetError();
    }
    return font;
}
inline TTF_Font* loadFontOutline(const std::string& path, int size, int outline) {
    TTF_Font* font = loadFont(path, size);
    TTF_SetFontOutline(font, outline);
    return font;
}

inline Mix_Music* loadMusic(const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        Debug::out << "Unable to load music '" << path.c_str() << "': " << Mix_GetError();
    }
    return music;
}
