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

#define UP 	   0b00010000
#define DOWN   0b00000001
#define RIGHT  0b00001000
#define LEFT   0b00000010
#define CENTER 0b00000100


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

unsigned char gameOverTable[30][40]={
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

		{1,1,2,2,2,  2,2,2,2,1,  1,1,2,2,2,  2,2,2,1,1,  2,2,1,1,1,  1,1,1,2,2,  2,2,2,2,2,  2,2,2,2,1},
		{1,2,1,1,1,  1,1,1,2,1,  1,2,1,1,1,  1,1,1,2,1,  2,1,2,1,1,  1,1,2,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,2,1,  1,2,1,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,2,  2,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,2,  2,2,2,2,2,  2,2,2,2,1},

		{1,2,1,1,1,  2,2,2,2,1,  2,2,2,2,2,  2,2,2,2,2,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1},
		{1,2,2,2,2,  2,2,2,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,2,  2,2,2,2,2,  2,2,2,2,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

		{1,1,1,2,2,  2,2,1,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,2,2,2,2,  2,2,2,2,1,  1,2,2,2,2,  2,2,2,1,1},
		{1,1,2,1,1,  1,1,2,1,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,2,1},
		{1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,2,1},
		{1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,2,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,1,1,2,1},
		{1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,2,  2,2,2,2,2,  2,2,2,2,1,  2,1,2,2,2,  2,2,2,1,1},

		{1,2,1,1,1,  1,1,1,2,1,  1,2,1,1,1,  1,1,1,2,1,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,2,1,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,2,1,  1,1,2,1,1,  1,1,2,1,1,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,2,  1,1,1,1,1},
		{1,2,1,1,1,  1,1,1,2,1,  1,1,1,2,1,  1,2,1,1,1,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  2,1,1,1,1},
		{1,1,2,1,1,  1,1,2,1,1,  1,1,1,1,2,  2,1,1,1,1,  2,1,1,1,1,  1,1,1,1,1,  2,1,1,1,1,  1,2,1,1,1},
		{1,1,1,2,2,  2,2,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  2,2,2,2,2,  2,2,2,2,1,  2,1,1,1,1,  1,1,2,1,1},

		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},
		{1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1,  1,1,1,1,1},

};


// 2 - falling
// 3 - fallen.
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



u8 tmp_table2[16][8] = {
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

u8 table_next[4][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},

};


XIntc Intc;

unsigned char debouncer = 0;
unsigned char sNEXT[] = "NEXT";
unsigned char sLEVEL[] = "LEVEL";
unsigned char sPlayer2[] = "PLAYER2";
unsigned char sPlayer1[] = "PLAYER10000";
unsigned char sSCORE1[] = "SCORE1";
unsigned char sSCORE2[] = "SCORE2";
int score = 0x0;


int valOfY = 0;

char lastKey = 'n';

typedef enum {P_O, P_I, P_S, P_Z, P_L, P_J, P_T} piece_types_t;
			//o-kvadrat,I je linija,S je kontra z, Z je z, L je l, J je j, T je t
typedef enum {R_0, R_1, R_2, R_3} rotation_t;


//verovatno simboli: x i y koordinate, rot stanje rotacije i type - oblik
typedef struct {
	int x;
	int y;
	piece_types_t type;
	rotation_t rot;
} piece_gameplay_struct_t;

piece_gameplay_struct_t pieces[2];//napravljen niz od dva simbola

int peace1_x = 4;
int peace1_y = 4;
piece_types_t piece;
int start_flag = 1;

Xuint32 addr = XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR;

char getPressedKey(){
	char pressedKey;
	int button = Xil_In32(XPAR_MY_PERIPHERAL_0_BASEADDR);
	if ((button & UP) == 0){
		pressedKey = 'u';
	}else if ((button & DOWN) == 0){
		pressedKey = 'd';
	}else if ((button & RIGHT) == 0){
		pressedKey = 'r';
	}else if ((button & LEFT) == 0){
		pressedKey = 'l';
	}else if ((button & CENTER) == 0){
		pressedKey = 'c';
	}
	if (lastKey != pressedKey){
		lastKey = pressedKey;
		return pressedKey;
	}
	else {

		return 'n';
	}

}

