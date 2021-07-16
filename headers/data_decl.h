#pragma once
/*
 * Dependencies : must be included right at the top
*/

#include<stdio.h>

/*declaration of constants and file pointers for params*/

//Event detection event codes
#define HEALTH 0
#define AMMO 1
//#define INVSLOT 2   |in progress
//Frame rate
#define FRAME_RATE (1000/120)  //40 = (1000)/framerate  here, 25 FPS

//Initializing player variables
float speed = 0.0, xPos = 0, yPos = 0, fric = 0.0, time = 0, newTime = 0, universalTime = 0, health = 75, gravity = 0;
float realGravity = 0, jumpAcc = 0, jumpCooldown = 0, masterTime = 0;
int xTrue = 0, yTrue = 0, spaceTrue = 0, gravMode = 0;

//Initializing file system
FILE* physicsInput;
FILE* playerInfo;
FILE* inventoryData;
FILE* colliderData;
