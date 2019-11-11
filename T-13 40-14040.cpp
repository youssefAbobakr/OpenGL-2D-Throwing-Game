#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

//game Variables
int turn = 1;
float power = 0;
bool isShooting = false;
bool isPowerUp = true;
float powerUpY = -248;
float p1Health = 100;
float p2Health = 100;
bool projectileTraveling = false;
float projectileX, projectileY, t;
bool hit = false;
bool collieded = false;
float damage;
float pandaMultiplier = 1;
float robotMultiplier = 1;
bool scalePanda = false;
bool scaleRobot = false;

//bezier variables
int startPoint[2];
int controlPoint1[2];
int controlPoint2[2];
int endPoint[2];

//defender variables
float defXPos = -525;
float defYPos;
int defState = 1;

//kiteVariabels
float kiteX = -380;
float kiteY = 250;
bool isKiteRight = true;
bool isKiteUp = true;

//method declarations
void display();
void reshape(int w, int h);
void shoot(unsigned char key, int x, int y);
void charge(unsigned char key, int, int);
void init();
void timer(int);
int* bezier(double t, int* p0, int* p1, int* p2, int* p3);
void throwProjectile();
void didCollideWithDef();
void didHit();
void print(int x, int y, char* string);


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(1300, 750);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("iPanda");
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(charge);
	glutKeyboardUpFunc(shoot);

	glutMainLoop();
}

