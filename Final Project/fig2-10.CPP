#include <Windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GL.h>
#include "glut.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
using namespace std;

/******************* This program is designed for 4D data, please use the provided data to ensure it works as intended **********************/

int round(double);	// this needs to be up here for the iris class
int numOfDimensions = 0;	// this will be overwritten

class Iris
{
public:
	vector<double> data;		// the data points of the flower
	enum Types { SETOSA, VERSICOLOR, VIRGINICA };

	Types flowerType;
	
	Iris(vector<double> vec)
	{
		for (int i = 0; i < numOfDimensions; i++)
		{
			data.push_back(vec[i]);
		}

		int intType = round(vec[4]);

		if (intType == 1.0)
			flowerType = SETOSA;
		else if (intType == 2.0)
			flowerType = VERSICOLOR;
		else if(intType == 3.0)
			flowerType = VIRGINICA;
	}

	Types getType() { return flowerType; }
	double getData(int i) { return data[i]; }
	vector<double> getData() { return data; }

	void print()	// for debugging purposes only
	{
		double d1 = data[0];
		double d2 = data[1];
		double d3 = data[2];
		double d4 = data[3];

		printf("%0.1f %0.1f %0.1f %0.1f ", d1, d2, d3, d4);	// using printf here so doubles like 3.0 will print the .0

		if (flowerType == SETOSA)
			cout << "Setosa";
		else if(flowerType == VERSICOLOR)
			cout << "Versicolor";
		else if (flowerType == VIRGINICA)
			cout << "Virginica";
		else
			cout << "ERROR" << endl;
	}

};

void angleFileOpen();
void anglesAndOrigins(char*);
void Display();
void drawArrowHead(double, double, double, double);
void drawBaseLines();
void drawFlatHead(double, double, double, double);
void drawVector(int, double);
void drawVerticalLines();
void figure31();
void figure33();
void irisFileOpen();
void keyboard(unsigned char, int, int);
void myInit();
void readFileData(char*);

int startingWidth(int);

double getMax(int);

bool showArrowHead = true;
bool showFlatHead = false; 
bool showRed = true;
bool showGreen = true;
bool showBlue = true;
bool showBlack = true;
bool toggle = true;	// true means 3.1, false is 3.3

const int numOfPoints = 4;	// we will be using 4D data for this program
const int screenWidth = 640;
const int screenHeight = 480;

const double TWO_PI = 2 * 3.14159265;
const double DEFAULT_ZOOM = .95;
const double HEAD_SCALE = 6.0;
double zoom = DEFAULT_ZOOM;	// Arbitrary size, modifies the scale of the vectors

vector<float> angles;
vector<float> xAxis;
vector<float> yAxis;
vector<Iris> flowerVec;

// main functions
void main(int argc, char** argv)
{
	irisFileOpen();

	/*	This was for debugging purposes
	for (int i = 0; i < 150; i++)
	{
		flowerVec[i].print();
		cout << " | " << i << endl;
	}
	*/

	angleFileOpen();

	Display();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Figures 3.1 & 3.3");
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	myInit();
	glutMainLoop();
}

void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(4.20);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);

	if (toggle)
	{
		drawVerticalLines();
		drawBaseLines();
		figure31();
	}
	else
	{
		drawVerticalLines();
		drawBaseLines();
		figure33();
	}

	glEnd();
	glFlush();
}

void keyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch(theKey)
	{
	case 'a':	// changes end style to arrow heads
		showArrowHead = true;
		showFlatHead = false;
		Display();
		break;

	case 'f':	// changes end style to flat heads
		showArrowHead = false;
		showFlatHead = true;
		Display();
		break;

	case 'n':	// removes end style
		showArrowHead = false;
		showFlatHead = false;
		Display();
		break;

	case '1':	// toggles red lines | 1 or r
	case 'r':
		showRed = !showRed;
		Display();
		break;

	case '2':	// toggles green lines | 2 or g
	case 'g':
		showGreen = !showGreen;
		Display();
		break;

	case '3':	// togges black lines | 3 or b
	case 'b':
		showBlue = !showBlue;
		Display();
		break;

	case '4':	// toggles black lines | 4 or shift + b or l
	case 'B':
	case 'l':
		showBlack = !showBlack;
		Display();
		break;

	case 32:		// Switches between 3.1 and 3.3 | 32 is ascii for spacebar
		toggle = !toggle;
		Display();
		break;

	case 43:	// Increases vector size | ascii for plus (+)
		zoom += .05;
		Display();
		break;

	case 45:	// Decreases vector size | ascii for minus (-)
		zoom -= .05;
		Display();
		break;

	case '0':	// resets the vector scale adjustor
		zoom = DEFAULT_ZOOM;
		Display();
		break;

	case 'E':	// Exit the program | shift + e
		exit(-1);

	default:
		break;
	}
}

