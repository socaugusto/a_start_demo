#include "demo.h"

#include "astar.h"

#include <cstdint>
#include <vector>

namespace
{
astar::Node *nodes = nullptr;
astar::Node *begin = nullptr;
astar::Node *finish = nullptr;

int tilesWidth = 32;
int tilesHeight = 18;

uint32_t roundFloatToUint32(float);

void drawRectangle(demo::OffscreenBuffer *,
                   int32_t minX,
                   int32_t minY,
                   int32_t maxX,
                   int32_t maxY,
                   uint32_t color);
} // namespace

namespace demo
{
void initialize(void)
{
    nodes = new astar::Node[tilesWidth * tilesHeight];
    for (int x = 0; x < tilesWidth; x++)
    {
        for (int y = 0; y < tilesHeight; y++)
        {
            int pitch = y * tilesWidth;
            int nodeIndex = pitch + x;

            nodes[nodeIndex].x = x;
            nodes[nodeIndex].y = y;

            nodes[nodeIndex].isObstacle = false;
            nodes[nodeIndex].wasVisited = false;

            nodes[nodeIndex].parent = nullptr;
        }
    }

    for (int x = 0; x < tilesWidth; x++)
    {
        for (int y = 0; y < tilesHeight; y++)
        {
            int pitch = y * tilesWidth;
            int nodeIndex = pitch + x;

            if (y > 0)
            {
                nodes[nodeIndex].neighbours.push_back(&nodes[(y - 1) * tilesWidth + x]);
            }
            if (y < tilesHeight - 1)
            {
                nodes[nodeIndex].neighbours.push_back(&nodes[(y + 1) * tilesWidth + x]);
            }
            if (x > 0)
            {
                nodes[nodeIndex].neighbours.push_back(&nodes[y * tilesWidth + (x - 1)]);
            }
            if (x < tilesWidth - 1)
            {
                nodes[nodeIndex].neighbours.push_back(&nodes[y * tilesWidth + (x + 1)]);
            }
        }
    }

    begin = &nodes[(tilesHeight / 2) * tilesWidth + 2];
    finish = &nodes[(tilesHeight / 2) * tilesWidth + tilesWidth - 2];
}

void updateAndRender(Input *input, OffscreenBuffer *buffer)
{
    int nNodeSize = 30;
    int nNodeBorder = 5;

    int selectedX = input->mouse.x / nNodeSize;
    int selectedY = input->mouse.y / nNodeSize;

    bool wasMouseLeftButtonReleased = ((!input->keyboard.mouseL.endedDown)
                                       && (input->keyboard.mouseL.halfTransitionCount == 1));
    if (wasMouseLeftButtonReleased)
    {
        if (selectedX >= 0 && selectedX < tilesWidth)
        {
            if (selectedY >= 0 && selectedY < tilesHeight)
            {
                int pitch = selectedY * tilesWidth;
                int nodeIndex = pitch + selectedX;

                if (input->keyboard.ctrl.endedDown)
                {
                    finish = &nodes[nodeIndex];
                }
                else if (input->keyboard.shift.endedDown)
                {
                    begin = &nodes[nodeIndex];
                }
                else
                {
                    nodes[nodeIndex].isObstacle = !nodes[nodeIndex].isObstacle;
                }

                astar::Input data{};
                data.begin = begin;
                data.finish = finish;
                data.nodeVector = nodes;
                data.numberOfNodes = tilesWidth * tilesHeight;
                astar::solve(data);
            }
        }
    }

    drawRectangle(buffer, 0, 0, buffer->width, buffer->height, 0);

    uint32_t blueDark = 0x00000077;
    uint32_t blue = 0x000000FF;
    uint32_t grey = 0x00777777;
    uint32_t red = 0x00FF0000;
    uint32_t green = 0x0000FF00;
    uint32_t yellow = 0x00FFFF00;

    for (int x = 0; x < tilesWidth; x++)
    {
        for (int y = 0; y < tilesHeight; y++)
        {
            int pitch = y * tilesWidth;
            int nodeIndex = pitch + x;

            int minX = x * nNodeSize + nNodeBorder;
            int minY = y * nNodeSize + nNodeBorder;
            int maxX = (x + 1) * nNodeSize - nNodeBorder;
            int maxY = (y + 1) * nNodeSize - nNodeBorder;

            bool isObstacle = nodes[nodeIndex].isObstacle;
            drawRectangle(buffer, minX, minY, maxX, maxY, isObstacle ? grey : blueDark);

            if (nodes[nodeIndex].wasVisited)
            {
                drawRectangle(buffer, minX, minY, maxX, maxY, blue);
            }

            if (&nodes[nodeIndex] == begin)
            {
                drawRectangle(buffer, minX, minY, maxX, maxY, green);
            }

            if (&nodes[nodeIndex] == finish)
            {
                drawRectangle(buffer, minX, minY, maxX, maxY, red);
            }
        }
    }

    if (finish != nullptr)
    {
        astar::Node *p = finish;
        while (p->parent != nullptr && p->parent != begin)
        {
            drawRectangle(buffer,
                          p->parent->x * nNodeSize + nNodeBorder,
                          p->parent->y * nNodeSize + nNodeBorder,
                          (p->parent->x + 1) * nNodeSize - nNodeBorder,
                          (p->parent->y + 1) * nNodeSize - nNodeBorder,
                          yellow);

            p = p->parent;
        }
    }
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
                   int32_t minX,
                   int32_t minY,
                   int32_t maxX,
                   int32_t maxY,
                   uint32_t color)
{
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

    uint8_t *row
        = ((uint8_t *)buffer->memory + minX * buffer->bytesPerPixel + minY * buffer->pitch);
    for (int y = minY; y < maxY; ++y)
    {
        uint32_t *pixel = (uint32_t *)row;
        for (int x = minX; x < maxX; ++x)
        {
            *pixel++ = color;
        }
        row += buffer->pitch;
    }
}
} // namespace
