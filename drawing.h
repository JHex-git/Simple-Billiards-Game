#ifndef _DRAWING_H
#define _DRAWING_H

#include "definition.h"

void ShowForce();

void DrawPrompt();

void DrawGameMode();

void DrawStartMenu();

void DrawMessageBox();

void DrawButton(Button button);

void DrawScore();

void DrawBackground();

void DrawTable();

void DrawTableBall();

void DrawSideBall();

void DrawBall(Ball *B);

void DrawClub();

void DrawPlayerBag(PlayerBag playerbag);

void DrawPlayer();

void DrawTime(double time);

void UnshowTime();

void ShowWinner(PlayerType player);

void InitColorArray();

#endif