void init()
{
	glClearColor(0.525,0.671, 0.647, 1);	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); //Clear
	glLoadIdentity(); //Clears any transformations... resets coordinate 
	//glPointSize(10);
	//kite
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex2f(kiteX, kiteY);
	glVertex2f(kiteX + 30, kiteY + 40);
	glVertex2f(kiteX + 50, kiteY - 5);
	glColor3f(1, 1, 0);
	glVertex2f(kiteX, kiteY);
	glVertex2f(kiteX + 30, kiteY - 45);
	glVertex2f(kiteX + 50, kiteY - 5);
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-350, 0);
	glVertex2f(kiteX + 30, kiteY - 45);
	glEnd();
	

	//Ground
	glBegin(GL_QUADS);
	glColor3f(0.42, 0.576, 0.384);
	glVertex2f(-650, -375);
	glVertex2f(-650, 0);
	glVertex2f(650, 0);
	glVertex2f(650, -375);
	glEnd();

	//background Fence
	glColor3f(0.945, 0.906, 0.847);
	for (int x = -500; x <= 460; x += 40)
	{
		glBegin(GL_POLYGON);
		glVertex2f(x, 0);
		glVertex2f(x, 150);
		glVertex2f(x + 20, 175);
		glVertex2f(x + 40, 150);
		glVertex2f(x + 40, 0);
		glEnd();
	}

	//fence outline
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	for (int x = -500; x <= 460; x += 40)
	{
		glVertex2f(x, 0);
		glVertex2f(x, 150);
		glVertex2f(x, 150);
		glVertex2f(x + 20, 175);
		glVertex2f(x + 20, 175);
		glVertex2f(x + 40, 150);
		glVertex2f(x + 40, 150);
		glVertex2f(x + 40, 0);
	}
	glVertex2f(-650, 0);
	glVertex2f(650, 0);
	glEnd();


	//left Building
	glBegin(GL_QUADS);
	glColor3f(0.302, 0.294, 0.227);
	glVertex2f(-650, -375);
	glVertex2f(-650, 375);
	glVertex2f(-500, 375);
	glVertex2f(-500, 0);
	glEnd();

	//leftBuilding Outline
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-650, -375);
	glVertex2f(-575, -187.5);
	glVertex2f(-575, -187.5);
	glVertex2f(-500, 0);
	glEnd();
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-500,0);
	glVertex2f(-500, 375);
	glEnd();


	//right Building
	glBegin(GL_QUADS);
	glColor3f(0.302, 0.294, 0.227);
	glVertex2f(650, -375);
	glVertex2f(650, 375);
	glVertex2f(500, 375);
	glVertex2f(500, 0);
	glEnd();

	//right building Outline
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(650, -375);
	glVertex2f(575, -187.5);
	glVertex2f(575, -187.5);
	glVertex2f(500, 0);
	glEnd();
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(500, 0);
	glVertex2f(500, 375);
	glEnd();

	//barricade
	glBegin(GL_QUADS);
	glColor3f(0.827, 0.694, 0.49);
	glVertex2f(-30,-375);
	glVertex2f(-30, -50);
	glVertex2f(30, -50);
	glVertex2f(30, -375);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(-30, -50);
	glVertex2f(-10, 150);
	glVertex2f(10, 150);
	glVertex2f(30, -50);
	glEnd();

	//barricad Outline
	glLineWidth(6);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-30, -375);
	glVertex2f(-30, -50);
	glVertex2f(-30, -50);
	glVertex2f(30, -50);
	glVertex2f(30, -50);
	glVertex2f(30, -375);
	glEnd();

	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(-30, -50);
	glVertex2f(-10, 150);
	glVertex2f(-10, 150);
	glVertex2f(10, 150);
	glVertex2f(10, 150);
	glVertex2f(30, -50);
	glEnd();

	//Panda
	glPushMatrix();
	glScalef(pandaMultiplier, 1,0);
	glBegin(GL_QUADS);
	//frontView
	glColor3f(1, 1, 1);
	glVertex2f(-400, -88 );
	glVertex2f(-400  , -38  );
	glVertex2f(-325  , -38  );
	glVertex2f(-325  , -88  );
	//sideView
	glVertex2f(-325  , -88  );
	glVertex2f(-325  , -38  );
	glVertex2f(-309  , 2  );
	glVertex2f(-309  , -48  );
	//topView
	glVertex2f(-400  , -38  );
	glVertex2f(-384  , 2  );
	glVertex2f(-309  , 2  );
	glVertex2f(-325  , -38  );
	glEnd();

	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-400  ,-88  );
	glVertex2f(-400  , -38  );
	glVertex2f(-400  , -38  );
	glVertex2f(-325  , -38  );
	glVertex2f(-325  , -38  );
	glVertex2f(-325  , -88  );
	glVertex2f(-325  , -88  );
	glVertex2f(-400  , -88  );

	glVertex2f(-325  , -38  );
	glVertex2f(-309  , 2  );
	glVertex2f(-309  , 2  );
	glVertex2f(-309  , -48  );
	glVertex2f(-309  , -48  );
	glVertex2f(-325  , -88  );

	glVertex2f(-400  , -38  );
	glVertex2f(-384  , 2  );
	glVertex2f(-384  , 2  );
	glVertex2f(-309  ,2  );
	glVertex2f(-309  , 2  );
	glVertex2f(-325  , -38  );
	glEnd();

	//nose
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(-320  , -65 );
	glVertex2f(-320  , -52.5  );
	glVertex2f(-315  , -40  );
	glVertex2f(-315  , -52.5  );
	glEnd();

	//Ears
	glBegin(GL_TRIANGLES);
	glVertex2f(-350  , -20  );
	glVertex2f(-370  ,-30  );
	glVertex2f(-360  , -5  );
	glVertex2f(-365  , -5  );
	glVertex2f(-345  , -15  );
	glVertex2f(-355  , 10  );
	glEnd();

	//Panda Outline
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-400,-88);
	glVertex2f(-400, -38);
	glVertex2f(-400, -38);
	glVertex2f(-325, -38);
	glVertex2f(-325, -38);
	glVertex2f(-325, -88);
	glVertex2f(-325, -88);
	glVertex2f(-400, -88);

	glVertex2f(-325, -38);
	glVertex2f(-309, 2);
	glVertex2f(-309, 2);
	glVertex2f(-309, -48);
	glVertex2f(-309, -48);
	glVertex2f(-325, -88);

	glVertex2f(-400, -38);
	glVertex2f(-384, 2);
	glVertex2f(-384, 2);
	glVertex2f(-309,2);
	glVertex2f(-309, 2);
	glVertex2f(-325, -38);
	glEnd();
	glPopMatrix();


	//Robot
	glPushMatrix();
	glScalef(robotMultiplier, 1, 0);
	glBegin(GL_QUADS);
	//frontView
	glColor3f(0.35, 0.35, 0.35);
	glVertex2f(400, -88);
	glVertex2f(400, -38);
	glVertex2f(325, -38);
	glVertex2f(325, -88);
	//sideView
	glVertex2f(325, -88);
	glVertex2f(325, -38);
	glVertex2f(309, 2);
	glVertex2f(309, -48);
	//topView
	glVertex2f(400, -38);
	glVertex2f(384, 2);
	glVertex2f(309, 2);
	glVertex2f(325, -38);
	glEnd();

	glColor3f(1,1,0);
	glBegin(GL_QUADS);
	glVertex2f(325, -73);
	glVertex2f(325, -63);
	glVertex2f(309, -33);
	glVertex2f(309, -43);
	glEnd();

	//Robot Outline
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(400, -88);
	glVertex2f(400, -38);
	glVertex2f(400, -38);
	glVertex2f(325, -38);
	glVertex2f(325, -38);
	glVertex2f(325, -88);
	glVertex2f(325, -88);
	glVertex2f(400, -88);

	glVertex2f(325, -38);
	glVertex2f(309, 2);
	glVertex2f(309, 2);
	glVertex2f(309, -48);
	glVertex2f(309, -48);
	glVertex2f(325, -88);

	glVertex2f(400, -38);
	glVertex2f(384, 2);
	glVertex2f(384, 2);
	glVertex2f(309, 2);
	glVertex2f(309, 2);
	glVertex2f(325, -38);
	glEnd();
	glPopMatrix();

	//Defender
	glBegin(GL_QUADS);
	glVertex2f(defXPos, defYPos - 30);
	glVertex2f(defXPos, defYPos);
	glVertex2f(defXPos + 50, defYPos);
	glVertex2f(defXPos + 50, defYPos - 30);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex2f(defXPos + 50, defYPos);
	glVertex2f(defXPos + 70, defYPos - 15);
	glVertex2f(defXPos + 50, defYPos-30);
	glEnd();

	glBegin(GL_POINTS);
	glColor3f(0,0,0);
	glPointSize(3);
	glVertex2f(defXPos + 60, defYPos -15);
	glEnd();


	//Projectile Curve
	if (isShooting)
	{
		glPointSize(5);
		glColor3f(1, 1, 0);
		for (float t = 0; t < 1; t += 0.001)
		{
		glBegin(GL_POINTS);
			int* p = bezier(t, startPoint, controlPoint1, controlPoint2, endPoint);
			glVertex2f(p[0], p[1]);
		glEnd();
		}
	}

	//Projectile Motion
	if (projectileTraveling)
	{
		//glPushMatrix();
		//glTranslatef(-309 * turn, -48, 0);
		//glRotatef(t * 30 * turn, 0, 0, 1);
		//glTranslatef(309 * turn, 48, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(1,0,0);
		glVertex2f(projectileX, projectileY);
		glVertex2f(projectileX + 10 * turn, projectileY + 5);
		glVertex2f(projectileX + 10 * turn, projectileY - 5);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(0,0,0);
		glVertex2f(projectileX + 100 * turn, projectileY -2);
		glVertex2f(projectileX + 100 * turn, projectileY + 2);
		glVertex2f(projectileX + 10 * turn, projectileY + 2);
		glVertex2f(projectileX + 10 * turn, projectileY - 2);
		glEnd();
		//glPopMatrix();
	}


	//HUD
	//Panda Health Bar
	glBegin(GL_QUADS);
	glColor3f(0.75, 0, 0);
	glVertex2f(-620, 315);
	glVertex2f(-620, 345);
	glColor3f(0, 0.75, 0);
	glVertex2f(-100, 345);
	glVertex2f(-100, 315);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.39, 0.39, 0.39);
	glVertex2f(-100, 315);
	glVertex2f(-100, 345);
	glVertex2f(-100 - (520 * (100 - p1Health)/100), 345);
	glVertex2f(-100 - (520 * (100 - p1Health)/100), 315);
	glEnd();

	//Robot Health Bar
	glBegin(GL_QUADS);
	glColor3f(0.75, 0, 0);
	glVertex2f(620, 315);
	glVertex2f(620, 345);
	glColor3f(0, 0.75, 0);
	glVertex2f(100, 345);
	glVertex2f(100, 315);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.39, 0.39, 0.39);
	glVertex2f(100, 315);
	glVertex2f(100, 345);
	glVertex2f(100 + (520 * (100 - p2Health)/100), 345);
	glVertex2f(100 + (520 * (100 - p2Health)/100), 315);
	glEnd();

	//Health bar outlines
	glLineWidth(10);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-620, 315);
	glVertex2f(-620, 345);
	glVertex2f(-620, 345);
	glVertex2f(-100, 345);
	glVertex2f(-100, 345);
	glVertex2f(-100, 315);
	glVertex2f(-100, 315);
	glVertex2f(-620, 315);

	glVertex2f(620, 315);
	glVertex2f(620, 345);
	glVertex2f(620, 345);
	glVertex2f(100, 345);
	glVertex2f(100, 345);
	glVertex2f(100, 315);
	glVertex2f(100, 315);
	glVertex2f(620, 315);
	glEnd();

	//Throw power
	if (isShooting) 
	{
	glBegin(GL_QUADS);
	glColor3f(0.34, 0.34, 0.34);
	glVertex2f(-550 * turn, -250);
	glVertex2f(-520 * turn, -250);
	glVertex2f(-520 * turn, -50);
	glVertex2f(-550 * turn, -50);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(power, 1-power, 0);
	glVertex2f(-550 * turn, -250);
	glVertex2f(-520 * turn, -250);
	glVertex2f(-520 * turn, powerUpY + 200 * power);
	glVertex2f(-550 * turn, powerUpY + 200 * power);
	glEnd();
	
	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0);
	glVertex2f(-550 * turn, powerUpY + 10 + 200 * power);
	glVertex2f(-530 * turn, powerUpY + 200 * power);
	glVertex2f(-550 * turn, powerUpY -10 + 200 * power);
	glEnd();

	//powerBar Outline
	glLineWidth(5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-550 * turn, -250);
	glVertex2f(-550 * turn, -50);
	glVertex2f(-550 * turn, -50);
	glVertex2f(-520 * turn, -50);
	glVertex2f(-520 * turn, -50);
	glVertex2f(-520 * turn, -250);
	glVertex2f(-520 * turn, -250);
	glVertex2f(-550 * turn, -250);
	glEnd();
	}


	//winCondition
	if (p2Health <= 0)
	{
		p2Health = 0;
		char* message = "Panda Wins";
		print(0, 200, message);
	}
	if (p1Health <= 0)
	{
		p1Health = 0;
		char* message = "Robot Wins";
		print(0, 200, message);
	}
	glutSwapBuffers();
}

