#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include "glut.h"

class Point2
{
public:
	Point2() { x = y = 0.0f; }
	Point2 (float xx, float yy) { x = xx; y = yy; }
	void set (float xx, float yy) { x = xx; y = yy; }
	float getX() { return x; }
	float getY() { return y; }
	void draw(void)
	{
		glBegin(GL_POINTS);
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();
	}

private:
	float x, y;
};

class IntRect
{
public:
	IntRect() { l = 0; r = 100; b = 0; t = 100; }
	IntRect (int left, int right, int bottom, int top)
		{ l = left; r = right; b = bottom; t = top; }
	void set (int left, int right, int bottom, int top)
		{ l = left; r = right; b = bottom; t = top; }
	void draw(void);

private:
	int l, r, b, t;
};

class RealRect
{
	public:
	RealRect() { l = 0; r = 100; b = 0; t = 100; }
	RealRect (float left, float right, float bottom, float top)
		{ l = left; r = right; b = bottom; t = top; }
	void set (float left, float right, float bottom, float top)
		{ l = left; r = right; b = bottom; t = top; }
	void draw(void)
	{

	}

private:
	float l, r, b, t;

};

class Canvas
{
public:
	Canvas (int width, int height, char* windowTitle);
	void setWindow(float l, float r, float b, float t);
	void setViewport (int l, int r, int b, int t);
	Point2 tween(Point2 A, Point2 B, float t);
	IntRect getViewport(void);
	RealRect getWindow(void);
	float getWindowAspectRatio(void);
	void clearScreen();
	void setBackgroundColor(float r, float g, float b);
	void setColor(float r, float g, float b);
	void lineTo (float x, float y);
	void lineTo (Point2 p);
	void lineRel(float dx, float dy);
	void moveTo(float x, float y);
	void moveTo(Point2 p);
	void moveRel(float dx, float dy);
	void drawTween(Point2 A[], Point2 B[], int n, float t);

private:
	Point2 CP;
	IntRect viewport;
	RealRect window;
};