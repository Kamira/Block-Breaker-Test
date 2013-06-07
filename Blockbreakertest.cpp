/*
-lmingw32 -lSDLmain -lSDL -lopengl32 -lglu32
*/

#include "SDL.h"
#include "SDL_opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

bool collisionCC(float A_Center_X, float A_Center_Y, float A_Width, float A_Height,
                 float B_Center_X, float B_Center_Y, float B_Width, float B_Height){
    if      ( A_Center_Y + A_Height < B_Center_Y - B_Height ) return false;
    else if ( A_Center_Y - A_Height > B_Center_Y + B_Height ) return false;
    else if ( A_Center_X + A_Width  < B_Center_X - B_Width  ) return false;
    else if ( A_Center_X - A_Width  > B_Center_X + B_Width  ) return false;
    return true;
}

bool collisionCXY(float A_Center_X, float A_Center_Y, float A_Width, float A_Height,
                  float B_LeftTop_X, float B_LeftTop_Y, float B_Width, float B_Height){
    if      ( A_Center_Y + A_Height < B_LeftTop_Y ) return false;
    else if ( A_Center_Y - A_Height > B_LeftTop_Y + B_Height ) return false;
    else if ( A_Center_X + A_Width  < B_LeftTop_X ) return false;
    else if ( A_Center_X - A_Width  > B_LeftTop_X + B_Width  ) return false;
    return true;
}

struct Brick{
    float x;
    float y;
    bool isBreak;
};

