#include "utils.h"
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