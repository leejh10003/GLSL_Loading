#include <Windows.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tchar.h>
#include "physicsEngine.hpp"

#define _USE_MATH_DEFINES

#include <math.h>

#define MAX 36

object* objects;
sub sphere;
float angle_G;

sub initsphere()
{
	sub newsub;

	newsub.center.x = 0.0f;
	newsub.center.y = -0.8f;
	newsub.center.z = -1.4f;

	newsub.radius = 0.1f;

	newsub.velocity.x = 0;
	newsub.velocity.y = 0;
	newsub.velocity.z = 0;

	newsub.acc.x = 0;
	newsub.acc.y = 0;
	newsub.acc.z = 0;

	return newsub;

}

static v3f getVector(v3f v1, v3f v2)
{
	v3f nv;

	float len;


	nv.x = v2.x - v1.x;
	nv.y = v2.y - v1.y;
	nv.z = v2.z - v1.z;

	len = sqrt(pow(nv.x, 2) + pow(nv.y, 2) + pow(nv.z, 2));

	nv.x /= len;
	nv.y /= len;
	nv.z /= len;

	return nv;
}

static v3f getVectorA(v3f v1, v3f v2)
{
	v3f nv;

	nv.x = v2.x - v1.x;
	nv.y = v2.y - v1.y;
	nv.z = v2.z - v1.z;

	return nv;
}

static v3f CrossProduct(v3f v1, v3f v2)
{
	v3f nv;
	float len;

	nv.x = v1.y*v2.z - v1.z*v2.y;
	nv.y = v1.z*v2.x
		- v1.x*v2.z;
	nv.z = v1.x*v2.y - v1.y*v2.x;

	len = sqrt(pow(nv.x, 2) + pow(nv.y, 2) + pow(nv.z, 2));

	nv.x /= len;
	nv.y /= len;
	nv.z /= len;

	return nv;
}

static v3f setVertex(float x, float y, float z)
{
	v3f vertex;

	vertex.x = x;
	vertex.y = y;
	vertex.z = z;

	return vertex;

}

static v3f CrossProductA(v3f v1, v3f v2)
{
	v3f nv;

	nv.x = v1.y*v2.z - v1.z*v2.y;
	nv.y = v1.z*v2.x - v1.x*v2.z;
	nv.z = v1.x*v2.y - v1.y*v2.x;

	return nv;
}


static Face initface(v3f v1, v3f v2, v3f v3, v3f v4, COLOR rgb)
{
	Face newface;

	newface.vertex[0] = v1;
	newface.vertex[1] = v2;
	newface.vertex[2] = v3;
	newface.vertex[3] = v4;
	newface.color = rgb;
	newface.normal = CrossProduct(getVector(v1, v2), getVector(v1, v3));


	return newface;
}



static Object initobject(Face * faces, int size)
{
	Object newobj;
	newobj.stack = size - 1;

	for (int i = 0; i < size; i++)
		newobj.Faces[i] = faces[i];


	return newobj;
}


static float DotProduct(v3f v1, v3f v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


static float sizeofvector(v3f vec)
{
	float len = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));

	return len;
}


