/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : Freyja3dCursor
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.15:
 * Mongoose - Created, moved into it's own header from FreyjaModel 'test'
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJA3D_CURSOR_H_
#define GUARD__FREYJA_MONGOOSE_FREYJA3D_CURSOR_H_


#include <hel/math.h>
#include <hel/Ray.h>
#include <mstl/stack.h>

#include "freyja_events.h"
#include "FreyjaState.h"
#include "FreyjaOpenGL.h"


namespace freyja3d {

class Cursor
{
 public:

	typedef enum {
		Rotation,
		Translation,
		Scale,
		Invisible
	} Flags;


	typedef enum {

		eNone = 0,
		eX,
		eY,
		eZ,
		eAll

	} axis_t;


	const static vec_t min = 0.5f, mid = 2.4f, max = 1.744f;


	Cursor()
	{
		Reset();
	}

	~Cursor() 
	{ 
	}

	bool CheckForRayCollision(Ray &r)
	{
		bool ret = false;

		switch (GetMode())
		{
		case freyja3d::Cursor::Scale:
			break;


		case freyja3d::Cursor::Rotation:
			{
				mSelected = false;

				// if this returns true mark mToken in Control

				// Ray test to pick rings of rotation
				// 1. Take the ray and generate a rect prism
				// 2. Use line segment / bbox detection to find
				//    nearest hit
				// 3. Return true if hit found, and set selected axis

				//mPos <- center
				vec_t radius = max * 4.0f;
				const uint32 count = 64;

				// red
				//glColor3fv(RED);
				//mglDraw3dCircle(mPos.mVec, radius, count, 1, true);
				
				// green
				//glColor3fv(GREEN);
				//mglDraw3dCircle(mPos.mVec, radius, count, 2, true);

				// blue
				//glColor3fv(BLUE);
				//mglDraw3dCircle(mPos.mVec, radius, count, 0, true);

				vec_t x, z, i;
				Vector<vec_t> xs, zs;

				//glBegin(solid ? GL_LINE_LOOP : GL_LINES);

				for (i = 0.0f; i < count; ++i)
				{
					helSinCosf(helDegToRad(360.0f * (i / count)), &x, &z);
					
					x *= radius;
					z *= radius;
					
					xs.pushBack(x);
					zs.pushBack(z);

					/*
					switch (plane)
					{
					case 0:
						glVertex3f(center[0] + x, center[1] + z, center[2] + 0.0f);
						break;
						
					case 1:
						glVertex3f(center[0] + 0.0f, center[1] + x, center[2] + z);
						break;
						
					default:
						glVertex3f(center[0] + x, center[1] + 0.0f, center[2] + z);
						break;
					}
					*/
				}

				// Pick the closest 'hit' from each 'axis' ring
				Vec3 min, max;
				vec_t dist, best = 999.9f;
				bool hit;

				for (uint j = 0; j < (count-1); ++j)
				{
					{
					Vec3 a = mPos + Vec3(0.0f, xs[j  ], zs[j  ]);
					Vec3 b = mPos + Vec3(0.0f, xs[j+1], zs[j+1]);
					Vec3 n = mPos - a;
					n.normalize();
					Vec3 side = Vector3d::cross(a-b, n);

					min = a - n * -1 - side * -1;
					max = b - n * 1 - side * 1;

					hit = r.IntersectBox(min.mVec, max.mVec, dist);
					
					if (hit && dist < best)
					{
						mAxis = eX;
						mSelected = true;
						best = dist;
					}
					}

					{
					Vec3 a = mPos + Vec3(xs[j  ], 0.0f, zs[j  ]);
					Vec3 b = mPos + Vec3(xs[j+1], 0.0f, zs[j+1]);
					Vec3 n = mPos - a;
					n.normalize();
					Vec3 side = Vector3d::cross(a-b, n);

					min = a - n * -1 - side * -1;
					max = b - n * 1 - side * 1;

					hit = r.IntersectBox(min.mVec, max.mVec, dist);
					
					if (hit && dist < best)
					{
						mAxis = eY;
						mSelected = true;
						best = dist;
					}
					}

					{
					Vec3 a = mPos + Vec3(xs[j  ], zs[j  ], 0.0f);
					Vec3 b = mPos + Vec3(xs[j+1], zs[j+1], 0.0f);
					Vec3 n = mPos - a;
					n.normalize();
					Vec3 side = Vector3d::cross(a-b, n);

					min = a - n * -1 - side * -1;
					max = b - n * 1 - side * 1;

					hit = r.IntersectBox(min.mVec, max.mVec, dist);
					
					if (hit && dist < best)
					{
						mAxis = eZ;
						mSelected = true;
						best = dist;
					}
					}
				}

				ret = mSelected;
			}
			break;


		case freyja3d::Cursor::Translation:
			break;

		default:
			ret = false;
		}

		return ret;
	}

