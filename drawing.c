#include <math.h>
#include <string.h>
#include "extgraph.h"
#include "angle.h"
#include "drawing.h"

#define PROMPTWIDTH 3
#define PROMPTHEIGHT 0.4
extern GameModeType gameMode;
extern PlayerType player;
extern Ball *B[];
extern Bag *List;
extern Button buttons[];
extern double mx, my, force;
static char color[16][9];

void DrawStartMenu()
{
	int i;
	/* 背景 */
	SetPenColor("BackColor");
	MovePen(0, 0);
	StartFilledRegion(1);
	DrawLine(GetWindowWidth(), 0);
	DrawLine(0, GetWindowHeight());
	DrawLine(-GetWindowWidth(), 0);
	DrawLine(0, -GetWindowHeight());
	EndFilledRegion();
	/* 桌子 */
	SetPenColor("TableBrown");
	StartFilledRegion(1);
	DrawLine(0.1, 0);
	SetAngle(45);
	Forward(4.8);
	DrawLine(8, 0);
	SetAngle(-45);
	Forward(4.8);
	DrawLine(0.1, 0);
	DrawLine(0, 0.3);
	SetAngle(135);
	Forward(4.92);
	DrawLine(-8, 0);
	SetAngle(-135);
	Forward(4.92);
	DrawLine(0, -0.29);
	DrawLine(0.1, 0);
	EndFilledRegion();
	SetPenColor("TableBrown");
	StartFilledRegion(1);
	MovePen(0, 0.3);
	SetAngle(45);
	Forward(4.96);
	DrawLine(8, 0);
	SetAngle(-45);
	Forward(4.96);
	DrawLine(0, 0.4);
	SetAngle(135);
	Forward(4.7);
	DrawLine(-8.36, 0);
	SetAngle(-135);
	Forward(4.7);
	DrawLine(0, -0.4);
	EndFilledRegion();
	MovePen(0.1, 0);
	SetPenColor("DarkGreen");
	StartFilledRegion(1);
	DrawLine(14.8, 0);
	SetAngle(135);
	Forward(4.81);
	DrawLine(-8, 0);
	SetAngle(-135);
	Forward(4.84);
	EndFilledRegion();
	/* 画球点缀 */
	MovePen(4, 3);
	SetPenColor("Red");
	StartFilledRegion(1);
	DrawArc(0.28, 0, 360);
	EndFilledRegion();
	MovePen(12, 2.5);
	SetPenColor("Yellow");
	StartFilledRegion(1);
	DrawArc(0.3, 0, 360);
	EndFilledRegion();
	MovePen(11, 1.5);
	SetPenColor("White");
	StartFilledRegion(1);
	DrawArc(0.34, 0, 360);
	EndFilledRegion();
	MovePen(5, 1);
	SetPenColor("Blue");
	StartFilledRegion(1);
	DrawArc(0.38, 0, 360);
	EndFilledRegion();
	MovePen(13.8, 1.1);
	SetPenColor("Magenta");
	StartFilledRegion(1);
	DrawArc(0.37, 0, 360);
	EndFilledRegion();
	/* 画桌线 */
	SetPenColor("Black");
	MovePen(3.5, 3.4);
	DrawLine(0, 0.4);
	MovePen(11.5, 3.4);
	DrawLine(0, 0.4);
	/* 标题 */
	SetPenColor("White");
	SetPointSize(50);
	MovePen(GetWindowWidth() / 2.0 - TextStringWidth("双人台球")/2.0, 6.5);
	DrawTextString("双人台球");
	SetPointSize(1);
	for (i = 0; i <= EXIT; i++) {
		DrawButton(buttons[i]);
	}
}

