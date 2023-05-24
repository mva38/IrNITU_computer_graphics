#include <gl/gl.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "mEnu.h"
#include "tExtur.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb-master/stb_image.h"

using namespace std;

GLuint tex;
GLuint bg;
GLuint grass;
int currentX = 0;
int currentY = 0;
Character* fox = new Character();

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float lerp(float a, float b, float t) {//линейна€ интерпол€ци€
    return a + (b - a) * t;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1600,
                          900,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct;
    GetClientRect(hwnd, &rct);
    Load_Texture( "char.png", &tex, GL_REPEAT, GL_REPEAT, GL_NEAREST);
    Load_Texture( "bg.png", &bg, GL_REPEAT, GL_REPEAT, GL_NEAREST);
    Load_Texture( "tile.png", &grass, GL_REPEAT, GL_REPEAT, GL_NEAREST);
    firstdrawTiles(grass);//перва€ отрисовка коробок дл€ дальнейшей поддержки колизии
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Show_Background(bg);//отрисовыываем задний фон
            fox->move_ch();//передвижение персонажа перед проверкой на колиззию
            HandleCollisions(fox->x,fox->y,fox->jumping);//колиззи€
            drawTiles(grass);//отрисовка трав€ных тайлов
            draw(fox->x,fox->y,fox->frame,tex,fox->position);//отрисовка персонажа
            glMatrixMode(GL_PROJECTION);//плавна€ камера с интерпол€цией
            glLoadIdentity();
            const float smoothingFactor = 0.2; // ‘актор сглаживани€ (чем меньше значение, тем плавнее движение)

            // ¬ каждом кадре обновл€ем текущие координаты камеры с использованием интерпол€ции
            currentX = lerp(currentX, fox->x - 720 / 2, smoothingFactor);
            currentY = lerp(currentY, fox->y - 576 / 2, smoothingFactor);

            // »спользуем текущие координаты камеры дл€ установки вида
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(currentX + 80, currentX + 720 + 80, currentY + 220, currentY + 576 + 220, 0.5, -0.5);
            GetClientRect(hwnd, &rct);
            int width = rct.right - rct.left;
            int height =rct.bottom - rct.top;
            SwapBuffers(hDC);
            theta += 1.0f;
            Sleep (20);
        }
    }
    printf("0");
    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_MOUSEMOVE:
        Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_LBUTTONDOWN:
        MouseDown();
        break;

    case WM_LBUTTONUP:
        Menu_MouseUp();
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}



