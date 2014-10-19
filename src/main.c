/*
===============================================================================
 Name        : Snake
 Author      : Danijel Sokač
 Version     : 1.0
 Copyright   : Copyright (C)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC13xx.h"
#endif
#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#include "gpio.h"
#include "oled.h"
#include "i2c.h"
#include "ssp.h"
#include "timer32.h"
#include "acc.h"


/*typedef struct {
	uint8_t flag;// b/w |up | down | left | right|
				//	 4  | 3 |   2  |   1  |   0  |
	int8_t preth_x;
	int8_t preth_y;
}snake;*/


volatile uint8_t oled[OLED_DISPLAY_WIDTH - 36][OLED_DISPLAY_HEIGHT] = {{0},{0}};/* b/w |up | down | left | right|
					 	 	 	 	 	 	 	 	 	 	 	 	 	 	   	    4  | 3 |   2  |   1  |   0  |*/
static int8_t x, y, z, xoff, yoff, zoff;
volatile int8_t glava_x = 30, glava_y = 32, rep_x = 28, rep_y = 32;
volatile uint32_t sw3 = 1, msTicks = 0, score = 0, exscore = 0, delay, speed = 1;
static uint8_t buf[10];
static const char *Ascii = "0123456789";


void PIOINT0_IRQHandler(void){
	if(GPIOIntStatus(PORT0, 1)){
		sw3 = !sw3;
		GPIOIntClear(PORT0, 1);
	}
}
void SysTick_Handler(void) {
    msTicks++;
}

/*===============================Konverzija uint-a u string=====================================*/

void uintToString(int32_t value, uint32_t length){
	uint32_t pos = length;

	buf[pos] = '\0';

	do{
		buf[--pos] = Ascii[value % 10];
		value /= 10;
	}while(value && pos);

	while(pos > 0){
		buf[--pos] = ' ';
	}
}

