/*
-lSDLmain -lSDL -lGL
*/

#include "SDL.h"
#include "SDL_opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool checkcollision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh){
    if( Ay+Ah < By ) return false;
    else if (Ay > By+Bh) return false;
    else if (Ax+Aw < Bx) return false;
    else if (Ax > Bx+Bw) return false;

    return true;
}

struct Brick{
    float x;
    float y;
    float width;
    float height;
    bool isBreak;
};

int main(int argc, char* args[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    SDL_WM_SetCaption("Test Window", NULL);

    SDL_SetVideoMode(600,400,32,SDL_OPENGL);

    glClearColor(1,1,1,1);

    glViewport(0,0,600,400);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);


    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    bool isRunning = true;
    SDL_Event event;


    float myX = 260;
    float myY = 385;
    float myW = 80;
    float myH = 10;
    bool left = false, right = false;


    //The Ball
    float ballX = 299;
    float ballY = 385;
    float ballWH = 5;
    //Speed
    float vellX = 0.1;
    float vellY = -0.1;

    //Brick elements
    const static int BRICKS = 1003;
    Brick bricks[BRICKS];

    for ( int n = 0, x = 5, y = 10; n < BRICKS; n++, x+=10 ){
      if ( x+10 > 600 )
        {
          x = 5;
          y += 10;
        }
      bricks[n].x = x;
      bricks[n].y = y;
      bricks[n].width = 10;
      bricks[n].height = 10;
      bricks[n].isBreak = false;
    }


    //Game loop
    while ( isRunning ){
        //EVENTS
        while ( SDL_PollEvent(&event) ){
                if(event.type == SDL_QUIT){
                    isRunning = false;
                }

                if( event.type == SDL_KEYDOWN){
                    if (event.key.keysym.sym == SDLK_LEFT){
                        left = true;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT){
                        right = true;
                    }
                }
                else if (event.type == SDL_KEYUP){
                    if (event.key.keysym.sym == SDLK_LEFT){
                        left = false;
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT){
                        right = false;
                    }
                }
        }
        //LOGIC
        if (left){
            myX -= 0.2;}
        else if (right){
            myX += 0.2;
        }

        if (myX < 0){
            myX = 0;
        }else if (myX+myW > 600){
            myX = 600-myW;
        }


        if((ballX < 0)||(ballX+ballWH > 600)){
            vellX = -vellX;
        }
        if(ballY < 0){
            vellY = -vellY;
        }else if (ballY+ballWH > 400){
            myX = 260;
            myY = 385;
            myW = 80;
            myH = 10;
            ballX = 385;
            ballY = 299;
            ballWH = 5;
            vellX = 0.1;
            vellY = -0.1;

            for ( int n = 0, x = 5, y = 10; n < BRICKS; n++, x+=10 ){bricks[n].isBreak = false;}
        }


        if( checkcollision(ballX,ballY,ballWH,ballWH,myX,myY,myW,myH) ){
            vellY = -vellY;
        }

        ballX += vellX;
        for(int n=0; n < BRICKS; n++ ){
            if (checkcollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height)){
                if(!bricks[n].isBreak){
                    bricks[n].isBreak=true;
                    vellX = -vellX;
                }

            }
        }
        ballY += vellY;
        for(int n=0; n < BRICKS; n++ ){
            if (checkcollision(ballX,ballY,ballWH,ballWH,bricks[n].x,bricks[n].y,bricks[n].width,bricks[n].height)){
                if(!bricks[n].isBreak){
                    bricks[n].isBreak=true;
                    vellY = -vellY;
                }

            }
        }


        //Screen Render
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();

        glOrtho(0,600,400,0,-1,1);

        glColor4ub(0,0,0,255);

        glBegin(GL_QUADS);

        glVertex2f(myX,myY);
        glVertex2f(myX+myW,myY);
        glVertex2f(myX+myW,myY+myH);
        glVertex2f(myX,myY+myH);

        glEnd();

        glBegin(GL_QUADS);

        glVertex2f(ballX,ballY);
        glVertex2f(ballX+ballWH,ballY);
        glVertex2f(ballX+ballWH,ballY+ballWH);
        glVertex2f(ballX,ballY+ballWH);

        glEnd();

        glBegin(GL_QUADS); //Render the bricks

        for ( int n = 0; n < BRICKS; n++ )
        {
            if(!bricks[n].isBreak){
            glVertex2f(bricks[n].x,bricks[n].y);
            glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y);
            glVertex2f(bricks[n].x+bricks[n].width,bricks[n].y+bricks[n].height);
            glVertex2f(bricks[n].x,bricks[n].y+bricks[n].height);
          }
        }
        glEnd();

        glPopMatrix();

        SDL_GL_SwapBuffers();

    }


    SDL_Quit();

    return 0;
}