// void functions
void angleFileOpen() {
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select the angles and origins file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
		anglesAndOrigins(filename);

	else {
		std::cout << "test";
	}
}

void anglesAndOrigins(char* fileName)
{
	ifstream file(fileName);
	string str;

	// ignore first line(headers)
	if (getline(file, str));

	stringstream ss;

	int counter = 0;

	while(getline(file, str))
	{
		int tokenStart = 0;
		int tokenEnd = 0;

		while (str.size() > 0)
		{
			tokenStart = 0;
			tokenEnd = str.find_first_of(',');

			if (tokenEnd < 0) // if the is only one left
				tokenEnd = str.size();

			int tokenLength = tokenEnd - tokenStart;
			string currentToken = str.substr(tokenStart, tokenLength);

			float newData;

			ss << currentToken;
			ss >> newData;
			ss.clear();

			switch(counter)
			{
			case 0:
				angles.push_back(newData);
				break;

			case 1:
				xAxis.push_back(newData);
				break;

			case 2:
				yAxis.push_back(newData);
				break;
			}

			// remove token from string
			if (tokenEnd == str.size())	// last token left
			{
				str = "";
			} else						// more than one token left
			{
				str = str.substr(tokenEnd + 1, str.size() - tokenLength + 1);
			}

		} // while str.size

		counter++;
	} // while getline
}

void drawArrowHead(double angle, double scale, double endX, double endY)
{
	glVertex2f(endX, endY);
	glVertex2f(endX + cos((angle + 0.40) * TWO_PI) * scale, endY + sin((angle + 0.40) * TWO_PI) * scale);

	glVertex2f(endX + cos((angle - 0.40) * TWO_PI) * scale, endY + sin((angle - 0.40) * TWO_PI) * scale);
	glVertex2f(endX, endY);
}

void drawBaseLines()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	
	double baseLineScale = 10.0;

	if (toggle)
	{
		for (int i = 0; i < numOfDimensions; i++)
		{
			double startX = xAxis[i] * screenWidth;
			double startY = yAxis[i] * screenHeight;
		
			double cosAngle = cos(angles[i] * TWO_PI);
			double sinAngle = sin(angles[i] * TWO_PI);

			double length = screenWidth * 0.2 * zoom; // an arbitrary size
		
			double endX = startX + cosAngle * length;
			double endY = startY + sinAngle * length;

			if (showBlack)
			{
				// the body of the arrow
				glVertex2f(startX, startY);
				glVertex2f(endX, endY); // cos for x, sin for y

				// the arrow head
				if (showArrowHead)
					drawArrowHead(angles[i], baseLineScale, endX, endY);

				// the flat head
				else if (showFlatHead)
					drawFlatHead(angles[i], baseLineScale, endX, endY);
			}
		}
	}
	else
	{
		double startX = xAxis[0] * screenWidth;
		double startY = yAxis[0] * screenHeight;

		for (int i = 0; i < numOfDimensions; i++)
		{
			double cosAngle = cos(angles[i] * TWO_PI);
			double sinAngle = sin(angles[i] * TWO_PI);

			double length = screenWidth * 0.2 * zoom; // an arbitrary size
		
			double endX = startX + cosAngle * length;
			double endY = startY + sinAngle * length;

			if (showBlack)
			{
				// the body of the arrow
				glVertex2f(startX, startY);
				glVertex2f(endX, endY); // cos for x, sin for y

				// the arrow head
				if (showArrowHead)
					drawArrowHead(angles[i], baseLineScale, endX, endY);

				// the flat head
				else if (showFlatHead)
					drawFlatHead(angles[i], baseLineScale, endX, endY);
			}

			// make the next line start at the end of the one before it
			startX = endX;	
			startY = endY;
		}
	}
}

void drawFlatHead(double angle, double scale, double endX, double endY)
{
	glVertex2f(endX + cos((angle + 0.25) * TWO_PI) * scale, endY + sin((angle + 0.25) * TWO_PI) * scale);
	glVertex2f(endX + cos((angle - 0.25) * TWO_PI) * scale, endY + sin((angle - 0.25) * TWO_PI) * scale);
}

void drawVerticalLines()
{
	for (int i = 0; i < 5; i++)
	{
		glColor3f(0.7f, 0.7f, 0.7f);
		glVertex2i(screenWidth * (0.2 * i), 0);
		glVertex2i(screenWidth * (0.2 * i), screenHeight);
	}
}

