#include <math.h>
#include "check.h"

int CheckHorizontalBoundCollision(Ball *B)
{
	if ((B->y + B->vy/2 - RADIUS <= DOWNBOUND && B->vy < 0)
		|| (B->y + B->vy/2 + RADIUS >= UPBOUND && B->vy > 0))
		return TRUE;
	else return FALSE;
}

int CheckVerticalBoundCollision(Ball *B)
{
	if ((B->x + B->vx/2 - RADIUS <= LEFTBOUND && B->vx < 0)
		|| (B->x + B->vx/2 + RADIUS >= RIGHTBOUND && B->vx > 0))
		return TRUE;
	else return FALSE;
}

int CheckBallCollision(Ball *B1, Ball *B2)
{
	if (pow(B1->x + B1->vx/2 - B2->x - B2->vx, 2) + pow(B1->y + B1->vy/2 - B2->y - B2->vy/2, 2) <= pow(2 * RADIUS, 2))
		return TRUE;
	else return FALSE;
}

int CheckInHole(Ball *B)
{
	if (pow(RIGHTBOUND - B->x, 2) + pow(B->y - DOWNBOUND, 2) <= pow(HOLERADIUS, 2) ||
		pow(RIGHTBOUND - B->x, 2) + pow(UPBOUND - B->y, 2) <= pow(HOLERADIUS, 2) ||
		pow(B->x - LEFTBOUND, 2) + pow(B->y - DOWNBOUND, 2) <= pow(HOLERADIUS, 2) ||
		pow(B->x - LEFTBOUND, 2) + pow(UPBOUND - B->y, 2) <= pow(HOLERADIUS, 2) ||
		pow(B->x - (LEFTBOUND + RIGHTBOUND)/2.0, 2) + pow(B->y - UPBOUND, 2) <= pow(HOLERADIUS, 2) ||
		pow(B->x - (LEFTBOUND + RIGHTBOUND) / 2.0, 2) + pow(B->y - DOWNBOUND, 2) <= pow(HOLERADIUS, 2))
		return TRUE;
	else return FALSE;
}

extern double mx, my;
extern Button buttons[];

int CheckInButton(int i)
{
	if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].width &&
		my >= buttons[i].y && my <= buttons[i].y + buttons[i].height)
		return TRUE;
	else return FALSE;
}