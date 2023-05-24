#ifndef TEXTUR_H_INCLUDED
#define TEXTUR_H_INCLUDED
#include "main.h"
void Load_Texture( char *filename, GLuint *textureID, int swarp, int twarp, int filter);
void Show_Background(GLuint texture);
void drawTiles(GLuint texture_box);
void firstdrawTiles(GLuint texture_box);
void draw(int x, int y, int frame,GLuint textureID,int position);
void HandleCollisions(int x,int y,bool jumping);
void firstDrawCoins (GLuint texture_coin);
void DrawCoins (GLuint texture_coin);

#endif // TEXTUR_H_INCLUDED
