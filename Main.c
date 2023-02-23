#include "graphics.h"
#include "extgraph.h" 
#include "simpio.h"
#include "genlib.h"
#include "imgui.h"
#include "angle.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "definition.h"
#include "drawing.h" 

   
/*
 * Constants
 * ---------
 * The following constants control the sizes of the
 * various elements in the display.
 */
#define INITX 8

/* Function prototypes */
void MouseEventProcess(int x, int y, int button, int event);
void TimerEventProcess(int timerID);
void KeyboardEventProcess(int key, int event);
void CreateBall();
void InitBall();
void InitPlayerBag();
void InitButton();
void DestroyBag();

/* Global variables */
Ball *B[16];
Bag *List, *plainBag, *stripBag;
int isPutBall = TRUE;
int isMove = FALSE;
int firstIn = TRUE;
int startBall = TRUE;
int stop = FALSE;
int over = FALSE;
double mx, my;
double force;
PlayerBag playerBag[2];
PlayerType player = player1;
Button buttons[13];
GameModeType gameMode = startMenu;
char prompt[80] = "游戏开始";
int score[2];
int foulCount[2];
int target = 1;
double* ptime;

void Main()
{
    InitGraphics();
	SetWindowTitle("双人台球");
    InitColorArray();
    InitPlayerBag();
	InitButton();
    List = (Bag *)malloc(sizeof(Bag));
    List->next = NULL;
    plainBag = (Bag *)malloc(sizeof(Bag));
	plainBag->ballID = -3;
    stripBag = (Bag *)malloc(sizeof(Bag));
	stripBag->ballID = -3;
    plainBag->next = stripBag->next = NULL;
    CreateBall();
	DrawStartMenu();
	registerTimerEvent(TimerEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
}

void TimerEventProcess(int timerID)
{
	int i, j;
	static double time = TIMELIMIT;
	static int timeflag = 1;
	static int firstCollision = -1, collision = -1;
	static int foul = FALSE, again = FALSE, collisionListener = FALSE;
	Bag *tmp, *p;
	ptime = &time;
	switch (timerID) {
		case TIMER_10:
			if (gameMode == startMenu) {
				DrawStartMenu();
				cancelTimer(TIMER_10);
				cancelTimer(TIMER_20);
				cancelTimer(TIMER_COUNT);
			}
			else {
				if (!over && !stop) {
					if (isMove) {
						force = 0;
						if (gameMode == poolEight) {
							for (i = 0; i <= 15; i++) {
								if (B[i]->onTable) {
									B[i]->x += B[i]->vx;
									B[i]->y += B[i]->vy;
									SlowDown(B[i]);
									if (CheckInHole(B[i])) {
										B[i]->onTable = FALSE;
										B[i]->vx = B[i]->vy = 0;
										/* 添加至进袋 */
										if (i != 0) {
											for (p = List; p->next; p = p->next);
											tmp = (Bag*)malloc(sizeof(Bag));
											tmp->ballID = i;
											tmp->next = NULL;
											p->next = tmp;
											DrawSideBall();
											if (i == 8) {
												/* 游戏结束 */
												over = TRUE;
												time = TIMELIMIT;
												firstCollision = -1;
												foul = again = collisionListener = FALSE;
												isMove = FALSE;
												firstIn = TRUE;
												isPutBall = startBall = TRUE;
												InitBall();
												DestroyBag();
												InitPlayerBag();
												DrawMessageBox();
												strcpy(prompt, "游戏开始");
												foulCount[0] = foulCount[1] = 0;
												cancelTimer(TIMER_10);
												cancelTimer(TIMER_20, 20);
												cancelTimer(TIMER_COUNT);
												if (playerBag[player].n != 7) {
													ShowWinner(!player);
												}
												else {
													ShowWinner(player);
												}
												player = player1;
												DrawButton(buttons[AGAIN]);
												DrawButton(buttons[BACK]);
												break;
											}
											else {
												/* 确认全色球、花色球 */
												if (firstIn) {
													firstIn = FALSE;
													playerBag[player].flag = (i <= 7) ? plain : strip;
													playerBag[player].bag = (i <= 7) ? plainBag : stripBag;
													(playerBag[player].bag)->ballID = -player - 1;/*bag的首ballID为-1表明为左玩家，-2表明为右玩家*/
													playerBag[!player].flag = (i >= 9) ? plain : strip;
													playerBag[!player].bag = (i >= 9) ? plainBag : stripBag;
													(playerBag[!player].bag)->ballID = -(!player) - 1;
												}

												for (p = (i <= 7) ? plainBag : stripBag; p->next; p = p->next);
												tmp = (Bag*)malloc(sizeof(Bag));
												tmp->ballID = i;
												tmp->next = NULL;
												p->next = tmp;
												if (i <= 7) {
													if (playerBag[player].flag == plain) {
														playerBag[player].n++;
														again = TRUE;
													}
													else playerBag[!player].n++;
												}
												else {
													if (playerBag[player].flag == strip) {
														playerBag[player].n++;
														again = TRUE;
													}
													else playerBag[!player].n++;
												}
												DrawPlayerBag(playerBag[player]);
												DrawPlayerBag(playerBag[!player]);
											}
										}
										else {
											isPutBall = TRUE;
											foul = TRUE;
										}
										continue;
									}
									if (CheckHorizontalBoundCollision(B[i])) {
										B[i]->vy = -B[i]->vy * RESTITUTION;
										collisionListener = TRUE;
									}
									else if (CheckVerticalBoundCollision(B[i])) {
										B[i]->vx = -B[i]->vx * RESTITUTION;
										collisionListener = TRUE;
									}
									else {
										for (j = i + 1; j <= 15; j++) {
											if (B[j]->onTable) {
												if (CheckBallCollision(B[i], B[j])) {
													BallCollisionProcess(B[i], B[j]);
													if (i == 0 && firstCollision == -1) {
														firstCollision = j;
														if (playerBag[player].flag == plain && j >= 9 ||
															playerBag[player].flag == strip && j <= 7 ||
															j == 8 && playerBag[player].n < 7)
															foul = TRUE;
													}
												}
											}
										}
									}
								}
							}
						}
						else if (gameMode == caromFour) {
							for (i = 0; i <= 3; i++) {
								B[i]->x += B[i]->vx;
								B[i]->y += B[i]->vy;
								SlowDown(B[i]);
								if (CheckHorizontalBoundCollision(B[i])) {
									B[i]->vy = -B[i]->vy * RESTITUTION;
								}
								else if (CheckVerticalBoundCollision(B[i])) {
									B[i]->vx = -B[i]->vx * RESTITUTION;
								}
								else {
									for (j = i + 1; j <= 3; j++) {
										if (CheckBallCollision(B[i], B[j])) {
											BallCollisionProcess(B[i], B[j]);
											if (i == player || j == player) {
												if (collision == -1) collision = (i == player) ? j:i;
												else if (((i == player)?j:i) != collision && !again) {
													again = TRUE;
													collision == -1;
												}
											}
										}
									}
								}
							}
						}
						else if (gameMode == nineBall) {
							for (i = 0; i <= 9; i++) {
								if (B[i]->onTable) {
									B[i]->x += B[i]->vx;
									B[i]->y += B[i]->vy;
									SlowDown(B[i]);
									if (CheckInHole(B[i])) {
										B[i]->onTable = FALSE;
										B[i]->vx = B[i]->vy = 0;
										/* 添加至进袋 */
										if (i != 0) {
											for (p = List; p->next; p = p->next);
											tmp = (Bag*)malloc(sizeof(Bag));
											tmp->ballID = i;
											tmp->next = NULL;
											p->next = tmp;
											DrawSideBall();
											if (i == 9) {
												/* 游戏结束 */
												over = TRUE;
												time = TIMELIMIT;
												InitBall();
												DestroyBag();
												DrawMessageBox();
												strcpy(prompt, "游戏开始");
												cancelTimer(TIMER_10);
												cancelTimer(TIMER_20, 20);
												cancelTimer(TIMER_COUNT);
												if (!foul) {
													ShowWinner(player);
												}
												else {
													ShowWinner(!player);
												}
												foulCount[0] = foulCount[1] = 0;
												firstCollision = -1;
												foul = again = FALSE;
												isMove = FALSE;
												isPutBall = TRUE;
												target = 1;
												player = player1;
												DrawButton(buttons[AGAIN]);
												DrawButton(buttons[BACK]);
												break;
											}
											else {
												if (!foul) again = TRUE;
											}
										}
										else {
											isPutBall = TRUE;
											foul = TRUE;
										}
										continue;
									}
									if (CheckHorizontalBoundCollision(B[i])) {
										B[i]->vy = -B[i]->vy * RESTITUTION;
									}
									else if (CheckVerticalBoundCollision(B[i])) {
										B[i]->vx = -B[i]->vx * RESTITUTION;
									}
									else {
										for (j = i + 1; j <= 9; j++) {
											if (B[j]->onTable) {
												if (CheckBallCollision(B[i], B[j])) {
													BallCollisionProcess(B[i], B[j]);
													if (i == 0 && firstCollision == -1) {
														firstCollision = j;
													}
												}
											}
										}
									}
								}
							}
						}
						if (!over) {
							DrawTable();
							DrawTableBall();
							if (gameMode == poolEight) {
								DrawSideBall();
								if (playerBag[0].bag) {
									DrawPlayerBag(playerBag[0]);
									DrawPlayerBag(playerBag[1]);
								}
							}
							else if (gameMode == caromFour)
								DrawScore();
							else if (gameMode == nineBall)
								DrawSideBall();
							DrawPlayer();
							if (gameMode == poolEight) {
								for (i = 0; i <= 15; i++) {
									if (B[i]->onTable) {
										if (B[i]->vx != 0 || B[i]->vy != 0) {
											break;
										}
									}
								}
								if (i == 16) {
									isMove = FALSE;
									/* 犯规检查 */
									if (!again && collisionListener == FALSE) foul = TRUE;
									if (foul) {
										isPutBall = TRUE;
										foulCount[player]++;
										sprintf(prompt, "Player%d击球犯规，已连续犯规%d次", player + 1, foulCount[player]);
										if (foulCount[player] == 3) {
											/* 连犯三次规直接判负 */
											DrawPrompt();
											over = TRUE;
											time = TIMELIMIT;
											firstCollision = -1;
											foul = again = collisionListener = FALSE;
											isMove = FALSE;
											firstIn = TRUE;
											isPutBall = startBall = TRUE;
											InitBall();
											DestroyBag();
											InitPlayerBag();
											DrawMessageBox();
											strcpy(prompt, "游戏开始");
											foulCount[0] = foulCount[1] = 0;
											cancelTimer(TIMER_10);
											cancelTimer(TIMER_20);
											cancelTimer(TIMER_COUNT);
											ShowWinner(!player);
											player = player1;
											DrawButton(buttons[AGAIN]);
											DrawButton(buttons[BACK]);
											break;
										}
									}
									else {
										foulCount[player] = 0;
										if (!again) sprintf(prompt, "Player%d常规击球,交换击球方", player + 1);
										else sprintf(prompt, "Player%d进球，再来一杆", player + 1);
									}
									if (!again || foul) player = !player;
									firstCollision = -1;
									foul = again = collisionListener = FALSE;
									time = TIMELIMIT;
								}
							}
							else if (gameMode == caromFour) {
								for (i = 0; i <= 3; i++) {
									if (B[i]->vx != 0 || B[i]->vy != 0) {
										break;
									}
								}
								if (i == 4) {
									isMove = FALSE;
									if (!again) {
										sprintf(prompt, "Player%d常规击球,交换击球方", player + 1);
										player = !player;
									}
									else {
										sprintf(prompt, "Player%d击球得分，再来一杆", player + 1);
										score[player]++;
										if (score[player] == 6) {//一方得分6则游戏结束
											over = TRUE;
											InitBall();
											DrawScore();
											DrawMessageBox();
											strcpy(prompt, "游戏开始");
											foulCount[0] = foulCount[1] = 0;
											score[0] = score[1] = 0;
											time = TIMELIMIT;
											again = FALSE;
											collision = -1;
											cancelTimer(TIMER_10);
											cancelTimer(TIMER_20);
											cancelTimer(TIMER_COUNT);
											ShowWinner(player);
											player = player1;
											DrawButton(buttons[AGAIN]);
											DrawButton(buttons[BACK]);
											break;
										}
									}
									collision = -1;
									again = FALSE;
									time = TIMELIMIT;
								}
							}
							else if (gameMode == nineBall) {
								for (i = 0; i <= 9; i++) {
									if (B[i]->vx != 0 || B[i]->vy != 0)
										break;
								}
								if (i == 10) {
									isMove = FALSE;
									if (firstCollision != target) foul = TRUE;
									target = GetTarget();
									if (foul) {
										sprintf(prompt, "Player%d击球犯规，Player%d获自由球", player + 1, !player + 1);
										player = !player;
										isPutBall = TRUE;
									}
									else if (!again) {
										sprintf(prompt, "Player%d常规击球,交换击球方", player + 1);
										player = !player;
									}
									else {
										sprintf(prompt, "Player%d进球，再来一杆", player + 1);
									}
									firstCollision = -1;
									again = FALSE;
									time = TIMELIMIT;
									foul = FALSE;
								}
							}
						}
					}
				}
			}
			break;
		case TIMER_20:
			if ((gameMode == poolEight || gameMode == nineBall) && !over && !stop && isPutBall && !isMove) {
				if (mx <= RIGHTBOUND - RADIUS && mx >= LEFTBOUND + RADIUS
					&& my <= UPBOUND - RADIUS && my >= DOWNBOUND + RADIUS) {
					B[0]->x = mx;
					B[0]->y = my;
					B[0]->onTable = TRUE;
					if (!CheckInHole(B[0])) {
						if (!startBall || gameMode == nineBall) {
							DrawTable();
							DrawTableBall();
							DrawSideBall();
						}
						else {
							if (mx <= LEFTBOUND + 2.267) {
								DrawTable();
								DrawTableBall();
								DrawSideBall();
							}
						}
						if (playerBag[0].bag) {
							DrawPlayerBag(playerBag[0]);
							DrawPlayerBag(playerBag[1]);
						}
						DrawPlayer();
					}
				}
				UnshowTime();
				DrawTime(time);
			}
			if (!over && !stop && !isMove && (!isPutBall && (gameMode == poolEight || gameMode == nineBall) || gameMode == caromFour)) {
				DrawBackground();
				DrawTableBall();
				if (gameMode == poolEight) {
					DrawSideBall();
					if (playerBag[0].bag) {
						DrawPlayerBag(playerBag[0]);
						DrawPlayerBag(playerBag[1]);
					}
				}
				else if (gameMode == caromFour) {
					DrawScore();
				}
				else if (gameMode == nineBall)
					DrawSideBall();
				DrawPlayer();
				DrawTime(time);
				ShowForce();
				DrawClub();
			}
			break;
		case TIMER_FORCE:
			force += timeflag*0.01;
			if (force >= 0.25) timeflag = -1;
			else if (force <= 0) timeflag = 1;
			break;
		case TIMER_COUNT:
			/* 倒计时 */
			if (!stop) {
				time -= 0.1;
				if (time <= 0.0000001) {
					foulCount[player]++;
					sprintf(prompt, "Player%d超时犯规，已连续犯规%d次", player + 1, foulCount[player]);
					DrawBackground();
					DrawTableBall();
					DrawPlayer();
					if (gameMode == poolEight) {
						DrawSideBall();
						if (playerBag[0].bag) {
							DrawPlayerBag(playerBag[0]);
							DrawPlayerBag(playerBag[1]);
						}
					}
					else if (gameMode == caromFour) {
						DrawScore();
					}
					else if (gameMode == nineBall) {
						DrawSideBall();
					}
					if (foulCount[player] == 3) {
						DrawPrompt();
						over = TRUE;
						time = TIMELIMIT;
						firstCollision = -1;
						foul = again = collisionListener = FALSE;
						target = 1;
						isMove = FALSE;
						firstIn = TRUE;
						isPutBall = startBall = TRUE;
						InitBall();
						DestroyBag();
						InitPlayerBag();
						DrawMessageBox();
						strcpy(prompt, "游戏开始");
						foulCount[0] = foulCount[1] = 0;
						cancelTimer(TIMER_10);
						cancelTimer(TIMER_20);
						cancelTimer(TIMER_COUNT);
						ShowWinner(!player);
						player = player1;
						DrawButton(buttons[AGAIN]);
						DrawButton(buttons[BACK]);
					}
					else {
						player = !player;
						time = TIMELIMIT;
						isPutBall = TRUE;
					}
				}
			}
	}
}

void MouseEventProcess(int x, int y, int button, int event)
{
	mx = ScaleXInches(x);
	my = ScaleYInches(y);
	int i;
	switch (button) {
	case LEFT_BUTTON:
		switch (event) {
		case BUTTON_UP:
			switch (gameMode) {
			case startMenu:
				if (CheckInButton(POOLEIGHT)) {
					gameMode = poolEight;
					InitBall();
					DrawBackground();
					DrawTableBall();
					startTimer(TIMER_10, TIMER_10BLINK);
					startTimer(TIMER_20, 20);
					startTimer(TIMER_COUNT, 100);
				}
				else if (CheckInButton(CAROMFOUR)) {
					gameMode = caromFour;
					InitBall();
					DrawBackground();
					DrawTableBall();
					startTimer(TIMER_10, TIMER_10BLINK);
					startTimer(TIMER_20, 20);
					startTimer(TIMER_COUNT, 100);
				}
				else if (CheckInButton(NINEBALL)) {
					gameMode = nineBall;
					InitBall();
					DrawBackground();
					DrawTableBall();
					startTimer(TIMER_10, TIMER_10BLINK);
					startTimer(TIMER_20, 20);
					startTimer(TIMER_COUNT, 100);
				}
				else if (CheckInButton(TXTCONTINUE)) {
					/* 提供gameMode */
					FILE* fp;
					if (fp = fopen("txtdata.txt", "r")) {
						fscanf(fp, "%*[^1-9]%d", &gameMode);
						fclose(fp);
					}
					InitBall();
					InitPlayerBag();
					TxtRead();
					DrawBackground();
					DrawTableBall();
					if (gameMode == poolEight || gameMode == nineBall)
						DrawSideBall();
					if (gameMode == poolEight) {
						if (playerBag[0].bag) {
							DrawPlayerBag(playerBag[0]);
							DrawPlayerBag(playerBag[1]);
						}
					}
					if (gameMode == caromFour)
						DrawScore();
					startTimer(TIMER_10, TIMER_10BLINK);
					startTimer(TIMER_20, 20);
					startTimer(TIMER_COUNT, 100);
				}
				else if (CheckInButton(BICONTINUE)) {
					/* 提供gameMode */
					FILE* fp;
					if (fp = fopen("bidata.da", "r")) {
						fread(&gameMode, sizeof(int), 1, fp);
						fclose(fp);
					}
					InitBall();
					InitPlayerBag();
					BiRead();
					DrawBackground();
					DrawTableBall();
					DrawSideBall();
					if (playerBag[0].bag) {
						DrawPlayerBag(playerBag[0]);
						DrawPlayerBag(playerBag[1]);
					}
					startTimer(TIMER_10, TIMER_10BLINK);
					startTimer(TIMER_20, 20);
					startTimer(TIMER_COUNT, 100);
				}
				else if (CheckInButton(EXIT))
					exit(0);
				break;
			case poolEight:case caromFour:case nineBall:
				if (over) {
					if (CheckInButton(AGAIN)) {
						over = FALSE;
						DrawBackground();
						DrawTableBall();
						startTimer(TIMER_10, TIMER_10BLINK);
						startTimer(TIMER_20, 20);
						startTimer(TIMER_COUNT, 200);
					}
					else if (CheckInButton(BACK)) {
						over = FALSE;
						gameMode = startMenu;
						DrawStartMenu();
					}
				}
				else if (!stop) {
					if (gameMode == poolEight) {
						if (isPutBall) {
							for (i = 1; i <= 15; i++) {
								if (B[i]->onTable && CheckBallCollision(B[0], B[i]))
									break;
							}
							if (i == 16 && mx <= RIGHTBOUND - RADIUS && mx >= LEFTBOUND + RADIUS
								&& my <= UPBOUND - RADIUS && my >= DOWNBOUND + RADIUS && !CheckInHole(B[0])) {
								if (!startBall) isPutBall = FALSE;
								else {
									if (mx <= LEFTBOUND + 2.267) {
										isPutBall = FALSE;
										startBall = FALSE;
									}
								}
							}
						}
						else if (!isMove) {
							Hit();
							DrawBackground();
							DrawTableBall();
							if (playerBag[0].bag) {
								DrawPlayerBag(playerBag[0]);
								DrawPlayerBag(playerBag[1]);
							}
							cancelTimer(TIMER_FORCE);
						}
					}
					else if (gameMode == caromFour) {
						if (!isMove) {
							Hit();
							DrawBackground();
							DrawTableBall();
							DrawScore();
							cancelTimer(TIMER_FORCE);
						}
					}
					else if (gameMode == nineBall) {
						if (isPutBall) {
							for (i = 1; i <= 9; i++) {
								if (B[i]->onTable && CheckBallCollision(B[0], B[i]))
									break;
							}
							if (i == 10 && mx <= RIGHTBOUND - RADIUS && mx >= LEFTBOUND + RADIUS
								&& my <= UPBOUND - RADIUS && my >= DOWNBOUND + RADIUS && !CheckInHole(B[0]))
								isPutBall = FALSE;
						}
						else if (!isMove) {
							Hit();
							DrawBackground();
							DrawTableBall();
							cancelTimer(TIMER_FORCE);
						}
					}
				}
				break;
			}
			break;
		case BUTTON_DOWN:
			if (gameMode != startMenu) {
				if (!stop && !over) {
					if (CheckInButton(EXITGAME))
						exit(0);
					if (CheckInButton(SAVEEXIT) && !isMove) {
						stop = TRUE;
						DrawMessageBox();
						DrawButton(buttons[BISAVE]);
						DrawButton(buttons[TXTSAVE]);
					}
					if (((gameMode == poolEight || gameMode == nineBall) && !isPutBall || gameMode == caromFour) && !isMove) {
						startTimer(TIMER_FORCE, 100);
					}
				}
				else if (stop && !over) {
					if (CheckInButton(TXTSAVE)) {
						TxtSave();
						exit(0);
					}
					else if (CheckInButton(BISAVE)) {
						BiSave();
						exit(0);
					}
				}
			}
			break;
		}
		break;
	}
}

void KeyboardEventProcess(int key, int event)
{
	switch (event) {
	case KEY_UP:
		if (key == VK_ESCAPE) {
			if (gameMode != startMenu) {
				if (!isMove && !over && !stop) {
					firstIn = TRUE;
					isPutBall = startBall = TRUE;
					InitBall();
					DestroyBag();
					InitPlayerBag();
					DrawMessageBox();
					strcpy(prompt, "游戏开始");
					foulCount[0] = foulCount[1] = 0;
					cancelTimer(TIMER_10);
					cancelTimer(TIMER_20, 20);
					cancelTimer(TIMER_COUNT);
					player = player1;
					gameMode = startMenu;
					DrawStartMenu();
					*ptime = TIMELIMIT;
				}
			}
		}
	}
}

void CreateBall()
{
	int i;
	for (i = 0; i <= 15; i++) {
		B[i] = (Ball *)malloc(sizeof(Ball));
		B[i]->id = i;
	}
}
void InitBall()
{
	int i;
	for (i = 0; i <= 15; i++) {
		B[i]->vx = B[i]->vy = 0;
		B[i]->onTable = TRUE;
		B[i]->drawFunc = DrawBall;
	}

	if (gameMode == poolEight) {
		B[0]->x = LEFTBOUND + 2.267;
		B[0]->y = (UPBOUND + DOWNBOUND) /2.0;
		B[1]->x = INITX;
		B[1]->y = (UPBOUND + DOWNBOUND)/2.0;
		B[2]->x = B[3]->x = B[1]->x + sqrt(3)*(RADIUS + 0.03);
		B[2]->y = B[1]->y + (RADIUS + 0.03);
		B[3]->y = B[1]->y - (RADIUS + 0.03);
		B[4]->x = B[5]->x = B[6]->x = B[2]->x + sqrt(3)*(RADIUS + 0.03);
		B[4]->y = B[2]->y + (RADIUS + 0.03);
		B[5]->y = B[1]->y;
		B[6]->y = B[3]->y - (RADIUS + 0.03);
		B[7]->x = B[8]->x = B[9]->x = B[10]->x = B[4]->x + sqrt(3)*(RADIUS + 0.03);
		B[7]->y = B[4]->y + (RADIUS + 0.03);
		B[8]->y = B[2]->y;
		B[9]->y = B[3]->y;
		B[10]->y = B[6]->y - (RADIUS + 0.03);
		B[11]->x = B[12]->x = B[13]->x = B[14]->x = B[15]->x = B[7]->x + sqrt(3)*(RADIUS + 0.03);
		B[11]->y = B[7]->y + (RADIUS + 0.03);
		B[12]->y = B[4]->y;
		B[13]->y = B[5]->y;
		B[14]->y = B[6]->y;
		B[15]->y = B[10]->y - (RADIUS + 0.03);
	}
	else if (gameMode == caromFour) {
		B[0]->x = LEFTBOUND + 6*RADIUS;
		B[1]->x = B[0]->x + 6*RADIUS;
		B[3]->x = RIGHTBOUND - 6*RADIUS;
		B[2]->x = B[3]->x - 6*RADIUS;
		B[0]->y = B[1]->y = B[2]->y = B[3]->y = (UPBOUND + DOWNBOUND)/2.0;
	}
	else if (gameMode == nineBall) {
		B[0]->x = LEFTBOUND + 2.267;
		B[0]->y = (UPBOUND + DOWNBOUND) / 2.0;
		B[1]->x = INITX;
		B[1]->y = (UPBOUND + DOWNBOUND) / 2.0;
		B[2]->x = B[3]->x = B[1]->x + sqrt(3) * (RADIUS + 0.03);
		B[2]->y = B[1]->y + (RADIUS + 0.03);
		B[3]->y = B[1]->y - (RADIUS + 0.03);
		B[4]->x = B[9]->x = B[6]->x = B[2]->x + sqrt(3) * (RADIUS + 0.03);
		B[4]->y = B[2]->y + (RADIUS + 0.03);
		B[9]->y = B[1]->y;
		B[6]->y = B[3]->y - (RADIUS + 0.03);
		B[7]->x = B[8]->x = B[4]->x + sqrt(3) * (RADIUS + 0.03);
		B[7]->y = B[2]->y;
		B[8]->y = B[3]->y;
		B[5]->x = B[7]->x + sqrt(3) * (RADIUS + 0.03);
		B[5]->y = B[9]->y;
	}
}

void InitPlayerBag()
{
	playerBag[0].bag = playerBag[1].bag = NULL;
	playerBag[0].flag = playerBag[1].flag = special;
	playerBag[0].n = playerBag[1].n = 0;
}

void InitButton()
{
	int i;
	for (i = 0; i <= 11; i++) {
		buttons[i].width = BUTTONWIDTH;
		buttons[i].height = BUTTONHEIGHT;
	}
	/* startMenu界面按钮的初始化 */
	for (i = 0; i <= 5; i++) {
		buttons[i].x = GetWindowWidth() / 2.0 - BUTTONWIDTH / 2.0;
		buttons[i].y = 4.8 - i * 0.9;
	}
	buttons[POOLEIGHT].text = "美式普尔";
	buttons[CAROMFOUR].text = "四球开伦";
	buttons[2].text = "花式九球";
	buttons[BICONTINUE].text = "载入二进制存档";
	buttons[TXTCONTINUE].text = "载入文本存档";
	buttons[EXIT].text = "退出游戏";
	buttons[AGAIN].x = 5.5;
	buttons[AGAIN].y = 3.6;
	buttons[AGAIN].text = "再来一局";
	buttons[BACK].x = 7.5;
	buttons[BACK].y = 3.6;
	buttons[BACK].text = "返回主菜单";
	buttons[SAVEEXIT].x = 13.5;
	buttons[SAVEEXIT].y = 0.9;
	buttons[SAVEEXIT].text = "保存并退出游戏";
	buttons[EXITGAME].x = 13.5;
	buttons[EXITGAME].y = 0;
	buttons[EXITGAME].text = "退出游戏";
	buttons[BISAVE].x = 5.5;
	buttons[BISAVE].y = 3.6;
	buttons[BISAVE].text = "以二进制保存";
	buttons[TXTSAVE].x = 7.5;
	buttons[TXTSAVE].y = 3.6;
	buttons[TXTSAVE].text = "以文本保存";
}

void DestroyBag()
{
	Bag *p, *tmp;
	p = List->next;
	while (p) {
		tmp = p->next;
		free(p);
		p = tmp;
	}
	List->next = NULL;
	if (gameMode == poolEight) {
		plainBag->ballID = -3;
		p = plainBag->next;
		while (p) {
			tmp = p->next;
			free(p);
			p = tmp;
		}
		plainBag->next = NULL;
		stripBag->ballID = -3;
		p = stripBag->next;
		while (p) {
			tmp = p->next;
			free(p);
			p = tmp;
		}
		stripBag->next = NULL;
	}
}

int GetTarget()
{
	int i;
	for (i = 1; i <= 9; i++) {
		if (B[i]->onTable == TRUE) {
			return i;
		}
	}
}