//Projectile Functions
void throwProjectile()
{
	if (t > 1 || projectileY < -88 || hit) {
		projectileTraveling = false;
		t = 0;
		return;
	}
	int* p = bezier(t, startPoint, controlPoint1, controlPoint2, endPoint);
	projectileX = p[0];
	projectileY = p[1];
}
void shoot(unsigned char key, int x, int y)
{
	if (key == 32 && !projectileTraveling && p2Health > 0 && p1Health > 0)
	{
		float t = 0;
		projectileTraveling = true;
		damage = 25 * power;
		power = 0;
		isShooting = false;
		if (turn == 1)
			turn = -1;
		else
			turn = 1;
	}
}
void charge(unsigned char key, int x, int y)
{
	if (key == 32 && !projectileTraveling && p2Health > 0 && p1Health >0)
	{
		isShooting = true;
		switch (isPowerUp)
		{
		case true:
			if (power < 1)
				power += 0.05;
			else
				isPowerUp = false;
			break;
		case false:
			if (power > 0)
				power -= 0.05;
			else
				isPowerUp = true;
			break;
		}

		//bezier points declaration
		startPoint[0] = -309 * turn;
		startPoint[1] = -48;
		controlPoint1[0] = -100 * turn;
		controlPoint1[1] = 200;
		controlPoint2[0] = 100 * turn;
		controlPoint2[1] = 200 ;
		endPoint[0] = 700 * power * turn;
		endPoint[1] = -48;
	}
}

