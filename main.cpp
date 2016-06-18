//=============================================================================
// Sample Application for GLEW, and cwc Application/Window class using freeglut
//=============================================================================
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include <list>
#include <vector>
#include "glsl.h"
#include <bitset>
#include "physicsEngine.hpp"

//GLfloat angle = 1.0f;
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
	Vertex originalVertices[8];
	GLfloat r;
	GLfloat g;
	GLfloat b;
	bool rendering[6] = { false, false, false, false, false, false};
public:
	Vertex translatedVertices[8];
	void draw()
	{
		glBegin(GL_QUADS);
		if (rendering[0]) {
			glNormal3f(0.0f, 0.0f, 1.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[7].x, translatedVertices[7].y, translatedVertices[7].z);
			glVertex3f(translatedVertices[5].x, translatedVertices[5].y, translatedVertices[5].z);
			glVertex3f(translatedVertices[1].x, translatedVertices[1].y, translatedVertices[1].z);
			glVertex3f(translatedVertices[3].x, translatedVertices[3].y, translatedVertices[3].z);
		}

		if (rendering[1]) {
			glNormal3f(0.0f, 0.0f, -1.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[6].x, translatedVertices[6].y, translatedVertices[6].z);
			glVertex3f(translatedVertices[4].x, translatedVertices[4].y, translatedVertices[4].z);
			glVertex3f(translatedVertices[0].x, translatedVertices[0].y, translatedVertices[0].z);
			glVertex3f(translatedVertices[2].x, translatedVertices[2].y, translatedVertices[2].z);
		}

		if (rendering[2]) {
			glNormal3f(0.0f, 1.0f, 0.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[7].x, translatedVertices[7].y, translatedVertices[7].z);
			glVertex3f(translatedVertices[6].x, translatedVertices[6].y, translatedVertices[6].z);
			glVertex3f(translatedVertices[2].x, translatedVertices[2].y, translatedVertices[2].z);
			glVertex3f(translatedVertices[3].x, translatedVertices[3].y, translatedVertices[3].z);
		}

		if (rendering[3]) {
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[3].x, translatedVertices[3].y, translatedVertices[3].z);
			glVertex3f(translatedVertices[2].x, translatedVertices[2].y, translatedVertices[2].z);
			glVertex3f(translatedVertices[0].x, translatedVertices[0].y, translatedVertices[0].z);
			glVertex3f(translatedVertices[1].x, translatedVertices[1].y, translatedVertices[1].z);
		}

		if (rendering[4]) {
			glNormal3f(1.0f, 0.0f, 0.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[7].x, translatedVertices[7].y, translatedVertices[7].z);
			glVertex3f(translatedVertices[6].x, translatedVertices[6].y, translatedVertices[6].z);
			glVertex3f(translatedVertices[4].x, translatedVertices[4].y, translatedVertices[4].z);
			glVertex3f(translatedVertices[5].x, translatedVertices[5].y, translatedVertices[5].z);
		}

		if (rendering[5]) {
			glNormal3f(0.0f, -1.0f, 0.0f);
			glColor4f(r, g, b, 1.0f);
			glVertex3f(translatedVertices[5].x, translatedVertices[5].y, translatedVertices[5].z);
			glVertex3f(translatedVertices[4].x, translatedVertices[4].y, translatedVertices[4].z);
			glVertex3f(translatedVertices[0].x, translatedVertices[0].y, translatedVertices[0].z);
			glVertex3f(translatedVertices[1].x, translatedVertices[1].y, translatedVertices[1].z);
		}
		glEnd();
	}
	Cube(int x, int y, int z, GLfloat r, GLfloat g, GLfloat b, bool renderOrNot[6]) : r(r), g(g), b(b)
	{
		GLfloat xLower = x * 0.2f - 0.1f;
		GLfloat xUpper = x * 0.2f + 0.1f;
		GLfloat yLower = y * 0.2f - 0.1f;
		GLfloat yUpper = y * 0.2f + 0.1f;
		GLfloat zLower = z * 0.2f - 0.1f;
		GLfloat zUpper = z * 0.2f + 0.1f;

		for (int i = 0; i < 6; i++)
			rendering[i] = renderOrNot[i];
		for (char i = 0; i < 8; i++) {
			originalVertices[i].x = ((bitset<3>(i) & bitset<3>(CoordDirec::x)) != 0 ? xUpper : xLower);
			originalVertices[i].y = ((bitset<3>(i) & bitset<3>(CoordDirec::y)) != 0 ? yUpper : yLower);
			originalVertices[i].z = ((bitset<3>(i) & bitset<3>(CoordDirec::z)) != 0 ? zUpper : zLower);
			translatedVertices[i].x = originalVertices[i].x;
			translatedVertices[i].y = originalVertices[i].y;
			translatedVertices[i].z = originalVertices[i].z;
		}
	}
	void rotate(GLfloat angle)
	{
		for (int i = 0; i < 8; i++) {
			GLfloat xValue = originalVertices[i].x;
			GLfloat zValue = originalVertices[i].z;
			translatedVertices[i].x = xValue * cosf(angle / 180.f * M_PI) - zValue * sinf(angle / 180.f * M_PI);
			translatedVertices[i].z = zValue * cosf(angle / 180.f * M_PI) + xValue * sinf(angle / 180.f * M_PI);
		}
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
	GLfloat r;
	GLfloat g;
	GLfloat b;
	vector<vector<vector<Cube>>> cubes;
public:
	void drawPlatform()
	{
		for (int i = xStart; i < xEnd; i++)
			for (int j = yStart; j < yEnd; j++)
				for (int k = zStart; k < zEnd; k++) {
					cubes[i - xStart][j - yStart][k - zStart].draw();
				}
	}
	void rotate(GLfloat angle)
	{
		for (int i = xStart; i < xEnd; i++)
			for (int j = yStart; j < yEnd; j++)
				for (int k = zStart; k < zEnd; k++) {
					cubes[i - xStart][j - yStart][k - zStart].rotate(angle);
				}
	}
	Platform(GLint xStart, GLint xEnd, GLint yStart, GLint yEnd, GLint zStart, GLint zEnd, GLfloat r, GLfloat g, GLfloat b, bool renderOrNot[6]) : r(r), g(g), b(b), xStart(xStart), xEnd(xEnd + 1), yStart(yStart), yEnd(yEnd + 1), zStart(zStart), zEnd(zEnd + 1) {
		for (int i = 0; i < xEnd - xStart + 1; i++) {
			cubes.push_back(vector<vector<Cube>>());
			for (int j = 0; j < yEnd - yStart + 1; j++) {
				cubes[i].push_back(vector<Cube>());
				for (int k = 0; k < zEnd - zStart + 1; k++) {
					cubes[i][j].push_back(Cube(xStart + i, yStart + j, zStart + k, r, g, b, renderOrNot));
				}
			}
		}
	}
	vector<Cube*> getCubes()
	{
		vector<Cube*> forReturn;
		for (int i = xStart; i < xEnd; i++)
			for (int j = yStart; j < yEnd; j++)
				for (int k = zStart; k < zEnd; k++) 
					forReturn.push_back(&cubes[i - xStart][j - yStart][k - zStart]);
		return forReturn;
	}
};
class Body
{
protected:
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat angle = 0.0f;
public:
	GLfloat getAngle() { return angle; };
	vector<Platform> platforms;
	Body& addPlatform(vector<int> addPlatform, bool renderOrNot[6])
	{
		platforms.push_back(Platform(addPlatform[0],
			addPlatform[1],
			addPlatform[2],
			addPlatform[3],
			addPlatform[4],
			addPlatform[5], r, g, b, renderOrNot));
		return *this;
	}
	void drawPlatforms() {
		for (int i = 0; i < (int)platforms.size(); ++i)
			platforms[i].drawPlatform();
	}
	Body(GLfloat r, GLfloat g, GLfloat b) : r(r), g(g), b(b) {}
	void rotate(GLfloat angleDisplacement) {
		angle += angleDisplacement;
		for (int i = 0; i < (int)platforms.size(); ++i)
			platforms[i].rotate(angle);
	}
	int autoRotate()
	{
		int toReturn = 0;
		if ((angle > 0.0f && angle <= 45.0f) || (angle > 90.0f && angle <= 135.0f) || (angle > 180.0f && angle <= 225.0f) || (angle > 270.0f && angle <= 315.0f)) {
			angle -= 1.0f;
			toReturn = -1;
		}
		else if ((angle > 45.0f && angle < 90.0f) || (angle > 135.0f && angle < 180.0f) || (angle > 225.0f && angle < 270.0f) || (angle > 315.0f && angle < 360.0f)) {
			angle += 1.0f;
			toReturn = 1;
		}
		for (int i = 0; i < (int)platforms.size(); ++i)
			platforms[i].rotate(angle);
		return toReturn;
	}
	void angleStabilaze()
	{
		for (;angle < 0.0f || angle >= 360.0f;)
			if (angle >= 360.0f)
				angle -= 360.0f;
			else if (angle < 0.0f)
				angle += 360.0f;
	}
	vector<Cube*> getCubes()
	{
		vector<Cube*> forReturn;
		for (int i = 0; i < (int)platforms.size(); ++i) {
			vector<Cube*>temp = platforms[i].getCubes();
			forReturn.insert(forReturn.end(), temp.begin(), temp.end());
		}
		return forReturn;
	}
};
class myWindow : public cwc::glutWindow
{
protected:
	cwc::glShaderManager SM;
	cwc::glShader *shader;
	vector<Body> bodies;
	bool pressed = 0;
	vector<object> cubesToObject(vector<Cube*>& input) {
		vector<object> toReturn;
		for (vector<Cube*>::iterator iter = input.begin(); iter != input.end(); iter++) {
			Vertex* ref = ((*iter)->translatedVertices);
			toReturn.emplace_back(object{ { Face{ { v3f{ ref[7].x , ref[7].y, ref[7].z}, 
													v3f{ ref[5].x , ref[5].y, ref[5].z },
													v3f{ ref[1].x , ref[1].y, ref[1].z },
													v3f{ ref[3].x , ref[3].y, ref[3].z } } },
											Face{ { v3f{ ref[6].x , ref[6].y, ref[6].z },
													v3f{ ref[4].x , ref[4].y, ref[4].z },
													v3f{ ref[0].x , ref[0].y, ref[0].z },
													v3f{ ref[2].x , ref[2].y, ref[2].z } } },
											Face{ { v3f{ ref[7].x , ref[7].y, ref[7].z },
													v3f{ ref[6].x , ref[6].y, ref[6].z },
													v3f{ ref[2].x , ref[2].y, ref[2].z },
													v3f{ ref[3].x , ref[3].y, ref[3].z } } },
											Face{ { v3f{ ref[3].x , ref[3].y, ref[3].z },
													v3f{ ref[2].x , ref[2].y, ref[2].z },
													v3f{ ref[0].x , ref[0].y, ref[0].z },
													v3f{ ref[1].x , ref[1].y, ref[1].z } } },
											Face{ { v3f{ ref[7].x , ref[7].y, ref[7].z },
													v3f{ ref[6].x , ref[6].y, ref[6].z },
													v3f{ ref[4].x , ref[4].y, ref[4].z },
													v3f{ ref[5].x , ref[5].y, ref[5].z } } },
											Face{ { v3f{ ref[5].x , ref[5].y, ref[5].z },
													v3f{ ref[4].x , ref[4].y, ref[4].z },
													v3f{ ref[0].x , ref[0].y, ref[0].z },
													v3f{ ref[1].x , ref[1].y, ref[1].z } } }
													},6 });
			
		}
		return toReturn;
	}
public:
	myWindow() {}
	virtual void OnRender(void)
	{
		vector<Cube*> allBodyCubes[4] = { bodies[0].getCubes(), bodies[1].getCubes(), bodies[2].getCubes(), bodies[3].getCubes() };
		vector<Cube*> allObject;
		allObject.insert(allObject.end(), allBodyCubes[0].begin(), allBodyCubes[0].end());
		allObject.insert(allObject.end(), allBodyCubes[1].begin(), allBodyCubes[1].end());
		allObject.insert(allObject.end(), allBodyCubes[2].begin(), allBodyCubes[2].end());
		allObject.insert(allObject.end(), allBodyCubes[3].begin(), allBodyCubes[3].end());
		vector<object> convertedObjects = cubesToObject(allObject);
		objects = &convertedObjects[0];
		SetFaceNormals(convertedObjects.size());
		physicsOnRenderCallback();


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
		glRotatef(45.0f, .0f, -1.0f, 0.0f);
		glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		DrawSphere(&sphere);
		if (shader) shader->begin();
		//draw rotating platforms
		bodies[0].drawPlatforms();
		//draw blue static platforms
		bodies[1].drawPlatforms();
		bodies[2].drawPlatforms();
		//draw red static platforms - where magic goes on!
		bodies[3].drawPlatforms();
		if (shader) shader->end();

		angle_G = bodies[0].getAngle();
		glutSwapBuffers();

		Repaint();
	}