extern char prompt[80];
void DrawPrompt()
{
	SetPenColor("White");
	MovePen((LEFTBOUND + RIGHTBOUND)/2.0 - PROMPTWIDTH/2.0, UPBOUND + 0.6);
	StartFilledRegion(1);
	DrawLine(PROMPTWIDTH, 0);
	DrawArc(PROMPTHEIGHT / 2.0, -90, 180);
	DrawLine(-PROMPTWIDTH, 0);
	DrawArc(PROMPTHEIGHT / 2.0, 90, 180);
	EndFilledRegion();
	SetPenColor("Black");
	if (prompt) {
		MovePen((LEFTBOUND + RIGHTBOUND) / 2.0 - TextStringWidth(prompt) / 2.0, UPBOUND + 0.72);
		DrawTextString(prompt);
	}
}

void DrawGameMode()
{
	char buffer[10];
	SetPenColor("White");
	MovePen((LEFTBOUND + RIGHTBOUND) / 2.0 - 0.8, UPBOUND + 1.4);
	StartFilledRegion(1);
	DrawLine(1.6, 0);
	DrawLine(0, 0.4);
	DrawLine(-1.6, 0);
	DrawLine(0, -0.4);
	EndFilledRegion();
	SetPenColor("Black");
	switch (gameMode) {
	case poolEight:
		strcpy(buffer, "美式普尔");
		MovePen((LEFTBOUND + RIGHTBOUND) / 2.0 - TextStringWidth(buffer) / 2.0, UPBOUND + 1.53);
		DrawTextString("美式普尔");
		break;
	case caromFour:
		strcpy(buffer, "四球开伦");
		MovePen((LEFTBOUND + RIGHTBOUND) / 2.0 - TextStringWidth(buffer) / 2.0, UPBOUND + 1.53);
		DrawTextString("四球开伦");
		break;
	case nineBall:
		strcpy(buffer, "花式九球");
		MovePen((LEFTBOUND + RIGHTBOUND) / 2.0 - TextStringWidth(buffer) / 2.0, UPBOUND + 1.53);
		DrawTextString("花式九球");
		break;
	}

}

extern int score[2];
void DrawScore()
{
	char num[3];
	MovePen(LEFTPLAYERBAGSTARTX + RADIUS, PLAYERBAGY);
	SetPenColor("White");
	StartFilledRegion(1);
	DrawArc(RADIUS, 0, 360);
	EndFilledRegion();
	sprintf(num, "%d", score[0]);
	MovePen(LEFTPLAYERBAGSTARTX - TextStringWidth(num) / 2.0, PLAYERBAGY - 0.08);
	SetPenColor("Black");
	DrawTextString(num);
	MovePen(RIGHTPLAYERBAGSTARTX - RADIUS, PLAYERBAGY);
	SetPenColor("White");
	StartFilledRegion(1);
	DrawArc(RADIUS, -180, 360);
	EndFilledRegion();
	sprintf(num, "%d", score[1]);
	MovePen(RIGHTPLAYERBAGSTARTX - TextStringWidth(num) / 2.0, PLAYERBAGY - 0.08);
	SetPenColor("Black");
	DrawTextString(num);
}

void ShowForce()
{
	int i;
	SetPenColor("Black");
	MovePen(FORCEX - 0.06, FORCEY + FORCEHEIGHT + 0.1);
	StartFilledRegion(1);
	DrawLine(FORCEWIDTH + 0.12, 0);
	DrawLine(0, -1.98);
	DrawLine(-FORCEWIDTH - 0.12, 0);
	DrawLine(0, 1.98);
	EndFilledRegion();
	for (i = 0; i <= 10; i++) {
		switch (i) {
		case 0:
			SetPenColor("Red");
			break;
		case 1:
			SetPenColor("Orange");
			break;
		case 2:
			SetPenColor("Yellow");
			break;
		case 3:
			SetPenColor("Green");
			break;
		case 4:
			SetPenColor("DarkGreen");
			break;
		case 5:
			SetPenColor("Blue");
			break;
		case 6:
			SetPenColor("Cyan");
			break;
		case 7:
			SetPenColor("Violet");
			break;
		case 8:
			SetPenColor("Magenta");
			break;
		case 9:
			SetPenColor("Purple");
			break;
		case 10:
			SetPenColor("Gray");
			break;
		}
		MovePen(FORCEX + 0.03, FORCEY + FORCEHEIGHT - i * 0.15);
		StartFilledRegion(1);
		DrawLine(FORCEWIDTH - 0.06, 0);
		DrawLine(0, -0.15);
		DrawLine(-FORCEWIDTH + 0.06, 0);
		DrawLine(0, 0.15);
		EndFilledRegion();
	}
	SetPenColor("Dark Gray");
	MovePen(FORCEX + 0.03, FORCEY + FORCEHEIGHT - 11 * 0.15);
	StartFilledRegion(1);
	DrawLine(FORCEWIDTH - 0.06, 0);
	DrawLine(0, -0.13);
	DrawLine(-FORCEWIDTH + 0.06, 0);
	DrawLine(0, 0.13);
	EndFilledRegion();
	MovePen(FORCEX, FORCEY);
	SetPenColor("White");
	StartFilledRegion(1);
	MovePen(FORCEX, FORCEY - force * 7);
	DrawLine(FORCEWIDTH, 0);
	DrawLine(0, FORCEHEIGHT);
	DrawLine(-FORCEWIDTH, 0);
	DrawLine(0, -FORCEHEIGHT);
	EndFilledRegion();
}

