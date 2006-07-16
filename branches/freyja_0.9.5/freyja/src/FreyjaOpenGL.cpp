/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator scripopt.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Centralized various graphics util functions here
 *            Normalized the API to be more consistant
 ==========================================================================*/


#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef HAVE_OPENGL
//#   ifdef MACOSX
//#      include <OpenGL/OpenGL.h>
//#   else
#      include <GL/gl.h>
#      include <GL/glu.h>
//#   endif
#endif


#include <freyja/freyja.h>
#include <freyja/FreyjaPluginABI.h>
#include <hel/math.h>

#include "Freyja3dCursor.h"
#include "Texture.h"


const float RED[]          = {  1.0,  0.0,  0.0, 1.0 };
const float GREEN[]        = {  0.0,  1.0,  0.0, 1.0 };
const float BLUE[]         = {  0.0,  0.0,  1.0, 1.0 };
const float CYAN[]         = {  0.0,  1.0,  1.0, 1.0 };
const float ORANGE[]       = {  1.0,  7.0,  0.0, 1.0 };
const float YELLOW[]       = {  1.0,  1.0,  0.0, 1.0 };
const float BLACK[]        = {  0.0,  0.0,  0.0, 1.0 };
const float WHITE[]        = {  1.0,  1.0,  1.0, 1.0 };
const float NEXT_PURPLE[]  = {  0.3,  0.3,  0.5, 1.0 };
const float NEXT_PURPLE2[] = {  0.4,  0.4,  0.6, 1.0 };


bool mglHardwareExtTest(const char *ext)
{
	bool ret = false;
	
	
	if (strstr((const char*)glGetString(GL_EXTENSIONS), ext))
	{
		ret = true;
	}
	
	return ret;
}


void mgl2dProjection(const unsigned int width, const unsigned int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	glOrtho(0, width, 0, height, -99999, 99999);
	glTranslatef(0.0, 0.0, 99998.0);
	glMatrixMode(GL_MODELVIEW);
}



void mglDrawBbox(const vec3_t min, const vec3_t max, 
				 const vec4_t pointColor, const vec4_t lineColor)
{
	glPointSize(4.0);

	glColor4fv(pointColor);

	glBegin(GL_POINTS);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glEnd();


	glColor4fv(lineColor);

	glBegin(GL_LINES);
	// max, top quad
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], min[2]);

	// max-min, vertical quads
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min-max, vertical quads
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);

	// min, bottom quad
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);  
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], max[2]);
	glEnd();
}


void mglDrawSelectBox(const vec3_t min, const vec3_t max, const vec4_t lineColor)
{
	vec_t w = (max[0] - min[0]);
	vec_t h = (max[1] - min[1]);
	vec_t d = (max[2] - min[2]);
	vec_t sw = w * 0.33;
	vec_t sh = h * 0.33;
	vec_t sd = d * 0.33;


	glColor4fv(lineColor);

	//glBegin(GL_POINTS);
	//glVertex3f(max[0], max[1], max[2]);
	//glVertex3f(min[0], min[1], min[2]);
	//glVertex3f(max[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], max[2]);
	//glVertex3f(max[0], max[1], min[2]);
	//glVertex3f(min[0], min[1], max[2]);
	//glVertex3f(min[0], max[1], min[2]);
	//glVertex3f(max[0], min[1], min[2]);
	//glEnd();


	glBegin(GL_LINES);

	// A
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0] + sw, max[1], min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1] - sh, min[2]);
	glVertex3f(min[0], max[1], min[2]);
	glVertex3f(min[0], max[1], min[2] + sd);

	// B
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0] + sw, max[1], max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1] - sh, max[2]);
	glVertex3f(min[0], max[1], max[2]);
	glVertex3f(min[0], max[1], max[2] -sd);

	// C ( max )
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0] - sw, max[1], max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1] - sh, max[2]);
	glVertex3f(max[0], max[1], max[2]);
	glVertex3f(max[0], max[1], max[2] - sd);

	// D
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0] - sw, max[1], min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1] - sh, min[2]);
	glVertex3f(max[0], max[1], min[2]);
	glVertex3f(max[0], max[1], min[2] + sd);

	// E ( min )
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0] + sw, min[1], min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1] + sh, min[2]);
	glVertex3f(min[0], min[1], min[2]);
	glVertex3f(min[0], min[1], min[2] + sd);

	// F
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0] + sw, min[1], max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1] + sh, max[2]);
	glVertex3f(min[0], min[1], max[2]);
	glVertex3f(min[0], min[1], max[2] - sd);

	// G
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0] - sw, min[1], max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1] + sh, max[2]);
	glVertex3f(max[0], min[1], max[2]);
	glVertex3f(max[0], min[1], max[2] - sd);
	
	//H
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0] - sw, min[1], min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1] + sh, min[2]);
	glVertex3f(max[0], min[1], min[2]);
	glVertex3f(max[0], min[1], min[2] + sd);

	glEnd();
}