bool checkCollisionX1(u8 table[16][8]){ // OVDJE SMO DODALI OVO KAO PARAMETAR INACE RADILI SA TMP TABLE
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			// Go to tmp_table and check if there is any 2 above 3.
			if(table[y][x] == 3){
				if(table[y][x-1] == 2){
					return true;
				}

			}

		}
	}

	return false;

}

//ovde pomera simbol po koordinati u zavisnodti od pritisnutog tastera; ovde bi trebalo ograniciti da simbol ne prelazi sa leve na desnu stranu
void movingBlocks(piece_gameplay_struct_t* piece, u8 table[16][8]){

	char pressedKey = getPressedKey();


		switch(pressedKey){
			case 'l':
				if(piece->type == P_O){//kockica
					if(piece->x == 1){
						break;
					}
					if(table[piece->y-1][(piece->x)-2] == 3 || table[piece->y+1][(piece->x)-2] == 3 || table[piece->y][(piece->x)-2] == 3 ){//sprecavanje klipovanja kockice u levo
						break;
					}

				}

				if(piece->type == P_I){
					if(piece->rot == R_0 || piece->rot == R_2){//horizonatalno I
						if(piece->x == 2){
							break;
						}
						if(table[piece->y-1][(piece->x)-3] == 3 || table[piece->y+1][(piece->x)-3] == 3 || table[piece->y][(piece->x)-3] == 3 ){
							break;
						}
					}

					if(piece->rot == R_3 || piece->rot == R_1){//vertikalno
						if(piece->x == 0){
							break;
						}
						if(table[piece->y-2][(piece->x) -1] == 3 || table[piece->y-1][(piece->x) - 1] == 3 || table[piece->y+1][(piece->x) - 1] == 3 || table[piece->y][(piece->x) - 1] == 3 || table[piece->y+2][(piece->x) - 1] == 3){//sprecavanje klipovanja I u levo
							break;
						}
					}

				}

				if(piece->type == P_S || piece->type == P_Z){
					if(piece->rot == R_0 || piece->rot == R_2){//horizontalno obrnuto i obicno z
						if(piece->x == 1){
							break;
						}
					}
					if(piece->rot == R_1 || piece->rot == R_3){//vertikalno obrnuto i obicno z
						if(piece->x == 0){
							break;
						}
					}
				}

				if(piece->type == P_L || piece->type == P_J || piece->type == P_T){

					if(piece->rot == R_1){//za L,J,T u levo sve varijante
						if(piece->x == 0){
							break;
						}
					}
					if(piece->rot == R_3 || piece->rot == R_0 ||piece->rot == R_2 ){
						if(piece->x == 1){
							break;
						}
					}

				}

					piece->x--;
				break;


			case 'r':
				if(piece->type == P_O){
					if(piece->x == 7){
						break;
					}
					if(table[piece->y-1][(piece->x)+1] == 3 || table[piece->y+1][(piece->x)+1] == 3 || table[piece->y][(piece->x)+1] == 3 ){//sprecavanje klipovanja kockice u desno
						break;
					}
				}

				if(piece->type == P_I){
					if(piece->rot == R_0 || piece->rot == R_2){//horizonatalno I
						if(piece->x == 6){
							break;
						}
						if(table[piece->y-1][(piece->x)+2] == 3 || table[piece->y+1][(piece->x)+2] == 3 || table[piece->y][(piece->x)+2] == 3 ){
							break;
						}
					}
					if(piece->rot == R_3 || piece->rot == R_1){//vertikalno I
						if(piece->x == 7){
							break;
						}
						if(table[piece->y-2][(piece->x) + 1] == 3 || table[piece->y-1][(piece->x) + 1] == 3 || table[piece->y+1][(piece->x) + 1] == 3 || table[piece->y][(piece->x) + 1] == 3 || table[piece->y+2][(piece->x) + 1] == 3){//sprecavanje klipovanja I u desno
							break;
						}
					}

				}

				if(piece->type == P_S || piece->type == P_Z){//vertikalno i horizontalno obrnuto z i obicno z
						if(piece->x == 6){
							break;
						}
				}

				if(piece->type == P_L || piece->type == P_J || piece->type == P_T){
					if(piece->rot == R_3){//za L,J,T u levo sve varijante
							if(piece->x == 7){
								break;
							}
					}
					if(piece->rot == R_1 || piece->rot == R_0 ||piece->rot == R_2 ){
						if(piece->x == 6){
							break;
						}
					}

				}

					piece->x++;
				break;

			case 'c':
				if(piece->type == P_I){//sprecavanje da se vertikalna linija promeni u horizontalnu kada je na ivici
					if(piece->rot == R_3 || piece->rot == R_1){
						if(piece->x == 7 || piece->x==0){
							break;
						}

						if(table[piece->y][(piece->x) + 1] == 3  || table[piece->y][(piece->x) -2] == 3 || table[piece->y][(piece->x) -1] == 3){
							break;
						}
					}
				}

				if(piece->type == P_S || piece->type == P_Z){//sprecavanje da se obrnuto z i obicno z promeni u horizontalno kada je na levoj ivici
					if(piece->rot == R_1 || piece->rot == R_3){
						if(piece->x == 0){
							break;
						}
					}
				}

				if(piece->type == P_L || piece->type == P_J || piece->type == P_T){//sprecavanje da se rotiraju uz ivicu j,L,t
					if(piece->rot == R_1 || piece->rot == R_3){
						if(piece->x == 7 || piece->x == 0){
							break;
						}
					}
				}


				piece->rot = (piece->rot + 1) % 4;
				break;


			case 'd':
				piece->y++;
				break;
		}


}

