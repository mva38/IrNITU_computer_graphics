#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "../stb-master/stb_image.h"
#include "main.h"
#include "mEnu.h"
#include <algorithm>


Grass* grass_tile;

using namespace std;
int cnt = 0;
int cnt_coin = 0;
int TileMap[6][22] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };


void Load_Texture( char *filename, GLuint *textureID, int swarp, int twarp, int filter)
{
    int twidth, thight, tcnt;
    unsigned char *data=stbi_load(filename,&twidth,&thight,&tcnt,4);
    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filter);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filter);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,twidth,thight,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

}

void Show_Background(GLuint texture)
{
    static float svertix[]= {-300,-100,  -300,980,  2100,980,  2100,-100};
    static float TexCord[]= {0,1, 0,0, 1,0, 1,1 };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}
void draw(int x, int y, int frame, GLuint ch,int position) {
    glBindTexture(GL_TEXTURE_2D, ch);
    glPushMatrix();
    const float frameWidth = 1.f / 14;
	const float frameHeight = 1.f / 7;
        glBegin(GL_QUADS);
            glTexCoord2d(frameWidth*frame, frameHeight*(animation+1));
            glVertex2f(x-20, y-2);

            glTexCoord2d(frameWidth*(frame+position), frameHeight*(animation+1));
            glVertex2f(x+70+20, y-2);

            glTexCoord2d(frameWidth*(frame+position), frameHeight*animation);
            glVertex2f(x+70+20, y+110+30);

            glTexCoord2d(frameWidth*frame, frameHeight*animation);
            glVertex2f(x-20, y+110+30);
    glEnd();
    glPopMatrix();

}



void firstdrawTiles (GLuint texture_grass) {
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (TileMap[i][j] == 1)
            {
                glBindTexture( GL_TEXTURE_2D, texture_grass );
            }
            else
            {
                continue;
            }
            Grass* temp = (Grass*)realloc(grass_tile, (cnt+1)*sizeof(Grass));
            if (temp == nullptr) {
            } else {
                grass_tile = temp;
                grass_tile[cnt] = Grass((j-1)*80,i*80-80);
                cnt++;
            }
            glPushMatrix();
                glTranslatef(j, -i, 0);

                glBegin (GL_QUADS);
                    glTexCoord2d(0.0, 0.0);
                    glVertex2f((j-1)*80, i*80-80);

                    glTexCoord2d(1.0, 0.0);
                    glVertex2f(j*80, i*80-80);

                    glTexCoord2d(1.0, 1.0);
                    glVertex2f(j*80, i*80);

                    glTexCoord2d(0.0, 1.0);
                    glVertex2f((j-1)*80, i*80);
                glEnd();
            glPopMatrix();
        }
    }
}


void drawTiles (GLuint texture_box) {
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (TileMap[i][j] == 1)
            {
                glBindTexture( GL_TEXTURE_2D, texture_box );
            }
            else //otherwise
            {
                continue;
            }
            glPushMatrix();
                glTranslatef(j, -i, 0);

                glBegin (GL_QUADS);
                    glTexCoord2d(0.0, 1.0);
                    glVertex2f((j-1)*80, i*80-80);

                    glTexCoord2d(1.0, 1.0);
                    glVertex2f(j*80, i*80-80);

                    glTexCoord2d(1.0, 0.0);
                    glVertex2f(j*80, i*80);

                    glTexCoord2d(0.0, 0.0);
                    glVertex2f((j-1)*80, i*80);
                glEnd();
            glPopMatrix();
        }
    }
}
void HandleCollisions(int x,int y,bool jumping) {
    for (int i = 0;i<cnt;i++) {
        if (y + 110 <= grass_tile[i].y) {
            continue;
        }
        if (x + 70 <= grass_tile[i].x || x >= grass_tile[i].x + 80) {
            continue;
        }
        float overlapX = std::min(x + 70, (int)grass_tile[i].x + 80) - std::max(x, (int)grass_tile[i].x);
        float overlapY = std::min(y + 110, (int)grass_tile[i].y + 80) - std::max(y, (int)grass_tile[i].y);
        if (overlapX > 0 && overlapY > 0) {
            if (overlapX > overlapY) {
                if (y < grass_tile[i].y) {
                    fox->y = grass_tile[i].y - 110;
                    fox->jumping = false;
                } else {
                    fox->y = grass_tile[i].y + 80;
                    fox->jumping = false;
                }
            } else {
                if (x < grass_tile[i].x) {
                    fox->x = grass_tile[i].x - 70;
                } else {
                    fox->x = grass_tile[i].x + 80;
                }
            }
        }
    }
}

