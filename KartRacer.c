/*
*	Project Name: Kart Racer
*	Group Members: Will Carhart, Alex Alvarez, Matt Saiki
*   Class: COMP 350-01
*	Professor: Dr. Pruski
*	Date: 5/23/16
*	Description: Simple 3D racing game (see README and Info Sheet)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define PI 3.1415f

//forward declare function-prototypes
void init(void);
void initializeRandos(void);
void display(void);
void startGame(void);
void speedometer(void);
void minimap(void);
void checkForLap(void);
void quadPoly(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3], float r, float g, float b);
void quadPolyAlpha(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3], float r, float g, float b, float a);
void quadPolyWithSize(GLfloat v[3], float rad, float r, float g, float b);
void triPoly(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], float r, float g, float b);
void cylinder(float radius, float height, int n, float r, float g, float b);
void square(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b);
void triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b);
void smallLines(float xCen, float yCen, float rad, float r, float g, float b);
void cube(float r, float g, float b);
void cubeAlpha(float r, float g, float b, float a);
void wedge(float r, float g, float b);
void wedgeR(float r, float g, float b, float r2, float g2, float b2);
void wedgeL(float r, float g, float b, float r2, float g2, float b2);
void arrow(void);
void car1(float r, float g, float b);
void car2(float r, float g, float b);
void car3(float r, float g, float b);
void track(void);
void gate(int start);
void circle(float xCen, float yCen, float rad, float r, float g, float b);
void hollowCircle(float xCen, float yCen, float rad, float r, float g, float b);
void drawGround(void);
void make(void);
void updateCamera(int i);
void pressKey(char key, int mouse_x, int mouse_y);
void keyOperations(void);
void releaseKey(char key, int mouse_x, int mouse_y);
void reshape(int w, int h);
void mouseButtonPress(int butt, int state, int x, int y);
void text(int x, int y, float r, float g, float b, char* string);

//globals
int w;					//screen width
int h;					//screen height
float angle;			//camera angle of rotation
float dir_x;			//camera x direction
float dir_z;			//camera z direction
float x;				//camera x
float y;				//camera y
float z;				//camera z
float cam_dist;			//camera height

float delta_angle;		//change in camera angle
float delta_move;		//change in forward camera position
float y_move;			//change in up down camera position
int x_start;			//tracking start of mouse motion
float speed;			//speed of camera
float max_speed;		//max speed of car
float accel;			//acceleration vector of car
float movement;			//total movement for each update

float xs;				//speedometer position
float ys;
float theta_s;			//speedometer angle

int mouse_x;			//mouse x
int mouse_y;			//mouse y

float car_x;			//position of car
float car_y;
float car_z;
float car_dist;			//camera distance from car
float car_r;			//color of car
float car_g;
float car_b;

int car_selection;		//which car you are using
int num_cars;			//total number of car blueprints

float xPos;				//position of camera for alternative camera mode
float yPos;
float zPos;
int orbit;				//used as boolean to toggle camera mode
float phi;				//camera angle for alternative camera mode

char* str;				//for outputting text on the screen
char* str2;

bool* keyStates;		//which keys are pressed down

int go;					//used as control boolean
int game_state;			//controls state of game
						//0 = start game
						//1 = game running
						//2 = end game

int headlights;			//if headlights are on or not

float* xloc;			//positions for grass blades
float* yloc;
float** locs;

time_t current_time;	//for timing of start lights
time_t start_time;
time_t start_timer_time;
time_t current_timer_time;
float time_diff;
float timer_time_diff;
bool has_timer_started;
int lap_ended;

bool show_minimap; 		//whether or not to display minimap
bool show_timer;		//whether or not to display the lap time

//vertices
float cube_m[][3] =
	{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
float wedge_m[][3] = 
	{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}};

//initializes OpenGL environment
void init(void){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.001, 10000.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 1.0, 1.0);
	initializeRandos();
}

//initializes locations of grass blades on ground
void initializeRandos(void){
	float rando;
	int i, j, k;
	for(i = 0; i < 50000; i++){
		rando = (((float)rand() / RAND_MAX) * 400) - 200;
		xloc[i] = rando;
	}
	for(j = 0; j < 50000; j++){
		rando = (((float)rand() / RAND_MAX) * 400) - 200;
		yloc[j] = rando;
	}

	for(k = 0; k < 50000; k++){
		locs[k][0] = xloc[k];
		locs[k][1] = 0.01;
		locs[k][2] = yloc[k];
	}
}

//display callback, shows minimap, speedometer, and scene
void display(void){
	if(game_state == 0){
		if(go == 0){
			glutKeyboardFunc(NULL);
			glutKeyboardUpFunc(NULL);
		}
		else{
			glutKeyboardFunc(pressKey);
			glutKeyboardUpFunc(releaseKey);
		}
	}
	if(game_state == 1){
		glutKeyboardFunc(pressKey);
		glutKeyboardUpFunc(releaseKey);
	}
	if(go == 1 && game_state == 1)
		if(!has_timer_started){
			start_timer_time = time(NULL);
			has_timer_started = true;
		}

	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(orbit == 0)
		gluLookAt(x - (car_dist*dir_x), (y+cam_dist), z - (car_dist*dir_z), (x + dir_x), (y+cam_dist), (z + dir_z), 0.0f, 1.0f, 0.0f);
	else
		gluLookAt(xPos, yPos, zPos, x, y, z, 0.0f, 1.0f, 0.0f);

	if(delta_move)
		updateCamera(0);

	drawGround();
	make();

	//display text and 2D elements
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	speedometer();
	if(show_minimap)
		minimap();
	if(game_state == 0)
		startGame();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

//starts the game, using red, yellow, and green lights
void startGame(void){
	current_time = time(NULL);
	time_diff = difftime(current_time, start_time);
	if(time_diff <= 0.05){
		keyStates['w'] = true;
		keyStates['W'] = true;
		keyStates['A'] = true;
		keyStates['a'] = true;
	}
	if(time_diff > 0.05 && time_diff <= 1.0){
		movement = 0.0f;
		keyStates['w'] = false;
		keyStates['W'] = false;
		keyStates['A'] = false;
		keyStates['a'] = false;
		keyStates[' '] = true;
	}
	if(time_diff <= 4.5){
		movement = delta_move * accel;
		circle(3*w / 8, 0.75*h, 0.05*w, 1.0, 0.0, 0.0);
		circle(w/2, 0.75*h, 0.05*w, 1.0, 0.0, 0.0);
		circle(5*w / 8, 0.75*h, 0.05*w, 1.0, 0.0, 0.0);
		keyStates[' '] = false;
	}
	if(time_diff >= 4.5 && time_diff <= 7.0){
		circle(3*w / 8, 0.75*h, 0.05*w, 1.0, 1.0, 0.0);
		circle(w/2, 0.75*h, 0.05*w, 1.0, 1.0, 0.0);
		circle(5*w / 8, 0.75*h, 0.05*w, 1.0, 1.0, 0.0);
	}
	if(time_diff >= 7.0 && time_diff <= 8.0){
		circle(3*w / 8, 0.75*h, 0.05*w, 0.0, 1.0, 0.0);
		circle(w/2, 0.75*h, 0.05*w, 0.0, 1.0, 0.0);
		circle(5*w / 8, 0.75*h, 0.05*w, 0.0, 1.0, 0.0);
		go = 1;
	}
	if(time_diff > 8.0)
		game_state = 1;
}

//draws the speedometer in the bottom left corner
void speedometer(void){
	float to_sprint = (delta_move+accel-1)*100*.85;
	sprintf(str, "%.3g", to_sprint);
	if(delta_move+accel-1 < 0.005){
		float zero = 0.000;
		sprintf(str, "%.2g", zero);
	}
	text(0.09*w, 0.12*h, 0.0, 0.0, 0.0, str);

	glLineWidth(50.0);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0.1*w, 0.0);
		glVertex2f(xs, ys);
	glEnd();

	square((0.1*w - 50), 0.1*h, (0.1*w + 50), 0.1*h, (0.1*w + 50), 0.15*h, (0.1*w - 50), 0.15*h, 1.0, 1.0, 1.0);
	smallLines(0.1*w, 0.0, 250, 1.0, 1.0, 1.0);

	if(delta_move+accel-1 > 0.005)
		text(0.1*w + 75, 0.12*h, 0.0, 1.0, 0.0, "D");
	else if(delta_move+accel-1 < -0.005)
		text(0.1*w + 75, 0.12*h, 1.0, 0.0, 0.0, "R");
	else
		text(0.1*w + 75, 0.12*h, 1.0, 1.0, 0.0, "N");

	glLineWidth(50.0);
	circle(0.1*w, 0.0, 50, 1.0, 0.0, 0.0);
	circle(0.1*w, 0.0, 250, 0.0, 0.0, 0.0);
	hollowCircle(0.1*w, 0.0, 250, 1.0, 1.0, 1.0);

	if(delta_move+accel-1 >= 0)
		theta_s = 30 - 4.8*(delta_move+accel)/max_speed;
	xs = 0.1*w + 250*cos(theta_s);
	ys = 250*sin(theta_s);
}

//draws the minimap in the top right corner
void minimap(void){
	float xold = 1 - (x + dir_x);
	float zold = z + dir_z;
	float xnew = 0.3*xold + 0.9*w;
	float znew = 0.3*zold + 0.9*h;
	float x1, y1, x2, y2;

	glColor3f(car_r, car_g, car_b);
	glPointSize(8.0);
	glBegin(GL_POINTS);
		glVertex2f(xnew, znew);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
		glVertex2f(0.85*w, 0.82*h);
		glVertex2f(0.85*w, 0.98*h);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(0.95*w, 0.82*h);
		glVertex2f(0.95*w, 0.98*h);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(0.85*w, 0.82*h);
		glVertex2f(0.95*w, 0.82*h);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(0.85*w, 0.98*h);
		glVertex2f(0.95*w, 0.98*h);
	glEnd();

	glColor3f(0.05, 0.05, 0.05);
	glBegin(GL_LINES);
		x1 = 20.0 * 0.3;
		y1 = 60.0 * 0.3;
		glVertex2f(0.9*w, 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -20 * 0.3;
		y1 = 100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -100.0 * 0.3;
		y1 = 100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -120.0 * 0.3;
		y1 = 110.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -150.0 * 0.3;
		y1 = 150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -180.0 * 0.3;
		y1 = 100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -180.0 * 0.3;
		y1 = 0.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -180.0 * 0.3;
		y1 = -100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -150.0 * 0.3;
		y1 = -125.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -100.0 * 0.3;
		y1 = -150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = -50.0 * 0.3;
		y1 = -125.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 0.0 * 0.3;
		y1 = -100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 50.0 * 0.3;
		y1 = -50.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 100.0 * 0.3;
		y1 = 0.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 100.0 * 0.3;
		y1 = 50.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 50.0 * 0.3;
		y1 = 100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 50.0 * 0.3;
		y1 = 150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 100.0 * 0.3;
		y1 = 175.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 150.0 * 0.3;
		y1 = 175.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 175.0 * 0.3;
		y1 = 100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 175.0 * 0.3;
		y1 = 0.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 175.0 * 0.3;
		y1 = -100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 175.0 * 0.3;
		y1 = -150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 150.0 * 0.3;
		y1 = -150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 100.0 * 0.3;
		y1 = -150.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();glBegin(GL_LINES);
		x1 = 50.0 * 0.3;
		y1 = -100.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 0.0 * 0.3;
		y1 = -55.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
	glBegin(GL_LINES);
		x1 = 0.0 * 0.3;
		y1 = 0.0 * 0.3;
		glVertex2f(x2 + 0.9*w, y2 + 0.9*h);
		glVertex2f(x1 + 0.9*w, y1 + 0.9*h);
		x2 = x1;
		y2 = y1;
	glEnd();
}

//checks to see if the car has completed a lap
void checkForLap(void){
	float xpos = x + dir_x;
	float zpos = z + dir_z;

	if(xpos > -2.0 && xpos < 2.0){
		if(zpos > -10.0 && zpos < 10.0)
			lap_ended++;
	}
}

//draws a four sided polygon in the affine space
void quadPoly(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3], float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glVertex3fv(v4);
	glEnd();
}

//draws a four sided polygon in the affine space with an alpha channel
void quadPolyAlpha(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat v4[3], float r, float g, float b, float a){
	glColor4f(r, g, b, a);
	glBegin(GL_POLYGON);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
		glVertex3fv(v4);
	glEnd();
}

//draws a four sided polygon of equal side length in the affine space
void quadPolyWithSize(GLfloat v[3], float rad, float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex3f(v[0], v[1], v[2]);
		glVertex3f(v[0]+rad, v[1], v[2]);
		glVertex3f(v[0]+rad, v[1], v[2]+rad);
		glVertex3f(v[0], v[1], v[2]+rad);
	glEnd();
}

//draws the 2D rectangles for the track
void trackRect(float x1, float z1, float x2, float z2, float x3, float z3, float x4, float z4){
	glColor3f(0.05, 0.05, 0.05);
	glBegin(GL_POLYGON);
		glVertex3f(x1, 0.02, z1);
		glVertex3f(x2, 0.02, z2);
		glVertex3f(x3, 0.02, z3);
		glVertex3f(x4, 0.02, z4);
	glEnd();
}

//draws a thre sided polygon in the affine space
void triPoly(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex3fv(v1);
		glVertex3fv(v2);
		glVertex3fv(v3);
	glEnd();
}

//draws a 3D cylinder in the affine space
void cylinder(float radius, float height, int n, float r, float g, float b){
	float theta;
	int i, j, k, x;
	float x1, z1;

	//initialize 2D array
	float** lo = (float**) malloc(n*sizeof(float*));
	float** hi = (float**) malloc(n*sizeof(float*));
	for(x = 0; x < n; x++){
		lo[x] = (float*) malloc(3*sizeof(float));
		hi[x] = (float*) malloc(3*sizeof(float));
	}

	//bottom circle
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_FAN);
		for(i = 0; i < n; i++){
			theta = i * 2 * PI / n;
			x1 = radius*cos(theta);
			z1 = radius*sin(theta);
			lo[i][0] = x1;
			lo[i][1] = 0.0;
			lo[i][2] = z1;
			glVertex3f(x1, 0.0, z1);
		}
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	//top circle
	glBegin(GL_TRIANGLE_FAN);
		for(j = 0; j < n; j++){
			theta = j * 2 * PI / n;
			x1 = radius*cos(theta);
			z1 = radius*sin(theta);
			hi[j][0] = x1;
			hi[j][1] = height;
			hi[j][2] = z1;
			glVertex3f(x1, height, z1);
		}
		glVertex3f(0.0, height, 0.0);
	glEnd();

	//connecting the two circles
	for(k = 0; k < n-1; k++)
		quadPoly(lo[k], lo[k+1], hi[k+1], hi[k], r, g, b);
	quadPoly(lo[0], lo[k], hi[k], hi[0], r, g, b);

	//deallocate memory
	free(lo);
	free(hi);
}

//draws a 2D square
void square(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
		glVertex2f(x4, y4);
	glEnd();
}

//draws a 2D triangle
void triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b){
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glVertex2f(x3, y3);
	glEnd();
}

//draws the small lines for the speedometer
void smallLines(float xCen, float yCen, float rad, float r, float g, float b){
	int i;
	glColor3f(r, g, b);
	glLineWidth(1.0);
	glBegin(GL_LINES);
		for(i = 330; i > 0; i--){
			glVertex2f(xCen + 225*cos(i), yCen + 225*sin(i));
			glVertex2f(xCen + 250*cos(i), yCen + 250*sin(i));
		}
	glEnd();

	glBegin(GL_LINES);
		for(i = 40; i > 0; i--){
				glVertex2f(xCen + 200*cos(i), yCen + 200*sin(i));
				glVertex2f(xCen + 250*cos(i), yCen + 250*sin(i));
			}
	glEnd();
}

//draws a 3D cube in the affine space
void cube(float r, float g, float b){
	quadPoly(cube_m[0], cube_m[1], cube_m[2], cube_m[3], r, g, b);
	quadPoly(cube_m[1], cube_m[2], cube_m[6], cube_m[5], r, g, b);
	quadPoly(cube_m[2], cube_m[3], cube_m[7], cube_m[6], r, g, b);
	quadPoly(cube_m[5], cube_m[6], cube_m[7], cube_m[4], r, g, b);
	quadPoly(cube_m[0], cube_m[1], cube_m[5], cube_m[4], r, g, b);
	quadPoly(cube_m[0], cube_m[3], cube_m[7], cube_m[4], r, g, b);
}

//draws a 3D cube in the affine space with an alpha channel
void cubeAlpha(float r, float g, float b, float a){
	quadPolyAlpha(cube_m[0], cube_m[1], cube_m[2], cube_m[3], r, g, b, a);
	quadPolyAlpha(cube_m[1], cube_m[2], cube_m[6], cube_m[5], r, g, b, a);
	quadPolyAlpha(cube_m[2], cube_m[3], cube_m[7], cube_m[6], r, g, b, a);
	quadPolyAlpha(cube_m[5], cube_m[6], cube_m[7], cube_m[4], r, g, b, a);
	quadPolyAlpha(cube_m[0], cube_m[1], cube_m[5], cube_m[4], r, g, b, a);
	quadPolyAlpha(cube_m[0], cube_m[3], cube_m[7], cube_m[4], r, g, b, a);
}

//draws a 3D wedge in the affine space
void wedge(float r, float g, float b){
	quadPoly(wedge_m[0], wedge_m[1], wedge_m[2], wedge_m[3], r, g, b);
	quadPoly(wedge_m[0], wedge_m[3], wedge_m[5], wedge_m[4], r, g, b);
	quadPoly(wedge_m[1], wedge_m[2], wedge_m[5], wedge_m[4], r, g, b);
	triPoly(wedge_m[2], wedge_m[3], wedge_m[5], r, g, b);
	triPoly(wedge_m[0], wedge_m[1], wedge_m[4], r, g, b);
}

//draws a 3D wedge in the affine space with a different color on the right face
void wedgeR(float r, float g, float b, float r2, float g2, float b2){
	quadPoly(wedge_m[0], wedge_m[1], wedge_m[2], wedge_m[3], r, g, b);
	quadPoly(wedge_m[0], wedge_m[3], wedge_m[5], wedge_m[4], r, g, b);
	quadPoly(wedge_m[1], wedge_m[2], wedge_m[5], wedge_m[4], r, g, b);
	triPoly(wedge_m[2], wedge_m[3], wedge_m[5], r2, g2, b2);
	triPoly(wedge_m[0], wedge_m[1], wedge_m[4], r, g, b);
}

//draws a 3D wedge in the affine space with a different color on the left face
void wedgeL(float r, float g, float b, float r2, float g2, float b2){
	quadPoly(wedge_m[0], wedge_m[1], wedge_m[2], wedge_m[3], r, g, b);
	quadPoly(wedge_m[0], wedge_m[3], wedge_m[5], wedge_m[4], r, g, b);
	quadPoly(wedge_m[1], wedge_m[2], wedge_m[5], wedge_m[4], r, g, b);
	triPoly(wedge_m[2], wedge_m[3], wedge_m[5], r, g, b);
	triPoly(wedge_m[0], wedge_m[1], wedge_m[4], r2, g2, b2);
}

//draws a 3D arrow in the affine space
void arrow(void){
	glPushMatrix();
	cube(1.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	cube(1.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
	cube(1.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, 0.0, 0.5);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(1.5, 1.5, 1.0);
	wedge(1.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0, 0.0, 0.5);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glScalef(1.5, -1.5, 1.0);
	wedge(1.0, 0.0, 0.0);
	glPopMatrix();
}

//1st car design: sports car
void car1(float r, float g, float b){
	//front bumper
	glPushMatrix();
	glTranslatef(-1.5, 0.35, -0.05);
	glScalef(3.0, 0.2, 0.05);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//back bumper
	glPushMatrix();
	glTranslatef(-1.5, 0.35, 4.6);
	glScalef(3.0, 0.2, 0.05);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//headlights
	glPushMatrix();
	glTranslatef(-1.2, 0.95, -0.1);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	if(headlights == 1){
		glPushMatrix();
		glTranslatef(-1.2, 0.95, -0.11);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		cylinder(0.17, 0.1, 30, 1.0, 1.0, 1.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.2, 0.95, -0.11);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		cylinder(0.17, 0.1, 30, 1.0, 1.0, 1.0);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(1.2, 0.95, -0.1);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	//grill
	glPushMatrix();
	glTranslatef(-0.9, 0.95, -0.01);
	glScalef(1.8, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 0.87, -0.01);
	glScalef(1.6, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0.79, -0.01);
	glScalef(1.4, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.71, -0.01);
	glScalef(1.2, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//tail lights
	glPushMatrix();
	glTranslatef(-1.2, 0.95, 4.6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0.95, 4.6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	if(keyStates['s'] || keyStates['S'] || keyStates['e'] || keyStates['E']){
		if(delta_move+accel-1 > 0.005){
			glPushMatrix();
			glTranslatef(-1.2, 0.95, 4.6);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 0.0, 0.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.2, 0.95, 4.6);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 0.0, 0.0);
			glPopMatrix();
		}
		else{
			glPushMatrix();
			glTranslatef(-1.2, 0.95, 4.6);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 1.0, 1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.2, 0.95, 4.6);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
	}

	//front wheel-wells
	glPushMatrix();
	glTranslatef(0.5, 0.25, 0.0);
	glScalef(1.0, 1.0, 0.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.95, 0.0);
	glScalef(1.0, 0.3, 1.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.95, 0.0);
	glScalef(1.0, 0.3, 1.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 0.0);
	glScalef(1.0, 1.0, 0.2);
	cube(r, g, b);
	glPopMatrix();

	//front wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, 0.7);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, 0.7);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, 0.7);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, 0.7);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//rear wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, 3.9);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, 3.9);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, 3.9);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, 3.9);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	//hood scoop
	glPushMatrix();
	glTranslatef(-0.3, 1.25, 0.3);
	glScalef(0.6, 0.1, 0.3);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 1.25, 0.6);
	glScalef(0.6, 0.1, 0.1);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedge(0.0, 0.0, 0.0);
	glPopMatrix();

	//car body
	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.0);
	glScalef(1.0, 1.0, 1.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 1.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 1.2);
	cube(r, g, b);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 3.0);
	glScalef(1.0, 1.0, 0.4);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 3.0);
	glScalef(1.0, 1.0, 0.4);
	cube(r, g, b);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 3.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.51, 1.25, 2.0);
	glScalef(0.01, 0.5, 0.05);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 1.25, 2.0);
	glScalef(0.01, 0.5, 0.05);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.51, 1.25, 2.95);
	glScalef(0.01, 0.5, 0.05);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 1.25, 2.95);
	glScalef(0.01, 0.5, 0.05);
	cube(r, g, b);
	glPopMatrix();

	//rear wheel-wells
	glPushMatrix();
	glTranslatef(0.5, 0.95, 3.4);
	glScalef(1.0, 0.3, 0.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.95, 3.4);
	glScalef(1.0, 0.3, 0.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 4.4);
	glScalef(1.0, 1.0, 0.2);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 4.4);
	glScalef(1.0, 1.0, 0.2);
	cube(r, g, b);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 4.0);
	glScalef(1.0, 1.0, 0.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.95, 4.0);
	glScalef(1.0, 0.3, 0.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.95, 4.0);
	glScalef(1.0, 0.3, 0.6);
	cube(r, g, b);
	glPopMatrix();

	//spoiler
	glPushMatrix();
	glTranslatef(-1.5, 1.45, 4.3);
	glScalef(1.0, 0.05, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.45, 4.3);
	glScalef(1.0, 0.05, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.5, 1.45, 4.3);
	glScalef(1.0, 0.05, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 1.15, 4.45);
	cylinder(0.05, 0.3, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.15, 4.45);
	cylinder(0.05, 0.3, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	//roof
	glPushMatrix();
	glTranslatef(-0.5, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 1.75, 2.0);
	glScalef(1.0, 0.02, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.75, 2.0);
	glScalef(1.0, 0.02, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.75, 2.0);
	glScalef(1.0, 0.02, 1.0);
	cube(r, g, b);
	glPopMatrix();

	//back window
	glPushMatrix();
	glTranslatef(0.5, 1.25, 3.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 1.25, 3.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.4, 1.25, 3.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.25, 3.0);
	glScalef(0.1, 0.5, 1.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedgeR(r, g, b, 0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1.25, 3.0);
	glScalef(0.1, 0.5, 1.0);
	glRotatef(270.0, 0.0, 1.0, 0.0);
	wedgeL(r, g, b, 0.1, 0.1, 0.1);
	glPopMatrix();

	//windshield
	glPushMatrix();
	glTranslatef(-0.5, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 1.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.25, 2.0);
	glScalef(0.1, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	wedgeL(r, g, b, 0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.4, 1.25, 2.0);
	glScalef(0.1, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	wedgeR(r, g, b, 0.1, 0.1, 0.1);
	glPopMatrix();
}

//2nd car design: F1 racecar
void car2(float r, float g, float b){
	//car body
	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(2.0, 0.5, 1.0);
	wedge(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, -2.0);
	glScalef(3.0, -0.1, 0.5);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 0.3, 0.99);
	glScalef(0.6, 0.3, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8, 0.3, 0.99);
	glScalef(0.6, 0.3, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 2.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 3.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 4.0);
	glScalef(1.0, 0.5, 0.5);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.75, 3.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	wedge(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 3.0);
	glScalef(1.0, 0.5, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	wedge(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.75, 2.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glScalef(1.0, 0.5, 1.0);
	wedge(0.0, 0.0, 0.0);
	glPopMatrix();

	//front wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//front axle
	glPushMatrix();
	glTranslatef(1.0, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.05, 1.0, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.5, -0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.05, 1.0, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//rear axle
	glPushMatrix();
	glTranslatef(1.0, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.05, 1.0, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.05, 1.0, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//rear wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.45, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, 4.2);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//spoiler
	glPushMatrix();
	glTranslatef(-0.3, 0.55, 4.0);
	glScalef(0.1, 0.5, 0.5);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.55, 4.0);
	glScalef(-0.1, 0.5, 0.5);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.05, 4.0);
	glScalef(3.0, 0.05, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.25, 4.0);
	glScalef(3.0, 0.05, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.0, 3.8);
	glScalef(0.05, 0.4, 0.8);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1.0, 3.8);
	glScalef(-0.05, 0.4, 0.8);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//winshield
	glPushMatrix();
	glTranslatef(-0.5, 0.75, 1.0);
	glScalef(0.05, 0.25, 1.0);
	cubeAlpha(0.1, 0.1, 0.1, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.75, 1.0);
	glScalef(-0.05, 0.25, 1.0);
	cubeAlpha(0.1, 0.1, 0.1, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.75, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.25, 1.0);
	wedge(0.1, 0.1, 0.1);
	glPopMatrix();

	//tail pipes
	glPushMatrix();
	glTranslatef(-0.25, 0.5, 4.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.1, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.5, 4.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.1, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0.5, 4.5);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.1, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();
}

//3rd car design: pickup truck
void car3(float r, float g, float b){
	//rear wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, 4.6);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.55, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, 4.6);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, 4.6);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.55, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, 4.6);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//tail lights
	glPushMatrix();
	glTranslatef(-1.2, 0.95, 6.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0.95, 6.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	if(keyStates['s'] || keyStates['S'] || keyStates['e'] || keyStates['E']){
		if(delta_move+accel-1 > 0.005){
			glPushMatrix();
			glTranslatef(-1.2, 0.95, 6.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 0.0, 0.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.2, 0.95, 6.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 0.0, 0.0);
			glPopMatrix();
		}
		else{
			glPushMatrix();
			glTranslatef(-1.2, 0.95, 6.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 1.0, 1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.2, 0.95, 6.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			cylinder(0.17, 0.11, 30, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
	}

	//back bumper
	glPushMatrix();
	glTranslatef(-1.5, 0.35, 6.0);
	glScalef(3.0, 0.2, 0.05);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//headlights
	glPushMatrix();
	glTranslatef(-1.2, 0.95, -1.6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	if(headlights == 1){
		glPushMatrix();
		glTranslatef(-1.2, 0.95, -1.61);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		cylinder(0.17, 0.1, 30, 1.0, 1.0, 1.0);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.2, 0.95, -1.61);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		cylinder(0.17, 0.1, 30, 1.0, 1.0, 1.0);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(1.2, 0.95, -1.6);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	cylinder(0.2, 0.1, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	//grill
	glPushMatrix();
	glTranslatef(-0.9, 0.95, -1.51);
	glScalef(1.8, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8, 0.87, -1.51);
	glScalef(1.6, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, 0.79, -1.51);
	glScalef(1.4, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, 0.71, -1.51);
	glScalef(1.2, 0.05, 0.01);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	//front wheels
	glPushMatrix();
	glTranslatef(-1.0, 0.5, -0.70);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.55, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.01, 0.5, -0.70);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(1.5, 0.5, -0.70);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.55, 0.5, 30, 0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 0.5, -0.70);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	if(keyStates['a'] || keyStates['A'])
		glRotatef(30.0, 1.0, 0.0, 0.0);
	if(keyStates['d'] || keyStates['D'])
		glRotatef(-30.0, 1.0, 0.0, 0.0);
	cylinder(0.35, 0.5, 30, 0.1, 0.1, 0.1);
	glPopMatrix();

	//truck body
	glPushMatrix();
	glTranslatef(-0.5, 0.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, -1.5);
	glScalef(1.0, 1.1, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, -1.5);
	glScalef(1.0, 1.0, 0.1);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, -1.5);
	glScalef(1.0, 1.0, 0.1);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.15, -1.5);
	glScalef(1.0, 0.2, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.15, -1.5);
	glScalef(1.0, 0.2, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 1.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.25, 0.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 1.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.25, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 2.25, 0.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 2.25, 0.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 2.25, 0.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 2.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 2.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 2.25, 1.0);
	glScalef(1.0, 0.5, 1.0);
	cube(r, g, b);
	glPopMatrix();

	//truck bed
	glPushMatrix();
	glTranslatef(-1.4, 1.25, 2.0);
	glScalef(2.8, 0.01, 4.0);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 2.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 3.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 3.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 3.0);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 4.0);
	glScalef(1.0, 1.0, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1.05, 4.0);
	glScalef(1.0, 0.2, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.05, 4.0);
	glScalef(1.0, 0.2, 1.6);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 0.25, 5.2);
	glScalef(1.0, 1.0, 0.8);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 0.25, 5.2);
	glScalef(1.0, 1.0, 0.8);
	cube(r, g, b);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.25, 5.2);
	glScalef(1.0, 1.0, 0.8);
	cube(r, g, b);
	glPopMatrix();

	//windows
	glPushMatrix();
	glTranslatef(-1.3, 1.6, -0.01);
	glScalef(2.6, 0.9, 0.01);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.51, 1.6, 0.2);
	glScalef(-0.01, 0.9, 1.6);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.51, 1.6, 0.2);
	glScalef(0.01, 0.9, 1.6);
	cube(0.1, 0.1, 0.1);
	glPopMatrix();

	//bed railings
	glPushMatrix();
	glTranslatef(-1.4, 1.0, 2.5);
	glScalef(0.1, 1.5, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.0, 5.5);
	glScalef(0.1, 1.5, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3, 1.0, 2.5);
	glScalef(0.1, 1.5, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3, 1.0, 5.5);
	glScalef(0.1, 1.5, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.45, 2.0);
	glScalef(0.1, 0.3, 4.0);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.95, 2.0);
	glScalef(0.1, 0.3, 4.0);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3, 1.45, 2.0);
	glScalef(0.1, 0.3, 4.0);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3, 1.95, 2.0);
	glScalef(0.1, 0.3, 4.0);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.45, 6.0);
	glScalef(2.8, 0.3, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.4, 1.95, 6.0);
	glScalef(2.8, 0.3, 0.1);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();
}

//draws the track, with track and gates
void track(void){
	glPushMatrix();
	gate(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.0, 0.0, 60.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(9.5, 0.0, -9.0, 0.0, -29.0, 60.0, -10.5, 60.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.0, 0.0, 100.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-29.0, 60.0, -10.5, 60.0, 27.0, 93.0, 14.0, 106.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100.0, 0.0, 100.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(27.0, 93.0, 14.0, 106.5, 100.0, 109.0, 100.0, 90.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(120.0, 0.0, 110.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(100.0, 109.0, 100.0, 90.5, 127.0, 103.0, 114.0, 116.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150.0, 0.0, 150.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(127.0, 103.0, 114.0, 116.5, 150.0, 159.0, 150.0, 140.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(180.0, 0.0, 100.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(150.0, 159.0, 150.0, 140.5, 170.5, 100.0, 189.5, 100.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(180.0, 0.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(170.5, 100.0, 189.5, 100.0, 189.5, 0.0, 170.5, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(180.0, 0.0, -100.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(189.5, 0.0, 170.5, 0.0, 170.5, -100.0, 189.5, -100.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(150.0, 0.0, -125.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(170.5, -100.0, 189.5, -100.0, 157.0, -132.0, 143.0, -118.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100.0, 0.0, -150.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(157.0, -132.0, 143.0, -118.5, 100.0, -140.5, 100.0, -159.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50.0, 0.0, -125.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(100.0, -140.5, 100.0, -159.5, 43.5, -132.0, 57.0, -118.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -100.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(43.5, -132.0, 57.0, -118.5, 7.0, -93.0, -6.5, -106.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50.0, 0.0, -50.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(7.0, -93.0, -6.5, -106.5, -56.5, -57.0, -43.0, -43.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0, 0.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-56.5, -57.0, -43.0, -43.5, -90.5, 0.0, -109.5, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0, 0.0, 50.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-90.5, 0.0, -109.5, 0.0, -107.0, 56.5, -93.0, 43.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50.0, 0.0, 100.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-107.0, 56.5, -93.0, 43.0, -40.5, 100.0, -59.0, 100.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50.0, 0.0, 150.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-40.5, 100.0, -59.0, 100.0, -56.5, 143.0, -43.0, 156.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0, 0.0, 175.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-56.5, 143.0, -43.0, 156.5, -100.0, 184.5, -100.0, 165.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150.0, 0.0, 175.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-100.0, 184.5, -100.0, 165.5, -143.0, 168.0, -157.0, 181.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-175.0, 0.0, 100.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-143.0, 168.0, -157.0, 181.5, -184.0, 100.0, -165.5, 100.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-175.0, 0.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-184.0, 100.0, -165.5, 100.0, -165.0, 0.0, -184.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-175.0, 0.0, -100.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-165.0, 0.0, -184.0, 0.0, -184.0, -100.0, -165.0, -100.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-175.0, 0.0, -150.0);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-184.0, -100.0, -165.0, -100.0, -168.0, -143.5, -181.5, -157.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-150.0, 0.0, -150.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-168.0, -143.5, -181.5, -157.0, -150.0, -159.5, -150.0, -140.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0, 0.0, -150.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-150.0, -159.5, -150.0, -140.5, -107.0, -143.5, -93.0, -157.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50.0, 0.0, -100.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-107.0, -143.5, -93.0, -157.0, -43.0, -107.0, -57.0, -93.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -55.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gate(0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-43.0, -107.0, -57.0, -93.5, -7.0, -48.5, 7.0, -62.0);
	glPopMatrix();

	glPushMatrix();
	trackRect(-7.0, -48.5, 7.0, -62.0, 9.8, 0.0, -9.5, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-12.0, -0.9, -87.0);
	glRotatef(225.0, 0.0, 1.0, 0.0);
	arrow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-37.5, -0.9, -88.5);
	glRotatef(315.0, 0.0, 1.0, 0.0);
	arrow();
	glPopMatrix();
}

//draws a 3D gated checkpoint in the affine space
void gate(int start){
	glPushMatrix();
	glTranslatef(-9.5, 0.0, 0.0);
	glScalef(0.5, 5.0, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9.5, 0.0, 0.0);
	glScalef(0.5, 5.0, 0.5);
	cube(0.0, 0.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9.5, 5.0, 0.0);
	glScalef(19.5, 2.0, 0.5);
	if(start == 1)
		cube(1.0, 1.0, 1.0);
	else
		cube(0.1, 0.1, 0.1);
	glPopMatrix();
}

//draws a 2D circle
void circle(float xCen, float yCen, float rad, float r, float g, float b){
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle
	
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(r, g, b);
		glVertex2f(xCen, yCen); // center of circle
		for(i = 0; i <= triangleAmount; i++)
			glVertex2f(xCen + (rad * cos(i *  2 * PI / triangleAmount)), yCen + (rad * sin(i * 2 * PI / triangleAmount)));
	glEnd();
}

//draws the outline of a 2D circle
void hollowCircle(float xCen, float yCen, float rad, float r, float g, float b){
	int i;
	int lineAmount = 150; //# of triangles used to draw circle
	
	glBegin(GL_LINE_LOOP);
		glColor3f(r, g, b);
		for(i = 0; i <= lineAmount;i++) { 
			glVertex2f(xCen + (rad * cos(i * 2 * PI / lineAmount)), yCen + (rad* sin(i * 2 * PI / lineAmount)));
		}
	glEnd();
}

//draws the ground, including gridlines, green ground, and darker green grass blades
void drawGround(void){
	int i, j;
	glLineWidth(1);
	for(i = 0; i < 400; i++){
		glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-200.0 + i, 0.0, 200.0);
			glVertex3f(-200.0 + i, 0.0, -200.0);
		glEnd();
	}

	for(j = 0; j < 400; j++){
		glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-200.0, 0.0, 200.0 - j);
			glVertex3f(200.0, 0.0, 200.0 - j);
		glEnd();
	}

	glPushMatrix();
	float ground[][3] = {{-200.0, -0.01, -200.0}, {200.0, -0.01, -200.0}, {200.0, -0.01, 200.0}, {-200.0, -0.01, 200.0}};
	quadPoly(ground[0], ground[1], ground[2], ground[3], 0.25, 1.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	int k;
	for(k = 0; k < 50000; k++){
		quadPolyWithSize(locs[k], 0.2, 0.0, 0.55, 0.0);
	}
	glPopMatrix();
}

//draws the whole scene, with cars and track
void make(void){
    glPushMatrix();
    glTranslatef(car_x, car_y, car_z);
    glScalef(1.0, 1.0, 1.0);
    glRotatef(-18.325*PI*(angle+delta_angle), 0.0, 1.0, 0.0);
    switch(car_selection){
    	case 0:
    		car1(car_r, car_g, car_b);
    		break;		
    	case 1:
    		car2(car_r, car_g, car_b);
    		break;
    	case 2:
    		car3(car_r, car_g, car_b);
    		break;
    }
    
    glPopMatrix();

    glPushMatrix();
    track();
    glPopMatrix();
}

//updates the camera's position after keyboard input
void updateCamera(int i){
	switch(i){
	case 0:
		movement = delta_move * accel;
		x += (movement * dir_x);
		z += (movement * dir_z);
		car_x = x;
		car_z = z;
		break;
	}
}

//callback for if key is pressed
void pressKey(char key, int mouse_x, int mouse_y){
	keyStates[key] = true;
}

//manages all of the key operations
void keyOperations(void){
	if(keyStates['w'] || keyStates['W']){
		if(delta_move+accel < max_speed){
			delta_move = speed;
			accel += 0.005;
		}
	}

	if(keyStates['s'] || keyStates['S']){
		if(delta_move+accel-2 > -1.0 * max_speed){
			delta_move = speed;
			accel -= 0.005;
		}
	}

	if(keyStates['e'] || keyStates['E']){
		if(abs(delta_move+accel) > 0.0){
			delta_move = speed;
			accel -= 0.02;
			if(abs(delta_move+accel) < 0.01){
				delta_move = 1.0;
				speed = 1.0;
			}
		}
	}

	if(keyStates[' ']){
		if(game_state == 0){
			if(abs(delta_move+accel) > 0.0){
				delta_move = speed;
				accel -= 0.1;
				if(abs(delta_move+accel) < 0.01){
					delta_move = 1.0;
					speed = 1.0;
				}
			}
		}
	}

	if(keyStates['a'] || keyStates['A']){
		if(delta_move+accel-1 >= 0){
			delta_angle -= 0.01f;
			dir_x = sin(angle + delta_angle);
			dir_z = -cos(angle + delta_angle);
		}
		else{
			delta_angle += 0.01f;
			dir_x = sin(angle + delta_angle);
			dir_z = -cos(angle + delta_angle);
		}
	}

	if(keyStates['d'] || keyStates['D']){
		if(delta_move+accel-1 >= 0){
			delta_angle += 0.01f;
			dir_x = sin(angle + delta_angle);
			dir_z = -cos(angle + delta_angle);
		}
		else{
			delta_angle -= 0.01f;
			dir_x = sin(angle + delta_angle);
			dir_z = -cos(angle + delta_angle);
		}
	}

	if(keyStates['i'] || keyStates['I'])
		if(car_dist > 0.0)
			car_dist -= 0.2;

	if(keyStates['o'] || keyStates['O'])
		if(car_dist < 10.0)
			car_dist += 0.2;

	if(keyStates['u'] || keyStates['U'])
		if(cam_dist < 2.0)
			cam_dist += 0.1;

	if(keyStates['j'] || keyStates['J'])
		if(cam_dist > 0.0)
			cam_dist -= 0.1;

	if(keyStates['1'])
		car_dist = 1.0;

	if(keyStates['2'])
		car_dist = 3.0;

	if(keyStates['3'])
		car_dist = 5.0;

	if(keyStates['4'])
		car_dist = 10.0;

	if(keyStates['0']){
		xPos = car_dist * sin(phi);
		yPos = 4.0f;
		zPos = car_dist * cos(phi);

		orbit = 1;
	}

	if(keyStates['9'])
		orbit = 0;

	if(keyStates['h'] || keyStates['H'])
		headlights = 1;

	if(keyStates['y'] || keyStates['Y'])
		headlights = 0;

	if(keyStates['b'] || keyStates['B'])
		show_minimap = true;

	if(keyStates['n'] || keyStates['N'])
		show_minimap = false;

	if(keyStates['c'] || keyStates['C'])
		show_timer = true;

	if(keyStates['v'] || keyStates['V'])
		show_timer = false;

	if(keyStates['r'] || keyStates['R']){
		lap_ended = 0;
		start_timer_time = time(NULL);
	}

	if(keyStates['q'] || keyStates['Q'])
		exit(0);
}

//callback for if key is released
void releaseKey(char key, int mouse_x, int mouse_y){ 	
	keyStates[key] = false;
} 

//callback for mouse button presses
void mouseButtonPress(int butt, int state, int x, int y){
	if(butt == GLUT_LEFT_BUTTON)
		if(state == GLUT_DOWN)
			car_selection = (car_selection+1) % num_cars;
}

//timer callback, used to implement rolling friction
void timer(int n){
	if(!keyStates['e'] || !keyStates['E']){
		if(!keyStates['w'] || !keyStates['W']){
			if(delta_move+accel-1 > 0.0)
				accel -= 0.005;
		}

		if(!keyStates['s'] || !keyStates['S']){
			if(delta_move+accel-1 < 0.0)
				accel += 0.005;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(n, timer, 1);
}

//reshape callback, maintains aspect ratio
void reshape(int w, int h){
	if(h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	float aspect_ratio =  w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect_ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

//displays 2D text on the screen
void text(int x, int y, float r, float g, float b, char* string){
	glColor3f(r, g, b);
	glLoadIdentity();
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);

	for(i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

//creates the menu for the car color selection
void menu(int value){
	switch(value){
		case 0:
			car_r = 1.0;
			car_g = 0.0;
			car_b = 0.0;
			break;
		case 1:
			car_r = 0.0;
			car_g = 1.0;
			car_b = 0.0;
			break;
		case 2:
			car_r = 0.0;
			car_g = 0.0;
			car_b = 1.0;
			break;
		case 3:
			car_r = 1.0;
			car_g = 0.0;
			car_b = 1.0;
			break;
		case 4:
			car_r = 1.0;
			car_g = 1.0;
			car_b = 0.0;
			break;
		case 5:
			car_r = 0.0;
			car_g = 1.0;
			car_b = 1.0;
			break;
		case 6:
			car_r = 0.55;
			car_g = 0.27;
			car_b = 0.07;
			break;
		case 7:
			car_r = 0.98;
			car_g = 0.27;
			car_b = 0.0;
			break;
		case 8:
			car_r = 1.0;
			car_g = 1.0;
			car_b = 1.0;
			break;
		case 9:
			car_r = 0.0;
			car_g = 0.0;
			car_b = 0.0;
			break;
		case 10:
			car_r = 0.5;
			car_g = 1.0;
			car_b = 0.0;
			break;
		case 11:
			car_r = 1.0;
			car_g = 0.5;
			car_b = 0.31;
			break;
		case 12:
			car_r = 0.42;
			car_g = 0.35;
			car_b = 0.80;
			break;
		case 13:
			car_r = 0.0;
			car_g = 0.98;
			car_b = 0.6;
			break;
		case 14:
			car_r = 1.0;
			car_g = 0.08;
			car_b = 0.58;
			break;
		case 15:
			car_r = 0.0;
			car_g = 0.5;
			car_b = 0.5;
			break;
		case 16:
			car_r = 1.0;
			car_g = 0.39;
			car_b = 0.28;
			break;
		case 17:
			car_r = 0.5;
			car_g = 0.5;
			car_b = 0.0;
			break;
		case 18:
			car_r = 1.0;
			car_g = 0.84;
			car_b = 0.0;
			break;
		case 19:
			car_r = 1.0;
			car_g = 0.97;
			car_b = 0.86;
			break;
	}
}

//main function, sets up globals, allocates memory, and starts the game loop
int main(int argc, char** argv){
	//validate command line arguments -- none in this case

	//initialize globals
	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);
	angle = 0.0f;
	dir_x = -1.0f;
	dir_z = 0.0f;
	x = 15.0f;
	y = 2.0f;
	z = 0.0f;
	delta_angle = 0.0f;
	delta_move = 0;
	x_start = 0;
	speed = 1.0f;
	max_speed = 2.0f;
	accel = 0.0f;
	car_x = car_y = car_z = 0.0f;
	car_dist = 5.0;
	movement = delta_move * accel;
	headlights = 0;
	cam_dist = 1.0f;

	car_r = 1.0;
	car_g = car_b = 0.0;

	xPos = x;
	yPos = y;
	zPos = z;
	orbit = 0;
	phi = 0.0f;

	game_state = 0;
	go = 0;

	xs = 500;
	ys = 500;
	theta_s = 110.0;

	car_selection = 0;
	num_cars = 3;

	show_minimap = false;
	show_timer = true;
	has_timer_started = false;
	lap_ended = 0;

	//allocate memory
	str = calloc(80, sizeof(char));
	str2 = calloc(80, sizeof(char));
	keyStates = calloc(256, sizeof(bool));

	xloc = calloc(50000, sizeof(float));
	yloc = calloc(50000, sizeof(float));	
	locs = (float**) calloc(50000, sizeof(float*));
	int x;
	for(x = 0; x < 50000; x++)
		locs[x] = (float*) calloc(3, sizeof(float));

	//GL initializations
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Kart Racer");
	glEnable(GL_DEPTH_TEST);

	//register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(pressKey);
	glutKeyboardUpFunc(releaseKey);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseButtonPress);
	glutTimerFunc(10, timer, 1);

	//menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Magenta", 3);
	glutAddMenuEntry("Yellow", 4);
	glutAddMenuEntry("Cyan", 5);
	glutAddMenuEntry("Brown", 6);
	glutAddMenuEntry("Orange", 7);
	glutAddMenuEntry("White", 8);
	glutAddMenuEntry("Black", 9);
	glutAddMenuEntry("Chartreuse", 10);
	glutAddMenuEntry("Coral", 11);
	glutAddMenuEntry("Purple", 12);
	glutAddMenuEntry("Spring Green", 13);
	glutAddMenuEntry("Pink", 14);
	glutAddMenuEntry("Teal", 15);
	glutAddMenuEntry("Salmon", 16);
	glutAddMenuEntry("Olive", 17);
	glutAddMenuEntry("Gold", 18);
	glutAddMenuEntry("Comsilk", 19);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	start_time = time(NULL);

	//start program
	init();
	glutMainLoop();

	//deallocate memory
	free(str);
	free(str2);
	free(keyStates);
	free(xloc);
	free(yloc);
	free(locs);

	return 1;
}