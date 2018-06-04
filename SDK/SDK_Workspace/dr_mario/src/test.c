#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "vga_periph_mem.h"
#include "xil_exception.h"
#include "xintc.h"
#include <math.h>
#include <stdbool.h>

#define TEST_MODE 0




#define ZNAK_KOCKICA        0x1B
#define ZNAK_KOCKICA1       0x26
#define VERTIKALNA_IVICA 	0x2D
#define HORIZONTALNA_IVICA	0x3C
#define UGAO_GORNJI_DESNI	0x3D
#define UGAO_GORNJI_LEVI	0x3B
#define UGAO_DONJI_LEVI		0x2F
#define UGAO_DONJI_DESNI	0x3A
#define BACKGROUND_ZNAK		0x00

#define LOW 	40
#define MIDDLE  27
#define HIGH    14



XIntc Intc;
enum Stanja {
		ON,
		OFF,
		PADANJE,
		SREDJIVANJE,
		SREDJIVANJE_PADANJE,
		KRAJ
	} stanje;

unsigned char sNEXT[] = "NEXT";
//unsigned char sDR_MARIO[] = "DR MARIO";
unsigned char sLEVEL[] = "LEVEL";
unsigned char sPlayer2[] = "PLAYER2";
unsigned char sPlayer1[] = "PLAYER1";
unsigned char sSCORE1[] = "SCORE1";
unsigned char sSCORE2[] = "SCORE2";

unsigned char backround[30][40]={
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,0,0,  0,0,0,0,0,  0,0,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,0,0,  0,0,0,0,0,  0,0,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

				{1,1,1,1,1,  2,2,2,2,2,  2,2,2,2,2,  1,1,1,1,1,  1,1,1,1,1,  2,2,2,2,2,  2,2,2,2,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},

				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,2,2,2,2,  2,2,2,2,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},

				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  2,2,2,2,2,  2,2,2,2,2,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},

				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1,  1,1,1,1,1,  2,0,0,0,0,  0,0,0,0,2,  1,1,1,1,1},
				{1,1,1,1,1,  2,2,2,2,2,  2,2,2,2,2,  1,1,1,1,1,  1,1,1,1,1,  2,2,2,2,2,  2,2,2,2,2,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

				{1,1,1,1,1,  0,0,0,0,0,  0,0,0,0,1,  1,1,1,1,1,  1,1,1,1,1,  1,0,0,0,0,  0,0,0,0,0,  1,1,1,1,1},
				{1,1,1,1,1,  0,0,0,0,0,  0,0,0,0,1,  1,0,0,0,0,  0,0,0,0,1,  1,0,0,0,0,  0,0,0,0,0,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,0,0,0,0,  0,0,0,0,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
				{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

};




u8 tmp_table[16][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},

};



u8 table1[16][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},

};

u8 table2[16][8] = {
	{0,0,3,3,3,0,0,0},
	{0,0,0,3,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},

};

u8 table_next[4][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},

};




typedef enum {P_O, P_I, P_S, P_Z, P_L, P_J, P_T} pieces_t;
typedef enum {R_0, R_1, R_2, R_3} rotation_t;
void drawPeaces(void);
Xuint32 addr = XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR;

void drawSign(int x, int y, int boja, int znak){
	set_cursor(y*40+x);
	print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
}


void drawTable(u8 table[16][8], int table_x, int table_y){
	for(int y = 0; y < 16 ; y++){
		int out_y = y + table_y;
			for(int x = 0; x < 8; x++){
				int out_x = x + table_x;
				switch(table[y][x]){
					case 0: drawSign(out_x, out_y, 2, BACKGROUND_ZNAK);    break;
					case 2: drawSign(out_x, out_y, 2, ZNAK_KOCKICA)   ;    break;
					case 3: drawSign(out_x, out_y, 2, ZNAK_KOCKICA1)   ;    break;
				}
			}
	}
}

void drawTable_next(u8 table[4][4], int table_x, int table_y){
	for(int y = 0; y < 4 ; y++){
		int out_y = y + table_y;
			for(int x = 0; x < 4; x++){
				int out_x = x + table_x;
				switch(table[y][x]){
					case 0: drawSign(out_x, out_y, 2, BACKGROUND_ZNAK);    break;
					case 2: drawSign(out_x, out_y, 2, ZNAK_KOCKICA)   ;    break;
				}
			}
	}
}



void drawMap(){
	drawTable(tmp_table, 6, 6);// MIJENJALI
	drawTable(table1, 6+8+12, 6);
}


