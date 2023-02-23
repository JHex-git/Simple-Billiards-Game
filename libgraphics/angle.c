#include<math.h>
#include<stdio.h> 
#include"graphics.h"

#define PIE 3.14159265359 

static double degree;

void Turn(double angle)
{
	angle = angle/180*PIE;
	degree -= angle;
	if(degree < -PIE) {
		degree += 2 * PIE;
	} else if(degree > PIE) {
		degree -= 2 * PIE;
	}
}

void Move(double distance)
{
	double x, y;
	x = GetCurrentX();
	y = GetCurrentY();
	x += distance*cos(degree);
	y += distance*sin(degree);
	MovePen(x,y);
}

void Forward(double distance)
{
	DrawLine(distance*cos(degree), distance*sin(degree));
}

void SetAngle(double angle)
{
	degree = angle/180*PIE;
}

double GetAngle()
{
	return degree/PIE*180;
}
