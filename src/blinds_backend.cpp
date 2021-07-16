/*
 * Blinds Game Engine Version 1.1
 * by siriusBl4ck (Saurav Kale)  (C) 2020
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "Windows.h"    //TODO : Have to remove platform dependency using time.h
#include "../headers/texture_loader.h"
#include "../headers/data_decl.h"
static GLuint texture;
#include "../headers/blinds_UI.h"



//Initializer for collider
int number_of_colliders;

float ratio;

//Collision system
struct trigger
{
	int trigCode;
	float colR, colG, colB;
	float dimX1, dimX2, dimX3, dimX4, dimY1, dimY2, dimY3, dimY4;
	float reflex;
	struct trigger* next;
};

//Singly linked list for collider
typedef struct trigger trigger;
trigger* colliderSystem;

void colliderSetup() {
	trigger* p, * q;
	int i;
	fscanf(colliderData, "#Events[%d]\n", &number_of_colliders);
	if (number_of_colliders < 1) {
		printf("WARNING: Event System disabled |n<1 in colliderData");
		number_of_colliders = 0;
	}
	else {
		colliderSystem = (trigger*)malloc(sizeof(trigger));

		fscanf(colliderData, "eventCode[%d]\n", &(colliderSystem->trigCode));
		fscanf(colliderData, "spriteColor[%f,%f,%f]\n", &(colliderSystem->colR), &(colliderSystem->colG), &(colliderSystem->colB));
		fscanf(colliderData, "spriteDraw[(%f,%f)(%f,%f)(%f,%f)(%f,%f)]\n", &(colliderSystem->dimX1), &(colliderSystem->dimY1), &(colliderSystem->dimX2), &(colliderSystem->dimY2), &(colliderSystem->dimX3), &(colliderSystem->dimY3), &(colliderSystem->dimX4), &(colliderSystem->dimY4));
		fscanf(colliderData, "onBreach : %f\n", &(colliderSystem->reflex));

		colliderSystem->next = NULL;

		q = colliderSystem;

		for (i = 0; i < number_of_colliders - 1; i++) {
			p = (trigger*)malloc(sizeof(trigger));
			fscanf(colliderData, "eventCode[%d]\n", &(p->trigCode));
			fscanf(colliderData, "spriteColor[%f,%f,%f]\n", &(p->colR), &(p->colG), &(p->colB));
			fscanf(colliderData, "spriteDraw[(%f,%f)(%f,%f)(%f,%f)(%f,%f)]\n", &(p->dimX1), &(p->dimY1), &(p->dimX2), &(p->dimY2), &(p->dimX3), &(p->dimY3), &(p->dimX4), &(p->dimY4));
			fscanf(colliderData, "onBreach : %f\n", &(p->reflex));

			p->next = NULL;
			q->next = p;
			q = p;
		}

		printf("Collider System Initialized...\n");
		/*p = colliderSystem;
		for (i = 0; i < number_of_colliders; i++) {
			printf("%d\n", p->trigCode);
			printf("%f, %f, %f\n", p->colR, p->colG, p->colB);
			printf("(%f,%f)(%f,%f)(%f,%f)(%f,%f)\n", (p->dimX1), (p->dimY1), (p->dimX2), (p->dimY2), (p->dimX3), (p->dimY3), (p->dimX4), (p->dimY4));
			printf("%f", p->reflex);
			p = p->next;
		}*/
	}
}
//Note: add the box for box collider for irregularly shaped stuff
void spawnCollider() {
	int i;
	//static float texAnim = 0;
	//(texAnim < 0.7) ? texAnim+=0.01 : texAnim = 0;  | TODO: Animation
	//int boxX, boxY;  |in progress (box collider)
	trigger* p = colliderSystem;
	for (i = 0; i < number_of_colliders; i++) {
		glColor3f(p->colR, p->colG, p->colB);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1);
		glVertex3f(p->dimX1, p->dimY1, 0);
		glTexCoord2d(0.3, 1);
		glVertex3f(p->dimX2, p->dimY2, 0);
		glTexCoord2d(0.3, 0.8);
		glVertex3f(p->dimX3, p->dimY3, 0);
		glTexCoord2d(0, 0.8);
		glVertex3f(p->dimX4, p->dimY4, 0);
		glEnd();
		p = p->next;
	}
	glColor3f(1, 1, 1);

	//Collider Logic
	p = colliderSystem;
	for (i = 0; i < number_of_colliders; i++) {
		if ((xPos > (p->dimX1 - 0.25)) && (xPos < (p->dimX3 + 0.25))) {
			if ((yPos > (p->dimY1 - 0.25)) && (yPos < (p->dimY3+0.25))) {
				printf("COLLISION | Health : %f\n", health);
				health += p->reflex;
			}
		}
		p = p->next;
	}
}