//nigde se ne poziva
int returnMin(piece_gameplay_struct_t* piece){

	int minX;
	switch (piece->type) {

		case P_O:
			minX = -1;
			break;

		case P_I:
			switch(piece->rot){
			case R_0:
			case R_2:
				minX = -2;
				break;
			case R_1:
			case R_3:
				minX = 0;
				break;
			}
		break;

		case P_Z:
			switch(piece->rot){
			case R_0:
			case R_2:
				minX = -1;


			break;

			case R_1:
			case R_3:
				minX = 0;


			break;
			}
		break;

		case P_S:
			switch(piece->rot){
			case R_0:
			case R_2:
				minX = -1;


			break;

			case R_1:
			case R_3:
				minX = 0;


			break;
			}
		break;


		case P_J:
			switch(piece->rot){
			case R_0:
				minX = -1;


			break;

			case R_1:
				minX = 0;


			break;

			case R_2:
				minX = -1;


			break;

			case R_3:
				minX = -1;


			break;

			}
		break;


		case P_L:
			switch(piece->rot){
			case R_0:
				minX = -1;
			break;

			case R_1:
				minX = 0;
			break;

			case R_2:
				minX = -1;
			break;

			case R_3:
				minX = -1;
			break;
			}
		break;


		case P_T:
			switch(piece->rot){
			case R_0:
				minX = -1;


			break;

			case R_1:
				minX = 0;

			break;


			case R_2:
				minX = -1;

			break;
			case R_3:
				minX = -1;

			break;
			}
		break;



		}
	return minX;

}


int returnMax(piece_gameplay_struct_t* piece){

	int maxX;
	switch (piece->type) {

		case P_O:
			maxX = 0;

		break;

		case P_I:
			switch(piece->rot){
			case R_0:
			case R_2:

				maxX = 1;

			break;

			case R_1:
			case R_3:
				maxX = 0;


			break;
			}
		break;


		case P_Z:
			switch(piece->rot){
			case R_0:
			case R_2:
				maxX = 1;


			break;

			case R_1:
			case R_3:
				maxX = 1;


			break;
			}
		break;

		case P_S:
			switch(piece->rot){
			case R_0:
			case R_2:
				maxX = 1;


			break;

			case R_1:
			case R_3:
				maxX = 1;


			break;
			}
		break;


		case P_J:
			switch(piece->rot){
			case R_0:
				maxX = 1;


			break;

			case R_1:
				maxX = 1;


			break;

			case R_2:
				maxX = 1;


			break;

			case R_3:
				maxX = 0;


			break;

			}
		break;


		case P_L:
			switch(piece->rot){
			case R_0:
				maxX = 1;


			break;

			case R_1:
				maxX = 1;


			break;

			case R_2:
				maxX = 1;

			break;

			case R_3:
				maxX = 0;


			break;
			}
		break;


		case P_T:
			switch(piece->rot){
			case R_0:
				maxX = 1;


			break;

			case R_1:
				maxX = 1;

			break;


			case R_2:
				maxX = 1;

			break;
			case R_3:
				maxX = 0;

			break;
			}
		break;



		}
	return maxX;

}