int collision(sub * sphere, Object * object, int size)
{
	float length = 0.2;
	float distance;
	float delta;
	float delta_line;
	float len1, len2;
	float len3, len4;
	float dist_line;
	float proj;
	float s, t;
	float a, b, c;
	float a1, b1, c1;
	float l, m, n;
	float p, q, r;
	float p1, q1, r1;
	float x, y, z;
	float x1, y1, z1;
	float w1, w2;
	float constnumber;
	float w11, w22;
	float ans1, ans2, ans;
	v3f normal2line;
	v3f v_proj;
	v3f v_proj_line;
	v3f v_tmp;
	v3f normal_tmp;
	v3f v_line;
	v3f v_fix;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j <= object[i].stack; j++)
		{
			distance = abs(
				object[i].Faces[j].normal.x*(sphere->center.x - object[i].Faces[j].vertex[0].x) +
				object[i].Faces[j].normal.y*(sphere->center.y - object[i].Faces[j].vertex[0].y) +
				object[i].Faces[j].normal.z*(sphere->center.z - object[i].Faces[j].vertex[0].z)
			)
				/
				sqrt(
					pow(object[i].Faces[j].normal.x, 2) +
					pow(object[i].Faces[j].normal.y, 2) +
					pow(object[i].Faces[j].normal.z, 2)
				);

			
			constnumber = object[i].Faces[j].normal.x*object[i].Faces[j].vertex[0].x + object[i].Faces[j].normal.y*object[i].Faces[j].vertex[0].y +
				object[i].Faces[j].normal.z*object[i].Faces[j].vertex[0].z;

			constnumber *= -(1);

			if (DotProduct(getVector(sphere->center, object[i].Faces[j].vertex[0]), object[i].Faces[j].normal) < 0)
			{
				normal_tmp.x = (-1)*object[i].Faces[j].normal.x;
				normal_tmp.y = (-1)*object[i].Faces[j].normal.y;
				normal_tmp.z = (-1)*object[i].Faces[j].normal.z;


			}
			else
			{

				normal_tmp.x = object[i].Faces[j].normal.x;
				normal_tmp.y = object[i].Faces[j].normal.y;
				normal_tmp.z = object[i].Faces[j].normal.z;
			}

			if (distance <= sphere->radius)
			{
				delta = sphere->radius - distance;

				v_proj.x = sphere->center.x + distance*normal_tmp.x;
				v_proj.y = sphere->center.y + distance*normal_tmp.y;
				v_proj.z = sphere->center.z + distance*normal_tmp.z;

				proj = DotProduct(getVectorA(object[i].Faces[j].vertex[0], v_proj),
					getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1])) /

					pow(sizeofvector(getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1])), 2);

				normal2line.x = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1]).x*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).x;
				normal2line.y = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1]).y*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).y;
				normal2line.z = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1]).z*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).z;

				len1 = sizeofvector(normal2line);

				proj = DotProduct(getVectorA(object[i].Faces[j].vertex[2], v_proj),
					getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[3])) /

					pow(sizeofvector(getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[3])), 2);

				normal2line.x = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[3]).x*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).x;
				normal2line.y = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[3]).y*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).y;
				normal2line.z = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[3]).z*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).z;

				len2 = sizeofvector(normal2line);


				proj = DotProduct(getVectorA(object[i].Faces[j].vertex[0], v_proj),
					getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[3])) /

					pow(sizeofvector(getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[1])), 2);

				normal2line.x = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[3]).x*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).x;
				normal2line.y = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[3]).y*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).y;
				normal2line.z = getVectorA(object[i].Faces[j].vertex[0], object[i].Faces[j].vertex[3]).z*proj - getVectorA(object[i].Faces[j].vertex[0], v_proj).z;

				len3 = sizeofvector(normal2line);

				proj = DotProduct(getVectorA(object[i].Faces[j].vertex[2], v_proj),
					getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[1])) /

					pow(sizeofvector(getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[1])), 2);

				normal2line.x = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[1]).x*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).x;
				normal2line.y = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[1]).y*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).y;
				normal2line.z = getVectorA(object[i].Faces[j].vertex[2], object[i].Faces[j].vertex[1]).z*proj - getVectorA(object[i].Faces[j].vertex[2], v_proj).z;

				len4 = sizeofvector(normal2line);

				if (abs(len1 + len2-0.2)<0.001 && abs(len3 + len4 - 0.2)<0.001)
				{

					if (sizeofvector(sphere->velocity) != 0)
					{
						sphere->center.x -= delta*getVector(setVertex(0, 0, 0), sphere->velocity).x;
						sphere->center.y -= delta*getVector(setVertex(0, 0, 0), sphere->velocity).y;
						sphere->center.z -= delta*getVector(setVertex(0, 0, 0), sphere->velocity).z;

						proj = DotProduct(sphere->velocity, normal_tmp) / DotProduct(normal_tmp, normal_tmp);

						v_fix.x = normal_tmp.x*proj;
						v_fix.y = normal_tmp.y*proj;
						v_fix.z = normal_tmp.z*proj;

						sphere->velocity.x -= 2 * v_fix.x;
						sphere->velocity.y -= 2 * v_fix.y;
						sphere->velocity.z -= 2 * v_fix.z;
					}

					proj = DotProduct(sphere->acc, normal_tmp) / DotProduct(normal_tmp, normal_tmp);

					v_fix.x = normal_tmp.x*proj;
					v_fix.y = normal_tmp.y*proj;
					v_fix.z = normal_tmp.z*proj;

					sphere->acc.x -= v_fix.x;
					sphere->acc.y -= v_fix.y;
					sphere->acc.z -= v_fix.z;

					return 1;
				}
				else
				{
					for (int k = 0; k < 4; k++)
					{
						proj = DotProduct(getVectorA(object[i].Faces[j].vertex[k], sphere->center),
							getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4])) /

							pow(sizeofvector(getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[k + 1])), 2);

						normal2line.x = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).x*proj - getVectorA(object[i].Faces[j].vertex[k], sphere->center).x;
						normal2line.y = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).y*proj - getVectorA(object[i].Faces[j].vertex[k], sphere->center).y;
						normal2line.z = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).z*proj - getVectorA(object[i].Faces[j].vertex[k], sphere->center).z;

						dist_line = sizeofvector(normal2line);

						if (dist_line < sphere->radius)
						{
							delta_line = sphere->radius - dist_line;

							v_proj_line.x = sphere->center.x + normal2line.x;
							v_proj_line.y = sphere->center.y + normal2line.y;
							v_proj_line.z = sphere->center.z + normal2line.z;


							if (sizeofvector(getVectorA(object[i].Faces[j].vertex[k], v_proj_line)) +
								sizeofvector(getVectorA(object[i].Faces[j].vertex[(k + 1) % 4], v_proj_line)) == 0.2)
							{
								

								l = getVectorA(object[i].Faces[j].vertex[k], sphere->center).x;
								m = getVectorA(object[i].Faces[j].vertex[k], sphere->center).y;
								n = getVectorA(object[i].Faces[j].vertex[k], sphere->center).z;

								p = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).x;
								q = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).y;
								r = getVectorA(object[i].Faces[j].vertex[k], object[i].Faces[j].vertex[(k + 1) % 4]).z;

								x = sphere->velocity.x;
								y = sphere->velocity.y;
								z = sphere->velocity.z;

								s = (l*p + m*q + n*r) / (p*p + q*q + r*r);
								t = (p*x + q*y + r*z) / (p*p + q*q + r*r);

								c = (p*s - l)*(p*s - l) + (q*s - m)*(q*s - m) + (r*s - n)*(r*s - n) + ((s*t - m)*(r*s - n) + (p*s - l)*(s*r - n) + (p*s - l)*(s*t - m)) - sphere->radius*sphere->radius;
								a = (p*t - x)*(p*t - x) + (q*t - y)*(q*t - y) + (r*t - z)*(r*t - z) + ((p*t - x)*(q*t - y) + (q*t - y)*(r*t - z) + (p*t - x)*(r*t - z));
								b = 2 * ((p*s - l)*(p*t - x) + (q*s - m)*(q*t - y) + (r*s - n)*(r*t - z) + (q*t - y)*(p*s - l) + (s*t - m)*(p*t - x) + (q*t - y)*(s*r - n) + (s*t - m)*(r*t - z) + (p*s - l)*(r*t - z) + (p*t - x)*(s*r - n));

								w1 = (-b + sqrt(b*b - 4 * a*c)) / 2 * a;
								w2 = (-b - sqrt(b*b - 4 * a*c)) / 2 * a;



								if (w1 < 0)
								{
									if (sizeofvector(sphere->velocity) != 0)
									{
										sphere->center.x += w1*sphere->velocity.x;
										sphere->center.y += w1*sphere->velocity.y;
										sphere->center.z += w1*sphere->velocity.z;

										proj = DotProduct(sphere->velocity, normal2line) / DotProduct(normal2line, normal2line);

										v_fix.x = normal2line.x*proj;
										v_fix.y = normal2line.y*proj;
										v_fix.z = normal2line.z*proj;

										sphere->velocity.x -= 1.5 * v_fix.x;
										sphere->velocity.y -= 1.5 * v_fix.y;
										sphere->velocity.z -= 1.5 * v_fix.z;
									}

									if (sizeofvector(sphere->acc) != 0)
									{
										proj = DotProduct(sphere->acc, normal2line) / DotProduct(normal2line, normal2line);

										v_fix.x = normal2line.x*proj;
										v_fix.y = normal2line.y*proj;
										v_fix.z = normal2line.z*proj;

										sphere->acc.x -= v_fix.x;
										sphere->acc.y -= v_fix.y;
										sphere->acc.z -= v_fix.z;
									}
									return 1;
								}
								else if (w2 < 0)
								{
									if (sizeofvector(sphere->velocity) != 0)
									{
										sphere->center.x += w2*sphere->velocity.x;
										sphere->center.y += w2*sphere->velocity.y;
										sphere->center.z += w2*sphere->velocity.z;

										proj = DotProduct(sphere->velocity, normal2line) / DotProduct(normal2line, normal2line);

										v_fix.x = normal2line.x*proj;
										v_fix.y = normal2line.y*proj;
										v_fix.z = normal2line.z*proj;


										sphere->velocity.x -= 1.5 * v_fix.x;
										sphere->velocity.y -= 1.5 * v_fix.y;
										sphere->velocity.z -= 1.5 * v_fix.z;
									}

									if (sizeofvector(sphere->acc) != 0)
									{
										proj = DotProduct(sphere->acc, normal2line) / DotProduct(normal2line, normal2line);

										v_fix.x = normal2line.x*proj;
										v_fix.y = normal2line.y*proj;
										v_fix.z = normal2line.z*proj;

										sphere->acc.x -= v_fix.x;
										sphere->acc.y -= v_fix.y;
										sphere->acc.z -= v_fix.z;
									}
									return 1;

								}
							}
							else
							{
								for (int l = 0; l < 4; l++)
								{
									if (sizeofvector(getVectorA(object[i].Faces[j].vertex[l], sphere->center)) < sphere->radius)
									{
		

										p1 = getVectorA(object[i].Faces[j].vertex[l], sphere->center).x;
										q1 = getVectorA(object[i].Faces[j].vertex[l], sphere->center).y;
										r1 = getVectorA(object[i].Faces[j].vertex[l], sphere->center).z;

										x1 = sphere->velocity.x;
										y1 = sphere->velocity.y;
										z1 = sphere->velocity.z;

										c1 = p1*p1 + q1*q1 + r1*r1 - sphere->radius*sphere->radius;
										b1 = 2 * (p1*x1 + q1*y1 + r1*z1);
										a1 = (x1*x1 + y1*y1 + z1*z1);

										w11 = (-b1 + sqrt(b1*b1 - 4 * a1*c1)) / 2 * a1;
										w22 = (-b1 - sqrt(b1*b1 - 4 * a1*c1)) / 2 * a1;

										if (w11 < 0)
										{
											sphere->center.x += w11*sphere->velocity.x;
											sphere->center.y += w11*sphere->velocity.y;
											sphere->center.z += w11*sphere->velocity.z;

											proj = DotProduct(sphere->velocity, getVectorA(object[i].Faces[j].vertex[l], sphere->center)) /
												DotProduct(getVectorA(object[i].Faces[j].vertex[l], sphere->center), getVectorA(object[i].Faces[j].vertex[l], sphere->center));

											v_fix.x = getVectorA(object[i].Faces[j].vertex[l], sphere->center).x*proj;
											v_fix.y = getVectorA(object[i].Faces[j].vertex[l], sphere->center).y*proj;
											v_fix.z = getVectorA(object[i].Faces[j].vertex[l], sphere->center).z*proj;


											sphere->velocity.x -= 1.5*v_fix.x;
											sphere->velocity.y -= 1.5*v_fix.y;
											sphere->velocity.z -= 1.5*v_fix.z;


											return 1;

										}
										else if (w22 < 0)
										{
											sphere->center.x += w22*sphere->velocity.x;
											sphere->center.y += w22*sphere->velocity.y;
											sphere->center.z += w22*sphere->velocity.z;

											proj = DotProduct(sphere->velocity, getVectorA(object[i].Faces[j].vertex[l], sphere->center)) /
												DotProduct(getVectorA(object[i].Faces[j].vertex[l], sphere->center), getVectorA(object[i].Faces[j].vertex[l], sphere->center));


											v_fix.x = getVectorA(object[i].Faces[j].vertex[l], sphere->center).x*proj;
											v_fix.y = getVectorA(object[i].Faces[j].vertex[l], sphere->center).y*proj;
											v_fix.z = getVectorA(object[i].Faces[j].vertex[l], sphere->center).z*proj;


											sphere->velocity.x -= 1.5*v_fix.x;
											sphere->velocity.y -= 1.5*v_fix.y;
											sphere->velocity.z -= 1.5*v_fix.z;

											return 1;

										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
};


void DrawSphere(sub * sphere)
{
	float radius = sphere->radius;
	float theta = 360 / MAX;
	float rad = M_PI / 180;
	float z[MAX + 1];
	float radius_i;
	Face faces[MAX][MAX];

	v2f vertex[MAX + 1][MAX + 1];

	for (int i = 0; i < MAX + 1; i++)
		z[i] = radius*sin(i*theta*rad);

	for (int i = 0; i < MAX + 1; i++)
	{
		radius_i = radius*cos(asin(z[i] / radius));

		for (int j = 0; j < MAX + 1; j++)
		{
			vertex[i][j].x = radius_i*cos(j*theta*rad);
			vertex[i][j].y = radius_i*sin(j*theta*rad);
		}

	}

	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			faces[i][j].vertex[0] = setVertex(vertex[i][j].x + sphere->center.x, vertex[i][j].y + sphere->center.y, z[i] + sphere->center.z);
			faces[i][j].vertex[1] = setVertex(vertex[i][j + 1].x + sphere->center.x, vertex[i][j + 1].y + sphere->center.y, z[i] + sphere->center.z);
			faces[i][j].vertex[2] = setVertex(vertex[i + 1][j + 1].x + sphere->center.x, vertex[i + 1][j + 1].y + sphere->center.y, z[i + 1] + sphere->center.z);
			faces[i][j].vertex[3] = setVertex(vertex[i + 1][j].x + sphere->center.x, vertex[i + 1][j].y + sphere->center.y, z[i + 1] + sphere->center.z);

		}
	}

	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			glBegin(GL_POLYGON);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glVertex3f(faces[i][j].vertex[0].x, faces[i][j].vertex[0].y, faces[i][j].vertex[0].z);
			glVertex3f(faces[i][j].vertex[1].x, faces[i][j].vertex[1].y, faces[i][j].vertex[1].z);

			glVertex3f(faces[i][j].vertex[2].x, faces[i][j].vertex[2].y, faces[i][j].vertex[2].z);

			glVertex3f(faces[i][j].vertex[3].x, faces[i][j].vertex[3].y, faces[i][j].vertex[3].z);


			glEnd();
		}
	}




}



void SetFaceNormals(int objectArrayElementNum)
{
	for(int i=0;i<objectArrayElementNum;i++)
		for (int j = 0;j < 6;j++)
		{
			objects[i].Faces[j].normal = CrossProduct(getVector(objects[i].Faces[j].vertex[0], objects[i].Faces[j].vertex[1]),
				getVector(objects[i].Faces[j].vertex[0], objects[i].Faces[j].vertex[2]));
		}
};



void physicsOnIdleCallback(int objectArrayElementNum)
{
	int iscollision = collision(&sphere, objects, objectArrayElementNum);

	if (iscollision == 0)
	{
		sphere.acc.z = 0;
		sphere.acc.y = 0;
		sphere.acc.x = 0;
	}
	

	glutPostRedisplay();
}

void physicsOnRenderCallback()
{
	sphere.velocity.x += sphere.acc.x;
	sphere.velocity.y += sphere.acc.y;
	sphere.velocity.z += sphere.acc.z;

	sphere.center.x += sphere.velocity.x;
	sphere.center.y += sphere.velocity.y;
	sphere.center.z += sphere.velocity.z;
}