void reshape(int w, int h)
{
	if (h == 0) h = 1;
	
	ratio = w * 1.0 / h;

	screen_height = h;
	screen_width = w;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(60, 1, 1, 10);
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(130, ratio, 1, 10);

	gluLookAt(0, 0, -2, 
		0, 0, 2, 
		0, 1, 0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	time += 0.01;
	masterTime += 0.01;
	static float jumpHit = 0;
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	spawnCollider();

	glPushMatrix();

	glTranslatef(xPos, yPos, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (masterTime < 2) {
		texture = rawTextureLoad("sprites/spriteSheet4.raw", 300, 300);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex3f(0.25, 0.25, 0.0);
	glTexCoord2d(0.3, 0); glVertex3f(-0.25, 0.25, 0.0);
	glTexCoord2d(0.3, 0.3); glVertex3f(-0.25, -0.25, 0.0);
	glTexCoord2d(0, 0.3); glVertex3f(0.25, -0.25, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if (xTrue == 1) {
		if (speed - fric * (time * time) > 0) {
			xPos += speed - fric * (time * time);
		}
	}
	else if (xTrue == -1) {
		if (speed - fric * (time * time) > 0) {
			xPos -= speed - fric * (time * time);
		}
	}
	else if (yTrue == 1) {
		if (speed - fric * (time * time) > 0) {
			yPos += speed - fric * (time * time);
		}
	}
	else if (yTrue == -1) {
		if (speed - fric * (time * time) > 0) {
			yPos -= speed - fric * (time * time);
		}
	}
	/*if (gravMode && (spaceTrue == 1)) {
		//if ((masterTime - jumpHit) < jumpCooldown*1000) {
			yPos += 0.5*jumpAcc * (masterTime - jumpHit)*(masterTime - jumpHit);
			printf("%d, %d, %d\n", masterTime, jumpHit, masterTime - jumpHit);
			/*}
		/*else if ((masterTime - jumpHit) > (2 * jumpCooldown)) {
			jumpHit = masterTime;
		}
	}
	else if (gravMode && (spaceTrue == 0)) {
		jumpHit = masterTime;
	}*/
	//Gravity
	yPos -= 0.5 * gravity * (masterTime * masterTime);

	glPopMatrix();

	showUI();

	glFlush();

	printf("%f, %f\n", xPos, yPos);

	glutSwapBuffers();
}

//Timekeeper
void khronos() {
	universalTime = GetTickCount();
	float currentTime = GetTickCount();
	if ((currentTime - newTime) > FRAME_RATE) {
		//printf("%d\n", universalTime);
		newTime = GetTickCount();
		render();
	}
}

void keyboardControls(unsigned char key, int x, int y) {
	//printf("%f, %f\n", xPos, yPos);
	switch (key) {
	case 97:
		xTrue = 1;
		if (gravMode) {
			yTrue = 0;
		}
		time = 0;
		break;
	case 100:
		xTrue = -1;
		if (gravMode == 0) {
			yTrue = 0;
		}
		time = 0;
		break;
	case 119:
		if (gravMode == 0) {
			yTrue = 1;
			xTrue = 0;
			time = 0;
		}
		break;
	case 115:
		if (gravMode == 0) {
			yTrue = -1;
			xTrue = 0;
			time = 0;
		}
		break;
	case 32: spaceTrue = 1; break;
	case 27: exit(0);
	//default: xPos = 0; yPos = 0;
	}
}

void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (selectedItem->prev != NULL) {
			selectedItem = selectedItem->prev;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (selectedItem->next != NULL) {
			selectedItem = selectedItem->next;
		}
		break;
	}
}

int main(int argc, char* argv[])
{
	//Get params from file data
	physicsInput = fopen("params/physics.txt", "r"); //Gravity, friction, movement speed, 
	playerInfo = fopen("params/playerInfo.txt", "r");
	inventoryData = fopen("params/inventorySettings.txt", "r");
	colliderData = fopen("params/colliderData.txt", "r");
	//Read physics data
	fscanf(physicsInput, "Movement Speed : %f\n", &speed);
	fscanf(physicsInput, "Friction : %f\n", &fric);
	fscanf(physicsInput, "Gravity : %f\n", &realGravity);
	if (realGravity > 0) {
		gravMode = 1;
		gravity = realGravity;
		fscanf(physicsInput, "Jump acceleration : %f", &jumpAcc);
		fscanf(physicsInput, "Jump cooldown : %f", &jumpCooldown);
	}
	else {
		gravMode = 0;
		printf("Gravity off | Gravity<=0 in physics\n");
	}
	//Read player stats
	fscanf(playerInfo, "xPos : %f\n", &xPos);
	fscanf(playerInfo, "yPos : %f\n", &yPos);
	fscanf(playerInfo, "Health : %f\n", &health);
	//Read textures
	texture = rawTextureLoad("sprites/spriteSheet1.raw", 256, 256);
	//Read inventory system data
	inventorySetup();
	//Read collider system data
	colliderSetup();
	//Start counting time
	universalTime = GetTickCount();
	newTime = GetTickCount();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Blinds Game Engine V1.1");
	//glutFullScreen();

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutIdleFunc(khronos);
	glutKeyboardFunc(keyboardControls);
	glutSpecialFunc(specialKeys);

	glutMainLoop();

	return 0;
}
