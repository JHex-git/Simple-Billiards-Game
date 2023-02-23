#ifndef _CHECK_H
#define _CHECK_H

#include "definition.h"

int CheckHorizontalBoundCollision(Ball *B);

int CheckVerticalBoundCollision(Ball *B);

int CheckBallCollision(Ball *B1, Ball *B2);

int CheckInHole(Ball *B);

int CheckInButton(int i);

#endif
