#include <GL/glut.h>
#include <stdlib.h>
#include "SOIL_Lib/SOIL.h"
#include <stdio.h>
#include<iostream>
#include<windows.h>
#include<bits/stdc++.h>
#include<mmsystem.h>
//#include<WIN.LIB>
using namespace std;

GLuint Textures[100];

int time_of_game=0;
float speed_multi=0.0002;

float game_speed = 5;
int retry_check = 0;
int score = 0;
int best = 0;

float x_of_ball=0;

float ball_size_x = 0.25;
float ball_size_y = 0.25;
float ball_position_x = 0;
float ball_position_y = 2;
float ball_center_x = ball_size_x/2;
float ball_center_y = ball_size_y/2;

float background_size_x = 5;
float background_size_y = 5;
float background_center_x = background_size_x/2;
float background_center_y = background_size_y/2;

float background_position_x = 0;
float background_position_y = 0;

float background_texture_u = 1;
float background_texture_v = 5;

int number_of_balls = 0,number_of_spike=0;
float bar_x[100];
float bar_y[100];
float bar_texture_id[100];

float spike_x[5];
float spike_y[5];
float spike_texture_id[5];

float bar_size_x = 0.6;
float bar_size_y = 0.2;
float bar_center_x = bar_size_x/2;
float bar_center_y = bar_size_y/2;

int time_to_come = 0,time_to_come_spike=1000;
float fixed_x[] = {-2,-1.5,-1,0,1,1.5,2};

bool game_running = true;

void update_ball_up()
{
    if (game_running == false)
        return;

    else
    {
         ball_position_y = ball_position_y + 0.0016 * game_speed;
    }
}

void update_ball()
{
    if (game_running == false)
        return;

    else
    {
         ball_position_y = ball_position_y - 0.0002 * game_speed;
    }
}

void Is_Collision()
{
    for (int i = 0; i < number_of_balls; i++)
    {
        float x1 = bar_x[i] - bar_size_x/2;
        float y1 = bar_y[i] - bar_size_y/2;
        float x2 = bar_x[i] + bar_size_x/2;
        float y2 = bar_y[i] + bar_size_y/2;

        float x3 = ball_position_x - ball_size_x/2;
        float y3 = ball_position_y - ball_size_y/2;
        float x4 = ball_position_x + ball_size_x/2;
        float y4 = ball_position_y + ball_size_y/2;

        if (x2 >= x3 && x1 <= x4)
        {
            if (y2 >= y3 && y1 <= y4)
            {
                //return false;
                update_ball_up();
            }
        }
    }
    update_ball();
    //return false;
}


bool Is_spike_Collision()
{
    for (int i = 0; i < number_of_spike; i++)
    {
        float x1 = spike_x[i] - bar_size_x/2;
        float y1 = spike_y[i] - bar_size_y/2;
        float x2 = spike_x[i] + bar_size_x/2;
        float y2 = spike_y[i] + bar_size_y/2;

        float x3 = ball_position_x - ball_size_x/2;
        float y3 = ball_position_y - ball_size_y/2;
        float x4 = ball_position_x + ball_size_x/2;
        float y4 = ball_position_y + ball_size_y/2;

        if (x2 >= x3 && x1 <= x4)
        {
            if (y2 >= y3 && y1 <= y4)
            {
                return true;
            }
        }
    }
    return false;
}


void Add_bar(float starting_x, float starting_y, int tex_id)
{
    if (number_of_balls < 100)
    {
        bar_x[number_of_balls] = starting_x;
        bar_y[number_of_balls] = starting_y;
        bar_texture_id[number_of_balls] = tex_id;
        number_of_balls++;
    }
}


void Update_bars()
{
    if (game_running == false)
        return;

    if (number_of_balls > 0)
    {
        for (int i = number_of_balls - 1; i >= 0; i--)
        {
            bar_y[i] = bar_y[i] + speed_multi * game_speed;
            if (bar_y[i] > 3)
            {
                for (int j = i+1; j < number_of_balls; j++)
                {
                    bar_x[j-1] = bar_x[j];
                    bar_y[j-1] = bar_y[j];
                    bar_texture_id[j-1] = bar_texture_id[j];
                }
                number_of_balls--;
                score++;
            }
        }
    }
}

void Add_Spike(float starting_x, float starting_y, int tex_id)
{
    if (number_of_spike < 5)
    {
        spike_x[number_of_spike] = starting_x;
        spike_y[number_of_spike] = starting_y;
        spike_texture_id[number_of_spike] = tex_id;
        number_of_spike++;
    }
}


