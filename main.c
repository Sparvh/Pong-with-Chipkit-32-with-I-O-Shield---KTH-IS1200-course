#include <pic32mx.h>
#include <stdint.h>
#include "time4io.c"
//#include <stdlib.h>
//#include "mipslab.h"

void user_isr(void);
void init_mcu(void);
void init_tim(void);
int getsw (void);
int getbtns(void);

void init_display(void);
void display_update(void);
void display_cls(void);

//Buttons
int button;
int button1;
int switches;

//Test
int pixeltestx = 14;
int pixeltesty = 13;

//scores
int Player1Score = 0;
int Player2Score = 0;

//player 1
int player1x = 0;
int player1y = 12;
int player1w = 3;
int player1h = 8;

//player 2
int player2x = 125;
int player2y = 12;
int player2w = 3;
int player2h = 8;

//ball
int ballx = 64;
int bally = 16;

// Winning
int temp1;
int temp;
int ten = 10;
int eleven = 11;
int twelve = 12;

int slut1 = 0;
int collisionThing = 1;

char textbuffer[4][16];

const uint8_t screen[128][4];
uint8_t bufferstate[512];
const uint8_t slut[5];


#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


const uint8_t const font[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 94, 0, 0, 0, 0,
    0, 0, 4, 3, 4, 3, 0, 0,
    0, 36, 126, 36, 36, 126, 36, 0,
    0, 36, 74, 255, 82, 36, 0, 0,
    0, 70, 38, 16, 8, 100, 98, 0,
    0, 52, 74, 74, 52, 32, 80, 0,
    0, 0, 0, 4, 3, 0, 0, 0,
    0, 0, 0, 126, 129, 0, 0, 0,
    0, 0, 0, 129, 126, 0, 0, 0,
    0, 42, 28, 62, 28, 42, 0, 0,
    0, 8, 8, 62, 8, 8, 0, 0,
    0, 0, 0, 128, 96, 0, 0, 0,
    0, 8, 8, 8, 8, 8, 0, 0,
    0, 0, 0, 0, 96, 0, 0, 0,
    0, 64, 32, 16, 8, 4, 2, 0,
    0, 62, 65, 73, 65, 62, 0, 0,
    0, 0, 66, 127, 64, 0, 0, 0,
    0, 0, 98, 81, 73, 70, 0, 0,
    0, 0, 34, 73, 73, 54, 0, 0,
    0, 0, 14, 8, 127, 8, 0, 0,
    0, 0, 35, 69, 69, 57, 0, 0,
    0, 0, 62, 73, 73, 50, 0, 0,
    0, 0, 1, 97, 25, 7, 0, 0,
    0, 0, 54, 73, 73, 54, 0, 0,
    0, 0, 6, 9, 9, 126, 0, 0,
    0, 0, 0, 102, 0, 0, 0, 0,
    0, 0, 128, 102, 0, 0, 0, 0,
    0, 0, 8, 20, 34, 65, 0, 0,
    0, 0, 20, 20, 20, 20, 0, 0,
    0, 0, 65, 34, 20, 8, 0, 0,
    0, 2, 1, 81, 9, 6, 0, 0,
    0, 28, 34, 89, 89, 82, 12, 0,
    0, 0, 126, 9, 9, 126, 0, 0,
    0, 0, 127, 73, 73, 54, 0, 0,
    0, 0, 62, 65, 65, 34, 0, 0,
    0, 0, 127, 65, 65, 62, 0, 0,
    0, 0, 127, 73, 73, 65, 0, 0,
    0, 0, 127, 9, 9, 1, 0, 0,
    0, 0, 62, 65, 81, 50, 0, 0,
    0, 0, 127, 8, 8, 127, 0, 0,
    0, 0, 65, 127, 65, 0, 0, 0,
    0, 0, 32, 64, 64, 63, 0, 0,
    0, 0, 127, 8, 20, 99, 0, 0,
    0, 0, 127, 64, 64, 64, 0, 0,
    0, 127, 2, 4, 2, 127, 0, 0,
    0, 127, 6, 8, 48, 127, 0, 0,
    0, 0, 62, 65, 65, 62, 0, 0,
    0, 0, 127, 9, 9, 6, 0, 0,
    0, 0, 62, 65, 97, 126, 64, 0,
    0, 0, 127, 9, 9, 118, 0, 0,
    0, 0, 38, 73, 73, 50, 0, 0,
    0, 1, 1, 127, 1, 1, 0, 0,
    0, 0, 63, 64, 64, 63, 0, 0,
    0, 31, 32, 64, 32, 31, 0, 0,
    0, 63, 64, 48, 64, 63, 0, 0,
    0, 0, 119, 8, 8, 119, 0, 0,
    0, 3, 4, 120, 4, 3, 0, 0,
    0, 0, 113, 73, 73, 71, 0, 0,
    0, 0, 127, 65, 65, 0, 0, 0,
    0, 2, 4, 8, 16, 32, 64, 0,
    0, 0, 0, 65, 65, 127, 0, 0,
    0, 4, 2, 1, 2, 4, 0, 0,
    0, 64, 64, 64, 64, 64, 64, 0,
    0, 0, 1, 2, 4, 0, 0, 0,
    0, 0, 48, 72, 40, 120, 0, 0,
    0, 0, 127, 72, 72, 48, 0, 0,
    0, 0, 48, 72, 72, 0, 0, 0,
    0, 0, 48, 72, 72, 127, 0, 0,
    0, 0, 48, 88, 88, 16, 0, 0,
    0, 0, 126, 9, 1, 2, 0, 0,
    0, 0, 80, 152, 152, 112, 0, 0,
    0, 0, 127, 8, 8, 112, 0, 0,
    0, 0, 0, 122, 0, 0, 0, 0,
    0, 0, 64, 128, 128, 122, 0, 0,
    0, 0, 127, 16, 40, 72, 0, 0,
    0, 0, 0, 127, 0, 0, 0, 0,
    0, 120, 8, 16, 8, 112, 0, 0,
    0, 0, 120, 8, 8, 112, 0, 0,
    0, 0, 48, 72, 72, 48, 0, 0,
    0, 0, 248, 40, 40, 16, 0, 0,
    0, 0, 16, 40, 40, 248, 0, 0,
    0, 0, 112, 8, 8, 16, 0, 0,
    0, 0, 72, 84, 84, 36, 0, 0,
    0, 0, 8, 60, 72, 32, 0, 0,
    0, 0, 56, 64, 32, 120, 0, 0,
    0, 0, 56, 64, 56, 0, 0, 0,
    0, 56, 64, 32, 64, 56, 0, 0,
    0, 0, 72, 48, 48, 72, 0, 0,
    0, 0, 24, 160, 160, 120, 0, 0,
    0, 0, 100, 84, 84, 76, 0, 0,
    0, 0, 8, 28, 34, 65, 0, 0,
    0, 0, 0, 126, 0, 0, 0, 0,
    0, 0, 65, 34, 28, 8, 0, 0,
    0, 0, 4, 2, 4, 2, 0, 0,
    0, 120, 68, 66, 68, 120, 0, 0,
};
void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4)
        return;
    if(!s)
        return;
    
    for(i = 0; i < 16; i++)
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void setPixel(const short x, const short y, const uint8_t state) {
	
	if ( x < 128 && x >= 0){
		if (y < 32 && y >= 0){
  uint8_t bit = 0x1 << (y - (y/8 * 8));
  if(state == 0) {
    bufferstate[x + (y/8) * 128] &= ~bit;
  } else {
    bufferstate[x + (y/8) * 128] |= bit;
  }
		}
	}
}