void DrawMessageBox()
{
	SetPenColor("White");
	MovePen((LEFTBOUND + RIGHTBOUND)/2.0 - MESSAGEBOXWIDTH/2, (DOWNBOUND + UPBOUND)/2 - MESSAGEBOXHEIGHT/2);
	StartFilledRegion(1);
	DrawLine(MESSAGEBOXWIDTH, 0);
	DrawLine(0, MESSAGEBOXHEIGHT);
	DrawLine(-MESSAGEBOXWIDTH, 0);
	DrawLine(0, -MESSAGEBOXHEIGHT);
	EndFilledRegion();
}

void DrawButton(Button button)
{
	SetPenColor("ButtColor");
	StartFilledRegion(1);
	MovePen(button.x, button.y);
	DrawLine(button.width, 0);
	DrawLine(0, button.height);
	DrawLine(-button.width, 0);
	DrawLine(0, -button.height);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(button.x + button.width / 2.0 - TextStringWidth(button.text) / 2.0, button.y + button.height / 2.0 - 0.05);
	DrawTextString(button.text);
}

void DrawBackground()
{
	/* 背景 */ 
	SetPenColor("BackColor");
	MovePen(0, 0);
	StartFilledRegion(1);
	DrawLine(GetWindowWidth(), 0);
	DrawLine(0, GetWindowHeight());
	DrawLine(-GetWindowWidth(), 0);
	DrawLine(0, -GetWindowHeight());
	EndFilledRegion();

	DrawTable();
	MovePen(0.1, 0.1);
	DrawTextString("按下Esc回到主菜单");
	/* 画袋 */
	if (gameMode != caromFour) {
		MovePen(LEFTBOUND - 0.6, DOWNBOUND + RADIUS);
		SetPenColor("White");
		DrawArc(RADIUS, 0, -180);
		DrawLine(0, 28 * RADIUS);
		DrawArc(RADIUS, 180, -180);
		DrawLine(0, -28 * RADIUS);
	}
}

