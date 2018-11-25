#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include "glut.h"
#include "Canvas.h"

class Canvas;

Canvas::Canvas(int width, int height, char* windowTitle)
{
	char* argv[1];
	char dummyString[8];
	argv[0] = dummyString;
	int argc = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(20, 20);
	glutCreateWindow(windowTitle);
	setWindow(0, (float)width, 0, (float)height);
	setViewport(0, width, 0, height);
	CP.set(0.0f, 0.0f);
}

void Canvas::moveTo(float x, float y)
{
	CP.set(x, y);
}

void Canvas::lineTo(float x, float y)
{
	glBegin(GL_LINES);
		glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
		glVertex2f((GLfloat)x, (GLfloat)y);
	glEnd();
	CP.set(x, y);
	glFlush();
}

void Canvas::setWindow(float l, float r, float b, float t)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
	window.set(l, r, b, t);
}

void Canvas::moveRel(float dx, float dy)
{
	CP.set(CP.getX() + dx, CP.getY() + dy);
}

void Canvas:: lineRel (float dx, float dy)
{
	float x = CP.getX() + dx, y = CP.getY() + dy;
	lineTo(x, y);
	CP.set(x, y);
}

void Canvas::setViewport(int l, int r, int b, int t)
{
	glViewport(l, b, r-l, t - b);
}

Point2 Canvas::tween(Point2 A, Point2 B, float t)
{
	float newX;
	float newY;

	newX = (A.getX() + ( t * (B.getX()-A.getX())));
	newY = (A.getY() + ( t * (B.getY()-A.getY())));

	Point2 tmp (newX, newY);

	return tmp;
}

void Canvas::drawTween(Point2 A[], Point2 B[], int n, float t)
{
	for (int i = 0; i < n; i++)
	{
		Point2 P;
		P = tween(A[i], B[i], t);
		if (i == 0)
			moveTo(P.getX(), P.getY());
		else
			lineTo(P.getX(), P.getY());
	}
}

IntRect Canvas::getViewport()
{
	IntRect rect;

	GLint dims[4];
	glGetIntegerv(GL_VIEWPORT, dims);
	rect.set(dims[0], dims[1], dims[0] + dims[2], dims[1] + dims[3]);

	return rect;
}

void Canvas::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Canvas::setBackgroundColor(float r, float g, float b)
{
	glClearColor(r, g, b, 0.0);
}

void Canvas::setColor(float r, float g, float b)
{
	glColor3f(r, g, b);
}


void Canvas::lineTo(Point2 p)
{
	lineTo(p.getX(), p.getY());
}


void Canvas::moveTo(Point2 p)
{
	double x = p.getX();
	double y = p.getY();

	moveTo(x, y);
}