void drawPeace(int table_x, int table_y, int x, int y, pieces_t piece, rotation_t rot) {
	int boja = 3;
	x += table_x;
	y += table_y;

//	for (int y_table = -1; y_table < 15; y_table++){
//		for (int x_table = 6; x_table < 14; x_table++){
//			tmp_table[y_table][x_table] = 2;
//
//		}
//	}

	switch (piece) {
	case P_O:
		drawSign(x, y, boja, ZNAK_KOCKICA);
		drawSign(x-1, y, boja, ZNAK_KOCKICA);
		drawSign(x, y-1, boja, ZNAK_KOCKICA);
		drawSign(x-1, y-1, boja, ZNAK_KOCKICA);

/*		tmp_table[y-7][x+2] = 2;
		tmp_table[y-7][x+2-1] = 2;
		tmp_table[y-7-1][x+2] = 2;
		tmp_table[y-7-1][x+2-1] = 2;*/

		break;

	case P_I:
		switch(rot){
		case R_0:
			drawSign(x-2, y, boja, ZNAK_KOCKICA);
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
//			tmp_table[y-7][x+2-2] = 2;
//			tmp_table[y-7][x+2-1] = 2;
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7][x+2+1] = 2;

			break;
		case R_1:
			drawSign(x, y-2, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_Z:
		switch(rot){
		case R_0:
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x-1, y-1, boja, ZNAK_KOCKICA);
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7][x+2+1] = 2;
//			tmp_table[y-7-1][x+2] = 2;
//			tmp_table[y-7-1][x+2-1] = 2;

			break;
		case R_1:
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y-1, boja, ZNAK_KOCKICA);
			break;
		}
		break;

	case P_S:
		switch(rot){
		case R_0:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y-1, boja, ZNAK_KOCKICA);
//			tmp_table[y-7][x+2-1] = 2;
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7-1][x+2] = 2;
//			tmp_table[y-7-1][x+2+1] = 2;

			break;
		case R_1:
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y+1, boja, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_J:
		switch(rot){
		case R_0:
			drawSign(x-1, y-1, boja, ZNAK_KOCKICA);
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
//			tmp_table[y-7-1][x+2-1] = 2;
//			tmp_table[y-7][x+2-1] = 2;
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7][x+2+1] = 2;

			break;
		case R_1:
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y-1, boja, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y+1, boja, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSign(x-1, y+1, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			break;

		}
		break;


	case P_L:
		switch(rot){
		case R_0:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y-1, boja, ZNAK_KOCKICA);
//			tmp_table[y-7][x+2-1] = 2;
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7][x+2+1] = 2;
//			tmp_table[y-7-1][x+2+1] = 2;

			break;
		case R_1:
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y+1, boja, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x-1, y+1, boja, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSign(x-1, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_T:
		switch(rot){
		case R_0:
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
//			tmp_table[y-7-1][x+2] = 2;
//			tmp_table[y-7][x+2] = 2;
//			tmp_table[y-7][x+2-1] = 2;
//			tmp_table[y-7][x+2+1] = 2;

			break;
		case R_1:
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x+1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSign(x-1, y, boja, ZNAK_KOCKICA);
			drawSign(x, y-1, boja, ZNAK_KOCKICA);
			drawSign(x, y, boja, ZNAK_KOCKICA);
			drawSign(x, y+1, boja, ZNAK_KOCKICA);
			break;
		}
		break;


	}
}

int peace0_x = 4;
int peace0_y = 4;
pieces_t piece;
int start_flag=1;



void drawPeaceToTable(u8 tmp_table1[16][8],u8 tbl1[16][8]){ // Kopirali u table ono sto nam je u temp table koju inace iscrtavamo, prebacili u table sve na dvojke, ocistili temp radi naredne itearcije i prebacili iz table u temp dvojke

	int i;


	for (int y_table = 0;  y_table < 16; y_table++){
		for (int x_table = 0; x_table < 8; x_table++){
			table1[y_table][x_table] = tmp_table[y_table][x_table];

			if(tmp_table[y_table][x_table] != 0)
				xil_printf("aaaa = %d\n\r", tmp_table[y_table][x_table]);
		}
		//xil_printf("\n\r");
	}

	for (int y_table = 0; y_table < 16; y_table++){
		for (int x_table = 0; x_table < 8; x_table++){
			if (table1[y_table][x_table] == 2){
				table1[y_table][x_table] = 3;
			}
		}
	}

	for (int y_table = 0; y_table < 16; y_table++){
		for (int x_table = 0; x_table < 8; x_table++){
			tmp_table[y_table][x_table] = 0;

		}
	}


	for (int y_table = 0; y_table < 16; y_table++){
		for (int x_table = 0; x_table < 8; x_table++){
			tmp_table[y_table][x_table] = table1[y_table][x_table];

		}
	}

#if 0
	for(i = 0; i < 8; i++) {
			xil_printf("%d", tbl1[peace0_y][i]);
		}
		xil_printf("\n\r");
#endif
}

