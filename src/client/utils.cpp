#include "utils.h"
#include "glm/glm.hpp"
#include <algorithm>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

static void imageReleaseCb(void* _ptr, void* _userData)
{
    BX_UNUSED(_ptr);
    bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
    bimg::imageFree(imageContainer);
}

void loadBlockTexture(
    bgfx::TextureHandle& handle, int startLayer, const char* top,
    const char* bottom, const char* front, const char* back, const char* left,
    const char* right)
{
    // calc args
    if (strcmp("!empty", bottom) == 0) {
        bottom = top;
    }
    if (strcmp("!empty", front) == 0) {
        front = bottom;
    }
    if (strcmp("!empty", back) == 0) {
        back = front;
    }
    if (strcmp("!empty", left) == 0) {
        left = back;
    }
    if (strcmp("!empty", right) == 0) {
        right = left;
    }
    const char* textures[] = {top, bottom, front, back, left, right};
    // loadTexture
    for (int i = startLayer; i < startLayer + 6; i++) {
        ifstream inFile(textures[i - startLayer], ios::in | ios::binary);
        inFile.seekg(0, ios_base::end);
        uint32_t size = inFile.tellg();
        inFile.seekg(0, ios_base::beg);
        void* data = new char[size];
        // Remember to free it
        inFile.read((char*)data, size);
        inFile.close();
        if (NULL != data) {
            static bx::DefaultAllocator s_allocator;
            static bx::AllocatorI* g_allocator = &s_allocator;
            bimg::ImageContainer* imageContainer = bimg::imageParse(
                g_allocator, data, size, bimg::TextureFormat::RGBA8);
            delete (char*)data;
            if (NULL != imageContainer) {
                const bgfx::Memory* mem = bgfx::makeRef(
                    imageContainer->m_data, imageContainer->m_size,
                    imageReleaseCb, imageContainer);
                bgfx::updateTexture2D(
                    handle, i, 0, 0, 0, imageContainer->m_width,
                    imageContainer->m_height, mem);
            }
        }
    }
    return;
}
bgfx::TextureHandle loadTexture(
    const char* _filePath, uint64_t _flags, uint8_t _skip,
    bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
    BX_UNUSED(_skip);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
    static bx::DefaultAllocator s_allocator;
    static bx::AllocatorI* g_allocator = &s_allocator;

    ifstream inFile(_filePath, ios::in | ios::binary);
    inFile.seekg(0, ios_base::end);
    uint32_t size = inFile.tellg();
    inFile.seekg(0, ios_base::beg);
    void* data = new char[size];
    inFile.read((char*)data, size);
    inFile.close();
    if (NULL != data) {
        bimg::ImageContainer* imageContainer =
            bimg::imageParse(g_allocator, data, size);

        if (NULL != imageContainer) {
            if (NULL != _orientation) {
                *_orientation = imageContainer->m_orientation;
            }

            const bgfx::Memory* mem = bgfx::makeRef(
                imageContainer->m_data, imageContainer->m_size, imageReleaseCb,
                imageContainer);
            delete (char*)data;

            if (imageContainer->m_cubeMap) {
                handle = bgfx::createTextureCube(
                    uint16_t(imageContainer->m_width),
                    1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                    bgfx::TextureFormat::Enum(imageContainer->m_format), _flags,
                    mem);
            } else if (1 < imageContainer->m_depth) {
                handle = bgfx::createTexture3D(
                    uint16_t(imageContainer->m_width),
                    uint16_t(imageContainer->m_height),
                    uint16_t(imageContainer->m_depth),
                    1 < imageContainer->m_numMips,
                    bgfx::TextureFormat::Enum(imageContainer->m_format), _flags,
                    mem);
            } else if (bgfx::isTextureValid(
                           0, false, imageContainer->m_numLayers,
                           bgfx::TextureFormat::Enum(imageContainer->m_format),
                           _flags)) {
                handle = bgfx::createTexture2D(
                    uint16_t(imageContainer->m_width),
                    uint16_t(imageContainer->m_height),
                    1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                    bgfx::TextureFormat::Enum(imageContainer->m_format), _flags,
                    mem);
            }

            if (bgfx::isValid(handle)) {
                bgfx::setName(handle, _filePath);
            }

            if (NULL != _info) {
                bgfx::calcTextureSize(
                    *_info, uint16_t(imageContainer->m_width),
                    uint16_t(imageContainer->m_height),
                    uint16_t(imageContainer->m_depth),
                    imageContainer->m_cubeMap, 1 < imageContainer->m_numMips,
                    imageContainer->m_numLayers,
                    bgfx::TextureFormat::Enum(imageContainer->m_format));
            }
        }
    }

    return handle;
}

