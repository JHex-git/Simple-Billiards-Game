#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "definition.h"

void Hit();

void BallCollisionProcess(Ball *B1, Ball *B2);

void SlowDown(Ball *B);
 
#endif