	void Display()
	{
		glPushMatrix();
		glTranslatef(mPos.mVec[0], mPos.mVec[1], mPos.mVec[2]);

		switch (GetMode())
		{
		case freyja3d::Cursor::Scale:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);
			Draw3dCursorScale(min, mid, max);
			glPopAttrib();
			break;


		case freyja3d::Cursor::Rotation:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);

			glPushMatrix();
			if (GetMode() == freyja3d::Cursor::Rotation)
			{
				glRotatef(mRotate.mVec[0], 1, 0, 0);
				glRotatef(mRotate.mVec[1], 0, 1, 0);
				glRotatef(mRotate.mVec[2], 0, 0, 1);
			}
			Draw3dCursorRot(min, mid, max);
			glPopMatrix();
			glPopAttrib();
			break;

		case freyja3d::Cursor::Translation:
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glClear(GL_DEPTH_BUFFER_BIT);
			Draw3dCursorLoc(min, mid, max);
			glPopAttrib();
			break;

		default:
		case freyja3d::Cursor::Invisible:
			break;
		}

		glPopMatrix();
	}

	Flags GetMode() { return mMode; }

	void SetMode(Flags n) { mMode = n; }

	void Reset()
	{
		mSelected = false;
		mAxis = eNone;
		mMode = Invisible;
		mPos = Vector3d(0.0f, 0.0f, 0.0f);
		mScale = Vector3d(1.0f, 1.0f, 1.0f);
		mRotate = Vector3d(0.0f, 0.0f, 0.0f);//Quaternion();
	}

	bool mSelected;

	axis_t mAxis;

	Vector3d mLastPos;

	Vector3d mPos;

	Vector3d mScale;

	Vector3d mRotate;

	//Quaternion mRotate;


 private:

	void Draw3dCursorLoc(const vec_t min, const vec_t mid, const vec_t max)
	{
		static int drawList = -1;

		if (drawList == -1)
		{
			drawList = glGenLists(1);
			glNewList(drawList, GL_COMPILE);
			mglDrawAxisWithCones(min, mid, max, 8);
			glEndList();
		}
		else
		{
			glCallList(drawList);
		}
	}

	void Draw3dCursorRot(const vec_t min, const vec_t mid, const vec_t max)
	{
		vec3_t center = {0.0f, 0.0f, 0.0f};
		vec_t radius = max * 4.0f;// / 2.0f;
		const uint32 count = 64;

		// red
		glColor3fv(RED);
		mglDraw3dCircle(center, radius, count, 1, true);

		// green
		glColor3fv(GREEN);
		mglDraw3dCircle(center, radius, count, 2, true);

		// blue
		glColor3fv(BLUE);
		mglDraw3dCircle(center, radius, count, 0, true);
	}


	void Draw3dCursorScale(const vec_t min, const vec_t mid, const vec_t max)
	{
		glBegin(GL_LINES);
      
		// X Axis, red
		glColor3fv(RED);
		glVertex3f(0.0,  0.0, 0.0);
		glVertex3f(mid,  0.0, 0.0);

		// Y Axis, green
		glColor3fv(GREEN);	
		glVertex3f(0.0,  mid, 0.0);
		glVertex3f(0.0,  0.0, 0.0);	
      
		// Z Axis, blue
		glColor3fv(BLUE);
		glVertex3f(0.0,  0.0,  mid);
		glVertex3f(0.0,  0.0,  0.0);
		glEnd();


		glBegin(GL_QUADS);
      
		// X Axis, Red box
		glColor3fv(RED);
		glVertex3f(mid+min,  -min, min);
		glVertex3f(mid+min,  min, min);
		glVertex3f(mid-min,  min, min);
		glVertex3f(mid-min,  -min, min);

		glVertex3f(mid+min,  -min, -min);
		glVertex3f(mid+min,  min, -min);
		glVertex3f(mid-min,  min, -min);
		glVertex3f(mid-min,  -min, -min);

		glVertex3f(mid+min,  min, -min);
		glVertex3f(mid+min,  min, min);
		glVertex3f(mid-min,  min, min);
		glVertex3f(mid-min,  min, -min);

		glVertex3f(mid+min,  -min, -min);
		glVertex3f(mid+min,  -min, min);
		glVertex3f(mid-min,  -min, min);
		glVertex3f(mid-min,  -min, -min);

		glVertex3f(mid+min,  min, -min);
		glVertex3f(mid+min,  min, min);
		glVertex3f(mid+min,  -min, min);
		glVertex3f(mid+min,  -min, -min);


		// Y Axis, green
		glColor3fv(GREEN);	
		glVertex3f(-min, mid+min,  min);
		glVertex3f(min,mid+min,   min);
		glVertex3f(min,mid-min,   min);
		glVertex3f(-min,mid-min,   min);

		glVertex3f(-min,mid+min,   -min);
		glVertex3f(min,mid+min,   -min);
		glVertex3f( min,mid-min,  -min);
		glVertex3f(-min,mid-min,   -min);

		glVertex3f( min,mid+min,  -min);
		glVertex3f( min,mid+min,  min);
		glVertex3f(min,mid-min,   min);
		glVertex3f( min,mid-min,  -min);

		glVertex3f( -min,mid+min,  -min);
		glVertex3f( -min,mid+min,  min);
		glVertex3f(-min,mid-min,   min);
		glVertex3f(-min,mid-min,   -min);

		glVertex3f(min,mid+min,   -min);
		glVertex3f(min,mid+min,   min);
		glVertex3f( -min,mid+min,  min);
		glVertex3f(-min,mid+min,   -min);

      
		// Z Axis, blue
		glColor3fv(BLUE);
		glVertex3f(-min, min, mid+min);
		glVertex3f(min,   min, mid+min);
		glVertex3f(min,   min, mid-min);
		glVertex3f(-min,   min, mid-min);

		glVertex3f(-min,    -min, mid+min);
		glVertex3f(min,   -min, mid+min);
		glVertex3f( min,  -min, mid-min);
		glVertex3f(-min,   -min, mid-min);

		glVertex3f( min,  -min, mid+min);
		glVertex3f( min,  min, mid+min);
		glVertex3f(min,   min, mid-min);
		glVertex3f( min,  -min, mid-min);

		glVertex3f( -min,  -min, mid+min);
		glVertex3f( -min,  min, mid+min);
		glVertex3f(-min,   min, mid-min);
		glVertex3f(-min,   -min, mid-min);

		glVertex3f(min,   -min, mid+min);
		glVertex3f(min,   min, mid+min);
		glVertex3f( -min,  min, mid+min);
		glVertex3f(-min,   -min, mid+min);


		glEnd();
	}


	Flags mMode;
};

} //namespace freyja3d 

#endif // GUARD__FREYJA_MONGOOSE_FREYJA3DCURSOR_H_