bool testCollision(u8 table[16][8]){
	int i;
	if (peace0_y == 19 || table[peace0_y][peace0_x] == 3){ // ovaj drugi uslov je stavljao ispod ifa

			return true;
		}
		return false;
}

void fallPeaces(void){
	peace0_y++;
//	drawPeaceToTable(tmp_table,table1);

	if (testCollision(tmp_table)){
		xil_printf("tmp_tbl=%d\n\r", tmp_table[19][4]);
		//drawPeaceToTable(tmp_table, table1);
		table1[peace0_y][peace0_x] = tmp_table[peace0_y][peace0_x];

		xil_printf("table1=%d\n\r", table1[19][4]);
		piece=rand()%7;
		peace0_y=4;
		drawNextInTable();
	}
	else{

	}


}

void testPeaces(void) {
	drawPeace(0, 0, 4, 4, P_O, R_0);
	drawPeace(5, 0, 4, 4, P_I, R_0);
	drawPeace(10, 0, 4, 4, P_I, R_1);
	drawPeace(15, 0, 4, 4, P_S, R_0);
	drawPeace(20, 0, 4, 4, P_S, R_1);
	drawPeace(25, 0, 4, 4, P_Z, R_0);
	drawPeace(30, 0, 4, 4, P_Z, R_1);
	drawPeace(0, 5, 4, 4, P_J, R_0);
	drawPeace(5, 5, 4, 4, P_J, R_1);
	drawPeace(10, 5, 4, 4, P_J, R_2);
	drawPeace(15, 5, 4, 4, P_J, R_3);
	drawPeace(20, 5, 4, 4, P_L, R_0);
	drawPeace(25, 5, 4, 4, P_L, R_1);
	drawPeace(30, 5, 4, 4, P_L, R_2);
	drawPeace(0, 10, 4, 4, P_L, R_3);
	drawPeace(5, 10, 4, 4, P_T, R_0);
	drawPeace(10, 10, 4, 4, P_T, R_1);
	drawPeace(15, 10, 4, 4, P_T, R_2);
	drawPeace(20, 10, 4, 4, P_T, R_3);


}
void fill3(pieces_t piece, u8 table[16][8],int x_table, int y_table){

	switch (piece){

		case P_O:
				table[y_table][x_table] = 3;
				table[y_table][x_table-1] = 3;
				table[y_table-1][x_table] = 3;
				table[y_table-1][x_table-1] = 3;
				break;
		case P_I:
				table[y_table][x_table-2] = 3;
				table[y_table][x_table-1] = 3;
				table[y_table][x_table] = 3;
				table[y_table][x_table+1] = 3;
				break;

		case P_Z:
				table[y_table][x_table] = 3;
				table[y_table][x_table+1] = 3;
				table[y_table-1][x_table] = 3;
				table[y_table-1][x_table-1] = 3;
				break;

		case P_S:
				table[y_table][x_table-1] = 3;
				table[y_table][x_table] = 3;
				table[y_table-1][x_table] = 3;
				table[y_table-1][x_table+1] = 3;
				break;

		case P_J:
				table[y_table-1][x_table-1] = 3;
				table[y_table][x_table-1] = 3;
				table[y_table][x_table] = 3;
				table[y_table][x_table+1] = 3;
				break;

		case P_L:
				table[y_table][x_table-1] = 3;
				table[y_table][x_table] = 3;
				table[y_table][x_table+1] = 3;
				table[y_table-1][x_table+1] = 3;
				break;
		case P_T:
				table[y_table-1][x_table] = 3;
				table[y_table][x_table] = 3;
				table[y_table][x_table-1] = 3;
				table[y_table][x_table+1] = 3;
				break;

	}

}



void drawPeaces(void) {

	drawPeace(6, 3, peace0_x, peace0_y, piece, R_0);

}

void drawNext(pieces_t piece, rotation_t rot){
		drawTable_next(table_next,16,14);
		drawPeace(18, 15, 0, 0, piece, R_0);
	}

void drawNextInTable(void){

	drawNext(piece, R_0);
	for(int i = 0; i<10000;i++){}
	drawPeace(6, 3, peace0_x, peace0_y, piece, R_0);
	//fill3(piece, tmp_table, peace0_x, val);


}




