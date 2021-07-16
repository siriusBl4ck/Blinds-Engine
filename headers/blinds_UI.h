#pragma once
/*
 * Dependencies : must be included right at the top
*/

//Dimension of health bar
#define LENGTH_OF_BAR 5
#define WIDTH_OF_BAR 0.5
//Dimension of inventory bar
#define HEIGHT_OF_INV 1
#define HEIGHT_OF_INFO 1.5
#define LENGTH_OF_INFO 5

//Initializer for UI
int screen_width, screen_height;
float healthBar_x = 5, healthBar_y = 4;
float inventoryBar_x = 5, inventoryBar_y = -4;
float selectedItemInfo_x = -5.6, selectedItemInfo_y = 4.2;
int number_of_items;

static GLuint Infotexture;

//Struct for items in inventory
struct item
{
	int itemCode;
	char itemName[25];
	int instances;
	struct item* prev, * next;
};

//Doubly Linked List for inventory
typedef struct item item;
item* inventory, * selectedItem, * temp;

//Sets up the DLL for Inventory Management
void inventorySetup() {
	item* p, * q;
	int i;
	fscanf(inventoryData, "%d\n", &(number_of_items));
	if (number_of_items < 1) {
		number_of_items = 0;
		printf("WARNING: Inventory disabled | n<1 in inventorySettings file\n");
	}
	else {
		inventory = (item*)malloc(sizeof(item));
		fscanf(inventoryData, "%d\n", &(inventory->itemCode));
		fscanf(inventoryData, "%[^\n]s\n", (inventory->itemName));
		fscanf(inventoryData, "%d\n", &(inventory->instances));
		inventory->next = NULL;
		inventory->prev = NULL;

		q = inventory;

		for (i = 0; i < number_of_items - 1; i++) {
			p = (item*)malloc(sizeof(item));

			fscanf(inventoryData, "%d\n", &(p->itemCode));
			fscanf(inventoryData, "%[^\n]s\n", (p->itemName));
			fscanf(inventoryData, "%d\n", &(p->instances));

			p->prev = q;
			q->next = p;
			p->next = NULL;
			q = p;
		}

		selectedItem = inventory;
		temp = inventory;

		printf("Inventory loaded successfully...\n");
		/*p = inventory;
		for (i = 0; i < number_of_items; i++) {
			printf("%d, %s\n", p->itemCode, p->itemName);
			p = p->next;
		}*/

	}
}

//UI Renderer
void showUI() {
	int i = 0, offset = 1, len;
	char instances[25];
	//printf("Screen : %d, %d\n", screen_width, screen_width);
	//Health Bar Background
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(healthBar_x, healthBar_y, 0);
	glVertex3f(healthBar_x - LENGTH_OF_BAR, healthBar_y, 0);
	glVertex3f(healthBar_x - LENGTH_OF_BAR, healthBar_y - WIDTH_OF_BAR, 0);
	glVertex3f(healthBar_x, healthBar_y - WIDTH_OF_BAR, 0);
	//Health Bar Foreground
	glColor3f(0, 1, 0);
	glVertex3f(healthBar_x, healthBar_y, 0);
	glVertex3f(healthBar_x, healthBar_y - WIDTH_OF_BAR, 0);
	glVertex3f(-LENGTH_OF_BAR * (health / 100)+ healthBar_x, healthBar_y - WIDTH_OF_BAR, 0);
	glVertex3f(-LENGTH_OF_BAR * (health / 100) + healthBar_x, healthBar_y, 0);
	glEnd();
	glFlush();
	//Inventory bar
	
	temp = inventory;
	for (i = 0; i < (number_of_items * offset); i += offset) {
		if (selectedItem->itemCode == temp->itemCode) {
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_QUADS);
			glTexCoord2d(0.62, 1);
			glVertex3f(inventoryBar_x - i, inventoryBar_y, 0);
			glTexCoord2d(0.88, 1);
			glVertex3f(inventoryBar_x - i - offset + 0.05, inventoryBar_y, 0);
			glTexCoord2d(0.88, 0.75);
			glVertex3f(inventoryBar_x - i - offset + 0.05, inventoryBar_y + HEIGHT_OF_INV, 0);
			glTexCoord2d(0.62, 0.75);
			glVertex3f(inventoryBar_x - i, inventoryBar_y + HEIGHT_OF_INV, 0);
			glEnd();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
		else {
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_QUADS);
			glTexCoord2d(0, 1);
			glTexCoord2d(0.4, 1);
			glVertex3f(inventoryBar_x - i, inventoryBar_y, 0);
			glTexCoord2d(0.55, 1);
			glVertex3f(inventoryBar_x - i - offset + 0.05, inventoryBar_y, 0);
			glTexCoord2d(0.55, 0.8);
			glVertex3f(inventoryBar_x - i - offset + 0.05, inventoryBar_y + HEIGHT_OF_INV, 0);
			glTexCoord2d(0.4, 0.8);
			glVertex3f(inventoryBar_x - i, inventoryBar_y + HEIGHT_OF_INV, 0);
			glEnd();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
		temp = temp->next;
		//glVertex3f(/*-(screen_width / 1000) + */inventoryBar_x - i - 0.05, inventoryBar_y + 0.05, 0);
		//glVertex3f(/*-(screen_width / 1000) + */inventoryBar_x - i - offset + 0.05 + 0.05, inventoryBar_y + 0.05, 0);
		//glVertex3f(/*-(screen_width / 1000) + */inventoryBar_x - i - offset + 0.05 + 0.05, inventoryBar_y + HEIGHT_OF_INV - 0.05, 0);
		//glVertex3f(/*-(screen_width / 1000) + */inventoryBar_x - i - 0.05, inventoryBar_y + HEIGHT_OF_INV - 0.05, 0);
	
	}
	glFlush();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Equipped Item info
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0.4);
	glVertex3f(selectedItemInfo_x, selectedItemInfo_y, 0);
	glTexCoord2d(0, 0.4);
	glVertex3f(selectedItemInfo_x + LENGTH_OF_INFO, selectedItemInfo_y, 0);
	glTexCoord2d(0, 0.6);
	glVertex3f(selectedItemInfo_x + LENGTH_OF_INFO, selectedItemInfo_y - HEIGHT_OF_INFO, 0);
	glTexCoord2d(1, 0.6);
	glVertex3f(selectedItemInfo_x, selectedItemInfo_y - HEIGHT_OF_INFO, 0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0);
	glRasterPos3f(selectedItemInfo_x + LENGTH_OF_INFO - 1.5, selectedItemInfo_y - 0.7, 0);
	len = strlen(selectedItem->itemName);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (selectedItem->itemName)[i]);
	}
	glRasterPos3f(selectedItemInfo_x + LENGTH_OF_INFO - 1.5, selectedItemInfo_y - 0.9, 0);
	sprintf(instances, "Ammo: %d", selectedItem->instances);
	len = strlen(instances);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, instances[i]);
	}
	glColor3f(1, 1, 1);
}