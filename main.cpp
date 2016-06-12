//=============================================================================
// Sample Application for GLEW, and cwc Application/Window class using freeglut
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include <list>
#include <vector>
#include "glsl.h"
#include <bitset>

using namespace std;
//-----------------------------------------------------------------------------
typedef enum CoordinateDirection {
	x = 0b100,
	y = 0b10,
	z = 0b1
}CoordDirec;
typedef enum BiggerSmaller {
	smaller = 0,
	bigger = 1
}BigSmall;
struct Vertex
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
class Cube
{
protected:
	Vertex vertices[8];
public:
	void draw()
	{
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
		glVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);

		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(vertices[7].x, vertices[7].y, vertices[7].z);
		glVertex3f(vertices[6].x, vertices[6].y, vertices[6].z);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);

		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(vertices[5].x, vertices[5].y, vertices[5].z);
		glVertex3f(vertices[4].x, vertices[4].y, vertices[4].z);
		glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
		glVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
		glEnd();
	}
	Cube(int x, int y, int z)
	{
		GLfloat xLower = x * 0.2f - 0.1f;
		GLfloat xUpper = x * 0.2f + 0.1f;
		GLfloat yLower = y * 0.2f - 0.1f;
		GLfloat yUpper = y * 0.2f + 0.1f;
		GLfloat zLower = z * 0.2f - 0.1f;
		GLfloat zUpper = z * 0.2f + 0.1f;
		for (char i = 0; i < 8; i++) {
			vertices[i].x = ((bitset<3>(i) & bitset<3>(CoordDirec::x)) != 0 ? xUpper : xLower);
			vertices[i].y = ((bitset<3>(i) & bitset<3>(CoordDirec::y)) != 0 ? yUpper : yLower);
			vertices[i].z = ((bitset<3>(i) & bitset<3>(CoordDirec::z)) != 0 ? zUpper : zLower);
		}
	}
	void printVertices()
	{
		for (int i = 0; i < 8; i++)
			cout << "(" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << endl;
	}
};
class Platform
{
protected:
	GLint xStart;
	GLint yStart;
	GLint zStart;
	GLint xEnd;
	GLint yEnd;
	GLint zEnd;
	vector<vector<vector<Cube>>> cubes;
public:
	void drawPlatform()
	{
		for(int i = xStart; i < xEnd ; i++)
			for(int j = yStart; j < yEnd ; j++)
				for (int k = zStart; k < zEnd ; k++){
					cubes[i - xStart][j - yStart][k - zStart].draw();
				}
	}
	Platform(GLint xStart, GLint xEnd, GLint yStart, GLint yEnd, GLint zStart, GLint zEnd) : xStart(xStart), xEnd(xEnd + 1), yStart(yStart), yEnd(yEnd + 1), zStart(zStart), zEnd(zEnd + 1) {
		for (int i = 0; i < xEnd - xStart + 1; i++) {
			cubes.push_back(vector<vector<Cube>>());
			for (int j = 0; j < yEnd - yStart + 1; j++) {
				cubes[i].push_back(vector<Cube>());
				for (int k = 0; k < zEnd - zStart + 1; k++) {
					cubes[i][j].push_back(Cube(xStart + i, yStart + j, zStart + k));
				}
			}
		}

	}
};
class Body
{
public:
	vector<Platform> platforms;
	Body& addPlatform(vector<int> addPlatform)
	{
		platforms.push_back(Platform(addPlatform[0],
									 addPlatform[1],
									 addPlatform[2],
									 addPlatform[3],
									 addPlatform[4],
									 addPlatform[5]));
		return *this;
	}
	void drawPlatforms(){
		for (int i = 0; i < (int)platforms.size(); ++i)
			platforms[i].drawPlatform();
	}
};
class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   vector<Body> bodies;
   GLfloat angle = 0.0f;
   bool pressed = 0;