void DrawTable()
{
	int i;
	SetPenColor("TableBrown");
	StartFilledRegion(1);
	MovePen(LEFTBOUND, DOWNBOUND + HOLERADIUS + 0.1);
	DrawLine(-0.5, 0);
	DrawLine(0, UPBOUND - DOWNBOUND - 2 * HOLERADIUS - 0.2);
	DrawLine(0.5, 0);
	DrawLine(0, 0.2 + 2 * HOLERADIUS + DOWNBOUND - UPBOUND);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(RIGHTBOUND, DOWNBOUND + HOLERADIUS + 0.1);
	DrawLine(0.5, 0);
	DrawLine(0, UPBOUND - DOWNBOUND - 2 * HOLERADIUS - 0.2);
	DrawLine(-0.5, 0);
	DrawLine(0, 0.2 + 2 * HOLERADIUS + DOWNBOUND - UPBOUND);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(LEFTBOUND + HOLERADIUS + 0.1, DOWNBOUND);
	DrawLine(RIGHTBOUND - LEFTBOUND - 2 * HOLERADIUS - 0.2, 0);
	DrawLine(0, -0.5);
	DrawLine(0.2 + 2 * HOLERADIUS + LEFTBOUND - RIGHTBOUND, 0);
	DrawLine(0, 0.5);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(LEFTBOUND + HOLERADIUS + 0.1, UPBOUND);
	DrawLine(RIGHTBOUND - LEFTBOUND - 2 * HOLERADIUS - 0.2, 0);
	DrawLine(0, 0.5);
	DrawLine(0.2 + 2 * HOLERADIUS + LEFTBOUND - RIGHTBOUND, 0);
	DrawLine(0, -0.5);
	EndFilledRegion();
	SetPenColor("DarkGreen");
	StartFilledRegion(1);
	MovePen(LEFTBOUND, DOWNBOUND);
	DrawLine(RIGHTBOUND - LEFTBOUND, 0);
	DrawLine(0, UPBOUND - DOWNBOUND);
	DrawLine(LEFTBOUND - RIGHTBOUND, 0);
	DrawLine(0, DOWNBOUND - UPBOUND);
	EndFilledRegion();

	/* 画边上的白点 */
	SetPenColor("White");
	for (i = 1; i <= 7; i++) {
		MovePen(LEFTBOUND + HOLERADIUS + i * (RIGHTBOUND - LEFTBOUND - 2 * HOLERADIUS) / 8, UPBOUND + 0.25);
		StartFilledRegion(1);
		DrawArc(0.1, 0, 360);
		EndFilledRegion();
	}
	for (i = 1; i <= 7; i++) {
		MovePen(LEFTBOUND + HOLERADIUS + i * (RIGHTBOUND - LEFTBOUND - 2 * HOLERADIUS) / 8, DOWNBOUND - 0.25);
		StartFilledRegion(1);
		DrawArc(0.1, 0, 360);
		EndFilledRegion();
	}
	for (i = 1; i <= 3; i++) {
		MovePen(LEFTBOUND - 0.25, DOWNBOUND + HOLERADIUS + i * (UPBOUND - DOWNBOUND - 2 * HOLERADIUS) / 4);
		StartFilledRegion(1);
		DrawArc(0.1, 90, 360);
		EndFilledRegion();
	}
	for (i = 1; i <= 3; i++) {
		MovePen(RIGHTBOUND + 0.25, DOWNBOUND + HOLERADIUS + i * (UPBOUND - DOWNBOUND - 2 * HOLERADIUS) / 4);
		StartFilledRegion(1);
		DrawArc(0.1, 90, 360);
		EndFilledRegion();
	}

	/* 画洞 */
	SetPenColor("Yellow");
	StartFilledRegion(1);
	MovePen(LEFTBOUND, DOWNBOUND + HOLERADIUS);
	DrawLine(0, 0.1);
	DrawLine(-0.5, 0);
	DrawLine(0, -0.6);
	DrawLine(HOLERADIUS, 0);
	DrawLine(0, -HOLERADIUS);
	DrawLine(0.6, 0);
	DrawLine(0, 0.5);
	DrawLine(-0.1 - HOLERADIUS, 0);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(LEFTBOUND - 0.5 + HOLERADIUS, DOWNBOUND - 0.5);
	DrawArc(HOLERADIUS, -90, -180);
	DrawLine(0, -2 * HOLERADIUS);
	EndFilledRegion();
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		MovePen(LEFTBOUND + HOLERADIUS, DOWNBOUND);
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 0, 360);
		EndFilledRegion();
	}
	SetPenColor("Yellow");
	StartFilledRegion(1);
	MovePen(RIGHTBOUND, DOWNBOUND);
	DrawLine(0, 0.1 + HOLERADIUS);
	DrawLine(0.5, 0);
	DrawLine(0, -0.6);
	DrawLine(-HOLERADIUS, 0);
	DrawLine(0, -HOLERADIUS);
	DrawLine(-0.6, 0);
	DrawLine(0, 0.5);
	DrawLine(0.1 + HOLERADIUS, 0);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(RIGHTBOUND + 0.5 - HOLERADIUS, DOWNBOUND - 0.5);
	DrawArc(HOLERADIUS, -90, 180);
	DrawLine(0, -2 * HOLERADIUS);
	EndFilledRegion();
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		MovePen(RIGHTBOUND, DOWNBOUND + HOLERADIUS);
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 90, 360);
		EndFilledRegion();
	}
	SetPenColor("Yellow");
	StartFilledRegion(1);
	MovePen(LEFTBOUND, UPBOUND);
	DrawLine(0, -0.1 - HOLERADIUS);
	DrawLine(-0.5, 0);
	DrawLine(0, 0.6);
	DrawLine(HOLERADIUS, 0);
	DrawLine(0, HOLERADIUS);
	DrawLine(0.6, 0);
	DrawLine(0, -0.5);
	DrawLine(-0.1 - HOLERADIUS, 0);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(LEFTBOUND - 0.5 + HOLERADIUS, UPBOUND + 0.5);
	DrawArc(HOLERADIUS, 90, 180);
	DrawLine(0, 2 * RADIUS);
	EndFilledRegion();
	MovePen(LEFTBOUND, UPBOUND + HOLERADIUS);
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 90, 360);
		EndFilledRegion();
	}
	SetPenColor("Yellow");
	StartFilledRegion(1);
	MovePen(RIGHTBOUND, UPBOUND);
	DrawLine(0, -0.1 - HOLERADIUS);
	DrawLine(0.5, 0);
	DrawLine(0, 0.6);
	DrawLine(-HOLERADIUS, 0);
	DrawLine(0, HOLERADIUS);
	DrawLine(-0.6, 0);
	DrawLine(0, -0.5);
	DrawLine(0.1 + HOLERADIUS, 0);
	EndFilledRegion();
	StartFilledRegion(1);
	MovePen(RIGHTBOUND + 0.5 - HOLERADIUS, UPBOUND + 0.5);
	DrawArc(HOLERADIUS, 90, -180);
	DrawLine(0, 2 * RADIUS);
	EndFilledRegion();
	MovePen(RIGHTBOUND, UPBOUND + HOLERADIUS);
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 90, 360);
		EndFilledRegion();
	}
	MovePen((LEFTBOUND + RIGHTBOUND) / 2 + HOLERADIUS, DOWNBOUND);
	SetPenColor("Yellow");
	StartFilledRegion(1);
	DrawLine(0.3, 0);
	DrawLine(0, -0.5);
	DrawLine(-0.6 - 2 * HOLERADIUS, 0);
	DrawLine(0, 0.5);
	DrawLine(0.3 + 2 * HOLERADIUS, 0);
	EndFilledRegion();
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 0, 360);
		EndFilledRegion();
	}
	MovePen((LEFTBOUND + RIGHTBOUND) / 2 + HOLERADIUS, UPBOUND);
	SetPenColor("Yellow");
	StartFilledRegion(1);
	DrawLine(0.3, 0);
	DrawLine(0, 0.5);
	DrawLine(-0.6 - 2 * HOLERADIUS, 0);
	DrawLine(0, -0.5);
	DrawLine(0.3 + 2 * HOLERADIUS, 0);
	EndFilledRegion();
	if (gameMode == poolEight || gameMode == nineBall) {
		SetPenColor("Black");
		StartFilledRegion(1);
		DrawArc(HOLERADIUS, 0, 360);
		EndFilledRegion();
	}
	DrawPrompt();
	DrawGameMode();
	DrawButton(buttons[SAVEEXIT]);
	DrawButton(buttons[EXITGAME]);
	SetPenColor("White"); 
	MovePen(LEFTBOUND + 2.267, DOWNBOUND);
	DrawLine(0, UPBOUND - DOWNBOUND);
}