//pocetna pozicija kursora; iscrtavanje simbola
void drawSignToScreen(int x, int y, int boja, int znak){
	set_cursor(y*40+x);
	print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
}

/*
void drawMap(){
	drawTable(tmp_table, 6, 6);// MIJENJALI
	drawTable(table1, 6+8+12, 6);
}



void drawTable(u8 table[16][8], int table_x, int table_y){
	for(int y = 0; y < 16 ; y++){
		int out_y = y + table_y;
			for(int x = 0; x < 8; x++){
				int out_x = x + table_x;
				switch(table[y][x]){
					case 0: drawSignToScreen(out_x, out_y, 2, BACKGROUND_ZNAK);    break;
					case 2: drawSignToScreen(out_x, out_y, 2, ZNAK_KOCKICA)   ;    break;
					case 3: drawSignToScreen(out_x, out_y, 2, ZNAK_KOCKICA1)   ;    break;
				}
			}
	}
}
*/

//kako tabela[4][4] moze imati vrednost 0 ili 1, enumeracija?
void drawTable_next(u8 table[4][4], int table_x, int table_y){
	for(int y = 0; y < 4 ; y++){
		int out_y = y + table_y;
			for(int x = 0; x < 4; x++){
				int out_x = x + table_x;
				switch(table[y][x]){
					case 0: drawSignToScreen(out_x, out_y, 2, BACKGROUND_ZNAK);	//crta crno polje
						break;
					case 2: drawSignToScreen(out_x, out_y, 2, ZNAK_KOCKICA);    //iscrtavanje
						break;
				}
			}
	}
}


void copyTable(u8 table1[16][8]){
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			tmp_table[y][x] = table1[y][x];
		}
	}
}



void drawPieceToScreen(int table_x, int table_y, int x, int y, piece_types_t piece, rotation_t rot) {
	int boja1 = 3;
	x += table_x;
	y += table_y;


	switch (piece) {
	case P_O:
		drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
		drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
		drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
		drawSignToScreen(x-1, y-1, boja1, ZNAK_KOCKICA);

		break;

	case P_I:
		switch(rot){
		case R_0:
			drawSignToScreen(x-2, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);

			break;
		case R_1:
			drawSignToScreen(x, y-2, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_Z:
		switch(rot){
		case R_0:
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x-1, y-1, boja1, ZNAK_KOCKICA);


			break;
		case R_1:
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y-1, boja1, ZNAK_KOCKICA);
			break;
		}
		break;

	case P_S:
		switch(rot){
		case R_0:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y-1, boja1, ZNAK_KOCKICA);


			break;
		case R_1:
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y+1, boja1, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_J:
		switch(rot){
		case R_0:
			drawSignToScreen(x-1, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);

			break;
		case R_1:
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y-1, boja1, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y+1, boja1, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSignToScreen(x-1, y+1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			break;

		}
		break;


	case P_L:
		switch(rot){
		case R_0:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y-1, boja1, ZNAK_KOCKICA);


			break;
		case R_1:
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y+1, boja1, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x-1, y+1, boja1, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSignToScreen(x-1, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			break;
		}
		break;


	case P_T:
		switch(rot){
		case R_0:
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);


			break;
		case R_1:
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			break;
		case R_2:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x+1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			break;
		case R_3:
			drawSignToScreen(x-1, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y-1, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y, boja1, ZNAK_KOCKICA);
			drawSignToScreen(x, y+1, boja1, ZNAK_KOCKICA);
			break;
		}
		break;


	}
}

void drawSign(int x, int y){
	// Drawing falling sign to tmp_table.
	tmp_table[y][x] = 2;
}

