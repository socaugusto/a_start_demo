#include "demo.h"

#include <cstdint>

namespace
{
uint32_t roundFloatToUint32(float);

void drawRectangle(demo::OffscreenBuffer *,
                   float fMinX,
                   float fMinY,
                   float fMaxX,
                   float fMaxY,
                   float r,
                   float g,
                   float b);
} // namespace

namespace demo
{
void initialize(void)
{}
void updateAndRender(Input *, OffscreenBuffer *buffer)
{
    drawRectangle(buffer,
                  0.0f,
                  0.0f,
                  (float)buffer->width,
                  (float)buffer->height,
                  1.0f,
                  0.0f,
                  1.0f);
}
} // namespace demo

namespace
{
uint32_t roundFloatToUint32(float value)
{
    uint32_t result = (uint32_t)(value + 0.5f);
    return result;
}

void drawRectangle(demo::OffscreenBuffer *buffer,
                   float fMinX,
                   float fMinY,
                   float fMaxX,
                   float fMaxY,
                   float r,
                   float g,
                   float b)
{
    int32_t minX = roundFloatToUint32(fMinX);
    int32_t minY = roundFloatToUint32(fMinY);
    int32_t maxX = roundFloatToUint32(fMaxX);
    int32_t maxY = roundFloatToUint32(fMaxY);

    if (minX < 0)
    {
        minX = 0;
    }

    if (minY < 0)
    {
        minY = 0;
    }

    if (maxX > buffer->width)
    {
        maxX = buffer->width;
    }

    if (maxY > buffer->height)
    {
        maxY = buffer->height;
    }

    uint32_t color = ((roundFloatToUint32(r * 255.0f) << 16)
                      | (roundFloatToUint32(g * 255.0f) << 8)
                      | (roundFloatToUint32(b * 255.0f) << 0));

    uint8_t *row
        = ((uint8_t *)buffer->memory + minX * buffer->bytesPerPixel + minY * buffer->pitch);
    for (int Y = minY; Y < maxY; ++Y)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int X = minX; X < maxX; ++X)
        {
            *pixel++ = color;
        }
        row += buffer->pitch;
    }
}
} // namespace