void DrawTableBall()
{
	int i;
	if (gameMode == poolEight) {
		for (i = 0; i <= 15; i++) {
			if (B[i]->onTable)
				B[i]->drawFunc(B[i]);
		}
	}
	else if (gameMode == caromFour) {
		for (i = 0; i <= 3; i++) {
			B[i]->drawFunc(B[i]);
		}
	}
	else if (gameMode == nineBall) {
		for (i = 0; i <= 9; i++) {
			B[i]->drawFunc(B[i]);
		}
	}
}

void DrawSideBall()
{
	Bag *p = List->next;
	int i, j = 0;
	char num[3];
	while (p) {
		i = 0;
		MovePen(LEFTBOUND - 0.6, DOWNBOUND + RADIUS*(2*j+1));
		if (p->ballID >= 9) {
			/* 绘制花色球 */
			SetPenColor("White");
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
			SetPenColor(color[p->ballID]);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 45);
			DrawLine(-RADIUS*sqrt(2), 0);
			DrawArc(RADIUS,135, 90);
			DrawLine(RADIUS*sqrt(2), 0);
			DrawArc(RADIUS, -45, 45);
			EndFilledRegion();
		}
		else {
			/* 绘制全色球 */
			SetPenColor(color[p->ballID]);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
		}
		/* 绘制编号的白色底 */ 
		SetPenColor("White");
		MovePen(LEFTBOUND - RADIUS + 2*RADIUS/5.0 - 0.6, DOWNBOUND + RADIUS*(2*j+1));
		StartFilledRegion(1);
		DrawArc(2*RADIUS/5.0, 0, 360);
		EndFilledRegion();
		/* 绘制球的编号 */ 
		SetPenColor("Black");
		if (p->ballID >= 10) num[i++] ='1'; 
		if (p->ballID != 0 ) num[i++] = p->ballID % 10 + '0';
		num[i] = '\0';
		MovePen(LEFTBOUND - RADIUS - TextStringWidth(num)/2 - 0.6, DOWNBOUND + RADIUS*(2*j+1) - GetFontHeight()/2);
		DrawTextString(num);
		p = p->next;
		j++;
	}
}