void Update_Spike()
{
    if (game_running == false)
        return;

    if (number_of_spike > 0)
    {
        for (int i = number_of_spike - 1; i >= 0; i--)
        {
            spike_y[i] = spike_y[i] + speed_multi * game_speed;
            if (spike_y[i] > 3)
            {
                for (int j = i+1; j < number_of_spike; j++)
                {
                    spike_x[j-1] = spike_x[j];
                    spike_y[j-1] = spike_y[j];
                    spike_texture_id[j-1] = spike_texture_id[j];
                }
                number_of_spike--;
                score+=2;
            }
        }
    }
}





static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluPerspective(60,ar,0.001,1000); //viewing angle,aspect ratio,near,far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5, 0, 0 , 0, 0, 1, 0); //eye, center , up
}

int rand_range(int min_n, int max_n)
{
    return min_n + ( rand() % ( max_n - min_n + 1 ) );
}

void game_over_function(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[3]);
        glPushMatrix();
        glTranslatef(-1, .5, 0 );

        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(0,0);
        glTexCoord2f(1,0);
        glVertex2f(2,0);
        glTexCoord2f(1,1);
        glVertex2f(2,1);
        glTexCoord2f(0,1);
        glVertex2f(0,1);

        glEnd();
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void retry(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[4]);
        glPushMatrix();
        glTranslatef(-1, -.5, 0 );

        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2f(0,0);
        glTexCoord2f(1,0);
        glVertex2f(2,0);
        glTexCoord2f(1,1);
        glVertex2f(2,1);
        glTexCoord2f(0,1);
        glVertex2f(0,1);

        glEnd();
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void showScore(string str,float x,float y,float z)
{
    glRasterPos3f(x,y,z);
    //glColor3f(1.0,1.0,1.0);
    string s = str;
    void * font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
}

void print_score(){
    retry_check  = 0;
    glPushMatrix();
        glColor3f(1.0f,0.0,1.0);
    glPopMatrix();
}