void init_mcu(void)
{
	/* Set up peripheral bus clock */
	/* OSCCONbits.PBDIV = 1; */
	OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */

	/* Enable LED1 through LED8 */
	TRISECLR = 0x00FF;

	/* Configure switches and buttonpins as inputs */
	TRISDSET = 0x0FE0; // Switches 4:1, Buttons 4:2
	TRISFSET = 0x0001; // Button 1

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	SPI2STATCLR = 0x40; 	/* SPI2STAT bit SPIROV = 0; */
 	SPI2CONSET = 0x40; 		/* SPI2CON bit CKP = 1; */
	SPI2CONSET = 0x20; 		/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x8000;	/* SPI2CON bit ON = 1; */
}

/* Init timer 2 with timeout interrupts */
void init_tim(void)
{
	/* Configure Timer 2 */
	T2CON     = 0x0;        	// reset control register
	T2CONSET  = (0x4 << 4);     // set prescaler to 1:16
	//PR2       = TMR2PERIOD; 	// set period length
	TMR2      = 0;          	// reset timer value
	T2CONSET  = (0x1 << 15);    // start the timer

	/* Enable Timer 2 interrupts */
	IPC(2)    = 0x4;        	// set interrupt priority to 4
	IECSET(0) = 0x1<<8;      	// timer 2 interrupt enable
	IFSCLR(0) = 0x1<<8;      	// clear timer 2 interrupt flag
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 128; j++)
			spi_send_recv(data[i*128 + j]);
	}
}


void getRect(int x, int y, int w, int h, const uint8_t state){
	
	int i,j;
	
	int yj = y;
	int xj = x;
	
	for (i = 0; i < w; i++){
		for (j = 0; j < h; j++){
			setPixel(x, y, state);
			y++;
		}
		y = yj;
		x++;
	}
}