	virtual void OnIdle() {
		vector<Cube*> allBodyCubes[4] = { bodies[0].getCubes(), bodies[1].getCubes(), bodies[2].getCubes(), bodies[3].getCubes() };
		vector<Cube*> allObject;
		allObject.insert(allObject.end(), allBodyCubes[0].begin(), allBodyCubes[0].end());
		allObject.insert(allObject.end(), allBodyCubes[1].begin(), allBodyCubes[1].end());
		allObject.insert(allObject.end(), allBodyCubes[2].begin(), allBodyCubes[2].end());
		allObject.insert(allObject.end(), allBodyCubes[3].begin(), allBodyCubes[3].end());
		vector<object> convertedObjects = cubesToObject(allObject);
		objects = &convertedObjects[0];
		if (pressed == false) {
			int ifRotated = bodies[0].autoRotate();
			float xtmp = sphere.center.x;
			float ztmp = sphere.center.z;
			sphere.center.x = xtmp * cosf(ifRotated / 180.f * M_PI) - ztmp * sinf(ifRotated / 180.f * M_PI);
			sphere.center.z = ztmp * cosf(ifRotated / 180.f * M_PI) + xtmp * sinf(ifRotated / 180.f * M_PI);

		}
		SetFaceNormals(convertedObjects.size());
		physicsOnIdleCallback(convertedObjects.size());
		angle_G = bodies[0].getAngle();
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
		GLfloat ambientColor0[] = { 0.6f, 0.6f, 0.6f, 0.1f };
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

		bodies.push_back(Body(0.5f, 0.5f, 1.0f));
		bodies.push_back(Body(0.5f, 0.5f, 1.0f));
		bodies.push_back(Body(0.5f, 0.5f, 1.0f));
		bodies.push_back(Body(1.0f, 0.5f, 0.5f));
		bool render[6] = { true, true, true, true, true, true };
		bool notRender[6] = { false, false, false, false, false, false };
		bool exceptUpperRight[6] = { true, true, false, true, false, true};
		bool upperRight[6] = { false, false, true, false, true, false };
		bodies[0].addPlatform({ 0, 0, -4, 1, 0, 0 }, render)
			.addPlatform({ 0, 3, 2, 2, 0, 0 }, render)
			.addPlatform({ 0, 0, -5, -5, -3, 0 }, render);
		bodies[1].addPlatform({ 0, 0, -5, -5, -7, -4 }, render);
		bodies[2].addPlatform({ 0, 0, 2, 2, -7, -4 }, render);
		bodies[3].addPlatform({ 0, 0, 2, 2, 4, 5 }, render)
			.addPlatform({ 4, 4, -5, -5, 0, 0 }, exceptUpperRight)
			.addPlatform({ -1, -1, 2, 2, 5, 5 }, upperRight);
		sphere = initsphere();
		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
		if (shader == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		//gettingVertices example
	}

	virtual void OnResize(int w, int h) {}
	virtual void OnClose(void) {}
	virtual void OnMouseDown(int button, int x, int y) {}
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y) {}