static void display(void)
{

    time_of_game++;
    if(time_of_game == 1000*30)
        speed_multi*=1.5;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///////////////////////////////////////////////////////////////////////////////////////
    //background
    ///////////////////////////////////////////////////////////////////////////////////////
    PlaySound("E:\\Graphics\\New folder\\Rapid_Roll\\Game\\Alarm03.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

    if (game_running == true)
        background_position_y = background_position_y - 0.0005 * game_speed;

    if (background_position_y < -3)
        background_position_y = 3;
    /*
    //background Quad 1
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[1]);

    glPushMatrix();
    glTranslatef(background_position_x - background_center_x, background_position_y - background_center_y, 0 );
    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(background_texture_u,0);
    glVertex2f(background_size_x,0);

    glTexCoord2f(background_texture_u,background_texture_v);
    glVertex2f(background_size_x,background_size_y);

    glTexCoord2f(0,background_texture_v);
    glVertex2f(0,background_size_y);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //background Quad 2
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[1]);
    glPushMatrix();
    glTranslatef(background_position_x - background_center_x, background_position_y - background_center_y + background_size_y, 0 );
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(background_texture_u,0);
    glVertex2f(background_size_x,0);
    glTexCoord2f(background_texture_u,background_texture_v);
    glVertex2f(background_size_x,background_size_y);
    glTexCoord2f(0,background_texture_v);
    glVertex2f(0,background_size_y);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //background Quad 3
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[1]);
    glPushMatrix();
    glTranslatef(background_position_x - background_center_x, background_position_y - background_center_y - background_size_y, 0 );
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(background_texture_u,0);
    glVertex2f(background_size_x,0);

    glTexCoord2f(background_texture_u,background_texture_v);
    glVertex2f(background_size_x,background_size_y);

    glTexCoord2f(0,background_texture_v);
    glVertex2f(0,background_size_y);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


    */
    //update_ball();

    ///////////////////////////////////////////////////////////////////////////////////////
    //bars
    ///////////////////////////////////////////////////////////////////////////////////////

   // Beep(2750,400);

   //PlaySound()
    if (game_running == true) time_to_come = time_to_come - 1;
    if (time_to_come < 0)
    {
        time_to_come = 500 + rand_range(0,1000);
        Add_bar(fixed_x[rand_range(0,6)],-3,Textures[2]);
    }

    Update_bars();

    for (int i = number_of_balls - 1; i >= 0; i--) {
    //bar ball quad creation
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bar_texture_id[i]);

    glPushMatrix();
    glTranslatef(bar_x[i] - bar_center_x, bar_y[i] - bar_center_y, 0 );

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(bar_size_x,0);
    glTexCoord2f(1,1);
    glVertex2f(bar_size_x,bar_size_y);
    glTexCoord2f(0,1);
    glVertex2f(0,bar_size_y);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //Spike
    ///////////////////////////////////////////////////////////////////////////////////////

    if (game_running == true) time_to_come_spike = time_to_come_spike - 1;
    if (time_to_come_spike < 0)
    {
        time_to_come_spike= 1500 + rand_range(0,1000);
        Add_Spike(fixed_x[rand_range(0,6)],-3,Textures[5]);
    }

    Update_Spike();

    for (int i = number_of_spike - 1; i >= 0; i--) {
    //bar ball quad creation
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spike_texture_id[i]);

    glPushMatrix();
    glTranslatef(spike_x[i] - bar_center_x, spike_y[i] - bar_center_y, 0 );

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(bar_size_x,0);
    glTexCoord2f(1,1);
    glVertex2f(bar_size_x,bar_size_y);
    glTexCoord2f(0,1);
    glVertex2f(0,bar_size_y);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    }





    ///////////////////////////////////////////////////////////////////////////////////////
    //ball
    ///////////////////////////////////////////////////////////////////////////////////////

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures[0]);

    glPushMatrix();
    glTranslatef(ball_position_x-ball_center_x, ball_position_y-ball_center_y, 0 );

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex2f(0,0);
    glTexCoord2f(1,0);
    glVertex2f(ball_size_x,0);
    glTexCoord2f(1,1);
    glVertex2f(ball_size_x,ball_size_y);
    glTexCoord2f(0,1);
    glVertex2f(0,ball_size_y);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    Is_Collision();

    //////////////////////////////////////////////////////////////////////////////////
    //Spike Collision
    //////////////////////////////////////////////////////////////////////////////////
    if(Is_spike_Collision())
    {
        game_running = false;
        system("CLS");
        printf("Collsion\n");
        print_score();
        //system("pause");
        if(score>=best)
                best = score;
        game_over_function();
        retry();
    }



    //////////////////////////////////////////////////////////////////////////////////
    //Collision
    //////////////////////////////////////////////////////////////////////////////////
    if (ball_position_y > 3 || ball_position_y <-3){
        game_running = false;
        system("CLS");
        printf("Collsion\n");
        print_score();
        //system("pause");
        if(score>=best)
                best = score;
        game_over_function();
        retry();
    }
    if(retry_check ==  1)
            glColor3f(1.0,1.0,1.0);

    stringstream ss,ss1;
    ss<<score;
    string str = ss.str();
    showScore("Score is : "+str,.5,1.07,3);
    ss1<<best;
    string st1 = ss1.str();
    showScore("Best : "+st1,.5,1,3);

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'r':
            game_running = true;
            ball_position_x = 0;
            ball_position_y = -2;
            number_of_balls = 0;
            time_to_come = 3000;
            retry_check = 1;
            score = 0;
            break;
        case 'a':
            if (game_running == true){
                ball_position_x = ball_position_x - 0.1;
            }
            if (ball_position_x < -2.0){
                ball_position_x = -2.0;
            }
            break;
        case 'd':
             if (game_running == true){
                ball_position_x = ball_position_x + 0.1;
             }
            if (ball_position_x > 2.0){
                ball_position_x = 2.0;
            }
            break;
        case 'l':
             if (game_running == true)
                ball_position_y = ball_position_y - 0.1;
            if (ball_position_y < -2)
                ball_position_y = -2;
            break;
        case 'j':
             if (game_running == true)
                ball_position_y = ball_position_y + 0.1;
            if (ball_position_y > 0)
                ball_position_y = 0;
            break;
    }
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(600,800);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Rapid Roll Game");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_NORMALIZE);

    glGenTextures(100,Textures);

    GLuint tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\3dshapes.png",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[0] = tex_2d;

    tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\background.jpg",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[1] = tex_2d;

    tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\rapid_back.png",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[2] = tex_2d;

	tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\game_over.png",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[3] = tex_2d;

	tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\retry.png",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[4] = tex_2d;

	tex_2d = SOIL_load_OGL_texture
	(
		"E:\\Graphics\\New folder\\Rapid_Roll\\Game\\top-rail-spike2.png",SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	Textures[5] = tex_2d;

    ///////////////////////////////////////////////////////////
    //background Texture Parameter
    ///////////////////////////////////////////////////////////
    glBindTexture(GL_TEXTURE_2D, Textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, Textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, Textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, Textures[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, Textures[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    game_running = true;

    glutMainLoop();

    return EXIT_SUCCESS;
}
