#pragma once

namespace demo
{
struct ButtonState
{
    int halfTransitionCount;
    bool endedDown;
};

struct ControlInput
{
    union {
        ButtonState buttons[3];
        struct
        {
            ButtonState ctrl;
            ButtonState shift;
            ButtonState mouseL;
        };
    };
};

struct Input
{
    struct Mouse
    {
        int x;
        int y;
        int z;
    } mouse;

    ControlInput keyboard;
};

struct OffscreenBuffer
{
    void *memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

void updateAndRender(Input *, OffscreenBuffer *);

void initialize(void);
} // namespace demo
