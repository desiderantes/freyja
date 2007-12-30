/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Comments: This is the Lua plugin ABI.
 *
 *           This file was generated by Mongoose's UpdateBindings-Lua.py
 *           - Mongoose <mongooseichiban@gmail.com>
 *
 * 2007.12.30:
 * Mongoose - Generated, Based on C ABI for freyja
 ==========================================================================*/

#include "SkeletonABI.h"
#include "BoneABI.h"
#include "LightABI.h"
#include "MaterialABI.h"
#include "PakABI.h"
#include "MeshABI.h"
#include "CameraABI.h"
#include "NodeABI.h"
#include "SceneNodeABI.h"
#include "SceneABI.h"
#include "FileABI.h"
#include "PixelBufferABI.h"
#include "freyja.h"
#include "LuaABI.h"
#ifdef LUAWRAPPER_FOUND



int lua_freyjaSkeletonCreate(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char* name = lua_tostring(L, 1);
	freyjaSkeletonCreate(name);
	return 0;
}


int lua_freyjaSkeletonDelete(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	freyjaSkeletonDelete(skeleton);
	return 0;
}


int lua_freyjaSkeletonGetBone(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	freyja_id bone = (int)lua_tonumber(L, 2);
	freyja_ptr userdata_value = freyjaSkeletonGetBone(skeleton, bone);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaSkeletonGetBoneCount(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	int integer_value = freyjaSkeletonGetBoneCount(skeleton);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaSkeletonGetRoot(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	freyja_ptr userdata_value = freyjaSkeletonGetRoot(skeleton);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaSkeletonSetRoot(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	freyja_ptr bone = lua_touserdata(L, 2);
	freyjaSkeletonSetRoot(skeleton, bone);
	return 0;
}


int lua_freyjaBoneCreate(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr skeleton = lua_touserdata(L, 1);
	const char* name = lua_tostring(L, 2);
	freyjaBoneCreate(skeleton, name);
	return 0;
}


int lua_freyjaBoneDelete(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr bone = lua_touserdata(L, 1);
	freyjaBoneDelete(bone);
	return 0;
}


int lua_freyjaLightCreate(lua_State *L)
{
	freyjaLightCreate();
	return 0;
}


int lua_freyjaLightDelete(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	index_t lightIndex = (int)lua_tonumber(L, 1);
	freyjaLightDelete(lightIndex);
	return 0;
}


int lua_freyjaLightPosition4v(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t lightIndex = (int)lua_tonumber(L, 1);
	vec4_t position;
	position[0] = lua_tonumber(L, 2);
	position[1] = lua_tonumber(L, 3);
	position[2] = lua_tonumber(L, 4);
	position[3] = lua_tonumber(L, 5);
	freyjaLightPosition4v(lightIndex, position);
	return 0;
}


int lua_freyjaLightAmbient(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t lightIndex = (int)lua_tonumber(L, 1);
	vec4_t ambient;
	ambient[0] = lua_tonumber(L, 2);
	ambient[1] = lua_tonumber(L, 3);
	ambient[2] = lua_tonumber(L, 4);
	ambient[3] = lua_tonumber(L, 5);
	freyjaLightAmbient(lightIndex, ambient);
	return 0;
}


int lua_freyjaLightDiffuse(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t lightIndex = (int)lua_tonumber(L, 1);
	vec4_t diffuse;
	diffuse[0] = lua_tonumber(L, 2);
	diffuse[1] = lua_tonumber(L, 3);
	diffuse[2] = lua_tonumber(L, 4);
	diffuse[3] = lua_tonumber(L, 5);
	freyjaLightDiffuse(lightIndex, diffuse);
	return 0;
}


int lua_freyjaLightSpecular(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t lightIndex = (int)lua_tonumber(L, 1);
	vec4_t specular;
	specular[0] = lua_tonumber(L, 2);
	specular[1] = lua_tonumber(L, 3);
	specular[2] = lua_tonumber(L, 4);
	specular[3] = lua_tonumber(L, 5);
	freyjaLightSpecular(lightIndex, specular);
	return 0;
}


int lua_freyjaMaterialCreate(lua_State *L)
{
	freyjaMaterialCreate();
	return 0;
}


int lua_freyjaMaterialClearAll(lua_State *L)
{
	freyjaMaterialClearAll();
	return 0;
}


int lua_freyjaMaterialName(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	const char* name = lua_tostring(L, 2);
	freyjaMaterialName(material, name);
	return 0;
}


int lua_freyjaMaterialClearFlag(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	uint32 flag = (int)lua_tonumber(L, 2);
	freyjaMaterialClearFlag(material, flag);
	return 0;
}


int lua_freyjaMaterialSetFlag(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	uint32 flag = (int)lua_tonumber(L, 2);
	freyjaMaterialSetFlag(material, flag);
	return 0;
}


int lua_freyjaMaterialFlags(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	uint32 flags = (int)lua_tonumber(L, 2);
	freyjaMaterialFlags(material, flags);
	return 0;
}


int lua_freyjaMaterialShaderName(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	const char* filename = lua_tostring(L, 2);
	freyjaMaterialShaderName(material, filename);
	return 0;
}


int lua_freyjaMaterialTextureName(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	const char* name = lua_tostring(L, 2);
	freyjaMaterialTextureName(material, name);
	return 0;
}


int lua_freyjaMaterialShader(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	index_t shader = (int)lua_tonumber(L, 2);
	freyjaMaterialShader(material, shader);
	return 0;
}


int lua_freyjaMaterialTexture(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	index_t texture = (int)lua_tonumber(L, 2);
	freyjaMaterialTexture(material, texture);
	return 0;
}


int lua_freyjaMaterialAmbient(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec4_t ambient;
	ambient[0] = lua_tonumber(L, 2);
	ambient[1] = lua_tonumber(L, 3);
	ambient[2] = lua_tonumber(L, 4);
	ambient[3] = lua_tonumber(L, 5);
	freyjaMaterialAmbient(material, ambient);
	return 0;
}


int lua_freyjaMaterialDiffuse(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec4_t diffuse;
	diffuse[0] = lua_tonumber(L, 2);
	diffuse[1] = lua_tonumber(L, 3);
	diffuse[2] = lua_tonumber(L, 4);
	diffuse[3] = lua_tonumber(L, 5);
	freyjaMaterialDiffuse(material, diffuse);
	return 0;
}


int lua_freyjaMaterialSpecular(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec4_t specular;
	specular[0] = lua_tonumber(L, 2);
	specular[1] = lua_tonumber(L, 3);
	specular[2] = lua_tonumber(L, 4);
	specular[3] = lua_tonumber(L, 5);
	freyjaMaterialSpecular(material, specular);
	return 0;
}


int lua_freyjaMaterialEmissive(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec4_t emissive;
	emissive[0] = lua_tonumber(L, 2);
	emissive[1] = lua_tonumber(L, 3);
	emissive[2] = lua_tonumber(L, 4);
	emissive[3] = lua_tonumber(L, 5);
	freyjaMaterialEmissive(material, emissive);
	return 0;
}


int lua_freyjaMaterialShininess(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec_t exponent = lua_tonumber(L, 2);
	freyjaMaterialShininess(material, exponent);
	return 0;
}


int lua_freyjaMaterialTransparency(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	vec_t transparency = lua_tonumber(L, 2);
	freyjaMaterialTransparency(material, transparency);
	return 0;
}


int lua_freyjaMaterialBlendSource(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	uint32 factor = (int)lua_tonumber(L, 2);
	freyjaMaterialBlendSource(material, factor);
	return 0;
}


int lua_freyjaMaterialBlendDestination(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	index_t material = (int)lua_tonumber(L, 1);
	uint32 factor = (int)lua_tonumber(L, 2);
	freyjaMaterialBlendDestination(material, factor);
	return 0;
}


int lua_freyjaPakBegin(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char* filename = lua_tostring(L, 1);
	freyjaPakBegin(filename);
	return 0;
}


int lua_freyjaPakAddDecoderFunction2s(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	const char* module = lua_tostring(L, 1);
	const char* symbol = lua_tostring(L, 2);
	freyjaPakAddDecoderFunction2s(module, symbol);
	return 0;
}


int lua_freyjaPakAddFullPathFile(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t pakIndex = (int)lua_tonumber(L, 1);
	const char* vfsFilename = lua_tostring(L, 2);
	int32 offset = (int)lua_tonumber(L, 3);
	int32 size = (int)lua_tonumber(L, 4);
	freyjaPakAddFullPathFile(pakIndex, vfsFilename, offset, size);
	return 0;
}


int lua_freyjaPakAddFullPathFileXOR(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t pakIndex = (int)lua_tonumber(L, 1);
	byte key = (int)lua_tonumber(L, 2);
	const char* vfsFilename = lua_tostring(L, 3);
	int32 offset = (int)lua_tonumber(L, 4);
	int32 size = (int)lua_tonumber(L, 5);
	freyjaPakAddFullPathFileXOR(pakIndex, key, vfsFilename, offset, size);
	return 0;
}


int lua_freyjaPakEnd(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	index_t pakIndex = (int)lua_tonumber(L, 1);
	freyjaPakEnd(pakIndex);
	return 0;
}


int lua_freyjaMeshCreate(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char* name = lua_tostring(L, 1);
	freyjaMeshCreate(name);
	return 0;
}


int lua_freyjaMeshDelete(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	freyjaMeshDelete(mesh);
	return 0;
}


int lua_freyjaMeshGetVertex(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	freyja_id vertex = (int)lua_tonumber(L, 2);
	freyja_ptr userdata_value = freyjaMeshGetVertex(mesh, vertex);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshGetFace(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	freyja_id face = (int)lua_tonumber(L, 2);
	freyja_ptr userdata_value = freyjaMeshGetFace(mesh, face);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshGetWeight(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	freyja_id weight = (int)lua_tonumber(L, 2);
	freyja_ptr userdata_value = freyjaMeshGetWeight(mesh, weight);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshGetWeightCount(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	int integer_value = freyjaMeshGetWeightCount(mesh);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaMeshSetMaterial(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr mesh = lua_touserdata(L, 1);
	freyja_ptr material = lua_touserdata(L, 2);
	freyjaMeshSetMaterial(mesh, material);
	return 0;
}


int lua_freyjaMeshSetPosition(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	freyja_ptr meshIndex = lua_touserdata(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaMeshSetPosition(meshIndex, xyz);
	return 0;
}


int lua_freyjaMeshCreateSheet(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 6 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t size = lua_tonumber(L, 4);
	uint32 rows = (int)lua_tonumber(L, 5);
	uint32 columns = (int)lua_tonumber(L, 6);
	freyja_ptr userdata_value = freyjaMeshCreateSheet(origin, size, rows, columns);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateLattice(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 7 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t size = lua_tonumber(L, 4);
	uint32 rows = (int)lua_tonumber(L, 5);
	uint32 columns = (int)lua_tonumber(L, 6);
	uint32 layers = (int)lua_tonumber(L, 7);
	freyja_ptr userdata_value = freyjaMeshCreateLattice(origin, size, rows, columns, layers);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateCube(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t size = lua_tonumber(L, 4);
	freyja_ptr userdata_value = freyjaMeshCreateCube(origin, size);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateRing(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 6 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t radius = lua_tonumber(L, 4);
	uint32 count = (int)lua_tonumber(L, 5);
	uint32 rings = (int)lua_tonumber(L, 6);
	freyja_ptr userdata_value = freyjaMeshCreateRing(origin, radius, count, rings);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateCircle(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t radius = lua_tonumber(L, 4);
	uint32 count = (int)lua_tonumber(L, 5);
	freyja_ptr userdata_value = freyjaMeshCreateCircle(origin, radius, count);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateCone(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 6 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t height = lua_tonumber(L, 4);
	vec_t radius = lua_tonumber(L, 5);
	uint32 wedges = (int)lua_tonumber(L, 6);
	freyja_ptr userdata_value = freyjaMeshCreateCone(origin, height, radius, wedges);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateCylinder(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 7 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t height = lua_tonumber(L, 4);
	vec_t radius = lua_tonumber(L, 5);
	uint32 sides = (int)lua_tonumber(L, 6);
	uint32 rings = (int)lua_tonumber(L, 7);
	freyja_ptr userdata_value = freyjaMeshCreateCylinder(origin, height, radius, sides, rings);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateSphere(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 6 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t radius = lua_tonumber(L, 4);
	uint32 sides = (int)lua_tonumber(L, 5);
	uint32 rings = (int)lua_tonumber(L, 6);
	freyja_ptr userdata_value = freyjaMeshCreateSphere(origin, radius, sides, rings);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaMeshCreateTube(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 7 )
		return 0;

	vec3_t origin;
	origin[0] = lua_tonumber(L, 1);
	origin[1] = lua_tonumber(L, 2);
	origin[2] = lua_tonumber(L, 3);
	vec_t height = lua_tonumber(L, 4);
	vec_t radius = lua_tonumber(L, 5);
	uint32 sides = (int)lua_tonumber(L, 6);
	uint32 rings = (int)lua_tonumber(L, 7);
	freyja_ptr userdata_value = freyjaMeshCreateTube(origin, height, radius, sides, rings);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaCameraCreate(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char* name = lua_tostring(L, 1);
	freyjaCameraCreate(name);
	return 0;
}


int lua_freyjaCameraDelete(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr camera = lua_touserdata(L, 1);
	freyjaCameraDelete(camera);
	return 0;
}


int lua_freyjaCameraPos3f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec_t x = lua_tonumber(L, 2);
	vec_t y = lua_tonumber(L, 3);
	vec_t z = lua_tonumber(L, 4);
	freyjaCameraPos3f(cameraIndex, x, y, z);
	return 0;
}


int lua_freyjaCameraPos3fv(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaCameraPos3fv(cameraIndex, xyz);
	return 0;
}


int lua_freyjaCameraTarget3f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec_t x = lua_tonumber(L, 2);
	vec_t y = lua_tonumber(L, 3);
	vec_t z = lua_tonumber(L, 4);
	freyjaCameraTarget3f(cameraIndex, x, y, z);
	return 0;
}


int lua_freyjaCameraTarget3fv(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaCameraTarget3fv(cameraIndex, xyz);
	return 0;
}


int lua_freyjaCameraUp3f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec_t x = lua_tonumber(L, 2);
	vec_t y = lua_tonumber(L, 3);
	vec_t z = lua_tonumber(L, 4);
	freyjaCameraUp3f(cameraIndex, x, y, z);
	return 0;
}


int lua_freyjaCameraUp3fv(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaCameraUp3fv(cameraIndex, xyz);
	return 0;
}


int lua_freyjaCameraRotQuat4f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec_t w = lua_tonumber(L, 2);
	vec_t x = lua_tonumber(L, 3);
	vec_t y = lua_tonumber(L, 4);
	vec_t z = lua_tonumber(L, 5);
	freyjaCameraRotQuat4f(cameraIndex, w, x, y, z);
	return 0;
}


int lua_freyjaCameraRotQuat4fv(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	index_t cameraIndex = (int)lua_tonumber(L, 1);
	vec4_t wxyz;
	wxyz[0] = lua_tonumber(L, 2);
	wxyz[1] = lua_tonumber(L, 3);
	wxyz[2] = lua_tonumber(L, 4);
	wxyz[3] = lua_tonumber(L, 5);
	freyjaCameraRotQuat4fv(cameraIndex, wxyz);
	return 0;
}


int lua_freyjaNodeDuplicate(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	freyjaNodeDuplicate(node);
	return 0;
}


int lua_freyjaNodeGetFlags(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	int integer_value = freyjaNodeGetFlags(node);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaNodeGetName(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	freyjaNodeGetName(node);
	return 0;
}


int lua_freyjaNodeGetParent(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	freyja_ptr userdata_value = freyjaNodeGetParent(node);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaNodeGetPosition(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaNodeGetPosition(node, xyz);
	return 0;
}


int lua_freyjaNodeIsSelected(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	int integer_value = freyjaNodeIsSelected(node);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaNodeSetFlags(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	byte flags = (int)lua_tonumber(L, 2);
	freyjaNodeSetFlags(node, flags);
	return 0;
}


int lua_freyjaNodeSetName(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	const char* name = lua_tostring(L, 2);
	freyjaNodeSetName(node, name);
	return 0;
}


int lua_freyjaNodeSetParent(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	freyja_ptr parent = lua_touserdata(L, 2);
	freyjaNodeSetParent(node, parent);
	return 0;
}


int lua_freyjaNodeSetPosition3fv(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	vec3_t xyz;
	xyz[0] = lua_tonumber(L, 2);
	xyz[1] = lua_tonumber(L, 3);
	xyz[2] = lua_tonumber(L, 4);
	freyjaNodeSetPosition3fv(node, xyz);
	return 0;
}


int lua_freyjaNodeSetPosition3f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	vec_t x = lua_tonumber(L, 2);
	vec_t y = lua_tonumber(L, 3);
	vec_t z = lua_tonumber(L, 4);
	freyjaNodeSetPosition3f(node, x, y, z);
	return 0;
}


int lua_freyjaNodeSetOrientationQuat4f(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 5 )
		return 0;

	freyja_ptr node = lua_touserdata(L, 1);
	vec_t w = lua_tonumber(L, 2);
	vec_t x = lua_tonumber(L, 3);
	vec_t y = lua_tonumber(L, 4);
	vec_t z = lua_tonumber(L, 5);
	freyjaNodeSetOrientationQuat4f(node, w, x, y, z);
	return 0;
}


int lua_freyjaSceneNodeGetBoundingBox(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 7 )
		return 0;

	freyja_ptr scenenode = lua_touserdata(L, 1);
	vec3_t min;
	min[0] = lua_tonumber(L, 2);
	min[1] = lua_tonumber(L, 3);
	min[2] = lua_tonumber(L, 4);
	vec3_t max;
	max[0] = lua_tonumber(L, 5);
	max[1] = lua_tonumber(L, 6);
	max[2] = lua_tonumber(L, 7);
	freyjaSceneNodeGetBoundingBox(scenenode, min, max);
	return 0;
}


int lua_freyjaSceneNodeGetBoundingSphereCenter(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 4 )
		return 0;

	freyja_ptr scenenode = lua_touserdata(L, 1);
	vec3_t center;
	center[0] = lua_tonumber(L, 2);
	center[1] = lua_tonumber(L, 3);
	center[2] = lua_tonumber(L, 4);
	freyjaSceneNodeGetBoundingSphereCenter(scenenode, center);
	return 0;
}


int lua_freyjaSceneNodeGetBoundingSphereRadius(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr scenenode = lua_touserdata(L, 1);
	vec_t real_value = freyjaSceneNodeGetBoundingSphereRadius(scenenode);
	lua_pushnumber(L, real_value);
	return 1;
}


int lua_freyjaSceneAddNode(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr scene = lua_touserdata(L, 1);
	freyja_ptr node = lua_touserdata(L, 2);
	freyjaSceneAddNode(scene, node);
	return 0;
}


int lua_freyjaSceneGetNode(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	freyja_ptr scene = lua_touserdata(L, 1);
	freyja_id node = (int)lua_tonumber(L, 2);
	freyja_ptr userdata_value = freyjaSceneGetNode(scene, node);
	lua_pushlightuserdata(L, userdata_value);
	return 1;
}


int lua_freyjaSceneGetSkeletonCount(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr scene = lua_touserdata(L, 1);
	int integer_value = freyjaSceneGetSkeletonCount(scene);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaSceneGetMeshCount(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr scene = lua_touserdata(L, 1);
	int integer_value = freyjaSceneGetMeshCount(scene);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaSceneGetPixelbufferCount(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr scene = lua_touserdata(L, 1);
	int integer_value = freyjaSceneGetPixelbufferCount(scene);
	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaFileOpen(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 2 )
		return 0;

	const char* filename = lua_tostring(L, 1);
	const char* mode = lua_tostring(L, 2);
	freyjaFileOpen(filename, mode);
	return 0;
}


int lua_freyjaFileClose(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	freyja_ptr file = lua_touserdata(L, 1);
	freyjaFileClose(file);
	return 0;
}


// Query hooks
extern "C" {	
	index_t freyjaQueryBegin();
	void freyjaQueryEnd(index_t query);
	void freyjaQueryInteger(index_t query, const char *symbol, int32 *i);
	void freyjaQueryFloat(index_t query, const char *symbol, vec_t *r);
	void freyjaQueryString(index_t query, const char *symbol, const char **s);
}


int lua_freyjaQueryInteger(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char *symbol = lua_tostring(L, 1);

	int integer_value;
	index_t q = freyjaQueryBegin();
	freyjaQueryInteger(q, symbol, &integer_value);
	freyjaQueryEnd(q);

	lua_pushnumber(L, integer_value);
	return 1;
}


int lua_freyjaQueryFloat(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char *symbol = lua_tostring(L, 1);

	float real_value;
	index_t q = freyjaQueryBegin();
	freyjaQueryFloat(q, symbol, &real_value);
	freyjaQueryEnd(q);

	lua_pushnumber(L, real_value);
	return 1;
}


int lua_freyjaQueryString(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char *symbol = lua_tostring(L, 1);

	const char *string_value;
	index_t q = freyjaQueryBegin();
	freyjaQueryString(q, symbol, &string_value);
	freyjaQueryEnd(q);

	lua_pushstring(L, string_value);
	return 1;
}



int lua_freyjaPrintMessage(lua_State *L)
{
	int stack_count = lua_gettop(L);
	if ( stack_count < 1 )
		return 0;

	const char *msg = lua_tostring(L, 1);
	freyjaPrintMessage(msg);
	return 0;
}


typedef struct {
	const char *symbol;
	int (*func)(lua_State*);
} lua_freyja_bind_t;


lua_freyja_bind_t gLibFreyja_LuaBinds[] = {
	{ "freyjaSkeletonCreate", lua_freyjaSkeletonCreate },
	{ "freyjaSkeletonDelete", lua_freyjaSkeletonDelete },
	{ "freyjaSkeletonGetBone", lua_freyjaSkeletonGetBone },
	{ "freyjaSkeletonGetBoneCount", lua_freyjaSkeletonGetBoneCount },
	{ "freyjaSkeletonGetRoot", lua_freyjaSkeletonGetRoot },
	{ "freyjaSkeletonSetRoot", lua_freyjaSkeletonSetRoot },
	{ "freyjaBoneCreate", lua_freyjaBoneCreate },
	{ "freyjaBoneDelete", lua_freyjaBoneDelete },
	{ "freyjaLightCreate", lua_freyjaLightCreate },
	{ "freyjaLightDelete", lua_freyjaLightDelete },
	{ "freyjaLightPosition4v", lua_freyjaLightPosition4v },
	{ "freyjaLightAmbient", lua_freyjaLightAmbient },
	{ "freyjaLightDiffuse", lua_freyjaLightDiffuse },
	{ "freyjaLightSpecular", lua_freyjaLightSpecular },
	{ "freyjaMaterialCreate", lua_freyjaMaterialCreate },
	{ "freyjaMaterialClearAll", lua_freyjaMaterialClearAll },
	{ "freyjaMaterialName", lua_freyjaMaterialName },
	{ "freyjaMaterialClearFlag", lua_freyjaMaterialClearFlag },
	{ "freyjaMaterialSetFlag", lua_freyjaMaterialSetFlag },
	{ "freyjaMaterialFlags", lua_freyjaMaterialFlags },
	{ "freyjaMaterialShaderName", lua_freyjaMaterialShaderName },
	{ "freyjaMaterialTextureName", lua_freyjaMaterialTextureName },
	{ "freyjaMaterialShader", lua_freyjaMaterialShader },
	{ "freyjaMaterialTexture", lua_freyjaMaterialTexture },
	{ "freyjaMaterialAmbient", lua_freyjaMaterialAmbient },
	{ "freyjaMaterialDiffuse", lua_freyjaMaterialDiffuse },
	{ "freyjaMaterialSpecular", lua_freyjaMaterialSpecular },
	{ "freyjaMaterialEmissive", lua_freyjaMaterialEmissive },
	{ "freyjaMaterialShininess", lua_freyjaMaterialShininess },
	{ "freyjaMaterialTransparency", lua_freyjaMaterialTransparency },
	{ "freyjaMaterialBlendSource", lua_freyjaMaterialBlendSource },
	{ "freyjaMaterialBlendDestination", lua_freyjaMaterialBlendDestination },
	{ "freyjaPakBegin", lua_freyjaPakBegin },
	{ "freyjaPakAddDecoderFunction2s", lua_freyjaPakAddDecoderFunction2s },
	{ "freyjaPakAddFullPathFile", lua_freyjaPakAddFullPathFile },
	{ "freyjaPakAddFullPathFileXOR", lua_freyjaPakAddFullPathFileXOR },
	{ "freyjaPakEnd", lua_freyjaPakEnd },
	{ "freyjaMeshCreate", lua_freyjaMeshCreate },
	{ "freyjaMeshDelete", lua_freyjaMeshDelete },
	{ "freyjaMeshGetVertex", lua_freyjaMeshGetVertex },
	{ "freyjaMeshGetFace", lua_freyjaMeshGetFace },
	{ "freyjaMeshGetWeight", lua_freyjaMeshGetWeight },
	{ "freyjaMeshGetWeightCount", lua_freyjaMeshGetWeightCount },
	{ "freyjaMeshSetMaterial", lua_freyjaMeshSetMaterial },
	{ "freyjaMeshSetPosition", lua_freyjaMeshSetPosition },
	{ "freyjaMeshCreateSheet", lua_freyjaMeshCreateSheet },
	{ "freyjaMeshCreateLattice", lua_freyjaMeshCreateLattice },
	{ "freyjaMeshCreateCube", lua_freyjaMeshCreateCube },
	{ "freyjaMeshCreateRing", lua_freyjaMeshCreateRing },
	{ "freyjaMeshCreateCircle", lua_freyjaMeshCreateCircle },
	{ "freyjaMeshCreateCone", lua_freyjaMeshCreateCone },
	{ "freyjaMeshCreateCylinder", lua_freyjaMeshCreateCylinder },
	{ "freyjaMeshCreateSphere", lua_freyjaMeshCreateSphere },
	{ "freyjaMeshCreateTube", lua_freyjaMeshCreateTube },
	{ "freyjaCameraCreate", lua_freyjaCameraCreate },
	{ "freyjaCameraDelete", lua_freyjaCameraDelete },
	{ "freyjaCameraPos3f", lua_freyjaCameraPos3f },
	{ "freyjaCameraPos3fv", lua_freyjaCameraPos3fv },
	{ "freyjaCameraTarget3f", lua_freyjaCameraTarget3f },
	{ "freyjaCameraTarget3fv", lua_freyjaCameraTarget3fv },
	{ "freyjaCameraUp3f", lua_freyjaCameraUp3f },
	{ "freyjaCameraUp3fv", lua_freyjaCameraUp3fv },
	{ "freyjaCameraRotQuat4f", lua_freyjaCameraRotQuat4f },
	{ "freyjaCameraRotQuat4fv", lua_freyjaCameraRotQuat4fv },
	{ "freyjaNodeDuplicate", lua_freyjaNodeDuplicate },
	{ "freyjaNodeGetFlags", lua_freyjaNodeGetFlags },
	{ "freyjaNodeGetName", lua_freyjaNodeGetName },
	{ "freyjaNodeGetParent", lua_freyjaNodeGetParent },
	{ "freyjaNodeGetPosition", lua_freyjaNodeGetPosition },
	{ "freyjaNodeIsSelected", lua_freyjaNodeIsSelected },
	{ "freyjaNodeSetFlags", lua_freyjaNodeSetFlags },
	{ "freyjaNodeSetName", lua_freyjaNodeSetName },
	{ "freyjaNodeSetParent", lua_freyjaNodeSetParent },
	{ "freyjaNodeSetPosition3fv", lua_freyjaNodeSetPosition3fv },
	{ "freyjaNodeSetPosition3f", lua_freyjaNodeSetPosition3f },
	{ "freyjaNodeSetOrientationQuat4f", lua_freyjaNodeSetOrientationQuat4f },
	{ "freyjaSceneNodeGetBoundingBox", lua_freyjaSceneNodeGetBoundingBox },
	{ "freyjaSceneNodeGetBoundingSphereCenter", lua_freyjaSceneNodeGetBoundingSphereCenter },
	{ "freyjaSceneNodeGetBoundingSphereRadius", lua_freyjaSceneNodeGetBoundingSphereRadius },
	{ "freyjaSceneAddNode", lua_freyjaSceneAddNode },
	{ "freyjaSceneGetNode", lua_freyjaSceneGetNode },
	{ "freyjaSceneGetSkeletonCount", lua_freyjaSceneGetSkeletonCount },
	{ "freyjaSceneGetMeshCount", lua_freyjaSceneGetMeshCount },
	{ "freyjaSceneGetPixelbufferCount", lua_freyjaSceneGetPixelbufferCount },
	{ "freyjaFileOpen", lua_freyjaFileOpen },
	{ "freyjaFileClose", lua_freyjaFileClose },
	{ "freyjaQueryInteger", lua_freyjaQueryInteger },
	{ NULL, NULL }
};


#ifdef LUAWRAPPER_FOUND
void freyja_lua_register_functions( lua_State* state )
{
	static bool init = false;
	
	if ( !init )
	{
		init = true;
		lua_freyja_bind_t *cur;
		unsigned int i = 0;
		
		while ( (cur = &gLibFreyja_LuaBinds[i++]) && cur->symbol )
		{
			lua_register( state, cur->symbol, cur->func );
		}
	}
}
#endif // LUAWRAPPER_FOUND

#endif // LUAWRAPPER_FOUND
