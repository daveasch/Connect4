/*
 * Copyright (c) 2009, 2014 University of Michigan, Ann Arbor.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of Michigan, Ann Arbor. The name of the University 
 * may not be used to endorse or promote products derived from this 
 * software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <string>
#include "graphics.h"
#include "connect4.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// window width and height
GLdouble width, height;

// mouse x and y
int mouseX, mouseY;

// GLUT window handle
int wd;

// Connect4 Board and Result
Board board;
Result r;

void init(void) {
    // initial window width and height,
    // within which we draw. (0,0) is the lower left corner
    width  = 600.0;
    height = 600.0;


    mouseX = 0;
    mouseY = 0;
    
    r = NoResult;

    return;
}

void drawCircle(int x, int y, int radius) {
    double theta = M_PI/20.0;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2i(x, y);
    for (double i = 0; i < 2*M_PI+theta; i += theta) {
        glVertex2i(x+radius*cos(i), y+radius*sin(i));
    }
    glEnd();
}

void display(void)
{
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);
    
    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.f, 1.f);
    
    
    int side_border = 25;
    int bottom_border = 50;
    int top_border = 75;
    // clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);
  
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2i(width, 0);
	glVertex2i(width, height);
	glVertex2i(0, height);
	glVertex2i(0, 0);
	glEnd();

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex2i(side_border,bottom_border);
    glVertex2i(width-side_border,bottom_border);
    glVertex2i(width-side_border,width-top_border);
    glVertex2i(side_border,width-top_border);
    glEnd();
    
    // Begin solution
    
    glColor3f(1.0, 1.0, 1.0);
	const int numCols = 7;
    int increment = 75;
    int radius = 25;
    for (int row = 1; row <= NUM_ROWS; ++row) {
        for (int col = 1; col <= NUM_COLS; ++col) {
            PieceType location = board.atLocation(row-1, col-1);
            if (location == Empty) {
                glColor3f(1.0, 1.0, 1.0);
            } else if (location == Player1) {
                glColor3f(0.0, 1.0, 1.0);
            } else {
                glColor3f(1.0, 0.0, 0.0);
            }
            drawCircle((col*increment), side_border+(row*increment), radius);
        }
    }
    glColor3f(1.0, 0.0, 0.0);
    for (int col = 1; col <= NUM_COLS; ++col) {
        glRasterPos2i(col*increment-5, 20);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, char('0'+col));
    }
    
    if(r == Win){
        std::string win_message;
        if(board.toMove() == 1) {
            win_message = "\"You underestimate my power!\" --- Anakin won!";
            glColor3f(1.0, 0.0, 0.0);
        }
        else {
			win_message = "\"It's over, Anakin; I have the high ground.\" --- Obi-Wan won!";
            glColor3f(0.0, 1.0, 1.0);
        }
        glRasterPos2i(side_border, height-side_border);
        for (int s = 0; s < win_message.length(); ++s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, win_message[s]);
        }
    } else if (r == Draw) {
        std::string draw = "Draw! Meesa getting very very scared!";
        glColor3f(1.0, 1.0, 0.0);
        glRasterPos2i(side_border, height-side_border);
        for (int s = 0; s < draw.length(); ++s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, draw[s]);
        }
    } else if (board.toMove() == 1) {
        glColor3f(0.0, 1.0, 1.0);
        glRasterPos2i(side_border, height-side_border);
        std::string message = "Obi-Wan, enter a move.";
        for (int s = 0; s < message.length(); ++s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[s]);
        }
        
    } else {
        glColor3f(1.0, 0.0, 0.0);
        glRasterPos2i(side_border, height-side_border);
        std::string message = "Ani, enter a move.";
        for (int s = 0; s < message.length(); ++s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[s]);
        }
    }
    
    if (r == IllegalMove) {
        std::string draw = "Illegal Move! I don't care which galaxy you're from, that's gotta hurt!";
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2i(side_border, height-bottom_border);
        for (int s = 0; s < draw.length(); ++s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, draw[s]);
        }
    }
    
    // End solution
    
    // force drawing to start
    glutSwapBuffers();

    return;
}

void reshape(int w, int h)
{
    glutPostRedisplay();
    
    return;
}

void refresh(void)
{
   glutPostRedisplay();

    return;
}

void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    if (key >= '1' && key <= '7' && r != Win && r != Draw) {
        r = board.makeMove(key-1-'0');
    }
    else if (r != Win && r != Draw) {
        r = IllegalMove;
    }
  
    glutPostRedisplay();

    return;
}

void cursor(int x, int y){
	// Account for reversal of y coordinates through new coord system
	mouseY = height - y;
	// print a token onto the cursor
	x = mouseX;
	y = mouseY;
	const int RADIUS = 25;
	drawCircle(x, y, RADIUS);
	// If clicked down, switch color????
    glutPostRedisplay();
    return;
}

void drag(int x, int y){
    glutPostRedisplay();
    return;
}

void mouse(int button, int state, int x, int y) {
	// Iterate through columns till column of click is found
	int minPixel = 37.5;
	int maxPixel = 112.5;
	int increment = 75;
	if (r != Win && r != Draw) {
		if (state == GLUT_DOWN) {
			if (y < 550) {
				for (int i = 0; i < NUM_COLS; i++) {
					if ((x >= minPixel) && (x <= maxPixel)) {
						r = board.makeMove(i);
						return;
					}
					minPixel += increment;
					maxPixel += increment;
				}
			}
			/*if (y > 550) {
				for (int i = 0; i < NUM_COLS; i++) {
					if ((x >= minPixel) && (x <= maxPixel)) {
						.// Dropping animation goes here;

			}
			r = IllegalMove;
			*/
			glutPostRedisplay();
			return;
		}
	}
}

int graphicsPlay(int argc, char *argv[])
{
    // perform initialization NOT OpenGL/GLUT dependent,
    // as we haven't created a GLUT window yet
    init();
    
    // initialize GLUT, let it extract command-line
    // GLUT options that you may provide
    // - NOTE THE '&' BEFORE argc
    glutInit(&argc, argv);
    
    // specify the display to be single
    // buffered and color as RGBA values
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    // set the initial window size
    glutInitWindowSize((int) width, (int) height);
  
    // create the window and store the handle to it
    wd = glutCreateWindow("Connect 4");
    
    // --- register callbacks with GLUT ---

    // register function that draws in the window
    glutDisplayFunc(display);

    // register function to handle window resizes
    glutReshapeFunc(reshape);

    // register the provided refresh()
    // function to call when system is idle
    glutIdleFunc(refresh);
    
    // register keyboard press event processing function
    glutKeyboardFunc(kbd);

    // register cursor moved event callback
    glutPassiveMotionFunc(cursor);

    // register mouse clicked event callback
    glutMouseFunc(mouse);

    // register mouse dragged event callback
    glutMotionFunc(drag);

    // Set up initial OpenGL context
    // clear color buffer to white
    glClearColor(1.0, 1.0, 1.0, 0.0);

    // start the GLUT main loop
    glutMainLoop();

    return 0;
}
