#include <gl/gl.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "camera.h"

float vert[] = {1, 1, 2,  1, -1, 2,  -1, -1, 2,  -1, 1, 2};// массив точек для отображения пола
float normal_vert[]={1, 1, 2,  1, -1, 2,  -1, -1, 2,  -1, 1, 2}; // массив точек нормалей
int n = 20;// кол-во тайлов на карте
float theta = 0;
using namespace std;

void Init_Light()
{
glEnable(GL_LIGHTING); //общее освещения для всего пространства
glEnable(GL_COLOR_MATERIAL);
glShadeModel(GL_SMOOTH);
glPushMatrix();
theta += 1;
GLfloat light_position[] = { -3.0f ,0.0f , -1.0f , 1.0f }; //позиция источника
GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //параметры
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //параметры
GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //параметры
glRotatef(theta,0,0,1);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
glEnable(GL_LIGHT0);
glPopMatrix();
}
void Init_Material()
{
glEnable(GL_COLOR_MATERIAL); //разрешения использования
//материала
 glShadeModel(GL_SMOOTH); // сглаживает границы
 GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
 GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
 GLfloat material_shininess[] = { 50.0f }; //блеск материала
 glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
 glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
 glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}



void Draw_Cube(){//рисование куба
GLfloat vertices[] = {
 -0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, 0.5f, -0.5f,
 -0.5f, 0.5f, -0.5f,
 -0.5f, -0.5f, 0.5f,
 0.5f, -0.5f, 0.5f,
 0.5f, 0.5f, 0.5f,
 -0.5f, 0.5f, 0.5f
};
GLuint indices[] = {
 0, 1, 2,
 2, 3, 0,
 1, 5, 6,
 6, 2, 1,
 7, 6, 5,
 5, 4, 7,
 4, 0, 3,
 3, 7, 4,
 4, 5, 1,
 1, 0, 4,
 3, 2, 6,
 6, 7, 3
};
GLfloat normals[] = {
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f
};
glColor3d(1,0.5,0);
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, vertices);
glEnableClientState(GL_NORMAL_ARRAY);
glNormalPointer(GL_FLOAT, 0, normals);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);
}


void MovePlayer(){// использование камеры
    Camera_MoveDirectional(GetKeyState('W')<0 ? 1 : (GetKeyState('S')< 0 ? -1 : 0),
                           GetKeyState('D')<0 ? 1 : (GetKeyState('A')< 0 ? -1 : 0),
                           0.1);
    Camer_AutoMoveByMouse(400,400,0.5);
}

void ShowWorld() // Отображение шахматной доски
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,&normal_vert);
    glEnable(GL_NORMALIZE);
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        for (int i = -n/2; i < n/2; i++)
            for (int j = -n/2; j < n/2; j++)
            {
                glPushMatrix();
                    if ((i + j) % 2 == 0)
                        glColor3f(1, 1, 1);
                    else
                        glColor3f(0.15, 0.07, 0.0);

                    glTranslatef(i * 2, j * 2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
                glLineWidth(3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_NORMAL_ARRAY);
    glBegin(GL_LINES);
        glColor3d(1,0,0);
        glVertex3f(-1000,0,0);
        glVertex3f(1000,0,0);
        glColor3d(0,1,0);
        glVertex3f(0,-1000,0);
        glVertex3f(0,1000,0);
        glColor3d(0,0,1);
        glVertex3f(0,0,-1000);
        glVertex3f(0,0,1000);
    glEnd( );
}
void windResize(HWND hwnd){//изменение разрешения окна
    RECT rct;
    GetClientRect(hwnd, &rct);
    int width = rct.right - rct.left;
    int height =rct.bottom - rct.top;
    glViewport(0, 0,width , height);
}
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

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
                          1000,
                          1000,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    glFrustum(-1,1, -1,1, 2,1000);//выставляем камеру
    glEnable(GL_DEPTH_TEST);//включаем карту глубины

    Init_Material();
    ShowCursor(false);//убираем курсор

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
           glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPushMatrix();//основные действия
                Init_Light();//создаем свет и материал
                Camera_Apply();
                MovePlayer();
                windResize(hwnd);
                ShowWorld();
                Draw_Cube();
            glPopMatrix();
            SwapBuffers(hDC);
            Sleep (1);
        }
        if(GetKeyState(VK_ESCAPE)<0){//при нажатии esc программа закрывается
            bQuit = TRUE;
        }
    }

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
        case WM_DESTROY:
            return 0;
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


