#pragma once
#include <windows.h>

namespace win32
{
struct OffscreenBuffer
{
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel = 4;
};

struct WindowDimension
{
    int width;
    int height;
};
} // namespace win32
