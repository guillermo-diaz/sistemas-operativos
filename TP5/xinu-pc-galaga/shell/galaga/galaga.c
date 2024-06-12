#include <xinu.h>
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"

extern unsigned char tecla_actual;
typedef unsigned short u16;
#define RGB(r, g, b) (r | (g << 5) | (b << 10))

// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
*/
//#define BUTTONS *(volatile unsigned int *)0x4000130

#define BUTTON_A	0x24 /*z*/
#define BUTTON_B	0x25 
#define BUTTON_SELECT	0x03
#define BUTTON_START	0x2c
#define BUTTON_RIGHT	0x20 /*D*/
#define BUTTON_LEFT	0x1e	/*A*/
#define BUTTON_UP	0x11 /*W*/
#define BUTTON_DOWN 0x1f /*S*/
#define BUTTON_R	'1'
#define BUTTON_L	'2'
#define BUTTON_ESC		0x01 /*ESC*/
#define BUTTON_LEFT_REL 0xa4
#define BUTTON_RIGHT_REL 0xa0
#define BUTTON_A_REL 0x9e


#define KEY_DOWN_NOW(key)  (tecla_actual == key)

//variable definitions
#define playerspeed 2
#define enemyspeed 1
#define fastXSpeed 3
#define fastYSpeed 2


void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void delay_galaga();
void waitForVBlank();

//helpers
void initialize();
void drawEnemies();
void endGame();
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY);

pid32 pid_game;
pid32 pid_puntaje;
pid32 pid_control;
pid32 pid_teclado;
unsigned char tecla_actual; //agrego tecla_actual xq la elimine del 

#define DAMAGED 1
#define SCORE 2
#define RESET 3

int puntuacion;
int vidas;
int teclas[3];

int flag_shoot;

//objects
struct Players {
	volatile u16 playerX;
	volatile u16 playerY;
};
struct Enemy {
	volatile u16 enemyX;
	volatile u16 enemyY;
};
struct FastEnemy {
	volatile u16 fastX;
	volatile u16 fastY;
};

int shoots[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int curr_shot = 0;
#define N_SHOOTS 10


int galaga_game(void) {
	//easy enemy wave set setup
	struct Enemy easyEnemies[9];
	for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyX = (28*a);
		easyEnemies[a].enemyY = 0;
	} 
	easyEnemies[1].enemyX = 240;
	easyEnemies[4].enemyX = 240;
	easyEnemies[8].enemyX = 240;
	//difficult enemies setup
	struct Enemy hardEnemies[9];
	for (int a = 0; a < 9; a++) {
		hardEnemies[a].enemyX = (28*a);
		hardEnemies[a].enemyY = 160;
	} 
	hardEnemies[3].enemyX = 240;
	hardEnemies[6].enemyX = 240;
	//player setup
	struct Players player;
	player.playerX = 120;
	player.playerY = 136;
	//fast enemy "boss" setup
	struct FastEnemy fast;
	fast.fastX = 0;
	fast.fastY = 30;

	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	//initalize title screen
	print_text_on_vga(10, 20, "GALAGA ");
	drawImage3(0, 0, 240, 160, titlescreen);
	while(1) {
		if (KEY_DOWN_NOW(BUTTON_START)) {
			break;
		}
	}	
	//start black screen for drawing
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}	
	while(1) {
		//go back to title screen if select button is pressed
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			//initialize();
			galaga();
		}
		//player shots 
		if (teclas[2] == 1) {
			if (shoots[curr_shot] == 0) {
				shoots[curr_shot] = 136*240 + player.playerX+9; /* 24 widht player */
				curr_shot++;
				if (curr_shot >= N_SHOOTS)
					curr_shot = 0;
			};
		
		}
		//player movement input
		if (teclas[0] == 1 && (player.playerX > 0)) {
			player.playerX -= playerspeed;
		}
		if (teclas[1] == 1 && (player.playerX <= 216)) {
			player.playerX += playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_UP) && (player.playerY > 25)) {
			player.playerY -= playerspeed;
		}
		if (KEY_DOWN_NOW(BUTTON_DOWN) && (player.playerY <= 136)) {
			player.playerY += playerspeed;
		}
		waitForVBlank();
		sleepms(20);

		//terminar
		if (KEY_DOWN_NOW(BUTTON_ESC)) {
			send(pid_control, -1);
		}
		//draw player
		drawImage3(player.playerX, player.playerY, 24, 24, playerImage);
		drawHollowRect(player.playerX - 1, player.playerY - 1, 26, 26, BLACK);
		drawHollowRect(player.playerX - 2, player.playerY - 2, 28, 28, BLACK);
		//draw easy enemies with downward movement
		for (int a = 0; a < 9; a++) {
			easyEnemies[a].enemyY += enemyspeed;
			drawImage3(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, enemy);
			if (collision(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, player.playerX, player.playerY)) {
				drawRect(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, BLACK); // Elimina al enemigo de la pantalla
                easyEnemies[a].enemyY = 0; // Restablece la posición del enemigo
				endGame();
				
			}	
			if (easyEnemies[a].enemyY >= 160) {
				easyEnemies[a].enemyY = 0;
			}		
		}

		for (int i = 0; i < N_SHOOTS; i++) {
			if (shoots[i] != 0) {
				drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
				drawImage3((shoots[i] % 240), (shoots[i] / 240), 5, 5, shoot);
				shoots[i] = shoots[i]-240*4;
				if (shoots[i] <=0)   shoots[i]=0;


				// para controlar si ya se envio el mensaje de colision se reinicia en cada bucle del galaga
				int collisionDetected = 0;

				// Verifica si hay colision con un enemigo
				for (int j = 0; j < 9; j++){
					if (collision(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 15, 15, shoots[i] % 240, shoots[i] / 240)){
						if (!collisionDetected){
							// Si hay colision y no se ha enviado el mensaje, lo envia
							send(pid_puntaje, SCORE);
							collisionDetected = 1; // Marca que ya se envio el mensaje
						}

						drawRect(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 20, 20, BLACK);
						drawRect((shoots[i] % 240), (shoots[i] / 240) + 4, 5, 5, BLACK);
						easyEnemies[j].enemyY = 0;
						shoots[i] = 0;
					}
				}
			}

			
			
			
		}
		
		//draw hard enemies
		for (int a = 0; a < 9; a++) {
			hardEnemies[a].enemyY += enemyspeed;
			drawImage3(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, enemy);
			if (collision(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, player.playerX, player.playerY)) {
				drawRect(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, BLACK); // Elimina al enemigo de la pantalla
                hardEnemies[a].enemyY = 0; // Restablece la posición del enemigo
				endGame();
			}	
			if (hardEnemies[a].enemyY >= 228) {
				hardEnemies[a].enemyY = 0;
			}
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}	
			//space enemies apart
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}		
			if ((easyEnemies[a].enemyY >= 120) && (hardEnemies[a].enemyY >=170)) {
				hardEnemies[a].enemyY = 160;
			}							
		}	
		//draw fast enemy
		drawImage3(fast.fastX, fast.fastY, 15, 15, boss);
		drawHollowRect(fast.fastX - 1, fast.fastY - 1, 17, 17, BLACK);
		drawHollowRect(fast.fastX - 2, fast.fastY - 2, 19, 19, BLACK);
		if(collision(fast.fastX, fast.fastY, 15, 15, player.playerX, player.playerY)) {
			drawRect(fast.fastX, fast.fastY, 15, 15, BLACK); // Elimina al enemigo de la pantalla
            fast.fastY = 0; // Restablece la posición del enemigo
			endGame();
		}		
		// fast.fastX += fastXSpeed;
		// fast.fastY += fastYSpeed;
		if (fast.fastX >= 240) {
			fast.fastX = 0;
		}
		if (fast.fastY >= 200) {
			fast.fastY = player.playerY - 20;
		}
	}	
	return 0;
}