void mglDrawAxis(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(-mid, 0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	//  Y arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
	//  Z arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, min);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, -min);


	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0, -mid, 0.0);	
	//  X arrowhead		
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
	//  Z arrowhead
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, min);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, -min);

      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0, -mid);
	//  Y arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  min,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0, -min,  max);
	//  X arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(min,  0.0,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(-min, 0.0,  max);
	glEnd();
}


void mglDraw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max)
{
	glBegin(GL_LINES);
      
	// X Axis, red
	glColor3fv(RED);
	glVertex3f(0.0,  0.0, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	//  Y arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  min, 0.0);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max, -min, 0.0);
	//  Z arrowhead
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, min);
	glVertex3f(mid,  0.0, 0.0);
	glVertex3f(max,  0.0, -min);


	// Y Axis, green
	glColor3fv(GREEN);	
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  0.0, 0.0);	
	//  X arrowhead		
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(min,  max, 0.0);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(-min, max, 0.0);
	//  Z arrowhead
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, min);
	glVertex3f(0.0,  mid, 0.0);
	glVertex3f(0.0,  max, -min);

      
	// Z Axis, blue
	glColor3fv(BLUE);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  0.0,  0.0);
	//  Y arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0,  min,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(0.0, -min,  max);
	//  X arrowhead
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(min,  0.0,  max);
	glVertex3f(0.0,  0.0,  mid);
	glVertex3f(-min, 0.0,  max);
	glEnd();
}

void mglDraw3dCursor(const vec_t min, const vec_t mid, const vec_t max)
{
	extern Freyja3dCursor gFreyjaCursor;

	glPushMatrix();
	glTranslatef(gFreyjaCursor.mPos.mVec[0], 
				 gFreyjaCursor.mPos.mVec[1], 
				 gFreyjaCursor.mPos.mVec[2]);

	switch (gFreyjaCursor.GetMode())
	{
	case Freyja3dCursor::Translation:
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glClear(GL_DEPTH_BUFFER_BIT);
		mglDraw3dCursorLoc(min, mid, max);
		glPopAttrib();
		break;

	default:
	case Freyja3dCursor::Invisible:
		break;
	}

	glPopMatrix();
}


/* Based on draw_sphere (public domain), 1995, David G Yu */
void mglDrawSphere(int numMajor, int numMinor, float radius)
{
	double majorStep = (M_PI / numMajor);
	double minorStep = (2.0 * M_PI / numMinor);
	double a, b, c, r0, r1;
	float z0, z1, x, y;
	int i, j;
	
	
	for (i = 0; i < numMajor; ++i)
	{
		a = i * majorStep;
		b = a + majorStep;
		r0 = radius * sin(a);
		r1 = radius * sin(b);
		z0 = radius * cos(a);
		z1 = radius * cos(b);
		
		glBegin(GL_TRIANGLE_STRIP);
		
		for (j = 0; j <= numMinor; ++j) 
		{
			c = j * minorStep;
			x = cos(c);
			y = sin(c);
			
			glNormal3f((x * r0) / radius, (y * r0) / radius, z0 / radius);
			glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
			glVertex3f(x * r0, y * r0, z0);
			
			glNormal3f((x * r1) / radius, (y * r1) / radius, z1 / radius);
			glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
			glVertex3f(x * r1, y * r1, z1);
		}
		
		glEnd();
	}
}