public:
	myWindow(){}
	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
		glRotatef(45.0f, .0f, -1.0f, 0.0f);
		glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if(shader) shader->begin();
		//draw rotating platforms
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
		bodies[0].drawPlatforms();
		glPopMatrix();
		//draw blue static platforms
		bodies[1].drawPlatforms();
		bodies[2].drawPlatforms();
		//draw red static platforms - where magic goes on!
		glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
		for (int i = 4; i < 6; i++) {
			drawCube(0.0f, 0.4f, i * 0.2f);
		}
		{
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);

			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);
			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);
			/*glNormal3f(0.0f, 0.0f, -1.0f);

			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);
			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);*/
			/*glNormal3f(0.0f, 1.0f, 0.0f);

			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);
			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);*/
			glNormal3f(-1.0f, 0.0f, 0.0f);

			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);
			glVertex3f(-0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glNormal3f(1.0f, 0.0f, 0.0f);

			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, 0.1f);
			glVertex3f(0.1f + 0.2f * 4, 0.1f - 1.0f, -0.1f);
			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glNormal3f(0.0f, -1.0f, 0.0f);

			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glVertex3f(0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, -0.1f);
			glVertex3f(-0.1f + 0.2f * 4, -0.1f - 1.0f, 0.1f);
			glEnd();
		}
		{
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			  
			/*glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);*/
			/*glNormal3f(0.0f, 0.0f, -1.0f);
			  
			glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);*/
			glNormal3f(0.0f, 1.0f, 0.0f);
			  
			glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);
			/*glNormal3f(-1.0f, 0.0f, 0.0f);
			  
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);*/
			glNormal3f(1.0f, 0.0f, 0.0f);
			  
			glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, 0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);
			/*glNormal3f(0.0f, -1.0f, 0.0f);
			  
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);
			glVertex3f(0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, -0.1f + 5 * 0.2f);
			glVertex3f(-0.1f - 0.2f, -0.1f + 0.4f, 0.1f + 5 * 0.2f);*/
			glEnd();
		}
		if (shader) shader->end();

      glutSwapBuffers();

		Repaint();
	}

	virtual void OnIdle() {
		if (pressed == false) {
			if ((angle >= 0.0f && angle < 45.0f) || (angle >= 90.0f && angle < 135.0f) || (angle >= 180.0f && angle < 225.0f) || (angle >= 270.0f && angle < 315.0f))
				angle -= 1.0f;
			else if ((angle >= 45.0f && angle < 90.0f) || (angle >= 135.0f && angle < 180.0f) || (angle >= 225.0f && angle < 270.0f) || (angle >= 315.0f && angle < 360.0f))
				angle += 1.0f;
			if (angle - 90.0f <= 1.0f && angle - 90.0f >= -1.0f) angle = 90.0f;
			else if (angle - 180.0f <= 1.0f && angle - 180.0f >= -1.0f) angle = 180.0f;
			else if (angle - 270.0f <= 1.0f && angle - 270.0f >= -1.0f) angle = 270.0f;
			else if (angle <= 1.0f || angle - 360.0f >= -1.0f)  angle = 0.0f;
		}
	}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
		glEnable(GL_LIGHTING);
		GLfloat lightColor0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //Color (0.5, 0.5, 0.5)
		GLfloat lightPos0[] = { 0.0f, 1.6f, 0.0f , 1.0f }; //Positioned at (4, 0, 8)
		GLfloat ambientColor0[] = {0.6f, 0.6f, 0.6f, 0.1f};
		GLfloat materialAmbient[] = { .1f, .1f, 0.1f, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
		glEnable(GL_LIGHT0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		bodies.push_back(Body());
		bodies.push_back(Body());
		bodies.push_back(Body());
		bodies.push_back(Body());
		bodies[0].addPlatform({ 0, 0, -4, 1, 0, 0 })
				 .addPlatform({ 0, 3, 2, 2, 0, 0 })
				 .addPlatform({ 0, 0, -5, -5, -3, 0 });
		bodies[1].addPlatform({ 0, 0, -5, -5, -7, -4 });
		bodies[2].addPlatform({ 0, 0, 2, 2, -7, -4 });
		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		  if (shader==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";

	}

	virtual void OnResize(int w, int h) {}
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		}
		else if (cAscii == 'A' || cAscii == 'a')
		{
			angle -= 5.0f;
			pressed = true;
		}
		else if (cAscii == 'D' || cAscii == 'd')
		{
			angle += 5.0f;
			pressed = true;
		}
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
		if (cAscii == 'f')
		{
			SetFullscreen(true);
		}
		else if (cAscii == 'w')
		{
			SetFullscreen(false);
		}
		else if (cAscii == 'A' || cAscii == 'a' || cAscii == 'D' || cAscii == 'd')
		{
			for (;angle < 0.0f || angle >= 360.0f;)
				if (angle >= 360.0f)
					angle -= 360.0f;
				else if (angle < 0.0f) 
					angle += 360.0f;
			pressed = false;
		}
	};
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