/*void retMinMax(min_max_struct* minMax, piece_gameplay_struct_t* piece){



	switch (piece->type) {

		case P_O:
			minMax->minX = -1;
			minMax->maxX = 0;
		break;

		case P_I:
			switch(piece->rot){
			case R_0:
			case R_2:

				minMax->minX = -2;
				minMax->maxX = 1;

			break;

			case R_1:
			case R_3:
				minMax->minX = 0;
				minMax->maxX = 0;

			break;
			}
		break;


		case P_Z:
			switch(piece->rot){
			case R_0:
			case R_2:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_1:
			case R_3:
				minMax->minX = 0;
				minMax->maxX = 1;

			break;
			}
		break;

		case P_S:
			switch(piece->rot){
			case R_0:
			case R_2:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_1:
			case R_3:
				minMax->minX = 0;
				minMax->maxX = 1;

			break;
			}
		break;


		case P_J:
			switch(piece->rot){
			case R_0:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_1:
				minMax->minX = 0;
				minMax->maxX = 1;

			break;

			case R_2:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_3:
				minMax->minX = -1;
				minMax->maxX = 0;

			break;

			}
		break;


		case P_L:
			switch(piece->rot){
			case R_0:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_1:
				minMax->minX = 0;
				minMax->maxX = 1;

			break;

			case R_2:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_3:
				minMax->minX = -1;
				minMax->maxX = 0;

			break;
			}
		break;


		case P_T:
			switch(piece->rot){
			case R_0:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;

			case R_1:
				minMax->minX = 0;
				minMax->maxX = 1;

			break;


			case R_2:
				minMax->minX = -1;
				minMax->maxX = 1;

			break;
			case R_3:
				minMax->minX = -1;
				minMax->maxX = 0;

			break;
			}
		break;



		}
	//









	return (&minMax);

}*/


void drawPiece(const piece_gameplay_struct_t* piece){
//iscrtava simbol u zavisnosti od rotacije postavljajuci konkretne koordinate

	switch (piece->type) {

		case P_O:
			drawSign(piece->x, piece->y);
			drawSign(piece->x-1, piece->y);
			drawSign(piece->x, piece->y-1);
			drawSign(piece->x-1, piece->y-1);

			break;

		case P_I:
			switch(piece->rot){
			case R_0:
			case R_2:
				drawSign(piece->x-2, piece->y);
				drawSign(piece->x-1,piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);

				break;
			case R_1:
			case R_3:
				drawSign(piece->x, piece->y-2);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				break;
			}
			break;


		case P_Z:
			switch(piece->rot){
			case R_0:
			case R_2:
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x-1, piece->y-1);


				break;
			case R_1:
			case R_3:
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x+1, piece->y-1);
				break;
			}
			break;

		case P_S:
			switch(piece->rot){
			case R_0:
			case R_2:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x+1, piece->y-1);


				break;
			case R_1:
			case R_3:
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x+1, piece->y+1);
				break;
			}
			break;


		case P_J:
			switch(piece->rot){
			case R_0:
				drawSign(piece->x-1, piece->y-1);
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);

				break;
			case R_1:
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				drawSign(piece->x+1, piece->y-1);
				break;
			case R_2:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x+1, piece->y+1);
				break;
			case R_3:
				drawSign(piece->x-1, piece->y+1);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				break;

			}
			break;


		case P_L:
			switch(piece->rot){
			case R_0:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x+1, piece->y-1);


				break;
			case R_1:
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				drawSign(piece->x+1,piece->y+1);
				break;
			case R_2:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x-1, piece->y+1);
				break;
			case R_3:
				drawSign(piece->x-1, piece->y-1);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				break;
			}
			break;


		case P_T:
			switch(piece->rot){
			case R_0:
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x+1,piece->y);


				break;
			case R_1:
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				drawSign(piece->x+1, piece->y);
				break;
			case R_2:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y);
				drawSign(piece->x+1, piece->y);
				drawSign(piece->x, piece->y+1);
				break;
			case R_3:
				drawSign(piece->x-1, piece->y);
				drawSign(piece->x, piece->y-1);
				drawSign(piece->x, piece->y);
				drawSign(piece->x, piece->y+1);
				break;
			}
			break;



		}

}


void fallPeace(piece_gameplay_struct_t* piece){
	piece->y++;

}


void clearing(){
	int x,y;
	for (x = 0; x < 8; x++){
		table1[valOfY][x] = 0;

	}




	updateTable(tmp_table,table1);

}
//brise red koji je ispunjen svim trojkama
/*void checkIfClear(piece_gameplay_struct_t* piece){
	int counter;
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			if(table1[y][x] == 3){
				counter++;
				}
			if (counter == 8){
				     // clearuj ukoliko je kaunter prebrojao osam kockica za redom
				valOfY =y;
				clearing(piece);
				for (int z=y; z >= 0; z--) {
							for (int k = 0; k < 8; k++) {
									if(z == 0){
										table1[z][k] = 0;
									}
									else{
										table1[z][k] = table1[z-1][k];
									}
								}
				}
					}
			}

		counter = 0;
	}

}*/


