#include <gl/gl.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "camera.h"

float vert[] = {1, 1, 2,  1, -1, 2,  -1, -1, 2,  -1, 1, 2};// массив точек дл€ отображени€ пола
float normal_vert[]={1, 1, 2,  1, -1, 2,  -1, -1, 2,  -1, 1, 2}; // массив точек нормалей
int n = 20;// кол-во тайлов на карте
float theta = 0;
using namespace std;

void Init_Light()
{
    glEnable(GL_LIGHTING); //общее освещени€ дл€ всего пространства
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glPushMatrix();
        theta += 1;
        GLfloat light_position[] = {  -3.0f ,0.0f , -1.0f , 1.0f }; //позици€ источника
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
glEnable(GL_COLOR_MATERIAL); //разрешени€ использовани€
//материала
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glShadeModel(GL_SMOOTH); // сглаживает границы
 GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
 GLfloat material_shininess[] = { 50.0f }; //блеск материала
 glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
 glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void drawPrism(float radius1,float radius2, float height, int sides, float xPos, float yPos, float zPos, float transparency) {
    float angle = (2 * M_PI) / sides;  // ”гол между соседними вершинами
    float halfHeight = height / 2.0f;  // ѕоловина высоты призм
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    glBegin(GL_TRIANGLES);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    for (int i = 0; i < sides; ++i) {
        GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, transparency };
        GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, transparency };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glColor4f(0, 0.5, 0.8, transparency);
        // ¬ершины нижнего сечени€
        float x1 = radius1 * cos(i * angle);
        float y1 = radius1 * sin(i * angle);
        float z1 = -halfHeight;

        float x2 = radius1 * cos((i + 1) * angle);
        float y2 = radius1 * sin((i + 1) * angle);
        float z2 = -halfHeight;

        // ¬ершины верхнего сечени€
        float x3 = radius2 * cos(i * angle);
        float y3 = radius2 * sin(i * angle);
        float z3 = halfHeight;

        float x4 = radius2 * cos((i + 1) * angle);
        float y4 = radius2 * sin((i + 1) * angle);
        float z4 = halfHeight;

        // Ѕокова€ грань
        glNormal3f(x1, y1, 0.0f);
        glVertex3f(x1, y1, z1);
        glNormal3f(x3, y3, 0.0f);
        glVertex3f(x3, y3, z3 + height);
        glNormal3f(x4, y4, 0.0f);
        glVertex3f(x4, y4, z4 + height);

        glNormal3f(x1, y1, 0.0f);
        glVertex3f(x1, y1, z1);
        glNormal3f(x2, y2, 0.0f);
        glVertex3f(x4, y4, z4 + height);
        glNormal3f(x2, y2, 0.0f);
        glVertex3f(x2, y2, z2);

        //  рыша
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, z3 + height);
        glVertex3f(x3, y3, z3 + height);
        glVertex3f(x4, y4, z4 + height);

        // ƒно
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 0.0f, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);

    }
    glEnd();

    glPopMatrix();
}
void MovePlayer(){// использование камеры
    Camera_MoveDirectional(GetKeyState('W')<0 ? 1 : (GetKeyState('S')< 0 ? -1 : 0),
                           GetKeyState('D')<0 ? 1 : (GetKeyState('A')< 0 ? -1 : 0),
                           0.1);
    Camer_AutoMoveByMouse(400,400,0.5);
}

void ShowWorld() // ќтображение шахматной доски
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,&normal_vert);
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
void windResize(HWND hwnd){//изменение разрешени€ окна
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
    glFrustum(-1,1, -1,1, 2,1000);//выставл€ем камеру
    glEnable(GL_DEPTH_TEST);//включаем карту глубины
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);
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
            glPushMatrix();//основные действи€
                Init_Light();//создаем свет и материал
                Camera_Apply();
                MovePlayer();
                windResize(hwnd);
                ShowWorld();
                drawPrism(0.5,1,0.5,13,0,0,-2,1);
                drawPrism(0.5,1,0.5,13,3,0,-2,0.8);
                drawPrism(0.5,1,0.5,13,6,0,-2,0.6);
                drawPrism(0.5,1,0.5,13,9,0,-2,0.4);
                drawPrism(0.5,1,0.5,13,12,0,-2,0.2);
                drawPrism(0.5,1,0.5,13,15,0,-2,0.1);
            glPopMatrix();
            SwapBuffers(hDC);
            Sleep (1);
        }
        if(GetKeyState(VK_ESCAPE)<0){//при нажатии esc программа закрываетс€
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