// ----------------------------------- CURRENT GAME STATISTICS -------------------------------
// it draws game statistic: current level, number of viruses, current game speed and current score
void drawGameState(){
/*
	//Score1
	set_cursor(27*40 + 7);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_score, 6);

    //Score2
    set_cursor(27*40 + 27);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_score2, 6);


	//LEVEL
	set_cursor(28*40 + 17);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 3, s_lvl, 2);
*/

}



// ------------------------------- DRAWING THE GAME STATIC MESSAGES --------------------------
// draws strings on background, its called only on the start of the game
void drawStaticGameMessages(){
	//Player1
	set_cursor(3*40 + 4);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sPlayer1, 7);

	// Player2
	set_cursor(3*40 + 29);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sPlayer2, 7);

	// NEXT
    set_cursor(13*40 + 17);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sNEXT, 4);



	//LEVEL
	set_cursor(27*40 + 17);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sLEVEL, 5);

    //SCORE1

    set_cursor(26*40 + 7);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sSCORE1, 6);

    //SCORE2
    set_cursor(26*40 + 27);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, sSCORE2, 6);
}

// -------------------------- DRAWING BACKGROUND -----------------------------------------
// drawing game background, it get called only once in begginning of the game
void drawBackground(){
	for(int y=0; y<30; y++)
		for(int x=0; x<40; x++){
			set_cursor(y*40+x);
			int boja = 0;
			int znak = 0;
			switch(backround[y][x]){
				case 0: znak = BACKGROUND_ZNAK;    break;
				case 1: znak = BACKGROUND_ZNAK;    break;
				case 2: znak = ZNAK_KOCKICA   ;    break;
				case 3: znak = ZNAK_KOCKICA1  ;   break;
				case 4: znak = UGAO_GORNJI_LEVI;   break;
				case 5: znak = UGAO_DONJI_DESNI;   break;
				case 6: znak = UGAO_GORNJI_DESNI;  break;
				case 7: znak = UGAO_DONJI_LEVI;    break;
			}
			print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
		}
}

void drawTmpTable(){
	for(int y=0; y<16; y++)
		for(int x=0; x<8; x++){
			set_cursor(y*40+x);
			int boja = 0;
			int znak = 0;
			switch(tmp_table[y][x]){
				case 0: znak = BACKGROUND_ZNAK;    break;
				case 2: znak = ZNAK_KOCKICA   ;    break;
				case 3: znak = ZNAK_KOCKICA1  ;   break;
			}
			print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
		}
}

// ---------------------------  DRAWING GAME TABLE ---------------------------------------
// on every interrupt call entire game table is drawn

void my_timer_interrupt_handler(void * baseaddr_p) {
	//drawing screen and counting interrupts.

	static int frame_cnt = 0;
	static int frame1_cnt = 0;
	frame_cnt++;
	frame1_cnt++;

#if TEST_MODE
	testPeaces();
#else
	drawMap();
	drawNextInTable();
	if(frame_cnt % 25 == 0){
		fallPeaces();

	}


#endif
}



// ------------------------- INITIALIZING PLATFORM  -------------------------------------------------
void initializingPlatform(){
    init_platform();

    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x1);// show frame      2
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x10, 0x2D1E4B);// foreground 4 tamno ljubicasta/plava 0xFF0000-crvena
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x14, 0x00FF00);// foreground 4 green
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0x0000FF);// foreground 4 plava
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x1c, 0xffff00);// foreground 4 zuta
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x20, 0x000000);// background color crna
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x24, 0xFF0000);// frame color      6




    clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

    int Status;
	Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);
/*	if (Status != XST_SUCCESS)
		xil_printf ("\r\nInterrupt controller initialization failure");
	else
		xil_printf("\r\nInterrupt controller initialized");
*/
	// Connect my_timer_interrupt_handler
	Status = XIntc_Connect (&Intc, 0,
							(XInterruptHandler) my_timer_interrupt_handler,(void *)0);
	/*
	if (Status != XST_SUCCESS)
		xil_printf ("\r\nRegistering MY_TIMER Interrupt Failed");
	else
		xil_printf("\r\nMY_TIMER Interrupt registered");
*/
	//start the interrupt controller in real mode
	Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	//enable interrupt controller
	XIntc_Enable (&Intc, XPAR_INTC_0_DEVICE_ID);
	//XPAR_VGA_PERIPH_MEM_0_INTERRUPT_O_MASK
	//XPAR_AXI_INTC_0_VGA_PERIPH_MEM_0_INTERRUPT_O_INTR
	microblaze_enable_interrupts();

}

int main() {

	initializingPlatform();


#if !TEST_MODE
	drawBackground();
	drawStaticGameMessages();


#endif

	while(1){

	}

	cleanup_platform();
	return 0;
}