int main(int argc, char* args[]){

    int Window_X = 600,
        Window_Y = 400;

    //Window Init
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
    SDL_WM_SetCaption("Test Title", NULL);                //Windows Title
    SDL_SetVideoMode(Window_X,Window_Y,32,SDL_OPENGL);    //Windows Size
    glClearColor(1,1,1,1);
    glViewport(0,0,Window_X,Window_Y);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    //for running
    bool isRunning = true,GameStart = false;
    SDL_Event event;


    //Box
    float Box_Center_X      = (float)Window_X / 2,
          Box_Center_Y      = (float)Window_Y - 10,
          Box_Width         = 40,
          Box_Height        = 5,
    //Ball
          Ball_Width         = 10,
          Ball_Height        = 10,
          Ball_Center_X      = (float)Window_X / 2,
          Ball_Center_Y      = Box_Center_Y - Box_Height - Ball_Height -1,
    //Speed
          Vel_X = 0,
          Vel_Y = 0;
    //Box Situation
    bool left  = false,
         right = false;
    //Brick
    int Brick_Number_X  = 100,
        Brick_Number_Y  = 50;
    float Brick_Width   = (float)Window_X / Brick_Number_X,
          Brick_Height  = 5;
    //Brick elements
    const static int BRICKS = Brick_Number_X * Brick_Number_Y;
    Brick bricks[BRICKS];
    float x = 0, y = 10;
    //Brick Init
    for ( int n = 0; n < BRICKS; n++, x+=Brick_Width ){
      if ( x+Brick_Width > Window_X )
        {
          x = 0;
          y += Brick_Height;
        }
      bricks[n].x = x;
      bricks[n].y = y;
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
                    else if (event.key.keysym.sym == SDLK_SPACE){
                        if(!GameStart){
                            GameStart = true;
                            Vel_X = 0.8;
                            Vel_Y = -0.8;
                        }
                    }
                }
        }

        //AutoPlay
        Box_Center_X = Ball_Center_X;
        //LOGIC
        if (left){
            Box_Center_X -= 5;
            if(!GameStart){
               Ball_Center_X -= 5;
            }
        }
        else if (right){
            Box_Center_X += 5;
            if(!GameStart){
               Ball_Center_X += 5;
            }
        }
        //*************************************
        if (Box_Center_X - Box_Width < 0){
            Box_Center_X = Box_Width;
            if(!GameStart){
                Ball_Center_X = Box_Center_X;
            }
        }else if (Box_Center_X + Box_Width > Window_X){
            Box_Center_X = Window_X - Box_Width;
            if(!GameStart){
                Ball_Center_X = Box_Center_X;
            }
        }
        //**************************************
        if ((Ball_Center_X - Ball_Width < 0)||(Ball_Center_X + Ball_Width > Window_X)){
            Vel_X = -Vel_X;
        }
        if(Ball_Center_Y - Ball_Height < 0){
            Vel_Y = -Vel_Y;
        }else if (Ball_Center_Y + Ball_Height > Window_Y){
            GameStart = false;
            Vel_X = 0;
            Vel_Y = 0;
            Box_Center_X = (float)Window_X /2;
            Box_Center_Y = (float)Window_Y -10;
            Ball_Height = 10;
            Ball_Width  = 10;
            Ball_Center_X = (float)Window_X/2;
            Ball_Center_Y = Box_Center_Y - Ball_Height - Box_Height -1;
            for(int n=0;n < BRICKS;n++){bricks[n].isBreak=false;}
        }
        //*****************************************
        if( collisionCC(Ball_Center_X,Ball_Center_Y,Ball_Width,Ball_Height,
                        Box_Center_X,Box_Center_Y,Box_Width,Box_Height) ){
            Vel_Y = -Vel_Y;
            if (!(fabs(Box_Center_X-Ball_Center_X)<2)){
            Vel_X = fabs(Box_Center_X-Ball_Center_X) / Box_Width * 5;}
            if(Box_Center_X-Ball_Center_X > 0){
                Vel_X = -Vel_X;
            }

            Ball_Height = Ball_Height * 0.9999;
            Ball_Width  = Ball_Width * 0.9999;
            Vel_Y = Vel_Y * 1.0001;
        }
        Ball_Center_Y += Vel_Y;
        //*****************************************
        for(int n=0; n < BRICKS; n++ ){
            if (collisionCXY(Ball_Center_X,Ball_Center_Y,Ball_Width,Ball_Height,
                             bricks[n].x,bricks[n].y,Brick_Width,Brick_Height)){
                if(!bricks[n].isBreak){bricks[n].isBreak=true;

                    float y1=bricks[n].y, y2=bricks[n].y+Brick_Height;
                    if(Ball_Center_Y+Ball_Height >= y1 ){
                        Vel_Y = fabs(Vel_Y);
                    }else if (Ball_Center_Y-Ball_Height <= y2){
                        Vel_Y = -fabs(Vel_Y);
                    }


                }
            }
        }

        Ball_Center_X += Vel_X;
        //*****************************************
        for(int n=0; n < BRICKS; n++ ){
            if (collisionCXY(Ball_Center_X,Ball_Center_Y,Ball_Width,Ball_Height,
                             bricks[n].x,bricks[n].y,Brick_Width,Brick_Height)){
                if(!bricks[n].isBreak){bricks[n].isBreak=true;

                    float x1=bricks[n].x, x2=bricks[n].x+Brick_Width;

                    if(Ball_Center_X+Ball_Width >= x1){
                        Vel_X = -fabs(Vel_X);
                    }else if (Ball_Center_X-Ball_Width <= x2){
                        Vel_X = fabs(Vel_X);
                    }
                }
            }
        }

        //Screen Render
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();

        glOrtho(0,Window_X,Window_Y,0,-1,1);

        glColor4ub(0,0,0,255);

        glBegin(GL_QUADS);
        glVertex2f(Box_Center_X-Box_Width,Box_Center_Y-Box_Height);
        glVertex2f(Box_Center_X+Box_Width,Box_Center_Y-Box_Height);
        glVertex2f(Box_Center_X+Box_Width,Box_Center_Y+Box_Height);
        glVertex2f(Box_Center_X-Box_Width,Box_Center_Y+Box_Height);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(Ball_Center_X-Ball_Width,Ball_Center_Y-Ball_Height);
        glVertex2f(Ball_Center_X+Ball_Width,Ball_Center_Y-Ball_Height);
        glVertex2f(Ball_Center_X+Ball_Width,Ball_Center_Y+Ball_Height);
        glVertex2f(Ball_Center_X-Ball_Width,Ball_Center_Y+Ball_Height);
        glEnd();

        glBegin(GL_QUADS); //Render the bricks
        for ( int n = 0; n < BRICKS; n++ )
        {
            if(!bricks[n].isBreak){
            glVertex2f(bricks[n].x,bricks[n].y);
            glVertex2f(bricks[n].x+Brick_Width,bricks[n].y);
            glVertex2f(bricks[n].x+Brick_Width,bricks[n].y+Brick_Height);
            glVertex2f(bricks[n].x,bricks[n].y+Brick_Height);
          }
        }
        glEnd();

        glPopMatrix();

        SDL_GL_SwapBuffers();

    }


    SDL_Quit();

    return 0;
}