void DrawBall(Ball* B)
{
	char num[3];
	int id = B->id, i = 0;
	if (gameMode == poolEight || gameMode == nineBall) {
		if (B->onTable) {
			if (B->id >= 9) {
				/* 绘制花色球 */
				SetPenColor("White");
				MovePen(B->x + RADIUS, B->y);
				StartFilledRegion(1);
				DrawArc(RADIUS, 0, 360);
				EndFilledRegion();
				SetPenColor(color[B->id]);
				StartFilledRegion(1);
				DrawArc(RADIUS, 0, 45);
				DrawLine(-RADIUS * sqrt(2), 0);
				DrawArc(RADIUS, 135, 90);
				DrawLine(RADIUS * sqrt(2), 0);
				DrawArc(RADIUS, -45, 45);
				EndFilledRegion();
			}
			else {
				/* 绘制全色球 */
				SetPenColor(color[B->id]);
				MovePen(B->x + RADIUS, B->y);
				StartFilledRegion(1);
				DrawArc(RADIUS, 0, 360);
				EndFilledRegion();
			}
			/* 绘制编号的白色底 */
			SetPenColor("White");
			MovePen(B->x + 2 * RADIUS / 5.0, B->y);
			StartFilledRegion(1);
			DrawArc(2 * RADIUS / 5.0, 0, 360);
			EndFilledRegion();
			/* 绘制球的编号 */
			SetPenColor("Black");
			if (id >= 10) num[i++] = '1';
			if (id != 0) num[i++] = id % 10 + '0';
			num[i] = '\0';
			MovePen(B->x - TextStringWidth(num) / 2, B->y - GetFontHeight() * 2/5);
			DrawTextString(num);
		}
	}
	else if (gameMode == caromFour) {
		if (B->id == 0 || B->id == 1) {
			SetPenColor("White");
			MovePen(B->x + RADIUS, B->y);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
		}
		if (B->id == 1) {
			SetPenColor("Black");
			MovePen(B->x + RADIUS/6.0, B->y);
			StartFilledRegion(1);
			DrawArc(RADIUS/6.0, 0, 360);
			EndFilledRegion();
		}
		if (B->id == 2 || B->id == 3) {
			SetPenColor("Red");
			MovePen(B->x + RADIUS, B->y);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
		}
		if (B->id == 3) {
			SetPenColor("Black");
			MovePen(B->x + RADIUS / 6.0, B->y);
			StartFilledRegion(1);
			DrawArc(RADIUS / 6.0, 0, 360);
			EndFilledRegion();
		}
	}
}