void checkIfClear(piece_gameplay_struct_t* piece, int score){
	int counter=0;
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			if(table1[y][x] == 3){
				counter++;
				}
			if (counter == 8){
				scoreRefresh();

				     // clearuj ukoliko je kaunter prebrojao osam kockica za redom
				valOfY =y;
				clearing();
				for (int z=y; z >= 0; z--) {
							for (int k = 0; k < 8; k++) {
									if(z == 0){
										table1[z][k] = 0;
									}
									else{
										table1[z][k] = table1[z-1][k];
									}
								}

				}
					}
			}

		counter = 0;
	}

}


bool checkCollision(u8 table[16][8]){ // OVDJE SMO DODALI OVO KAO PARAMETAR INACE RADILI SA TMP TABLE
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			// Go to tmp_table and check if there is any 2 above 3.
			if(table[y][x] == 3){
				if(table[y-1][x] == 2){
					return true;
				}

			}

			/*if(table[y][x] == 3){
				if(table[y][x-2] == 2){
					return true;
				}

			}*/
			// 2 is on the bottom.

			if(y == 15){
				if(table[y][x] == 2){
					return true;
				}
			}
		}
	}

	return false;

}

void updateTable(u8 table[16][8],u8 temp_table[16][8]) { // DODALI JEDAN PARAMETAR, TMP_TABELA
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			switch(temp_table[y][x]){
			case 2:
				table[y][x] = 3;
				break;
			case 3:
				table[y][x] = 3;
				break;
			}

		}
	}
}


void scoreRefresh(){
	score += 256;
	set_cursor(28*40 + 7);
	//score = 0xbabadeda;
	print_u32_hex(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, score);
}

void scoreInitialize(){
	score = 0;
	set_cursor(28*40 + 7);
	//score = 0xbabadeda;
	print_u32_hex(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 1, score);
}



void spawnInitialPiece(piece_gameplay_struct_t* piece) {
	piece->type = rand()%7;
	piece->rot = R_0;
	piece->x = 4;
	piece->y = 0;
	drawNext(piece->type, piece->rot);
}

void spawnNextPiece(piece_gameplay_struct_t* next_piece) {

	next_piece->type = rand()%7;
	next_piece->rot = R_0;
	next_piece->x = 4;
	next_piece->y = 0;
	drawActualNext(next_piece->type, next_piece->rot);
}


void spawnNewPiece(piece_gameplay_struct_t* piece, piece_gameplay_struct_t* next_piece) {

	piece->type = next_piece->type;
	piece->rot = R_0;
	piece->x = 4;
	piece->y = 0;
	drawNext(piece->type, piece->rot);

}


bool checkGameOver(piece_gameplay_struct_t* piece){
	for (int x = 0; x < 8; x++) {
		if (tmp_table[0][x] == 3){
				return true;
			}
		}
	return false;

}

void testPieces(void) {
	drawPieceToScreen(0, 0, 4, 4, P_O, R_0);
	drawPieceToScreen(5, 0, 4, 4, P_I, R_0);
	drawPieceToScreen(10, 0, 4, 4, P_I, R_1);
	drawPieceToScreen(15, 0, 4, 4, P_S, R_0);
	drawPieceToScreen(20, 0, 4, 4, P_S, R_1);
	drawPieceToScreen(25, 0, 4, 4, P_Z, R_0);
	drawPieceToScreen(30, 0, 4, 4, P_Z, R_1);
	drawPieceToScreen(0, 5, 4, 4, P_J, R_0);
	drawPieceToScreen(5, 5, 4, 4, P_J, R_1);
	drawPieceToScreen(10, 5, 4, 4, P_J, R_2);
	drawPieceToScreen(15, 5, 4, 4, P_J, R_3);
	drawPieceToScreen(20, 5, 4, 4, P_L, R_0);
	drawPieceToScreen(25, 5, 4, 4, P_L, R_1);
	drawPieceToScreen(30, 5, 4, 4, P_L, R_2);
	drawPieceToScreen(0, 10, 4, 4, P_L, R_3);
	drawPieceToScreen(5, 10, 4, 4, P_T, R_0);
	drawPieceToScreen(10, 10, 4, 4, P_T, R_1);
	drawPieceToScreen(15, 10, 4, 4, P_T, R_2);
	drawPieceToScreen(20, 10, 4, 4, P_T, R_3);


}