//Engine Functions
void reshape(int w, int h)
{	
	//update the viewport
	glViewport(0, 0, w, h); 
	
	//switch matrix mode to setup projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-650,650,-375,375);
	glMatrixMode(GL_MODELVIEW);
}
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
	
	if (scalePanda)
	{
		if (pandaMultiplier < 1.5)
			pandaMultiplier += 0.1;
		else {
			pandaMultiplier = 1;
			scalePanda = false;
		}
	}
	if (scaleRobot)
	{
		if (robotMultiplier < 1.5)
			robotMultiplier += 0.1;
		else {
			robotMultiplier = 1;
			scaleRobot = false;
		}
	}
	
	//defender Motion
	switch (defState)
	{
	case 1:
		if (defXPos < 475)
			defXPos += 5;
		else
			defState = -1;

		defYPos = 120 - 70 * -cos((defXPos) / 180);
		break;
	case -1:
		if (defXPos > -525)
			defXPos -= 5;
		else
			defState = 1;
		defYPos = 120 - 70 * -cos((defXPos) / 180);
		break;
	}

	//kiteMotion
	switch (isKiteRight)
	{
	case (true):
		if (kiteX < -300)
			kiteX += 0.4;
		else
			isKiteRight = false;
		break;

	case false:
		if (kiteX > -340)
			kiteX -= 0.4;
		else
			isKiteRight = true;
		break;
	}
	switch (isKiteUp)
	{
	case true:
		if (kiteY < 260)
			kiteY += 0.15;
		else
			isKiteUp = false;
		break;
	case(false):
		if (kiteY > 250)
			kiteY -= 0.15;
		else
			isKiteUp = true;
		break;
	}

	//projectile Motion
	if (collieded)
	{
		projectileTraveling = false;
		collieded = false;
	}
	if (hit)
	{
		projectileTraveling = false;
		if (turn == -1) {

			p2Health -= damage;
			scaleRobot = true;
		}
		else {
			p1Health -= damage;
			scalePanda = true;
		}

		hit = false;
	}
	if (projectileTraveling)
	{
		throwProjectile();
		t += 0.01;
		didCollideWithDef();
		didHit();
	}
	
}
int* bezier(double t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3) * p0[0] + 3 * t * pow((1 - t), 2) * p1[0] + 3 * pow(t, 2) * (1 - t) * p2[0] + pow(t, 3) * p3[0];
	res[1] = pow((1 - t), 3) * p0[1] + 3 * t * pow((1 - t), 2) * p1[1] + 3 * pow(t, 2) * (1 - t) * p2[1] + pow(t, 3) * p3[1];
	return res;
}

//collisionDetection
void didCollideWithDef()
{
	bool collisionX;
	if (turn == -1)
		collisionX = defXPos <= projectileX && defXPos+50 >= projectileX;
	else
		collisionX = defXPos + 50 >= projectileX && defXPos <= projectileX;

	bool collisionY = defYPos >= projectileY && defYPos - 30 <= projectileY + 4;

	collieded = collisionX && collisionY;
	if(collieded)
		t = 0;
}

void didHit()
{
	bool collisionX;
	if (turn == -1)
		collisionX = 309 <= projectileX && 400 >= projectileX;
	else
		collisionX = -309 >= projectileX && -400 <= projectileX;
	bool collisionY = -88 <= projectileY && 2 >= projectileY;
	if (collisionX || collisionY)
		printf(" %d,%d \n", collisionX, collisionY);
	hit = collisionX && collisionY;
	if(hit)
		t = 0;
}

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