bimg::ImageContainer* loadTextureRAW(
    const char* _filePath, uint64_t _flags, uint8_t _skip,
    bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
    BX_UNUSED(_skip);
    static bx::DefaultAllocator s_allocator;
    static bx::AllocatorI* g_allocator = &s_allocator;

    ifstream inFile(_filePath, ios::in | ios::binary);
    inFile.seekg(0, ios_base::end);
    uint32_t size = inFile.tellg();
    inFile.seekg(0, ios_base::beg);
    void* data = new char[size];
    inFile.read((char*)data, size);
    inFile.close();
    if (NULL != data) {
        bimg::ImageContainer* imageContainer = bimg::imageParse(
            g_allocator, data, size, bimg::TextureFormat::RGBA8);

        if (NULL != imageContainer) {
            if (NULL != _orientation) {
                *_orientation = imageContainer->m_orientation;
            }
            delete (char*)data;
            return imageContainer;
        }
    }
    return nullptr;
}

// Compute next-higher power of 2 efficiently, e.g. for power-of-2 texture
// sizes. Public Domain:
// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
inline uint32_t npot2(uint32_t orig)
{
    orig--;
    orig |= orig >> 1;
    orig |= orig >> 2;
    orig |= orig >> 4;
    orig |= orig >> 8;
    orig |= orig >> 16;
    return orig + 1;
}

bgfx::TextureHandle createInventoryCubeImage(
    const char* top, const char* left, const char* right)
{
    BX_UNUSED(0);
    static bx::DefaultAllocator s_allocator;
    static bx::AllocatorI* g_allocator = &s_allocator;

    if (strcmp("!empty", left) == 0) {
        left = top;
    }
    if (strcmp("!empty", right) == 0) {
        right = left;
    }

    bimg::ImageContainer* top_image = loadTextureRAW(top);
    bimg::ImageContainer* left_image = loadTextureRAW(left);
    bimg::ImageContainer* right_image = loadTextureRAW(right);

    uint32_t size = npot2(std::max({
        top_image->m_width,
        top_image->m_height,
        left_image->m_width,
        left_image->m_height,
        right_image->m_width,
        right_image->m_height,
    }));

    size = std::clamp(size, (uint32_t)4, (uint32_t)64);

    uint32_t cube_size = 9 * size;
    uint32_t offset = size / 2;

    bimg::ImageContainer* output = bimg::imageAlloc(
        g_allocator, bimg::TextureFormat::RGBA8, cube_size, cube_size, 0, 1,
        false, false);

    uint32_t* target = (uint32_t*)output->m_data;

    auto draw_image = [=](bimg::ImageContainer* image, float shade_factor,
                          int16_t xu, int16_t xv, int16_t x1, int16_t yu,
                          int16_t yv, int16_t y1,
                          std::initializer_list<glm::u16vec2> offsets) -> void {
        const uint32_t* source = (uint32_t*)image->m_data;
        float brightness = std::clamp(256 * shade_factor, 0.0f, 256.0f) / 256;
        for (uint16_t v = 0; v < size; v++) {
            for (uint16_t u = 0; u < size; u++) {
                uint8_t* color = (uint8_t*)source;
                color[0] *= brightness;
                color[1] *= brightness;
                color[2] *= brightness;
                int16_t x = xu * u + xv * v + x1;
                int16_t y = yu * u + yv * v + y1;
                for (const auto& off : offsets)
                    target[(y + off.y) * cube_size + (x + off.x) + offset] =
                        *source;
                source++;
            }
        }
    };

    draw_image(
        top_image, 1.000000f, 4, -4, 4 * (size - 1), 2, 2, 0,
        {
            {2, 0},
            {3, 0},
            {4, 0},
            {5, 0},
            {0, 1},
            {1, 1},
            {2, 1},
            {3, 1},
            {4, 1},
            {5, 1},
            {6, 1},
            {7, 1},
            {2, 2},
            {3, 2},
            {4, 2},
            {5, 2},
        });

    draw_image(
        left_image, 0.836660f, 4, 0, 0, 2, 5, 2 * size,
        {
            {0, 0}, {1, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {0, 2},
            {1, 2}, {2, 2}, {3, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3},
            {0, 4}, {1, 4}, {2, 4}, {3, 4}, {2, 5}, {3, 5},
        });

    draw_image(
        right_image, 0.670820f, 4, 0, 4 * size, -2, 5, 4 * size - 2,
        {
            {2, 0}, {3, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {0, 2},
            {1, 2}, {2, 2}, {3, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3},
            {0, 4}, {1, 4}, {2, 4}, {3, 4}, {0, 5}, {1, 5},
        });

    const bgfx::Memory* mem =
        bgfx::makeRef(output->m_data, output->m_size, imageReleaseCb, output);

    return bgfx::createTexture2D(
        uint16_t(output->m_width), uint16_t(output->m_height),
        1 < output->m_numMips, output->m_numLayers,
        bgfx::TextureFormat::Enum(output->m_format), 0UL, mem);
}