/*======================================Play Snake==============================================*/
void playSnake(void){
	int8_t preth_x, preth_y, oled_x, oled_y, tmp_x, tmp_y, food_x, food_y;

	do{
		food_x = rand() % (OLED_DISPLAY_WIDTH - 36);
		food_y = rand() % OLED_DISPLAY_HEIGHT;
	}while(oled[food_x][food_y] & 16);
	oled_putPixel(food_x, food_y, OLED_COLOR_BLACK);

	while(1){
			/*Pause*/
		while(!sw3){
			oled_putString(62, 50, (uint8_t *) "PAUSE", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		}
		oled_fillRect(62, 50, 93, 57, OLED_COLOR_WHITE);
		exscore = score;
		acc_read(&x, &y, &z);
		x -= xoff;
		y -= yoff;

		preth_x = glava_x;
		preth_y = glava_y;
			/*user right*/
		if(((x < -8)) && !(oled[glava_x][glava_y] & 2)){
			if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_x++;

			if ((oled[glava_x][glava_y] & 16) || (glava_x > (OLED_DISPLAY_WIDTH - 37))){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 17;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*user left*/
		else if(((x > 8)) && !(oled[glava_x][glava_y] & 1)){
			if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			else if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_x--;

			if ((oled[glava_x][glava_y] & 16) || (glava_x < 0)){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 18;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*user up*/
		else if(((y < -8)) && !(oled[glava_x][glava_y] & 4)){
			if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			else if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_y--;

			if ((oled[glava_x][glava_y] & 16) || (glava_y < 0)){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 24;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*user down*/
		else if(((y > 8)) && !(oled[glava_x][glava_y] & 8)){
			if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_y++;

			if ((oled[glava_x][glava_y] & 16) || (glava_y > (OLED_DISPLAY_HEIGHT - 1))){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 20;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*right*/
		else if(oled[glava_x][glava_y] & 1){
			if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_x++;

			if ((oled[glava_x][glava_y] & 16) || (glava_x > (OLED_DISPLAY_WIDTH - 37))){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 17;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*left*/
		else if(oled[glava_x][glava_y] & 2){
			if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			else if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_x--;

			if ((oled[glava_x][glava_y] & 16) || (glava_x < 0)){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 18;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*up*/
		else if(oled[glava_x][glava_y] & 8){
			if(oled[glava_x][glava_y] & 8){
				preth_y++;
			}
			else if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_y--;

			if ((oled[glava_x][glava_y] & 16) || (glava_y < 0)){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 24;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}
			/*down*/
		else if(oled[glava_x][glava_y] & 4){
			if(oled[glava_x][glava_y] & 4){
				preth_y--;
			}
			else if(oled[glava_x][glava_y] & 1){
				preth_x--;
			}
			else if(oled[glava_x][glava_y] & 2){
				preth_x++;
			}
			oled_x = glava_x;
			oled_y = glava_y;
			glava_y++;

			if ((oled[glava_x][glava_y] & 16) || (glava_y > (OLED_DISPLAY_HEIGHT - 1))){
				oled_clearScreen(OLED_COLOR_WHITE);
				oled_putString(21,28, (uint8_t *) "GAME OVER", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
				delay32Ms(1, 1000);
				return;
			}
			oled[glava_x][glava_y] = 20;
			oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		}

		while ((preth_x != rep_x) || (preth_y != rep_y)){
			//oled[oled_x][oled_y] = oled[preth_x][preth_y];

			tmp_x = preth_x;
			tmp_y = preth_y;

			if(oled[preth_x][preth_y] & 1){
				preth_x--;
			}
			else if(oled[preth_x][preth_y] & 2){
				preth_x++;
			}
			else if(oled[preth_x][preth_y] & 4){
				preth_y--;
			}
			else if(oled[preth_x][preth_y] & 8){
				preth_y++;
			}
			else
				break;

			oled_x = tmp_x;
			oled_y = tmp_y;
		}
		//oled[oled_x][oled_y] = oled[preth_x][preth_y];

		if((food_x == glava_x) && (food_y == glava_y)){
			do{
				food_x = rand() % (OLED_DISPLAY_WIDTH - 36);
				food_y = rand() % OLED_DISPLAY_HEIGHT;
			}while(oled[food_x][food_y] & 16);
			oled_putPixel(food_x, food_y, OLED_COLOR_BLACK);
			score++;
			uintToString(score, 5);
			oled_putString(62, 11, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		}
		else{
			oled[rep_x][rep_y] = 0;
			oled_putPixel(rep_x, rep_y, OLED_COLOR_WHITE);
			rep_x = oled_x;
			rep_y = oled_y;
		}
		if (((score % 5) == 0) && (exscore != score)){
			if(delay >= 20){
				delay -= 10;
				speed++;
				uintToString(speed, 5);
				oled_putString(62, 38, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			}
		}

		delay32Ms(1, delay);
	}
}

/*=========================================Glavni Program=========================================*/
int main(void) {
	uint32_t challange = 0, HighScore[3] = {0};
	uint8_t i, j;
	GPIOInit();
	I2CInit((uint32_t)I2CMASTER, 0);
	SSPInit();
	oled_init();
	acc_init();
	init_timer32(1, 10);

	SysTick_Config(SystemCoreClock / 1000);
	if ( !(SysTick->CTRL & (1<<SysTick_CTRL_CLKSOURCE_Msk)) )
	{
		LPC_SYSCON->SYSTICKCLKDIV = 0x08;
	}

		/*sw3*/
	GPIOSetDir(PORT0, 1, 0);
	GPIOSetInterrupt(PORT0, 1, 0, 1, 0);
	GPIOIntEnable(PORT0, 1);
	while(1){
		oled_clearScreen(OLED_COLOR_WHITE);
		oled_putString(33, 2, (uint8_t *) "SNAKE", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		oled_putString(27, 55, (uint8_t *) "Press sw3", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		//oled_putString(30, 55, (uint8_t *) "to play", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		if(challange){
			oled_putString(1, 10, (uint8_t *) "SCORE: ", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			uintToString(score, 8);
			oled_putString(34, 10, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			oled_putString(15, 18, (uint8_t *) "HIGH SCORES", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			if(challange){
				uintToString(HighScore[0], 9);
				oled_putString(18, 26, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			}
			if(challange > 1){
				uintToString(HighScore[1], 9);
				oled_putString(18, 34, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			}
			if(challange > 2){
				uintToString(HighScore[2], 9);
				oled_putString(18, 42, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
			}
		}
		while(sw3){
		}
		sw3 = 1;
		if(!challange){
			srand(msTicks);
		}

		glava_x = 30;
		glava_y = 32;
		rep_x = 28;
		rep_y = 32;
		challange++;
		oled_clearScreen(OLED_COLOR_WHITE);
		oled[glava_x][glava_y] = 17;
		oled[glava_x - 1][glava_y] = 17;
		oled[rep_x][rep_y] = 17;
		oled_putPixel(glava_x, glava_y, OLED_COLOR_BLACK);
		oled_putPixel(glava_x - 1, glava_y, OLED_COLOR_BLACK);
		oled_putPixel(rep_x, rep_y, OLED_COLOR_BLACK);

		score = exscore = 0;
		speed = 1;
		delay = 250;
		acc_read(&xoff, &yoff, &zoff);
		oled_line(60, 0, 60, 63, OLED_COLOR_BLACK);
		oled_putString(62, 3, (uint8_t *) "SCORE", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		uintToString(score, 5);
		oled_putString(62, 11, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		oled_putString(62, 30, (uint8_t *) "SPEED", OLED_COLOR_BLACK, OLED_COLOR_WHITE);
		uintToString(speed, 5);
		oled_putString(62, 38, buf, OLED_COLOR_BLACK, OLED_COLOR_WHITE);

		playSnake();

		for(i = 0; i < (OLED_DISPLAY_WIDTH - 37); i++){
			for(j = 0; j < OLED_DISPLAY_HEIGHT; j++){
				oled[i][j] = 0;
			}
		}

		if(score > HighScore[0]){
			HighScore[2] = HighScore[1];
			HighScore[1] = HighScore[0];
			HighScore[0] = score;
		}
		else if(score > HighScore[1]){
			HighScore[2] = HighScore[1];
			HighScore[1] = score;
		}
		else if(score > HighScore[2]){
			HighScore[2] = score;
		}
	}
	return 0 ;
}