void move_ballplayer2(){ //Fixar så att bollen rör sig mot spelare 2

		if (switches == 1){
		quicksleep(10000);
		ballx += 1;
		setPixel(ballx, bally, 1);
		}
		else {
		quicksleep(100000);
		ballx += 1;
		setPixel(ballx, bally, 1);
		}
		
}

void move_ballplayer1(){ //Fixar så att bollen rör sig mot spelare 1

		if (switches == 1){
		quicksleep(10000);
		ballx -= 1;
		setPixel(ballx, bally, 1);
		}
		 else {
		quicksleep(100000);
		ballx -= 1;
		setPixel(ballx, bally, 1);
		}
}

void move_balldown(){//Fixar så att bollen kan röra sig neråt på skärmen
		if (switches == 1){
		quicksleep(10000);
		bally += 1;
		setPixel(ballx, bally, 1);
			
		}
		else {
		quicksleep(100000);
		bally += 1;
		setPixel(ballx, bally, 1);
		}
}

void move_ballup(){ //Fixar så att bollen kan röra sig uppåt på skärmen
		if (switches == 1){
		quicksleep(10000);
		bally -= 1;
		setPixel(ballx, bally, 1);
		}
		else{
		quicksleep(100000);
		bally -= 1;
		setPixel(ballx, bally, 1);
		}
}

void ballColision1(){ //Ball colision for player 1
	if (ballx == 3){ //The x-position where player 1 is
	
	if (player1y < 16){
		int p1 = 0;
		int yvalue = player1y;
		int yvalueball = bally;
		
		for (p1 = 0; p1 < 8; p1++){
		if ( yvalueball == (yvalue + p1)){
			//quicksleep(100000);
			move_ballplayer2();
			slut1 = 1;
			break;
		}
		}
	}
		// if (player1y >= 16){
		// int p1 = 0;
		// int yvalue = player1y;
		// int yvalueball = bally;
		
		// for (p1 = 0; p1 < 8; p1++){
		// if ( yvalueball == (yvalue + p1)){
			quicksleep(100000);
			// move_ballplayer2();
			// slut1 = 1;
			// break;
		// }
		// }
	// }
	
	}
}

void ballColision2(){ //Ball colision for player 2

	if (ballx == 125){ //The x-position where player 2 is
	
	if (player2y < 16){
		int p2 = 0;
		int yvalue2 = player2y;
		int yvalueball2 = bally;
		
		for (p2 = 0; p2 < 8; p2++){
		if ( yvalueball2 == (yvalue2 + p2)){
			
			//quicksleep(100000);
			move_ballplayer1();
			slut1 = 0;
			break;
		}
		}
	}
}
}

void move_player1up(){
	if ( player1y >= 0 ){
		
	if (switches == 1){
	quicksleep(10000);
	player1y = player1y - 1;
		
	}
	else{
	quicksleep(100000);
	player1y = player1y - 1;
	}
	//getRect(player1x,player1y,player1w,player1h,1);
	//setPixel(pixeltestx, pixeltesty, 1);
	//getRect(player1x,player1y,player1w,player1h,1);
	}
}

void move_player1down(){
	
	if ( player1y < 26 ){
	if (switches == 1){
	quicksleep(10000);
	player1y = player1y + 1;
	}
	else{
	quicksleep(100000);
	player1y = player1y + 1;
	}
	
	
	
	//getRect(player1x,player1y,player1w,player1h,1);
	//setPixel(pixeltestx, pixeltesty, 1);
	//getRect(player1x,player1y,player1w,player1h,1);
	}
}

void move_player2up(){
	if ( player2y >= 0 ){
		
	if (switches == 1){
	quicksleep(10000);
	player2y = player2y - 1;}
	else{
	quicksleep(100000);
	player2y = player2y - 1;
	}
	//getRect(player1x,player1y,player1w,player1h,1);
	//setPixel(pixeltestx, pixeltesty, 1);
	//getRect(player1x,player1y,player1w,player1h,1);
	}
}
void move_player2down(){
	if ( player2y < 26 ){
		
	if (switches == 1){
	quicksleep(10000);
	player2y = player2y + 1;
	}
	else {
	quicksleep(100000);
	player2y = player2y + 1;
	}
	//getRect(player1x,player1y,player1w,player1h,1);
	//setPixel(pixeltestx, pixeltesty, 1);
	//getRect(player1x,player1y,player1w,player1h,1);
	}
}
void Menuscreen(){
}

void Victoryscreen(){
}

