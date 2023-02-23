#include "physics.h"
#include <math.h>

extern double mx, my, force;
extern int isMove;
extern Ball *B[];
extern GameModeType gameMode;
extern PlayerType player;
void Hit()
{
	double cosa, sina;
	int i;
	if (gameMode == poolEight || gameMode == nineBall) i = 0;
	else if (gameMode == caromFour) i = player;
	if (pow(B[i]->x - mx, 2) + pow(B[i]->y - my, 2) == 0) return;
	cosa = (B[i]->x - mx) / sqrt(pow(B[i]->x - mx, 2) + pow(B[i]->y - my, 2)),
	sina = (B[i]->y - my) / sqrt(pow(B[i]->x - mx, 2) + pow(B[i]->y - my, 2));
	B[i]->vx = force * cosa;
	B[i]->vy = force * sina;
	isMove = TRUE;
}

void BallCollisionProcess(Ball *B1, Ball *B2)
{
	double distance = sqrt(pow(B2->x - B1->x,2) + pow(B2->y - B1->y, 2)),
		cosa = (B2->x - B1->x) / distance,
		sina = (B2->y - B1->y) / distance,
		v_10 = B1->vx * cosa + B1->vy * sina,
		v_20 = B2->vx * cosa + B2->vy * sina,
		v1_t = (v_10 + v_20)/2.0 - RESTITUTION/2.0*(v_10 - v_20),
		v2_t = (v_10 + v_20)/2 - RESTITUTION/2.0*(v_20 - v_10),
		v1_n = -B1->vx*sina + B1->vy*cosa,
		v2_n = -B2->vx*sina + B2->vy*cosa;
	B1->vx = v1_t*cosa - v1_n*sina;
	B1->vy = v1_t*sina + v1_n*cosa;
	B2->vx = v2_t*cosa - v2_n*sina;
	B2->vy = v2_t*sina + v2_n*cosa;
}

void SlowDown(Ball *B)
{
	double orivx = B->vx, orivy = B->vy;
	if (B->vx == 0 && B->vy == 0) return;
	B->vx -= ACCELERATION * B->vx/sqrt(pow(B->vx, 2) + pow(B->vy, 2));
	B->vy -= ACCELERATION * B->vy/sqrt(pow(B->vx, 2) + pow(B->vy, 2));
	if (orivx * B->vx <= 0 && orivy * B->vy <= 0) {
		B->vx = 0;
		B->vy = 0;
	}
}
