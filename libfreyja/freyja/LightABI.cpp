/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the Light ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#include "LightABI.h"

index_t gFreyjaLightIndex = INDEX_INVALID;
Vector<FreyjaLight *>  gFreyjaLights;

// FIXME: Update to class internal pool system

index_t freyjaLightCreate()
{
	index_t lightIndex = gFreyjaLights.size();

	gFreyjaLights.pushBack(new FreyjaLight());

	return lightIndex;	
}


uint32 freyjaGetLightCount()
{
	return gFreyjaLights.size();
}


index_t freyjaGetCurrentLight()
{
	return gFreyjaLightIndex;
}


void freyjaCurrentLight(uint32 lightIndex)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLightIndex = lightIndex;
	}
}


void freyjaLightDelete(int32 lightIndex)
{
}


// Light mutators /////////////////

void freyjaLightPosition4v(uint32 lightIndex, vec4_t position)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->setPosition(position);
	}
}


void freyjaLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mAmbient[0] = ambient[0];
		gFreyjaLights[lightIndex]->mAmbient[1] = ambient[1];
		gFreyjaLights[lightIndex]->mAmbient[2] = ambient[2];
		gFreyjaLights[lightIndex]->mAmbient[3] = ambient[3];
	}
}


void freyjaLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mDiffuse[0] = diffuse[0];
		gFreyjaLights[lightIndex]->mDiffuse[1] = diffuse[1];
		gFreyjaLights[lightIndex]->mDiffuse[2] = diffuse[2];
		gFreyjaLights[lightIndex]->mDiffuse[3] = diffuse[3];
	}
}


void freyjaLightSpecular(uint32 lightIndex, vec4_t specular)
{
	if (lightIndex < gFreyjaLights.size())
	{
		gFreyjaLights[lightIndex]->mSpecular[0] = specular[0];
		gFreyjaLights[lightIndex]->mSpecular[1] = specular[1];
		gFreyjaLights[lightIndex]->mSpecular[2] = specular[2];
		gFreyjaLights[lightIndex]->mSpecular[3] = specular[3];
	}
}


// Light accessors /////////////////

void freyjaGetLightPosition4v(uint32 lightIndex, vec4_t position)
{
	if (lightIndex < gFreyjaLights.size())
	{
		position[0] = gFreyjaLights[lightIndex]->mPos[0];
		position[1] = gFreyjaLights[lightIndex]->mPos[1];
		position[2] = gFreyjaLights[lightIndex]->mPos[2];
		position[3] = gFreyjaLights[lightIndex]->mPos[3];
	}
}


void freyjaGetLightAmbient(uint32 lightIndex, vec4_t ambient)
{
	if (lightIndex < gFreyjaLights.size())
	{
		ambient[0] = gFreyjaLights[lightIndex]->mAmbient[0];
		ambient[1] = gFreyjaLights[lightIndex]->mAmbient[1];
		ambient[2] = gFreyjaLights[lightIndex]->mAmbient[2];
		ambient[3] = gFreyjaLights[lightIndex]->mAmbient[3];
	}
}


void freyjaGetLightDiffuse(uint32 lightIndex, vec4_t diffuse)
{
	if (lightIndex < gFreyjaLights.size())
	{
		diffuse[0] = gFreyjaLights[lightIndex]->mDiffuse[0];
		diffuse[1] = gFreyjaLights[lightIndex]->mDiffuse[1];
		diffuse[2] = gFreyjaLights[lightIndex]->mDiffuse[2];
		diffuse[3] = gFreyjaLights[lightIndex]->mDiffuse[3];
	}
}


void freyjaGetLightSpecular(uint32 lightIndex, vec4_t specular)
{
	if (lightIndex < gFreyjaLights.size())
	{
		specular[0] = gFreyjaLights[lightIndex]->mSpecular[0];
		specular[1] = gFreyjaLights[lightIndex]->mSpecular[1];
		specular[2] = gFreyjaLights[lightIndex]->mSpecular[2];
		specular[3] = gFreyjaLights[lightIndex]->mSpecular[3];
	}
}