void BallCollisionWall2(){ //Om bollen är längst uppe på skärmen påväg mot player 2
	if (bally == 0){
		move_ballplayer2();
		move_balldown();
		collisionThing = 1;
		
	}
	
}
void BallCollisionWall22(){ //Om bollen är längst nere på skärmen påväg mot player 2
	if (bally == 32){
		move_ballplayer2();
		move_ballup();
		collisionThing = 2;
	}
}

void BallCollisionWall1(){ //Om bollen är längst uppe på skärmen påväg mot player 1
	
		if (bally == 0){
		move_ballplayer1();
		move_balldown();
		collisionThing = 1;
		}
	
}

void BallCollisionWall11(){ //Om bollen är längst nere på skärmen påväg mot player 1
	
		if (bally == 32){
		move_ballplayer1();
		move_ballup();
		collisionThing = 2;
		}
}

int getsw (void) { //Switcharna
	
	int switches = (PORTD >> 8) & 0x000f;
	return switches;
}

int getbtns(void) {	//Input för knapp 4-2
	
	int button = (PORTD >> 5) & 0x0007;
	return button;
}

int getbtn1(void){ //Inputen för knapp1
	int button1 = (PORTF >> 1) & 0x0001;
	return button1;
}

void clear(){
	
	int i;
	
	for (i = 0; i < 512; i++){
		bufferstate[i] = 0;
	}
	
}


// void player1boardBS(){
	// int k;
	
	// for ( k = 50; k < 54; k++){
		// setPixel(pixeltestx, pixeltesty, 1);
		// pixeltestx = pixeltestx + 1;
		// pixeltesty = pixeltesty + 1;
	// }
	
// }

void Player1ScoreBoard()
{
	setPixel(pixeltestx, pixeltesty, 1);
	setPixel(pixeltestx, pixeltesty + 1, 1);
	setPixel(pixeltestx, pixeltesty + 2, 1);
	setPixel(pixeltestx, pixeltesty + 3, 1);

}

int main(void){
	
init_mcu(); //Initilize led lampor och basic osv
display_init(); //Initilize displayen 
TRISD = 0x0fe0;

int GAME = 0;

while (GAME == 0){
	int butten;
	butten = getbtns();
	display_string(0,"press 1");
	display_string(1,"for 1 player");
	display_string(2,"press 2");
	display_string(3,"for 2 player");
	display_update();
	
	if ( butten == 2 ){
		GAME = 1;
	}
	
}

while (GAME == 1){
	
	if (Player1Score == 3){
		GAME = 2;
	}
	
	if (Player2Score == 3) {
		GAME = 3;
	}
	
	else {
	
	clear();
	button = getbtns();
	switches = getsw();
	button1 = getbtn1();
	Player1ScoreBoard();
	//Player2ScoreBoard(Player1Score);
	
	if (ballx == 0){
	Player2Score = Player2Score + 1;
		ballx = 64;
		bally = 16;
		//Player1ScoreBoard();
	}
	
	if (ballx == 128){
		Player1Score = Player1Score + 1;
		ballx = 64;
		bally = 16;
		//Player1ScoreBoard();
	}
	
	

	
	getRect(player1x,player1y,player1w,player1h,1); //Skapar spelare 1
	
	getRect(player2x,player2y,player2w,player2h,1); //skapar spelare 2
	
	setPixel(ballx, bally, 1); //Sätter ut bollen i mitten
	
	ballColision1();
	ballColision2();
	
	if (button == 4){
		move_player1up();
	}	
	if (button == 2) {
		move_player1down();
	}
	if (button1 == 1){
	move_player2down();
	}
	if (button == 1){
	move_player2up();
	}
	
	if (slut1 == 1){
		
		BallCollisionWall2();
		BallCollisionWall22();
		
		if (collisionThing == 1){
			move_ballplayer2();
			move_balldown();
		}
		
		if (collisionThing == 2){
			move_ballplayer2();
			move_ballup();			
		}
	
	}
	
	if (slut1 == 0){
		
		BallCollisionWall1();
		BallCollisionWall11();
		
			if (collisionThing == 1){
			move_ballplayer1();
			move_balldown();
		}
		
		if (collisionThing == 2){
			move_ballplayer1();
			move_ballup();			
		}
	}
	
	display_image(0, bufferstate);
	}
	
}	

while (GAME == 2){
	
	display_string(0,"");
	display_string(1,"");
	display_string(2,"PLAYER 1 GJ!!!!");
	display_string(3,"");
	display_update();
	
}

while (GAME == 3){
	display_string(0,"");
	display_string(1,"");
	display_string(2,"PLAYER 2 GJ!!!!");
	display_string(3,"");
	display_update();
}

return 0;	
}

