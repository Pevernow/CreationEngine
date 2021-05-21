#ifndef _UTILS_H_
#define _UTILS_H_
#include <bgfx/bgfx.h>
#include <bimg/decode.h>

void loadBlockTexture(
    bgfx::TextureHandle& handle, int startLayer, const char* top,
    const char* bottom = "!empty", const char* front = "!empty",
    const char* back = "!empty", const char* left = "!empty",
    const char* right = "!empty");
bgfx::TextureHandle loadTexture(
    const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
    uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL,
    bimg::Orientation::Enum* _orientation = NULL);

#endif