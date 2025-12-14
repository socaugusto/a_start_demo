#include "win32_platform.h"

namespace
{
win32::OffscreenBuffer globalBackBuffer;
bool globalRunning;
} // namespace

namespace win32
{
void resizeDIBSection(OffscreenBuffer *, int width, int height);
WindowDimension getWindowDimension(HWND);
void displayBufferInWindow(OffscreenBuffer *, HDC, int, int);
LRESULT CALLBACK mainWindowCallback(HWND, UINT, WPARAM, LPARAM);
void processPendingMessages(void);
} // namespace win32

// https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program
int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE, LPSTR, int)
{
    WNDCLASS windowClass{};

    resizeDIBSection(&globalBackBuffer, 960, 540);

    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = win32::mainWindowCallback;
    windowClass.hInstance = Instance;
    // windowClass.hIcon = ;
    windowClass.lpszClassName = "AStarDemoWindowClass";

    if (RegisterClass(&windowClass))
    {
        // https://learn.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
        HWND window = CreateWindowEx(0 /*WS_EX_TOPMOST*/,
                                     windowClass.lpszClassName,
                                     "A* Star Demo",
                                     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     CW_USEDEFAULT,
                                     0,
                                     0,
                                     Instance,
                                     0);

        if (window)
        {
            globalRunning = true;

            while (globalRunning)
            {
                win32::processPendingMessages();

                win32::WindowDimension dimension = win32::getWindowDimension(window);

                HDC deviceContext = GetDC(window);
                win32::displayBufferInWindow(&globalBackBuffer,
                                             deviceContext,
                                             dimension.width,
                                             dimension.height);
                ReleaseDC(window, deviceContext);
            }
        }
        else
        {
            OutputDebugString("[FATAL] No window was created!! \n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        OutputDebugString("[FATAL] No window class was created!! \n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace win32
{
void resizeDIBSection(OffscreenBuffer *buffer, int width, int height)
{
    if (buffer->memory)
    {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }

    buffer->width = width;
    buffer->height = height;
    buffer->bytesPerPixel = 4;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = buffer->bytesPerPixel * (buffer->width * buffer->height);

    // https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
    buffer->memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    buffer->pitch = width * buffer->bytesPerPixel;
}

WindowDimension getWindowDimension(HWND window)
{
    WindowDimension result;

    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = clientRect.right - clientRect.left;
    result.height = clientRect.bottom - clientRect.top;

    return result;
}

void displayBufferInWindow(OffscreenBuffer *buffer,
                           HDC deviceContext,
                           int windowWidth,
                           int windowHeight)
{
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
    StretchDIBits(deviceContext,
                  0,
                  0,
                  windowWidth,
                  windowHeight,
                  0,
                  0,
                  buffer->width,
                  buffer->height,
                  buffer->memory,
                  &buffer->info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK mainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;

    switch (message)
    {
    case WM_SIZE:

        break;
    case WM_DESTROY:
    case WM_CLOSE:
        globalRunning = false;

        break;
    case WM_ACTIVATEAPP:

        break;
    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        HDC deviceContext = BeginPaint(window, &paint);
        WindowDimension dimension = getWindowDimension(window);
        displayBufferInWindow(&globalBackBuffer, deviceContext, dimension.width, dimension.height);
        EndPaint(window, &paint);
    }
    break;
    default:
    {
        Result = DefWindowProc(window, message, wParam, lParam);
    }
    break;
    }

    return Result;
}

void processPendingMessages(void)
{
    // https://learn.microsoft.com/en-us/windows/win32/learnwin32/window-messages
    MSG message;

    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch (message.message)
        {
        case WM_QUIT:
        {
            globalRunning = false;
        }
        break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
            break;

        default:
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        break;
        }
    }
}
} // namespace win32
