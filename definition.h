#ifndef _DEFINITION_H
#define _DEFINITION_H

#define MAXCOLORNUM 17
#define RADIUS 0.18
#define DOWNBOUND 1
#define UPBOUND 7
#define LEFTBOUND 1.5
#define RIGHTBOUND 13
#define HOLERADIUS 0.36
#define CLUBLENGTH 6
#define CLUBWIDTH 0.1
#define PIE 3.14159265359
#define MESSAGEBOXWIDTH 4
#define MESSAGEBOXHEIGHT 3
#define FORCEX 14
#define FORCEY 6
#define FORCEWIDTH 0.2
#define FORCEHEIGHT 0.05
#define LEFTPLAYERBAGSTARTX 2.5
#define RIGHTPLAYERBAGSTARTX 12
#define PLAYERBAGY 8.2
#define PLAYEREDGE 0.75
#define PLAYER1X LEFTBOUND
#define PLAYER2X (RIGHTBOUND - PLAYEREDGE)
#define PLAYERY 8
#define TIMELIMIT 20
#define TIMELENGTH 4
#define TIMEY 7.6
#define RESTITUTION ((gameMode == caromFour)? 0.8 : 0.75)
#define TIMER_10 1
#define TIMER_FORCE 2
#define TIMER_COUNT 3
#define TIMER_20 4
#define TIMER_10BLINK 5
#define ACCELERATION ((gameMode == caromFour)? 0.0003 : 0.00043)
#define FALSE 0
#define TRUE 1
#define POOLEIGHT 0
#define CAROMFOUR 1
#define NINEBALL 2
#define BICONTINUE 3
#define TXTCONTINUE 4
#define EXIT 5
#define AGAIN 6
#define BACK 7
#define SAVEEXIT 8
#define EXITGAME 9
#define BISAVE 10
#define TXTSAVE 11
#define STOPBACK 12
#define BUTTONWIDTH 1.6
#define BUTTONHEIGHT 0.8

typedef enum{
	startMenu,
	poolEight,
	caromFour,
	nineBall
}GameModeType;

typedef enum{
	player1 = 0,
	player2
}PlayerType;

typedef struct Ball{
	int id;
	double x, y;
	double vx, vy;
	int onTable;
	void (*drawFunc)(struct Ball *);
}Ball;

typedef struct Bag{
	int ballID;
	struct Bag *next;
}Bag;

typedef struct{
	Bag *bag;
	enum{special = -1, plain, strip}flag;
	int n;
}PlayerBag;

typedef struct {
	double x, y, width, height;
	char* text;
}Button;

#endif
