#include "tExtur.h"
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include "mEnu.h"
#include "tExtur.h"

void animation_1();
class Character {
public:
    int counter = 1;
    int frame = 1;
    bool jumping = false;
    bool on_ground = true;
    int position = 1;
    int y_speed = 25;
    int gravity = 12;
    int x = 0;
    int y = 0;
    int x_speed = 6;
    void update_frame(){
        if(frame == 7 ){
            frame = 1;
        }
        else{
            frame++;
        }
    }
    void move_ch(){
        this->y_speed = 0;
        animation = 0;
        if (counter == 3){
            this -> update_frame();
            counter = 1;
        }
        else{
            counter+=1;
        }
        if (GetKeyState('A')<0 ){
            animation = 2;
            this -> x -= this -> x_speed;
            this -> position = -1;
        }
        if (GetKeyState('D')<0){
            animation = 2;
            this -> x += this -> x_speed;
            this -> position = 1;
        }
        if (GetKeyState(VK_SHIFT)<0 && GetKeyState('A')<0){
            animation = 2;
            this -> x -= this -> x_speed*0.5;
            this -> position = -1;
        }
        if (GetKeyState(VK_SHIFT)<0 && GetKeyState('D')<0){
            animation = 2;
            this -> x += this -> x_speed*0.5;
            this -> position = 1;
        }
        if(GetKeyState('W')<0){
            animation = 3;
            this->y_speed = 25;
        }
        HandleJump();

    }
    void HandleJump() {
        if(!this->jumping){
            this->y += this->y_speed;
        }
        if(this->y > 280){
            this->jumping = true;
        }
        if(this->y >= 10){
            this->y -= this->gravity;
        }
        else{
            this->jumping = false;
        }
    }
};
class Grass{
public:
    float x;
    float y;
    Grass(float x, float y){
        this->x = x;
        this->y = y;
    }
};
extern Character* fox;
#endif
