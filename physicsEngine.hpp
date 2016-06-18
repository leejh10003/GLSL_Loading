#pragma once
typedef struct v3f
{
	float x;
	float y;
	float z;

}v3f;

typedef struct v2f
{
	float x;
	float y;

}v2f;

typedef struct color
{
	float r;
	float g;
	float b;

}COLOR;

typedef struct subject
{
	v3f center;
	float radius;
	v3f velocity;
	v3f acc;


}sub;
typedef struct face
{
	v3f vertex[4];
	v3f normal;
	COLOR color;

}Face;

typedef struct object
{
	Face Faces[100];
	int stack;
}Object;



sub initsphere();
int collision(sub * sphere, Object * object, int size);
void DrawSphere(sub * sphere);
void physicsOnIdleCallback(int objectArrayElementNum);
void physicsOnRenderCallback();
void physicsOnKeyDownCallback(int objectArrayElementNum);
void SetFaceNormals(int objectArrayElementNum);
extern object* objects;
extern sub sphere;
extern float angle_G;