void figure31()
{
	for (int i = 0; i < flowerVec.size(); i++)
	{
		for (int j = 0; j < numOfDimensions; j++)
		{
			double max = getMax(j);
			Iris currIris = flowerVec[i];

			double startX = xAxis[j] * screenWidth;
			double startY = yAxis[j] * screenHeight;

			double cosAngle = cos(angles[j] * TWO_PI);
			double sinAngle = sin(angles[j] * TWO_PI);

			double currData = currIris.getData(j);
			double length = (screenWidth * 0.2 * zoom * currData) / max; // determines the size of the line

			double endX = startX + cosAngle * length;
			double endY = startY + sinAngle * length;

			switch(currIris.flowerType)
			{
			case Iris::SETOSA:
				if (showRed)
				{
					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	// set line color to red
					
					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}

				break;

			case Iris::VERSICOLOR:
				if (showGreen)
				{
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);	//set line color to green
					
					// the body of the arrow
					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}

				break;

			case Iris::VIRGINICA:
				if (showBlue)
				{
					glColor4f(0.0f, 0.0f, 1.0f, 1.0f);	//set line color to blue

					// body of the arrow
					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}

				break;
			}	//switch
		}	// for j
	}	// for i
}

void figure33()
{
	for (int i = 0; i < flowerVec.size(); i++)
	{
		Iris currIris = flowerVec[i];
		
		double startX = xAxis[0] * screenWidth;
		double startY = yAxis[0] * screenHeight;		
		
		for (int j = 0; j < numOfDimensions; j++)
		{
			double max = getMax(j);

			double currData = currIris.getData(j);
			double length = screenWidth * 0.2 * zoom * currData / max; // determines the size of the line
			
			double cosAngle = cos(angles[j] * TWO_PI);
			double sinAngle = sin(angles[j] * TWO_PI);

			double endX = startX + cosAngle * length;
			double endY = startY + sinAngle * length;

			switch(currIris.flowerType)
			{
			case Iris::SETOSA:
				if (showRed == true)
				{
					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	//set line color to red

					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}
				break;

			case Iris::VERSICOLOR:
				if (showGreen)
				{
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);	//set line color to green
				
					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}
				break;

			case Iris::VIRGINICA:
				if (showBlue)
				{
					glColor4f(0.0f, 0.0f, 1.0f, 1.0f);	//set line color to blue
				
					// the body of the line
					glVertex2d(startX, startY);
					glVertex2d(endX, endY);

					// the arrow head
					if (showArrowHead)
						drawArrowHead(angles[j], HEAD_SCALE, endX, endY);
					
					// the flat head
					else if (showFlatHead)
						drawFlatHead(angles[j], HEAD_SCALE, endX, endY);
				}
				break;

			}	// switch case

			// make the next line start at the end of the one before it
			startX = endX;	
			startY = endY;

		}	// for j
	}	// for i
}

void irisFileOpen()
{
	// opens a file chooser dialog to selected a data file to load data from
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select the iris data file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE)
		readFileData(filename);

	else
		std::cout << "test";
}

void readFileData(char* fileName)
{
	ifstream file(fileName);
	string str;
	
	bool firstTimeThrough = true;
	int dimCounter = -1;
	
	// ignore first line(headers)
	if (getline(file, str));

	stringstream ss;
	// read data
	while (getline(file, str))
	{
		int tokenStart = 0;
		int tokenEnd = 0;
		// ignore empty lines
		if (str.size() == 0);
		else
		{
			vector<double> tempFlower;

			while (str.size() > 0)
			{
				tokenStart = 0;
				tokenEnd = str.find_first_of(',');
				

				if (tokenEnd < 0)
					tokenEnd = str.size();

				int tokenLength = tokenEnd - tokenStart;
				string currentToken = str.substr(tokenStart, tokenLength);

				double newData;

				// insert the token into newData and clear the StringStream
				ss << currentToken;
				ss >> newData;
				ss.clear();

				tempFlower.push_back(newData);

				// remove token from string
				if (tokenEnd == str.size())	// last token left
				{
					str = "";
				} else						// more than one token left
				{
					str = str.substr(tokenEnd + 1, str.size() - tokenLength + 1);
				}
				
				if (firstTimeThrough)
					dimCounter++;
			} // while
			
			if (firstTimeThrough)
			{
				numOfDimensions = dimCounter;
				firstTimeThrough = false;
			}
			
			Iris iris(tempFlower);
			flowerVec.push_back(iris);	

		} // else
	}

	ss.clear();
	file.close();
}

// int functions
int round(double d) // rounds a double to the nearest int
{
	return static_cast<int>(d + 0.5);
}

int startingWidth(int i)
{
		int section = screenWidth / 5;
		int half = section / 2;
		return half + (section * (i));
}

// double functions
double getMax(int j)
{
	double max = 0;
	for(int i = 0; i < flowerVec.size(); i++)
	{
		Iris tempIris = flowerVec[i];

		if (tempIris.getData(j) > max)
			max = tempIris.getData(j);
	}

	return max;
}