	virtual void OnKeyDown(int nKey, char cAscii)
	{
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		}
		else if (cAscii == 'A' || cAscii == 'a')
		{
			bodies[0].rotate(-5.0f);

			float xtmp = sphere.center.x;
			float ztmp = sphere.center.z;
			sphere.center.x = xtmp * cosf(-5.0f / 180.f * M_PI) - ztmp * sinf(-5.0f / 180.f * M_PI);
			sphere.center.z = ztmp * cosf(-5.0f / 180.f * M_PI) + xtmp * sinf(-5.0f / 180.f * M_PI);

			pressed = true;
		}
		else if (cAscii == 'D' || cAscii == 'd')
		{
			bodies[0].rotate(5.0f);

			float xtmp = sphere.center.x;
			float ztmp = sphere.center.z;
			sphere.center.x = xtmp * cosf(5.0f / 180.f * M_PI) - ztmp * sinf(5.0f / 180.f * M_PI);
			sphere.center.z = ztmp * cosf(5.0f / 180.f * M_PI) + xtmp * sinf(5.0f / 180.f * M_PI);
			pressed = true;
		}
		else if (cAscii == '1')
		{
			sphere.velocity.z -= 0.003f;
		}
		else if (cAscii == '3')
		{
			sphere.velocity.z += 0.003f;
		}
		else if (cAscii == '7')
		{
			sphere.velocity.x += 0.003f;
		}
		else if (cAscii == '9')
		{
			sphere.velocity.x -= 0.003f;
		}
		else if (cAscii == '0')
		{
			cout << sphere.center.x << sphere.center.z;
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
			bodies[0].angleStabilaze();
			pressed = false;
		}
	};
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() { std::cout << "Hello World!\n"; }
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