int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY) {
	//check if bottom right corner of enemy is within player
	if (((enemyX + enemyWidth) > playerX) && ( (enemyY + enemyHeight) 
		> playerY ) &&  ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyY + enemyWidth) < (playerY + 24))) {
		return 1;
	} 
	//check bottom left corner of enemy
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& ((enemyY + enemyHeight) > playerY)
		&& ((enemyY + enemyHeight) < (playerY + 24))
		) {
		return 1;
	}
	//check top left corner
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	//check top right corner
	if ( ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyX + enemyWidth) > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	return 0;
}

void endGame() {
	send(pid_puntaje, DAMAGED);

	//start Game Over State
	if (vidas <= 0){
		
		drawImage3(0, 0, 240, 160, gameover);
		drawHollowRect(0, 0, 240, 160, WHITE);
		while(1) {
			if (KEY_DOWN_NOW(BUTTON_SELECT) || KEY_DOWN_NOW(BUTTON_START)) {
				send(pid_puntaje, RESET);
				galaga();
			}
			if (KEY_DOWN_NOW(BUTTON_ESC)){
				send(pid_control, -1);
			}
		}

	}
}

void puntaje() {
	char buffer[32];
	puntuacion = 0;
	vidas = 3;
	while(1){
 		sprintf(buffer, "Vidas: %d    Score: %d        ", vidas, puntuacion);
		print_text_on_vga(4, 164, buffer);

		switch(receive()){
			case DAMAGED: vidas--; break;	
			case SCORE: puntuacion++; break;
			case RESET: puntuacion = 0; vidas = 3; break;
			default: break;
		}
	}
}

void teclado(){
	open(KEYBOARD, NULL, NULL);
    while (1) {
        read(KEYBOARD, &tecla_actual, 1);
		switch(tecla_actual){
			case BUTTON_LEFT: teclas[0] = 1; break;
			case BUTTON_RIGHT: teclas[1] = 1; break;
			case BUTTON_A: teclas[2] = 1; break;
			case BUTTON_A_REL: teclas[2] = 0; break;
			case BUTTON_LEFT_REL: teclas[0] = 0; break;
			case BUTTON_RIGHT_REL: teclas[1] = 0; break;
		}
		printf("TECLA: %d\n", tecla_actual);
    }  
	close(KEYBOARD);
}





int galaga(void){
	pid_game = create(galaga_game, 1024, 20, "Galaga Game", 0);
	pid_puntaje = create(puntaje, 1024, 20, "Galaga Score", 0);
	pid_teclado = create(teclado, 1024, 20, "Galaga Keyboard",0);
	pid_control = currpid;
	resume(pid_game);
	resume(pid_puntaje);
	resume(pid_teclado);
	receive();

	kill(pid_teclado);
	kill(pid_game);
	kill(pid_puntaje);

	return 0;
}
