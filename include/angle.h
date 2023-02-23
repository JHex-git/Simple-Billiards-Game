#ifndef _angle_h
#define _angle_h

/* Ω«∂»÷∆ */ 
/* Function: Turn
 * Usage: Turn(angle)
 * ---------------------
 * This procedure turns the angle of pen.
 * The parameters must be degree measure.
 * The pen will rotate counterclockwise when angle is minus.
 */
 
void Turn(double angle);


/* Function: Move
 * Usage: Move(distance)
 * -----------------------
 * This procedure will move the pen towards its angle,
 * but it will not draw a line.
 */
 
void Move(double distance);


/* Function: Forward
 * Usage: Forward(distance)
 * ---------------------------
 * This procedure will move the pen towards its angle,
 * and will simultaneously draw a line along the way.
 */
 
void Forward(double distance);


/* Function: SetAngle
 * Usage: SetAngle(angle)
 * ---------------------------
 * This procedure will set the angle to the appointed value
 */
 
void SetAngle(double angle);


/* Function: GetAngle
 * Usage: GetAngle()
 * ---------------------------
 * This procedure will return the current angle
 */
 
double GetAngle();
#endif