void DrawClub()
{
	int i;
	if (gameMode == poolEight || gameMode == nineBall) i = 0;
	else if (gameMode == caromFour) i = player;
	if (mx == B[i]->x && my == B[i]->y) SetAngle(0);
	else if (mx - B[i]->x >= 0) {
		SetAngle(asin((my - B[i]->y)/sqrt(pow(mx - B[i]->x, 2)+ pow(my - B[i]->y, 2))) * 180/PIE);
	}
	else {
		if (my - B[i]->y >= 0) {
			SetAngle(180 - asin((my - B[i]->y)/sqrt(pow(mx - B[i]->x, 2)+ pow(my - B[i]->y, 2))) * 180/PIE);
		}
		else if(my - B[i]->y < 0) {
			SetAngle(-180 - asin((my - B[i]->y)/sqrt(pow(mx - B[i]->x, 2)+ pow(my - B[i]->y, 2))) * 180/PIE);
		}
	}
	MovePen(B[i]->x, B[i]->y);
	Move(2*RADIUS);
	SetPenColor("Black");
	StartFilledRegion(1);
	Turn(-90);
	Forward(CLUBWIDTH/2);
	Turn(90);
	Forward(CLUBLENGTH);
	Turn(90);
	Forward(CLUBWIDTH);
	Turn(90);
	Forward(CLUBLENGTH);
	Turn(90);
	Forward(CLUBWIDTH/2);
	EndFilledRegion();
	SetPenColor("White");
	StartFilledRegion(1);
	Forward(CLUBWIDTH/2);
	Turn(90);
	Forward(CLUBLENGTH*2/3);
	Turn(90);
	Forward(CLUBWIDTH);
	Turn(90);
	Forward(CLUBLENGTH*2/3);
	Turn(90);
	Forward(CLUBWIDTH/2);
	EndFilledRegion();
	
	/* 辅助线的绘制 */
	Turn(-90);
	Move(3*RADIUS);
	Forward(3); 
} 

void DrawPlayerBag(PlayerBag playerbag)
{
	int li = 0, ri = 0, i = 0, sign;
	double x;
	char num[3];
	Bag* p = (playerbag.bag)->next;
	if ((playerbag.bag)->ballID == -1) {
		//画左边
		x = LEFTPLAYERBAGSTARTX;
		sign = 1;
	}
	else {
		x = RIGHTPLAYERBAGSTARTX;
		sign = -1;
	}
	for (; p; p = p->next) {		
		MovePen(x + RADIUS + 2 * li * RADIUS, PLAYERBAGY);
		if (playerbag.flag == plain) {	/* 绘制全色球 */
			SetPenColor(color[p->ballID]);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
		}
		else {
			SetPenColor("White");
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 360);
			EndFilledRegion();
			SetPenColor(color[p->ballID]);
			StartFilledRegion(1);
			DrawArc(RADIUS, 0, 45);
			DrawLine(-RADIUS*sqrt(2), 0);
			DrawArc(RADIUS, 135, 90);
			DrawLine(RADIUS*sqrt(2), 0);
			DrawArc(RADIUS, -45, 45);
			EndFilledRegion();
		}
		SetPenColor("White");
		MovePen(x + 2 * RADIUS / 5.0 + 2*li*RADIUS, PLAYERBAGY);
		StartFilledRegion(1);
		DrawArc(2 * RADIUS / 5.0, 0, 360);
		EndFilledRegion();
		SetPenColor("Black");
		if (p->ballID >= 10) num[i++] = '1';
		if (p->ballID != 0) num[i++] = p->ballID % 10 + '0';
		num[i] = '\0';
		MovePen(x - TextStringWidth(num) / 2 + 2*li*RADIUS, PLAYERBAGY - GetFontHeight() / 2);
		DrawTextString(num);
		li += sign;
		i = 0;
	}
}