/*void drawPieceToScreens(void) {

	drawPieceToScreen(6, 3, peace1_x, peace1_y, piece, R_0);

}*/

void drawNext(piece_types_t piece_t, rotation_t rot_t){
		drawTable_next(table_next,16,14);
	}

void drawActualNext(piece_types_t piece_t, rotation_t rot_t){
		drawPieceToScreen(18, 15, 0, 0, piece_t, rot_t);
	}

void drawNextInTable(piece_types_t piece_t){

	//piece_t = rand()%7;
	drawNext(piece_t, R_0);
	//


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

void drawToScreen(int table_x, int table_y, u8 table[16][8]){
	//int boja1;
	//int boja = boja1;
	for(int y=0; y<16; y++)
		for(int x=0; x<8; x++){
			int boja = 0;
			int znak = 0;
			switch (table[y][x]) {
			case 0:
				boja = 0; //0
				znak = BACKGROUND_ZNAK;
				break;
			case 2:
				boja = 1; //ZELENA 1
				znak = ZNAK_KOCKICA;
				break;
			case 3:
				boja = 2;//PLAVA 2
				znak = ZNAK_KOCKICA1;
				break;
			}
			 drawSignToScreen(table_x+x, table_y+y, boja, znak);
		}
}

// ---------------------------  DRAWING GAME TABLE ---------------------------------------
// on every interrupt call entire game table is drawn


void drawGameOver(){
	for(int y=0; y<30; y++)
			for(int x=0; x<40; x++){
				set_cursor(y*40+x);
				int boja = 2;
				int znak = 0;
				switch(gameOverTable[y][x]){
					case 0: znak = BACKGROUND_ZNAK;    break;
					case 1: znak = BACKGROUND_ZNAK;    break;
					case 2: znak = ZNAK_KOCKICA   ;    break;
					case 3: znak = ZNAK_KOCKICA1  ;   break;
				}
				print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, boja, znak);
			}
	}


void my_timer_interrupt_handler(void * baseaddr_p) {
	//scoreRefresh();
	/*for(int i = 0; i < 9; i++){
		int zapis;
		int boja = 1;
		zapis = boja<<6;
		int znak = i + 28;

		zapis|=znak;
		VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + TEXT_MEM_OFF + i*4, zapis);
	}*/

	/*for(int i = 0; i < 64; i++){
		int zapis;
		int boja = 1;
		zapis = boja<<6;
		int znak = i;

		zapis|=znak;
		VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + TEXT_MEM_OFF + i*4, zapis);
	}*/


			/*int zapis;
			int boja = 1;
			zapis = boja<<6;
			int znak = 0x1C;

			zapis|=znak;
			VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + TEXT_MEM_OFF +4, zapis);*/



	//drawing screen and counting interrupts.

	static int frame_cnt = 0;
	static int frame1_cnt = 0;
	frame_cnt++;
	frame1_cnt++;

#if TEST_MODE
	testPieces();
#else
	// Fall pieces on every second.
	if(frame_cnt % 25 == 0){
		// Just increment piece position. Nothing else.
		fallPeace(&pieces[0]);
	}
	movingBlocks(&pieces[0],table1);


	// Doing table1.

	// Draw (copy) table1 to tmp_table.
	checkIfClear(&pieces[0], score);
	copyTable(table1);



	// Draw piece1 to tmp_table.
	drawPiece(&pieces[0]);



	// Check collision between 2 and 3 in tmp_table.
	if(checkCollision(tmp_table)){
		// There is collision.


		// check if game is over
		// Copy tmp_table to table1, with converting all 3 to 2.
		updateTable(table1,tmp_table);

		if (checkGameOver(&pieces[0])){
			drawGameOver();
			return 0;
		}

		spawnNewPiece(&pieces[0],&pieces[1]);
		spawnNextPiece(&pieces[1]);


	}



	// Draw tmp_table to screen.
	drawToScreen(6, 6,tmp_table);

	// Doing table2.





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
	scoreInitialize();

	spawnInitialPiece(&pieces[0]);
	spawnNextPiece(&pieces[1]);
	//spawnNewPiece(&pieces[1]);

#endif

	while(1){

	}

	cleanup_platform();
	return 0;
}