void mglDrawBone(unsigned char type, const vec3_t pos)
{
	const vec_t min = 0.05f;
	const vec_t max = 0.50f;


	switch (type)
	{
	case 1:
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(pos[0], pos[1], pos[2]);
		glEnd();
		break;

	case 2:
		glBegin(GL_LINE_STRIP);
		glVertex3f(0.0f,   min,  0.0f);    // 0
		glVertex3f(-max,  0.0f, -max);     // 1
		glVertex3f( max,  0.0f, -max);     // 2
		glVertex3fv(pos);                        // Base
		glVertex3f(-max,  0.0f,-max);      // 1
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f( max,  0.0f,-max);      // 2
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f( 0.0f,  min, 0.0f);     // 0
		glVertex3f(-max,  0.0f, max);      // 4
		glVertex3fv(pos);                        // Base
		glVertex3f( max,  0.0f, max);      // 3
		glVertex3f(-max,  0.0f, max);      // 4
		glEnd();
		break;
	}
}


void mglDrawJoint(unsigned char type, const vec3_t pos)
{
	switch (type)
	{
	case 1:
		glBegin(GL_POINTS);
		glVertex3fv(pos);
		glEnd();
		break;
	case 2:
		mglDrawSphere(12, 12, 0.5f);
		break;
	case 3:
		mglDrawAxis(0.25f, 1.2f, 0.872f);
		break;
	}
}


void mglDrawGrid(vec3_t color, vec_t size, vec_t step, vec_t scale)
{
	vec_t x, z;


	/* Draw grid without using GL Scaling, which is 'teh bad' */
	glPushMatrix();
	glColor3fv(color);

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_LINE_LOOP);

		for (z = -size; z < size; z += step)
		{
			glVertex3f((x + step) * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, z * scale);	
			glVertex3f(x * scale, 0.0f, (z + step) * scale);
			glVertex3f((x + step) * scale, 0.0f, (z + step) * scale);
		}

		glEnd();
	}

	glPopMatrix();
}


void mglGetOpenGLViewport(int *viewportXYWH) // int[4]
{
	glGetIntegerv(GL_VIEWPORT, viewportXYWH);
}

void mglGetOpenGLModelviewMatrix(double *modelview) // double[16]
{
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
}


void mglGetOpenGLProjectionMatrix(double *projection) // double[16]
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
}



void mglApplyMaterial(uint32 materialIndex)
{
	vec4_t ambient, diffuse, specular, emissive;
	vec_t shininess;
	uint32 flags, texture, texture2, blendSrc, blendDest;


	if (materialIndex > freyjaGetMaterialCount())
	{
		materialIndex = 0;
	}

	flags = freyjaGetMaterialFlags(materialIndex);
	texture = freyjaGetMaterialTexture(materialIndex);
	blendSrc = freyjaGetMaterialBlendSource(materialIndex);
	blendDest = freyjaGetMaterialBlendDestination(materialIndex);
	freyjaGetMaterialAmbient(materialIndex, ambient);
	freyjaGetMaterialDiffuse(materialIndex, diffuse);
	freyjaGetMaterialSpecular(materialIndex, specular);
	freyjaGetMaterialEmissive(materialIndex, emissive);
	shininess = freyjaGetMaterialShininess(materialIndex);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	glMaterialfv(GL_FRONT, GL_SHININESS, &(shininess));

	// 2006.07.15 - Hey, these should use Texture class binding to
	// make sure the array ids match in the gl texture binding
	if (flags & fFreyjaMaterial_DetailTexture)
	{
		Texture::mSingleton->bindMultiTexture(texture, texture2);
	}
	else if (flags & fFreyjaMaterial_Texture) // Non-colored is ( id + 1)
	{
		glBindTexture(GL_TEXTURE_2D, texture+1);
	}
	else // Colored, first texture is a generated WHITE 32x32
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (flags & fFreyjaMaterial_Blending)
	{
		glBlendFunc(blendSrc, blendDest);
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}