void DrawPlayer()
{
	double x;
	SetPenColor("White");
	MovePen(PLAYER1X, PLAYERY);
	StartFilledRegion(1);
	DrawLine(PLAYEREDGE, 0);
	DrawLine(0, PLAYEREDGE);
	DrawLine(-PLAYEREDGE, 0);
	DrawLine(0, -PLAYEREDGE);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(PLAYER1X + PLAYEREDGE / 2 - 0.28, PLAYERY + PLAYEREDGE / 2 - 0.05);
	DrawTextString("Player1");
	
	SetPenColor("White");
	StartFilledRegion(1);
	MovePen(PLAYER2X, PLAYERY);
	DrawLine(PLAYEREDGE, 0);
	DrawLine(0, PLAYEREDGE);
	DrawLine(-PLAYEREDGE, 0);
	DrawLine(0, -PLAYEREDGE);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(PLAYER2X + PLAYEREDGE / 2 - 0.28, PLAYERY + PLAYEREDGE / 2 - 0.05);
	DrawTextString("Player2");

	if (player == player1) x = PLAYER1X;
	else x = PLAYER2X;
	MovePen(x, PLAYERY);
	SetPenColor("Red");
	DrawLine(PLAYEREDGE, 0);
	DrawLine(0, PLAYEREDGE);
	DrawLine(-PLAYEREDGE, 0);
	DrawLine(0, - PLAYEREDGE);
}

void DrawTime(double time)
{
	SetPenColor("White");
	if (time <= 5) SetPenColor("Red");
	switch (player) {
		case player1:
			MovePen(LEFTBOUND, TIMEY);
			DrawLine(TIMELENGTH * time/TIMELIMIT, 0);
			break;
		case player2:
			MovePen(RIGHTBOUND, TIMEY);
			DrawLine(-TIMELENGTH * time/TIMELIMIT, 0);
			break;
	}
}

void UnshowTime()
{
	SetPenColor("BackColor");
	switch (player) {
	case player1:
		StartFilledRegion(1);
		MovePen(LEFTBOUND, TIMEY-0.01);
		DrawLine(TIMELENGTH, 0);
		DrawLine(0, 0.02);
		DrawLine(-TIMELENGTH, 0);
		DrawLine(0, -0.02);
		EndFilledRegion();
		break;
	case player2:
		StartFilledRegion(1);
		MovePen(RIGHTBOUND, TIMEY - 0.01);
		DrawLine(-TIMELENGTH, 0);
		DrawLine(0, 0.02);
		DrawLine(TIMELENGTH, 0);
		DrawLine(0, -0.02);
		EndFilledRegion();
		break;
	}
}

void ShowWinner(PlayerType player)
{
	char buffer[20];
	SetPenColor("Black");
	MovePen(6.5 + TextStringWidth("获胜者：")/2.0, 5.5);
	DrawTextString("获胜者：");
	SetPenColor("Red");
	sprintf(buffer, "Player%d", player + 1);
	MovePen(6.6 + TextStringWidth(buffer) / 2.0, 5.2);
	DrawTextString(buffer);
}

void InitColorArray()
{
	int i;
	strcpy(color[0], "White");
	strcpy(color[1], "Yellow");
	strcpy(color[2], "Blue");
	strcpy(color[3], "Red");
	strcpy(color[4], "Purple");
	strcpy(color[5], "Magenta");
	strcpy(color[6], "Green");
	strcpy(color[7], "Brown");
	strcpy(color[8], "Black");
	for (i = 9; i <= 15; i++) {
		strcpy(color[i], color[i - 8]);
	}
}

