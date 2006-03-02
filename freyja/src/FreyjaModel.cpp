/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : FreyjaModel
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000.09.09:
 * Mongoose - Created
 ==========================================================================*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <freyja/FreyjaFileReader.h> 
#include <freyja/BoneABI.h> 
#include <mgtk/ResourceEvent.h>

#include "freyja_events.h"
#include "Texture.h"
#include "FreyjaModel.h"

Freyja3dCursor gFreyjaCursor;
BezierPatch FreyjaModel::gTestPatch;
unsigned int FreyjaModel::mFlags = 0;
FreyjaModel *gFreyjaModel = 0x0;

extern void freyja__setPrinter(FreyjaPrinter *printer, bool freyjaManaged);

// FIXME: Add set/clear methods to replace old get/set flags crap



void eTextureSlotLoadToggle()
{
	bool on = FreyjaModel::toggleFlag(FreyjaModel::fLoadTextureInSlot);

	freyja_print("Texture loading into current slot [%s]",
				on ? "on" : "off");
}

void eMaterialSlotLoadToggle()
{
	bool on = FreyjaModel::toggleFlag(FreyjaModel::fLoadMaterialInSlot);

	freyja_print("Texture loading into current slot [%s]",
				on ? "on" : "off");
}


void eModelUpload(char *filename)
{
	gFreyjaModel->loadModel(filename);
}


void eCurrentFaceFlagAlpha()
{
	freyjaPolygonFlagAlpha(gFreyjaModel->getCurrentPolygon(), 1);
}


void eTextureUpload(unsigned int id)
{
	byte *image;
	uint32 w, h, bpp, type;

	/* Texture image was stored as raw buffer */
	freyjaGetTextureImage(id, &w, &h, &bpp, &type, &image);
	freyja_print("!test");

	if (image)
	{
		switch (type)
		{
		case RGBA_32:
			gFreyjaModel->loadTextureBuffer(image, w, h, 32, Texture::RGBA);
			break;

		case RGB_24:
			gFreyjaModel->loadTextureBuffer(image, w, h, 24, Texture::RGB);
			break;

		case INDEXED_8:
			gFreyjaModel->loadTextureBuffer(image, w, h, 8, Texture::INDEXED);
			break;

		default:
			freyja_print("%s> ERROR: Unsupported texture colormap %d",
						"FreyjaModel::loadModel", type);
		}
	}
}


void FreyjaModelEventsAttach()
{
	ResourceEventCallbackString::add("eModelUpload", &eModelUpload);
	ResourceEventCallback::add("eTextureSlotLoadToggle", &eTextureSlotLoadToggle);
	ResourceEventCallback::add("eMaterialSlotLoadToggle", &eMaterialSlotLoadToggle);
	ResourceEventCallback::add("eCurrentFaceFlagAlpha", &eCurrentFaceFlagAlpha);
	ResourceEventCallbackUInt::add("eTextureUpload", &eTextureUpload);
}


/////////////////////////////////////////////////


FreyjaModel::FreyjaModel()
{
	/* Search ~/.freyja/plugins/ first */
	char *pluginDir = freyja_rc_map("plugins/");
	freyjaPluginAddDirectory(pluginDir);
	delete [] pluginDir;

	/* Start up freyja backend */
	freyjaSpawn();
	freyja__setPrinter(&mPrinter, false);

	/* Spawn 0th light */
	freyjaLightCreate();

	/* Spawn 0th material */
	int32 mIndex = freyjaMaterialCreate();
	freyjaCurrentMaterial(mIndex);

	vec4_t rgba = {0,0,0,1};

	freyjaMaterialName(mIndex, "Boring default");
	freyjaMaterialSpecular(mIndex, rgba);
	freyjaMaterialEmissive(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.9;
	freyjaMaterialDiffuse(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2;
	freyjaMaterialAmbient(mIndex, rgba);
	freyjaMaterialShininess(mIndex, 0.0f);

	gFreyjaModel = this;

	/* Initalize/reset private data members */
	clear();
}


FreyjaModel::~FreyjaModel()
{
	freyjaFree();
}


bool FreyjaModel::toggleFlag(option_flag_t flag)
{
	if (FreyjaModel::getFlags() & flag)
	{
		FreyjaModel::setFlags(flag, 0);
	}
	else
	{
		FreyjaModel::setFlags(flag, 1);
	}

	return (getFlags() & flag);
}


void FreyjaModel::setFlags(option_flag_t flag, int op)
{
	mFlags |= flag;

	if (!op)
		mFlags ^= flag;

	// Pretty output in freyja console
	if (flag & FL_DUMP_TEXTURE)
		freyja_print("Texture dumping is %s", op ? "on" : "off");
}


unsigned int FreyjaModel::getFlags()
{
	return mFlags;
}


void FreyjaModel::clear()
{
	mCachedVertexIndex = -1;
	mFlags = 0;
	mEdgeCount = 3;
	mTextureIndex = 0;
	mCurrentPlane = PLANE_XY;
	mTexCoordIndex = 0;
	mVertexIndex = 0;
	mGroupIndex = 0;
	mMeshIndex = 0;
	mBoneIndex = 0;
	mSkeletalFrameIndex = 0;
	mVertexFrameIndex = 0;
	mAnimationFrameIndex = 0;
	mPolygonIndex = 0;
	mSelectionBoxOrdering = 0;

	gFreyjaCursor.Reset();

	mScroll[0] = 0.0f;
	mScroll[1] = -18.0f;
	mScroll[2] = 0.0f;
	
	mSelectBBox[0][0] = mSelectBBox[0][1] = mSelectBBox[0][2] = -16.0;
	mSelectBBox[1][0] = mSelectBBox[1][1] = mSelectBBox[1][2] = 16.0;

	freyjaModelClear(0); // Only 1 edit model supported in this bulid
}


void FreyjaModel::printInfo()
{
	freyja_print("%d bones, %d meshes, %d polygons, %d vertices",
				freyjaGetCount(FREYJA_BONE), 
				freyjaGetCount(FREYJA_MESH), 
				freyjaGetCount(FREYJA_POLYGON), 
				freyjaGetCount(FREYJA_VERTEX));
}


// FIXME Move to libfreyja in fact a lot of these methods should be
// FIXME: Move to libfreyja?



void FreyjaModel::setPolygonMaterial(long polygonIndex, long material)
{
	// HACKY entry
	uint32 i;

	if (!mUVMap.empty())
	{
		for (i = mUVMap.begin(); i < mUVMap.end(); ++i)
		{
			freyjaPolygonSetMaterial1i(mUVMap[i], material);
		}

		return;
	}

	freyjaPolygonSetMaterial1i(polygonIndex, material);
}


void FreyjaModel::setCurrentPlane(freyja_plane_t p)
{
	mCurrentPlane = p;
}


freyja_plane_t FreyjaModel::getCurrentPlane()
{
	return mCurrentPlane;
}

unsigned int FreyjaModel::getCurrentMesh()
{
	return mMeshIndex;
}

unsigned int FreyjaModel::getCurrentPolygon()
{
	return mPolygonIndex;
}

unsigned int FreyjaModel::getCurrentGroup()
{
	return mGroupIndex;
}

unsigned int FreyjaModel::getCurrentSkeleton()
{
	return mSkeletalFrameIndex;
}

unsigned int FreyjaModel::getCurrentBone()
{
	return mBoneIndex;
}

unsigned int FreyjaModel::getCurrentTexCoord()
{
	return mTexCoordIndex;
}

unsigned int FreyjaModel::getCurrentVertex()
{
	return mVertexIndex;
}

unsigned int FreyjaModel::getCurrentVertexFrame()
{
	freyja_print("FreyjaModel::getCurrentVertexFrame> Not Implemented %s:%d\n",
				 __FILE__, __LINE__);
	return 0;
}

unsigned int FreyjaModel::getCurrentPolygonEdgeCount()
{
	return mEdgeCount;
}

unsigned int FreyjaModel::getCurrentTextureIndex()
{
	return mTextureIndex;
}

unsigned int FreyjaModel::getCurrentAnimationFrame()
{
	return mVertexFrameIndex;
}

unsigned int FreyjaModel::getCurrentAnimation()
{
	return mAnimationFrameIndex;
}


void FreyjaModel::setCurrentMesh(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_MESH) && freyjaIsMeshAllocated(index))
	{
		mMeshIndex = index;		
		mGroupIndex = freyjaGetMeshVertexGroupIndex(mMeshIndex, 0);
	}
}


void FreyjaModel::setCurrentPolygon(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_POLYGON))
	{
		mPolygonIndex = index;
	}
}


void FreyjaModel::setCurrentGroup(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_VERTEX_GROUP))
	{
		mGroupIndex = index;
		freyja_print("Group[%i]", getCurrentGroup());
	}
}

void FreyjaModel::setCurrentSkeleton(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_SKELETON))
	{
		mSkeletalFrameIndex = index;
	}
}


void FreyjaModel::setCurrentBone(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_BONE))
	{
		mBoneIndex = index;
		freyja_print("Bone Tag[%i]", getCurrentBone());
	}
}


void FreyjaModel::setCurrentTexCoord(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_TEXCOORD))
	{
		mTexCoordIndex = index;
	}
}


void FreyjaModel::setCurrentVertex(unsigned int index)
{
	if (index < freyjaGetCount(FREYJA_VERTEX))
	{
		freyjaCurrentVertex(index);
		mVertexIndex = index;
	}
}


void FreyjaModel::setCurrentVertexFrame(unsigned int index)
{
	freyja_print("FreyjaModel::setCurrentVertexFrame> Not Implemented %s:%d\n",
				 __FILE__, __LINE__);
}


unsigned int FreyjaModel::getAnimationFrameCount(unsigned int index)
{
	freyja_print("FreyjaModel::getAnimationFrameCount> Not Implemented %s:%d\n",
				 __FILE__, __LINE__);

	return 0;
}


void FreyjaModel::setCurrentAnimation(unsigned int index)
{
	freyja_print("FreyjaModel::setCurrentAnimation> Not Implemented %s:%d\n",
				 __FILE__, __LINE__);

	//index;
}


void FreyjaModel::setCurrentAnimationFrame(unsigned int index)
{
	freyja_print("FreyjaModel::setCurrentAnimationFrame> Not Implemented %s:%d\n",
				 __FILE__, __LINE__);

	//index;
}


int FreyjaModel::saveAnimation(char const *filename)
{
	char filename2[512];

	snprintf(filename2, 512, "%s-keyframe.smd", filename);
	return saveModel(filename2);
}


void FreyjaModel::setCurrentPolygonEdgeCount(unsigned int count)
{
	if (count < 3)
	{
		mEdgeCount = 3;
	}
	else if (count > 6) // New 6-side limit enforced
	{
		mEdgeCount = 6;
	}
	else
	{ 
		mEdgeCount = count;
	}
}


void FreyjaModel::setCurrentTextureIndex(unsigned int index)
{
	mTextureIndex = index;
	freyja_print("Texture[%i]", getCurrentTextureIndex());
}



///////////////////////////////////////////////////////////////////////////////

void FreyjaModel::getBoneTranslation(float *x, float *y, float *z)
{
	vec3_t xyz;

	freyjaGetBoneTranslation3fv(getCurrentBone(), xyz);
	*x = xyz[0];
	*y = xyz[1];
	*z = xyz[2];
}


void FreyjaModel::setBoneTranslation(float x, float y, float z)
{
	freyjaBoneTranslate3f(getCurrentBone(), x, y, z);
}


void FreyjaModel::getBoneRotation(float *x, float *y, float *z)
{
	vec3_t xyz;

	freyjaGetBoneRotationEuler3fv(getCurrentBone(), xyz);
	*x = HEL_RAD_TO_DEG(xyz[0]);
	*y = HEL_RAD_TO_DEG(xyz[1]);
	*z = HEL_RAD_TO_DEG(xyz[2]);
	freyja_print("%f %f %f", xyz[0], xyz[1], xyz[2]);
}


void FreyjaModel::setBoneRotation(float x, float y, float z)
{
	vec3_t xyz = {HEL_DEG_TO_RAD(x), HEL_DEG_TO_RAD(y), HEL_DEG_TO_RAD(z)};

	freyjaBoneRotateEuler3fv(getCurrentBone(), xyz);
}


void FreyjaModel::transform(int mode, freyja_transform_action_t action, 
							float x, float y, float z)
{
	switch (mode)
	{
	case FreyjaModel::TransformVertexFrame:	
		freyjaMeshFrameTransform(getCurrentMesh(), getCurrentGroup(),
									action, x, y, z);
		break;


	case FreyjaModel::TransformMesh:
		freyjaMeshTransform(getCurrentMesh(), getCurrentGroup(),
							action, x, y, z);
		break;


	case FreyjaModel::TransformBone:
		freyjaBoneTransform(getCurrentBone(), action, x, y, z);
		break;


	case TransformSelectedVertices:
		freyjaVertexListTransform(mList, action, x, y, z);
		break;


	case FreyjaModel::TransformScene:
		freyjaModelTransform(0, action, x, y, z);
		break;
	}
}


void FreyjaModel::setSceneTranslation(vec_t x, vec_t y, vec_t z)
{
	mScroll[0] = x;
	mScroll[1] = y;
	mScroll[2] = z;
}

void FreyjaModel::adjustSceneTranslation(vec_t x, vec_t y, vec_t z)
{
	mScroll[0] += x;
	mScroll[1] += y;
	mScroll[2] += z;
}

void FreyjaModel::getSceneTranslation(vec3_t scroll)
{
	scroll[0] = mScroll[0];
	scroll[1] = mScroll[1];
	scroll[2] = mScroll[2];
}


//// BONES /////////////////////////////////////////

unsigned int FreyjaModel::newBone(float x, float y, float z, unsigned char flag)
{
	index_t skeletonIndex, boneIndex;


	if (freyjaGetSkeletonCount() == 0)
	{
		skeletonIndex = freyjaSkeletonCreate();
		freyjaCurrentSkeleton(skeletonIndex);
		freyjaPrintMessage("No skeleton found, creating one...");
	}

	if (freyjaGetCurrentSkeleton() == INDEX_INVALID)
		freyjaCurrentSkeleton(0);

	skeletonIndex = freyjaGetCurrentSkeleton();
	boneIndex = freyjaBoneCreate(skeletonIndex);
	freyjaSkeletonAddBone(skeletonIndex, boneIndex);

	if (boneIndex == 0) // FIXME no root handling for 'dancer' skel yet
	{
		freyjaBoneName(boneIndex, "root");
	}
	else
	{
		char name[64];

		snprintf(name, 64, "bone %i", boneIndex);
		freyjaBoneName(boneIndex, name);
	}

	updateSkeletalUI();

	return boneIndex;
}


void FreyjaModel::addMeshToBone(unsigned int tag, unsigned int mesh)
{
	freyjaBoneAddMesh(getCurrentBone(), getCurrentMesh());
	updateSkeletalUI();
}


void FreyjaModel::removeMeshFromBone(unsigned int tag, unsigned int mesh)
{
	freyjaBoneRemoveMesh(getCurrentBone(), getCurrentMesh());
	updateSkeletalUI();
}


void FreyjaModel::selectBone(float xx, float yy)
{
	index_t boneIndex = getNearestBoneIndexInPlane(xx, yy, getCurrentPlane());

	if (boneIndex != INDEX_INVALID)
		setCurrentBone(boneIndex);
}

void FreyjaModel::connectBone(unsigned int master, unsigned int slave)
{
	freyjaBoneAddChild(master, slave);
	updateSkeletalUI();
}


void FreyjaModel::VertexBuffer(float xx, float yy)
{
	VertexSelect(xx, yy);

	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		freyja_print("Control> refused to use bad vertex in list");
		mList.clear();
	}
	else
	{
		mList.pushBack(mCachedVertexIndex);
	}
}


void FreyjaModel::PolygonSelectVertex(float xx, float yy)
{
	static Vector<uint32> vertices;


	VertexSelect(xx, yy);

	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not selecting polygon");
	}
	else
	{
		vertices.pushBack(mCachedVertexIndex);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> Polygon selected");

		mPolygonIndex = freyjaFindPolygonByVertices(vertices);
		vertices.clear();
	}
}


void FreyjaModel::PolygonDelVertex(float xx, float yy)
{
	static Vector<unsigned int> vertices;
	int32 polygonIndex;

	VertexSelect(xx, yy);

	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not removing polygon");
	}
	else
	{
		vertices.pushBack(mCachedVertexIndex);
	}

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> Polygon removed");

		polygonIndex = freyjaFindPolygonByVertices(vertices);
		freyjaPolygonDelete(polygonIndex);

		vertices.clear();
	}
}


void FreyjaModel::PolygonAddVertex(float xx, float yy)
{
	static Vector<uint32> vertices;
	static Vector<uint32> texcoords;
	int32 meshIndex, vertexIndex, polygonIndex;
	vec3_t pos;
	vec2_t uv;
	uint32 i;


	VertexSelect(xx, yy);
    
	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		vertices.clear();
		freyja_print("Control> Bad vertex, not building polygon");
	}
	else
	{
		vertices.pushBack(mCachedVertexIndex);
	}

	meshIndex = getCurrentMesh();

	if (vertices.size() == getCurrentPolygonEdgeCount())
	{
		freyja_print("Control> New Polygon made");

		/* Generate texcoords and record their ids, or set vertex UV */
		for (i = vertices.begin(); i < vertices.end(); ++i)
		{
			vertexIndex = vertices[i];

			if (freyjaIsVertexAllocated(vertexIndex))
			{
				/* A cheap planar like projection on XY plane */
				freyjaGetVertexXYZ3fv(vertexIndex, pos);
				freyjaGenerateUVFromXYZ(pos, uv+0, uv+1);
				freyjaVertexTexCoord2fv(vertexIndex, uv);
			}
			else
			{
				texcoords.pushBack(freyjaTexCoordCreate2f(0.5, 0.5));
			}
		}

		/* Allocate polygon */
		polygonIndex = freyjaPolygonCreate();
		freyjaPolygonSetMaterial1i(polygonIndex, 0);
		
		for (i = vertices.begin(); i < vertices.end(); ++i)
			freyjaPolygonAddVertex1i(polygonIndex, vertices[i]);

		for (i = texcoords.begin(); i < texcoords.end(); ++i)
			freyjaPolygonAddTexCoord1i(polygonIndex, texcoords[i]);

		/* Is it renderable/valid? */
		if (freyjaGetPolygonVertexCount(polygonIndex) != vertices.end())
		{
			printf("!FIXME: %s:%i\n", __FILE__, __LINE__);
			freyjaPolygonDelete(polygonIndex);
			return;
		}

		freyjaMeshAddPolygon(meshIndex, polygonIndex);

		texcoords.clear();
		vertices.clear();
	}
}


// FIXME: Generate a contiguous mesh sharing vertices and 
//        compose a single tristrip that reverses on edges
bool FreyjaModel::pasteSelectedPatch()
{
	Vector<unsigned int> transV, tmp;
	unsigned int index, i, n, cursor, divs = 7;


	gTestPatch.generatePolygonMesh(divs);

	freyjaBegin(FREYJA_MESH);
	freyjaMeshFlags1u(0x0);

	freyjaBegin(FREYJA_VERTEX_GROUP);
	freyjaGroupCenter3f(0, 0, 0);

	for (i = gTestPatch.vertices.begin(); i < gTestPatch.vertices.end(); ++i)
	{
		index = freyjaVertexCreate3fv(gTestPatch.vertices[i]->mVec);
		freyjaVertexNormal3f(index, 0, 1, 0);
		freyjaVertexTexCoord2fv(index, gTestPatch.texcoords[i]->mVec);
		transV.pushBack(index);
	}

	freyjaEnd(); // FREYJA_GROUP

	// FIXME: This is just a test case to get the facets right,
	//        notice some vertices will not be used that were created above
	for (i = 1, n = (divs + divs + 2) * (divs - 1); i < n; i += 2)
	{
		//freyja_print("! %i -> %i", i+divs+divs+1, i);
		tmp.pushBack(i+divs+divs+1);
		transV.assign(i+divs+divs+1, transV[i]);
	}

	//for (i = 0, n = tmp.end(); i < n; ++i)
	//	freyjaVertexDelete(tmp[i]);

	for (i = 0, cursor = divs + divs, n = (divs * (divs+1) * 2); i < n; ++i)
	{
		//if (i == 14 || i == 28 + 2 || i == 42 + 4) // Found for 7x7
		if (i == cursor)
		{
			i += 2;
			cursor += divs + divs + 2;
		}

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(gTestPatch.texture);
		freyjaPolygonVertex1i(transV[i]);
		freyjaPolygonVertex1i(transV[i+1]);
		freyjaPolygonVertex1i(transV[i+2]);
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH

	freyja_print("Mesh generated from patch...");

	return true;
}


bool FreyjaModel::pasteSelectedMesh()
{
	char c = freyjaModelPasteMesh(0);

	if (c == -1)
		return false;

	return c;
}


bool FreyjaModel::pasteVertexBuffer()
{
	char c = freyjaModelPasteMesh(0);

	if (c == -1)
		return false;

	return c;
}

	
bool FreyjaModel::copySelectedMesh()
{
	char c = freyjaModelCopyMesh(0, getCurrentMesh(), getCurrentGroup());

	if (c == -1)
		return false;

	return c;
}

	
bool FreyjaModel::copyVertexBuffer()
{
	char c = freyjaModelCopyVertexList(0, mList, getCurrentMesh(), getCurrentGroup());

	if (c == -1)
		return false;

	return c;
}


void FreyjaModel::cullUsingVertexBuffer()
{
	unsigned int i;


	if (mList.empty())
		return;

	for (i = mList.begin(); i < mList.end(); ++i)
	{
		freyjaVertexDelete(mList[i]);
	}	
}


void FreyjaModel::mirrorUsingVertexBuffer(bool x, bool y, bool z)
{
	unsigned int i;
	vec3_t *xyz;


	if (mList.empty())
		return;

	for (i = mList.begin(); i < mList.end(); ++i)
	{
		xyz = freyjaGetVertexXYZ(mList[i]);

		if (y)
			(*xyz)[0] = -(*xyz)[0];

		if (x)
			(*xyz)[1] = -(*xyz)[1];      

		if (z)
			(*xyz)[2] = -(*xyz)[2];      
	}
}


void FreyjaModel::MeshDel()
{
	freyja_print("Mesh[%u] deleted\n", mMeshIndex);
	freyjaMeshDelete(mMeshIndex);
}


void FreyjaModel::setNameBone(unsigned int boneIndex, const char *name)
{
	if (getFlags() & fDontUpdateBoneName)
		return;

	if (freyjaIsBoneAllocated(boneIndex) && name && name[1])
	{
		freyjaBoneName(boneIndex, name);
		freyja_print("bone[%i].name = '%s'", boneIndex, name);
		updateSkeletalUI();
	}
}


const char *FreyjaModel::getNameBone(unsigned int boneIndex)
{
	if (freyjaIsBoneAllocated(boneIndex))
	{
		return freyjaGetBoneNameString(boneIndex);
	}

	return 0x0;
}


void FreyjaModel::disconnectBone(unsigned int parent, unsigned int child)
{
	freyjaBoneRemoveChild(parent, child); 
	updateSkeletalUI();
}


void addVertexToBone(unsigned int bone, unsigned int vertex)
{
	freyjaVertexWeight(vertex, 1.0f, bone);
}


void FreyjaModel::removeVertexFromBone(unsigned int bone, unsigned int vertex)
{
	freyjaBoneRemoveVertex(bone, vertex);
}


void FreyjaModel::moveBoneCenter(float xx, float yy)
{
	uint32 boneIndex = getCurrentBone();
	vec3_t xyz;


	if (!freyjaIsBoneAllocated(boneIndex))
		return;

	freyjaGetBoneTranslation3fv(boneIndex, xyz);

	switch (getCurrentPlane())
	{
	case PLANE_XY:
		xyz[0] = xx;
		xyz[1] = yy;
		break;

	case PLANE_XZ:
		xyz[0] = xx;
		xyz[2] = yy;
		break;

	case PLANE_ZY: // side
		xyz[2] = xx;
		xyz[1] = yy;
		break;
	}

	freyjaBoneTranslate3fv(boneIndex, xyz);
}



void FreyjaModel::moveBone(float xx, float yy)
{
	uint32 boneIndex = getCurrentBone();
	vec_t x = 0, y = 0, z = 0;
	vec3_t xyz;


	if (!freyjaIsBoneAllocated(boneIndex))
		return;

	freyjaGetBoneTranslation3fv(boneIndex, xyz);

	switch (getCurrentPlane())
	{
	case PLANE_XY:
		x = xx - xyz[0];
		y = yy - xyz[1];
		z = 0;
		break;
	case PLANE_XZ:
		x = xx - xyz[0];
		y = 0;
		z = yy - xyz[2];
		break;
	case PLANE_ZY: //side
		x = 0;
		y = yy - xyz[1];
		z = xx - xyz[2];
		break;
	}
  
	transform(FreyjaModel::TransformBone, fTranslate, x, y, z);
}


void FreyjaModel::MeshMove(float xx, float yy)
{
	vec3_t xyz = {0, 0, 0};
	vec_t x = 0, y = 0, z = 0;


	freyjaGetMeshFrameCenter(getCurrentMesh(), getCurrentGroup(), xyz);

	switch (getCurrentPlane())
	{
	case PLANE_XY:
		x = xx - xyz[0];
		y = yy - xyz[1];
		z = 0;
		break;

	case PLANE_XZ:
		x = xx - xyz[0];
		y = 0;
		z = yy - xyz[2];
		break;

	case PLANE_ZY: // side
		x = 0;
		y = yy - xyz[1];
		z = xx - xyz[2];
		break;
	}

	transform(FreyjaModel::TransformMesh, fTranslate, x, y, z);
}


void FreyjaModel::getCurrentMeshCenter(vec3_t center)
{
	freyjaGetMeshFrameCenter(getCurrentMesh(), getCurrentGroup(), center);
}


void FreyjaModel::MeshMoveCenter(float xx, float yy)
{
	vec3_t center;


	freyjaGetMeshFrameCenter(getCurrentMesh(), getCurrentGroup(), center);

	switch (getCurrentPlane())
	{
	case PLANE_XY:
		center[0] = xx;
		center[1] = yy;
		break;

	case PLANE_XZ:
		center[0] = xx;
		center[2] = yy;
		break;

	case PLANE_ZY: // side
		center[2] = xx;
		center[1] = yy;
		break;
	}

	freyjaMeshFrameCenter(getCurrentMesh(), getCurrentGroup(), center);
}


void FreyjaModel::VertexNew(float xx, float yy)
{
	vec3_t pos = {0, 0, 0};
	vec2_t uv;


	if (freyjaGetMeshVertexGroupCount(getCurrentMesh()) == 0)
	{
		freyja_print("Point requires Mesh; Try Mesh->New");
		return;
	}

	switch (getCurrentPlane())
	{
	case PLANE_XY: // Front
		pos[0] = xx;
		pos[1] = yy;
		break;

	case PLANE_XZ: // Top
		pos[0] = xx;
		pos[2] = yy;
		break;

	case PLANE_ZY: // Side
		pos[1] = yy;
		pos[2] = xx;
		break;

	default:
		return;
	}

	mCachedVertexIndex = freyjaVertexCreate3fv(pos);

	if (freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		freyjaMeshVertexGroupAppendGobalVertex(getCurrentMesh(), 0, 
											   mCachedVertexIndex);

		if (mFlags & FL_VERTEX_UV)
		{
			freyjaGenerateUVFromXYZ(pos, uv+0, uv+1);
			freyjaVertexTexCoord2fv(mCachedVertexIndex, uv);
		}
    
		freyja_print("Vertex[%i] (%.3f %.3f %.3f), %i, %i", 
					mCachedVertexIndex, pos[0], pos[1], pos[2], -1, -1);
	}
}


void FreyjaModel::VertexCombine(float xx, float yy)
{
	static int32 v0 = -1;

  
	VertexSelect(xx, yy);

	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		return;
	}

	if (v0 < 0)
	{
		freyja_print("VertexCombine(%i, ?)", mCachedVertexIndex);

		v0 = mCachedVertexIndex;
	}
	else if (mCachedVertexIndex != v0)
	{
		freyja_print("VertexCombine(%i, %i)", v0, mCachedVertexIndex);

		freyjaVertexCombine(v0, mCachedVertexIndex);
		mCachedVertexIndex = v0;
		v0 = -1;
	}
}


void FreyjaModel::selectPatchControlPoint(Vector3d xyz)
{
	unsigned int besti = 0, bestj = 0, i, j;
	Vector3d u, v;
	vec_t dist, closest = 99999;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			v = xyz;
			u = Vector3d(gTestPatch.control[i][j]);
			dist = helDist3v(v.mVec, u.mVec);
			
			if (dist < closest)
			{
				closest = dist;

				besti = i;
				bestj = j;
			}
		}
	}

	gTestPatch.x = besti;
	gTestPatch.y = bestj;
}


void FreyjaModel::moveObject(transform_t type, Vector3d xyz)
{
	uint32 boneIndex = getCurrentBone();
	vec3_t pivot;


	switch (type)
	{
	case FreyjaModel::TransformBone:
		if (!freyjaIsBoneAllocated(boneIndex))
			return;

		freyjaGetBoneTranslation3fv(boneIndex, pivot);

		xyz = xyz - Vector3d(pivot);
		
		transform(FreyjaModel::TransformBone, fTranslate, 
				  xyz.mVec[0], xyz.mVec[1], xyz.mVec[2]);
		break;


	default:
		;
	}
}


void FreyjaModel::movePatchControlPoint(Vector3d xyz)
{
	gTestPatch.control[gTestPatch.x][gTestPatch.y] = xyz;
}


void FreyjaModel::selectPatchControlPoint(float xx, float yy)
{
	unsigned int besti = 0, bestj = 0, i, j, x, y;
	Vector3d u, v;
	vec_t dist, closest = 99999;


	switch (getCurrentPlane())
	{
	case PLANE_XY:
		x = 0;
		y = 1;
		break;

	case PLANE_XZ:
		x = 0;
		y = 2;
		break;

	case PLANE_ZY: // side
		x = 2;
		y = 1;
		break;
	}

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			v = Vector3d(xx, yy, 0);
			u = Vector3d(gTestPatch.control[i][j].mVec[x], 
						 gTestPatch.control[i][j].mVec[y], 0);
			dist = helDist3v(v.mVec, u.mVec);
			
			if (dist < closest)
			{
				closest = dist;

				besti = i;
				bestj = j;
			}
		}
	}

	gTestPatch.x = besti;
	gTestPatch.y = bestj;
}


void FreyjaModel::movePatchControlPoint(float xx, float yy)
{
	switch (getCurrentPlane())
	{
	case PLANE_XY:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = yy;
		break;

	case PLANE_XZ:
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[0] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = yy;
		break;

	case PLANE_ZY: // side
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[2] = xx;
		gTestPatch.control[gTestPatch.x][gTestPatch.y].mVec[1] = yy;
		break;
	}
}


void FreyjaModel::CursorPush()
{
	gFreyjaCursor.Push();
}


void FreyjaModel::CursorPop()
{
	gFreyjaCursor.Pop();
}


void FreyjaModel::CursorMove(float xx, float yy)
{
	gFreyjaCursor.SetMode( Freyja3dCursor::Translation );

	switch (getCurrentPlane())
	{
	case PLANE_XY: // front
		switch ( gFreyjaCursor.mAxis )
		{
		case 0:
			gFreyjaCursor.mPos.mVec[0] = xx;  // side to side on screen
			break;
		case 1:
			gFreyjaCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case 2:
			// In and out of screen by up and down mouse
			gFreyjaCursor.mPos.mVec[2] = yy;
			break;
		}
		break;

	case PLANE_XZ: // top
		switch ( gFreyjaCursor.mAxis )
		{
		case 0:
			gFreyjaCursor.mPos.mVec[0] = xx;  // side to side on screen
			break;
		case 1:
			// In and out of screen by up and down mouse
			gFreyjaCursor.mPos.mVec[1] = yy;
			break;
		case 2:
			gFreyjaCursor.mPos.mVec[2] = yy; // up and down on screen
			break;
		}
		break;

	case PLANE_ZY: // side
		switch ( gFreyjaCursor.mAxis )
		{
		case 0:
			// In and out of screen by up and down mouse
			gFreyjaCursor.mPos.mVec[0] = yy;
			break;
		case 1:
			gFreyjaCursor.mPos.mVec[1] = yy; // up and down on screen
			break;
		case 2:
			gFreyjaCursor.mPos.mVec[2] = xx; // side to side on screen
			break;
		}
		break;
	}	
}


void FreyjaModel::VertexMove(float xx, float yy)
{
	vec3_t xyz;

	if (!freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		freyja_print("FreyjaModel::VertexMove> No vertex to move!\n");
		return;
	}

	freyjaGetVertexXYZ3fv(mCachedVertexIndex, xyz);

	switch (getCurrentPlane())
	{
	case PLANE_XY: // front
		xyz[0] = xx;
		xyz[1] = yy;      
		break;

	case PLANE_XZ: // top
		xyz[0] = xx;
		xyz[2] = yy;
		break;

	case PLANE_ZY: // side
		xyz[2] = xx;               
		xyz[1] = yy;
		break;
	}

	freyjaVertexPosition3fv(mCachedVertexIndex, xyz);
}


void FreyjaModel::VertexSelect(float xx, float yy)
{
	/* Mongoose 2005.07.05, 
	 * Removed frame / group use */
	mCachedVertexIndex = getNearestVertexIndexInPlane(xx, yy, getCurrentPlane());

	freyja_print("!Selected %i\n", mCachedVertexIndex);
}


Vector<unsigned int> &FreyjaModel::getVertexSelectionList()
{
	return mList;
}


void FreyjaModel::getVertexSelection(vec3_t min, vec3_t max,
									 Vector<unsigned int> **list)
{
	min[0] = mSelectBBox[0][0];
	min[1] = mSelectBBox[0][1];
	min[2] = mSelectBBox[0][2];

	max[0] = mSelectBBox[1][0];
	max[1] = mSelectBBox[1][1];
	max[2] = mSelectBBox[1][2];

	*list = &mList;
}


void FreyjaModel::BBoxSelect(float xx, float yy)
{
	int x, y;
	float dist0, dist1;


	switch (getCurrentPlane())
	{
	case PLANE_XY:
		x = 0; 
		y = 1;
		break;
	case PLANE_XZ:
		x = 0; 
		y = 2;
		break;
	default:
		//case PLANE_YZ: // side
		x = 2; 
		y = 1;
		break;
	}

	dist0 = helDist3v((Vector3d(xx, yy, 0)).mVec,
					  (Vector3d(mSelectBBox[0][x], mSelectBBox[0][y], 0)).mVec);
	dist1 = helDist3v((Vector3d(xx, yy, 0)).mVec,
					  (Vector3d(mSelectBBox[1][x], mSelectBBox[1][y], 0)).mVec);

	if (dist0 < dist1)
		mSelectionBoxOrdering = 0;
	else
		mSelectionBoxOrdering = 1;
  
	//freyja_print("Vertex bbox select is %i", mSelectionBoxOrdering);
}


void FreyjaModel::BBoxMove(float xx, float yy)
{
	float swap;


	switch (getCurrentPlane())
	{
	case PLANE_XY:
		mSelectBBox[mSelectionBoxOrdering][0] = xx;
		mSelectBBox[mSelectionBoxOrdering][1] = yy;
		break;
	case PLANE_XZ:
		mSelectBBox[mSelectionBoxOrdering][0] = xx;
		mSelectBBox[mSelectionBoxOrdering][2] = yy;
		break;
	case PLANE_ZY: // side
		mSelectBBox[mSelectionBoxOrdering][2] = xx;               
		mSelectBBox[mSelectionBoxOrdering][1] = yy;
		break;
	}

	// Keep min and max!
	if (mSelectBBox[1][0] < mSelectBBox[0][0] ||
		mSelectBBox[0][0] > mSelectBBox[1][0])
	{
		swap = mSelectBBox[0][0];
		mSelectBBox[0][0] = mSelectBBox[1][0];
		mSelectBBox[1][0] = swap;
	}

	if (mSelectBBox[1][1] < mSelectBBox[0][1] ||
		mSelectBBox[0][1] > mSelectBBox[1][1])
	{
		swap = mSelectBBox[0][1];
		mSelectBBox[0][1] = mSelectBBox[1][1];
		mSelectBBox[1][1] = swap;
	}

	if (mSelectBBox[1][2] < mSelectBBox[0][2] ||
		mSelectBBox[0][2] > mSelectBBox[1][2])
	{
		swap = mSelectBBox[0][2];
		mSelectBBox[0][2] = mSelectBBox[1][2];
		mSelectBBox[1][2] = swap;
	}


	//freyja_print("Vertex bbox select is %i @ %f,%f", mSelectionBoxOrdering, xx, yy);
}	

void FreyjaModel::boxSelectionListBuild()
{
	Vector<unsigned int> *list;


	mList.clear();
	list = freyjaFindVerticesByBox(mSelectBBox);
	mList.copy(*list);
}


void FreyjaModel::MeshNew()
{
	int32 meshIndex = freyjaMeshCreate();
	freyja_print("Mesh[%i] created.", meshIndex);
}


void FreyjaModel::TexelSelect(float s, float t)
{
	Vector3d st = Vector3d(s, t, 0);
	uint32 i, j, polygonCount, texcoordCount, vertexCount;
	int32 meshIndex, polygonIndex, materialIndex, texcoordIndex, vertexIndex;
	int32 bestPolygon = -1;
	int32 bestTexcoord = -1;
	int32 bestVertex = -1;
	vec_t dist = 0.0;
	vec_t bestDist = 0.0;
	vec2_t uv;


	meshIndex = getCurrentMesh();

	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = freyjaGetMeshPolygonIndex(meshIndex, i);

		vertexCount = freyjaGetPolygonVertexCount(polygonIndex);
		texcoordCount = freyjaGetPolygonTexCoordCount(polygonIndex);

		if (vertexCount == 0)
			continue;
	
		materialIndex = freyjaGetPolygonMaterial(polygonIndex);

		// FIXME: Not the best way to pick
		if (materialIndex < 0 || 
			materialIndex != (int)getCurrentTextureIndex())
			continue;

		for (j = 0; j < texcoordCount; ++j)
		{
			texcoordIndex = freyjaGetPolygonTexCoordIndex(polygonIndex, j);

			if (!freyjaIsTexCoordAllocated(texcoordIndex))
				continue;

			freyjaGetTexCoord2fv(texcoordIndex, uv);

			// Mongoose: slight optimization, to avoid init branch  =)
			dist = helDist3v(st.mVec,
							 (Vector3d(uv[0], uv[1], 0)).mVec);

			if (dist < bestDist || bestTexcoord == -1)
			{ 
				bestPolygon = polygonIndex;
				bestTexcoord = texcoordIndex;
				bestDist = dist;
			}
		}

		/* Handle UV */
		if (texcoordCount == 0)
		{
			for (j = 0; j < vertexCount; ++j)
			{
				vertexIndex = freyjaGetPolygonVertexIndex(polygonIndex, j);
				
				if (!freyjaIsVertexAllocated(vertexIndex))
					continue;
				
				freyjaGetVertexTexcoord2fv(vertexIndex, uv);

				// Mongoose: slight optimization, to avoid init branch  =)
				dist = helDist3v(st.mVec, 
								 (Vector3d(uv[0], uv[1], 0)).mVec);
				
				if (dist < bestDist || bestVertex == -1)
				{ 
					bestPolygon = polygonIndex;
					bestVertex = vertexIndex;
					bestDist = dist;
				}
			}
		}
	}
      

	/* Mongoose: Now we set the new mTexCoordIndex, etc for cursor */
	if (bestVertex > -1)
	{
		mPolygonIndex = bestPolygon;
		mVertexIndex = bestVertex;
		return;
	}

	if (bestTexcoord > -1)
	{
		mPolygonIndex = bestPolygon;
		mTexCoordIndex = bestTexcoord;
	}
}


// FIXME: Shouldn't need direct structure access for this
void FreyjaModel::MeshSelect(float xx, float yy)
{
	int32 meshIndex = getNearestMeshIndexInPlane(xx, yy, getCurrentPlane());

	if (meshIndex < 0)
	{
		freyja_print("This model has no meshes\n");
	}
	else
	{
		setCurrentGroup(freyjaGetMeshVertexGroupIndex(meshIndex, 0));
		setCurrentMesh(meshIndex);
	}
}


void FreyjaModel::VertexDelete()
{
	if (freyjaIsVertexAllocated(mCachedVertexIndex))
	{
		freyjaVertexDelete(mCachedVertexIndex);  
		mCachedVertexIndex = -1;
	}
}


void FreyjaModel::TexelCombine(float s, float t)
{
	static int a = -1;


	TexelSelect(s, t);

	if (a == -1)
	{
		a = mTexCoordIndex;
		freyja_print("TexelCombine(%i, ?)", a);
	}
	else if ((int)mTexCoordIndex != a)
	{
		freyja_print("TexelCombine(%i, %i)", a, mTexCoordIndex);
		freyjaTexCoordCombine(a, mTexCoordIndex);
		a = -1;
	}
}


// FIXME: replace 'seen' hack with a capture list -> transform
void FreyjaModel::UVMapMotion(float s, float t)
{
	Vector<int32> seen;
	Vector3d u, v;
	vec2_t uv;
	int32 i, j, k, poly, texel, tex, iCount, jCount;


	u = Vector3d(s, t, 0);

	texel = getCurrentTexCoord();

	
	if (freyjaIsTexCoordAllocated(texel) && !mUVMap.empty())
	{
		freyjaGetTexCoord2fv(texel, uv);
		v = Vector3d(uv[0], uv[1], 0);
		u -= v;

		for (i = mUVMap.begin(), iCount = mUVMap.end(); i < iCount; ++i)
		{
			poly = mUVMap[i];
			jCount = freyjaGetPolygonTexCoordCount(poly);

			for (j = 0; j < jCount; ++j)
			{
				tex = freyjaGetPolygonTexCoordIndex(poly, j);

				for (k = seen.begin(); k < (int)seen.end(); ++k)
				{
					if (seen[k] == tex)
					{
						tex = -1;
						break;
					}
				}

				if (tex == -1) 
					continue;

				freyjaGetTexCoord2fv(tex, uv);
				uv[0] += u.mVec[0];
				uv[1] += u.mVec[1];
				freyjaTexCoord2fv(tex, uv);

				seen.pushBack(tex);
			}
		}

		return;
	}


	/* Look for polygons sharing this texcoord, start a new uvmap */
	if (texel)
	{		
		freyjaGetTexCoord2fv(texel, uv);
		v = Vector3d(uv[0], uv[1], 0);
		u -= v;

		iCount = freyjaGetTexCoordPolygonRefCount(texel);

		for (i = 0; i < iCount; ++i)
		{
			poly = freyjaGetTexCoordPolygonRefIndex(texel, i);

			jCount = freyjaGetPolygonTexCoordCount(poly);

			for (j = 0; j < jCount; ++j)
			{
				tex = freyjaGetPolygonTexCoordIndex(poly, j);

				for (k = seen.begin(); k < (int)seen.end(); ++k)
				{
					if (seen[k] == tex)
					{
						tex = -1;
						break;
					}
				}

				if (tex == -1) 
					continue;

				freyjaGetTexCoord2fv(tex, uv);
				uv[0] += u.mVec[0];
				uv[1] += u.mVec[1];
				freyjaTexCoord2fv(tex, uv);

				seen.pushBack(tex);
			}
		}
	}
	else
	{
		int32 vertex = getCurrentVertex();
		int32 vert;

		freyjaGetVertexTexcoord2fv(vertex, uv);

		v = Vector3d(uv[0], uv[1], 0);
		u -= v;

		iCount = freyjaGetVertexPolygonRefCount(vertex);

		for (i = 0; i < iCount; ++i)
		{
			poly = freyjaGetVertexPolygonRefIndex(vertex, i);
			jCount = freyjaGetPolygonVertexCount(poly);

			for (j = 0; j < jCount; ++j)
			{
				vert = freyjaGetPolygonVertexIndex(poly, j);

				for (k = seen.begin(); k < (int)seen.end(); ++k)
				{
					if (seen[k] == vert)
					{
						vert = -1;
						break;
					}
				}

				if (vert == -1) 
					continue;

				freyjaGetVertexTexcoord2fv(vert, uv);
				uv[0] += u.mVec[0];
				uv[1] += u.mVec[1];
				freyjaVertexTexCoord2fv(vert, uv);

				seen.pushBack(vert);
			}
		}
	}	
}


// Move to libfreyja?
void FreyjaModel::createPolyMappedUVMap(long seedPolygon)
{
	Vector<int32> uvMap, pending, tmp;
	int32 poly, poly2, texel, i, j, k, iCount, jCount;


	if (seedPolygon == -1)
	{
		mUVMap.clear();
		return;
	}

	// FIXME: Either optimize here or wait until new backed obsoletes
	// I want to stress your CPU for sure -- 'selection sort-o-rama'
	pending.pushBack(seedPolygon);

	while (!pending.empty())
	{
		poly = pending[0];

		//printf("createPolyMappedUVMap> Pending -> %i\n", pending[0]);

		uvMap.pushBack(pending[0]);
		tmp.clear();
		for (i = pending.begin(); i < (int)pending.end(); ++i)
		{
			for (j = uvMap.begin(); j < (int)uvMap.end(); ++j)
			{
				if (uvMap[j] == pending[i])
				{
					j = -1;
					break;
				}
			}

			if (i > 0 && j > -1)
				tmp.pushBack(pending[i]);
		}
		pending.copy(tmp);
		tmp.clear();

		//for (k = pending.begin(); k < (int)pending.end(); ++k)
		//	printf("%i ", pending[k]);
		//printf("\n");

		if (!freyjaIsPolygonAllocated(poly))
		{
			continue;
		}

		iCount = freyjaGetPolygonTexCoordCount(poly);

		for (i = 0; i < iCount; ++i)
		{
			texel = freyjaGetPolygonTexCoordIndex(poly, i);

			jCount = freyjaGetTexCoordPolygonRefCount(texel);

			for (j = 0; j < jCount; ++j)
			{
				poly2 = freyjaGetTexCoordPolygonRefIndex(texel, j);

				for (k = pending.begin(); k < (int)pending.end(); ++k)
				{
					if (pending[k] == poly2)
					{
						k = -1;
						break;
					}
				}

				if (k == -1)
					continue;

				pending.pushBack(poly2);
			}
		}

		tmp.clear();

		for (i = pending.begin(); i < (int)pending.end(); ++i)
		{
			if (!i)
				continue;

			for (j = uvMap.begin(); j < (int)uvMap.end(); ++j)
			{
				if (uvMap[j] == pending[i])
				{
					j = -1;
					break;
				}
			}

			if (j == -1)
				continue;

			for (k = tmp.begin(); k < (int)tmp.end(); ++k)
			{
				if (tmp[k] == pending[i])
				{
					k = -1;
					break;
				}
			}

			if (k == -1)
				continue;

			tmp.pushBack(pending[i]);
		}

		pending.copy(tmp);
	}


	long swap;

	for (i = uvMap.begin(); i < (int)uvMap.end(); ++i)
	{
		for (j = i; j < (int)uvMap.end(); ++j)
		{
			if (uvMap[j] < uvMap[i])
			{
				swap = uvMap[i];
				uvMap.assign(i, uvMap[j]);
				uvMap.assign(j, swap);
			}
		}
	}

	//printf("UVMap: {");
	//for (i = uvMap.begin(); i < (int)uvMap.end(); ++i)
	//	printf(" %i", uvMap[i]);
	//
	//printf("}\n");

	mUVMap.copy(uvMap);
}


void FreyjaModel::TexelMove(float s, float t)
{
	vec2_t *uv = freyjaGetTexCoordUV(getCurrentTexCoord());

	if (uv)
	{
		(*uv)[0] = s;
		(*uv)[1] = t;
	}
	else
	{
		uv = freyjaGetVertexUV(getCurrentVertex());
		
		if (uv)
		{
			(*uv)[0] = s;
			(*uv)[1] = t;
		}
	}
}


// FIXME Move to freyja_event?
#include <mgtk/mgtk_events.h>
extern void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree);
extern int gSkelTreeWidgetIndex;

mgtk_tree_t *generateSkeletalUI(uint32 skelIndex, uint32 rootIndex, 
								mgtk_tree_t *tree)
{
	uint32 i, boneChild, count;


	if (!freyjaIsBoneAllocated(rootIndex))
	{
		freyja_print("!generateSkeletalUI> ERROR: NULL skeletal bone!\n");
		return 0x0;
	}

	uint32 rootChildCount = freyjaGetBoneChildCount(rootIndex);
	const char *rootName = freyjaGetBoneNameString(rootIndex);
	uint32 rootSkelBID = freyjaGetBoneSkeletalBoneIndex(rootIndex);

	if (tree == 0x0)
	{
		tree = new mgtk_tree_t;
		snprintf(tree->label, 64, "root");	
		tree->parent = 0x0;
	}
	else
	{
		snprintf(tree->label, 64, "bone%03i", rootSkelBID);
	}

	if (rootName[0])
	{
		snprintf(tree->label, 64, "%s", rootName);
	}

	tree->id = rootIndex;
	tree->numChildren = rootChildCount;
	tree->children = 0x0;

#ifdef DEBUG_BONE_LOAD
	printf("-- %s : %i/%i children\n",  
		   tree->label, tree->numChildren, rootChildCount);
#endif

	if (tree->numChildren == 0)
		return tree->parent;

	tree->children = new mgtk_tree_t[tree->numChildren+1];

	for (count = 0, i = 0; i < rootChildCount; ++i)
	{
		boneChild = freyjaGetBoneChild(rootIndex, i);

		if (freyjaIsBoneAllocated(boneChild))
		{
			tree->children[count].parent = tree;
			generateSkeletalUI(skelIndex, boneChild, &tree->children[count++]);
		}
	}

	return (tree->parent) ? tree->parent : tree;
}


void updateSkeletonUI(uint32 skelIndex)
{
	mgtk_tree_t *tree;

	tree = generateSkeletalUI(skelIndex, 
							  freyjaGetSkeletonRootIndex(skelIndex), 0x0);

	mgtk_event_update_tree(gSkelTreeWidgetIndex, tree);
}


uint32 getCurrentSkeleton()
{ 
	return 0; // only 1 allowed in editor atm
}


void FreyjaModel::updateSkeletalUI()
{
	updateSkeletonUI(getCurrentSkeleton());
}


/* FIXME Quick hack to fix corruption on svn server by
 * using bits from my master copy, which was in middle of rewrite */
#include <GL/gl.h>
#include <freyja/FreyjaImage.h>

int FreyjaModel::loadMaterial(const char *filename)
{
	int32 mIndex;


	if (mFlags & fLoadMaterialInSlot)  
	{
		mIndex = freyjaGetCurrentMaterial();
	}
	else
	{
		mIndex = freyjaMaterialCreate();
	}

	//unsigned int m_flags; // FIXME pass to renderer

	float ambient[4];          /* Ambient color */

	float diffuse[4];          /* Diffuse color */

	float specular[4];         /* Specular color */

	float emissive[4];         /* Emissive color */

	float shininess;           /* Specular exponent */

	unsigned int texture;      /* Texture id */

	//unsigned int texture2;     /* Detail Texture id */

	unsigned int blend_src;    /* Blend source factor */

	unsigned int blend_dest;   /* Blend destination factor */


	FILE *f;
	unsigned int i, j, k, l, mode;
	char buffer[128];
	char buf[64];
	bool line_comment;
	char c;

	
	if (!filename || !filename[0])
	{
		return -1;
	}


	f = fopen(filename, "r");

	if (!f)
	{
		perror("Material::loadFile> ");
		return -2;
	}

	i = 0;
	buffer[0] = 0;
	line_comment = false;
	mode = 0;

	// Strip out whitespace and comments
	while (fscanf(f, "%c", &c) != EOF)
	{
		if (line_comment && c != '\n')
			continue;

		if (i > 126)
		{
			printf("Material::loadFile> Overflow handled\n");
			i = 126;
		}
		
		switch (c)
		{
		case ' ':
		case '\v':
		case '\t':
			break;
		case '#':
			buffer[i++] = 0;
			line_comment = true;
			break;
		case '\n':
			if (line_comment)
			{
				line_comment = false;
				i = 0;
				buffer[0] = 0;		 
				continue;
			}
			else if (buffer[0] == 0)
			{
				i = 0;
				continue;
			}

			buffer[i] = 0;

			if (buffer[0] == '[')
			{
				if (strncmp(buffer, "[Material]", 10) == 0)
				{
					mode = 1;
				}
				else
				{
					mode = 0;
				}
			}
			else if (mode == 1)
			{
				if (strncmp(buffer, "Shininess", 9) == 0)
				{
					for (j = 0, k = 10; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					shininess = atof(buf);
				}
				else if  (strncmp(buffer, "TextureName", 11) == 0)
				{
					for (j = 0, k = 12; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					//setTextureName(buf);
				}
				else if  (strncmp(buffer, "Name", 4) == 0)
				{
					for (j = 0, k = 5; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}
					
					freyjaMaterialName(mIndex, buf);

					//setName(buf);
				}
				else if  (strncmp(buffer, "EnableBlending", 14) == 0)
				{
					for (j = 0, k = 15; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					if (strncmp(buf, "true", 4) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
					}
					else if (strncmp(buf, "false", 5) == 0)
					{
						//FIXME m_flags |= Material::fEnable_Blending;
						//FIXME m_flags ^= Material::fEnable_Blending;
					}
				}
				else if (strncmp(buffer, "Blend", 5) == 0)
				{
					bool is_src = false;
					int val;


					if (strncmp(buffer, "BlendSource", 11) == 0)
					{
						is_src = true;
						k = 12;
					}
					else
					{
						k = 10;
					}

					for (j = 0; j < 63 && k < 126; ++j, ++k)
					{
						buf[j] = buffer[k];
						buf[j+1] = 0;
					}

					//printf("*** %s\n", buf);

					val = (strncmp(buf, "GL_ZERO", 11) == 0) ? GL_ZERO :
					(strncmp(buf, "GL_SRC_COLOR", 9) == 0) ? GL_SRC_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_SRC_COLOR", 22) == 0) ? GL_ONE_MINUS_SRC_COLOR :
					(strncmp(buf, "GL_DST_COLOR", 9) == 0) ? GL_DST_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_DST_COLOR", 22) == 0) ? GL_ONE_MINUS_DST_COLOR :
					(strncmp(buf, "GL_SRC_ALPHA", 9) == 0) ? GL_SRC_ALPHA :
					(strncmp(buf, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) ? GL_ONE_MINUS_SRC_ALPHA :
					(strncmp(buf, "GL_DST_ALPHA", 9) == 0) ? GL_DST_ALPHA :
					(strncmp(buf, "GL_ONE_MINUS_DST_ALPHA", 22) == 0) ? GL_ONE_MINUS_DST_ALPHA :
					(strncmp(buf, "GL_SRC_ALPHA_SATURATE", 21) == 0) ? GL_SRC_ALPHA_SATURATE :
					(strncmp(buf, "GL_CONSTANT_COLOR", 17) == 0) ? GL_CONSTANT_COLOR :
					(strncmp(buf, "GL_ONE_MINUS_CONSTANT_COLOR", 27) == 0) ? GL_ONE_MINUS_CONSTANT_COLOR :
					(strncmp(buf, "GL_ONE", 6) == 0) ? GL_ONE :
					(strncmp(buf, "GL_CONSTANT_ALPHA", 17) == 0) ? GL_CONSTANT_ALPHA :					GL_ONE_MINUS_CONSTANT_ALPHA;

					if (is_src)
					{
						blend_src = val;
					}
					else
					{
						blend_dest = val;
					}
				}
				else if (strncmp(buffer, "Ambient", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							ambient[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					ambient[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Diffuse", 7) == 0)
				{
					for (j = 0, k = 8, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							diffuse[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					diffuse[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Specular", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							specular[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					specular[l++] = atof(buf);
				}
				else if (strncmp(buffer, "Emissive", 8) == 0)
				{
					for (j = 0, k = 9, l = 0; j < 63 && k < 126; ++j, ++k)
					{
						if (buffer[k] == ',')
						{
							emissive[l++] = atof(buf);
							j = 0;
						}
						else
						{
							buf[j] = buffer[k];
							buf[j+1] = 0;
						}
					}
					
					emissive[l++] = atof(buf);
				}
			}

			i = 0;
			buffer[0] = 0;
			break;
		default:
			buffer[i++] = c;
		}
	}

	fclose(f);

	

	freyjaMaterialAmbient(mIndex, ambient);
	freyjaMaterialDiffuse(mIndex, diffuse);
	freyjaMaterialSpecular(mIndex, specular);
	freyjaMaterialEmissive(mIndex, emissive);
	freyjaMaterialShininess(mIndex, shininess);

	freyjaMaterialTexture(mIndex, texture);
	freyjaMaterialBlendDestination(mIndex, blend_dest);
	freyjaMaterialBlendSource(mIndex, blend_src);

	//unsigned int texture2;     /* Detail Texture id */

	return 0;
}


int FreyjaModel::loadTexture(const char *filename)
{
	FreyjaImage img;
	unsigned char *image;
	unsigned int w, h;
	int err = -1;

	// Mongoose 2002.01.10, Evil...
	if (FreyjaFileReader::compareFilenameExtention(filename, ".lst") == 0)
	{
		FILE *f;
		const unsigned int bufferSize = 256;
		unsigned int i = 0;
		char buffer[bufferSize];
		char c;


		f = fopen(filename, "r");
		
		if (!f)
		{
			perror(filename);
			return -1;
		}

		while (fscanf(f, "%c", &c) != EOF)
		{
			switch (c)
			{
			case ' ':
			case '\t':
			case '\n':
				break;
			case ';':
				printf("Loading texture from list '%s'\n", buffer);
				loadTexture(buffer);
				
				i = 0;
				buffer[0] = 0;
				break;
			default:
				if (i > bufferSize-1)
					i = bufferSize-1;
				
				buffer[i++] = c;
				buffer[i] = 0;
			}
		} 
		
		fclose(f);

		return 0;
	}
	
	printf("[FreyjaModel::loadTexture]\n");
	printf(" Loading texture '%s'\n", filename);

	if (!img.loadImage(filename))
	{
		img.getImage(&image);
		w = img.getWidth();
		h = img.getHeight();


		switch (img.getColorMode())
		{
		case FreyjaImage::RGBA_32:
			err = loadTextureBuffer(image, w, h, 32, Texture::RGBA);
			break;

		case FreyjaImage::RGB_24:
			err = loadTextureBuffer(image, w, h, 24, Texture::RGB);
			break;

		case FreyjaImage::INDEXED_8:
			err = loadTextureBuffer(image, w, h, 8, Texture::INDEXED);
			break;

		default:
			printf("MaterialManager: Use RGB_24 and RGBA_32 images only.\n");
			
			if (image)
				delete [] image;

			if (err >= 0)
			{
				freyjaMaterialTexture(freyjaGetCurrentMaterial(), err);
				freyjaMaterialSetFlag(freyjaGetCurrentMaterial(), fFreyjaMaterial_Texture);
				freyja_print("Material[%i].texture = { %i }", freyjaGetCurrentMaterial(), err);
				freyja_refresh_material_interface();
			}

			return -2;
		}
		
		delete [] image;
		
		printf("[Success]\n");

		return 0;
	}
	
	return -100;
}


int FreyjaModel::loadTextureBuffer(unsigned char *image, 
										unsigned int width, 
										unsigned int height, 
										unsigned int bpp, 
										Texture::ColorMode type)
{
	int err = 0;


	if (image == 0x0 || width == 0 || height == 0 || bpp == 0)
		return -1;

#ifdef NO_GLU_IN_UPLOAD_SCALE_HERE 
	FreyjaImage img;

	switch (type)
	{
	case Texture::RGBA:
		img.loadPixmap(image, width, height, FreyjaImage::RGBA_32);
		img.scaleImage(1024, 1024);
		delete [] image;
		img.getImage(&image);
		width = img.getWidth();
		height = img.getHeight();		
		break;

	default:
		;
	}
#endif

	if (mFlags & fLoadTextureInSlot)
	{
		err = mTexture.loadBufferSlot(image, width, height, type, bpp,
									  mTextureId);
	}
	else
	{
		err = mTextureId = mTexture.loadBuffer(image, width, height, type, bpp);

		printf("-- %i\n", mTextureId);
	}

	if (err < 0)
	{
		printf("MaterialManager::loadTextureBuffer> ERROR Loading buffer\n");
	}

	return err;
}


unsigned int FreyjaModel::getTexureId()
{
	return mTextureId;
}


int FreyjaModel::saveMaterial(const char *filename)
{
	int32 mIndex = freyjaGetCurrentMaterial();

	float ambient[4];          /* Ambient color */
	float diffuse[4];          /* Diffuse color */
	float specular[4];         /* Specular color */
	float emissive[4];         /* Emissive color */
	float shininess;           /* Specular exponent */
	unsigned int texture;      /* Texture id */
	//unsigned int texture2;     /* Detail Texture id */
	unsigned int blend_src;    /* Blend source factor */
	unsigned int blend_dest;   /* Blend destination factor */
	char *name;

	freyjaGetMaterialAmbient(mIndex, ambient);
	freyjaGetMaterialDiffuse(mIndex, diffuse);
	freyjaGetMaterialSpecular(mIndex, specular);
	freyjaGetMaterialEmissive(mIndex, emissive);
	shininess = freyjaGetMaterialShininess(mIndex);
	name = (char*)freyjaGetMaterialName(mIndex);
	texture = freyjaGetMaterialTexture(mIndex);
	blend_dest = freyjaGetMaterialBlendDestination(mIndex);
	blend_src = freyjaGetMaterialBlendSource(mIndex);



	FILE *f;

	
	if (!filename || !filename[0])
	{
		return -1;
	}


	f = fopen(filename, "w");

	if (!f)
	{
		perror("Material::saveFile> ");
		return -2;
	}

	fprintf(f, "[Material]\n");
	fprintf(f, "Name=%s\n", name);

	if (freyjaGetMaterialTextureName(mIndex))
	{
		fprintf(f, "TextureName=%s\n", freyjaGetMaterialTextureName(mIndex));
	}

	//fprintf(f, "EnableBlending=%s\n", 
	//		  (m_flags & fEnable_Blending) ? "true" : "false");

	fprintf(f, "BlendSource=%s\n", 
			  (blend_src == GL_ZERO) ? "GL_ZERO" :
			  (blend_src == GL_ONE) ? "GL_ONE" :
			  (blend_src == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_src == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_src == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_src == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_src == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_src == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_src == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_src == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_src == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_src == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_src == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_src == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");


	fprintf(f, "BlendDest=%s\n", 
			  (blend_dest == GL_ZERO) ? "GL_ZERO" :
			  (blend_dest == GL_ONE) ? "GL_ONE" :
			  (blend_dest == GL_SRC_COLOR) ? "GL_SRC_COLOR" :
			  (blend_dest == GL_ONE_MINUS_SRC_COLOR) ? "GL_ONE_MINUS_SRC_COLOR" :
			  (blend_dest == GL_DST_COLOR) ? "GL_DST_COLOR" :
			  (blend_dest == GL_ONE_MINUS_DST_COLOR) ? "GL_ONE_MINUS_DST_COLOR" :
			  (blend_dest == GL_SRC_ALPHA) ? "GL_SRC_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_SRC_ALPHA) ? "GL_ONE_MINUS_SRC_ALPHA" :
			  (blend_dest == GL_DST_ALPHA) ? "GL_DST_ALPHA" :
			  (blend_dest == GL_ONE_MINUS_DST_ALPHA) ? "GL_ONE_MINUS_DST_ALPHA" :
			  (blend_dest == GL_SRC_ALPHA_SATURATE) ? "GL_SRC_ALPHA_SATURATE" :
			  (blend_dest == GL_CONSTANT_COLOR) ? "GL_CONSTANT_COLOR" :
			  (blend_dest == GL_ONE_MINUS_CONSTANT_COLOR) ? "GL_ONE_MINUS_CONSTANT_COLOR" :
			  (blend_dest == GL_CONSTANT_ALPHA) ? "GL_CONSTANT_ALPHA" : "GL_ONE_MINUS_CONSTANT_ALPHA");

	fprintf(f, "Ambient=%f,%f,%f,%f\n", 
			  ambient[0], ambient[1], ambient[2], ambient[3]);
	fprintf(f, "Diffuse=%f,%f,%f,%f\n", 
			  diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	fprintf(f, "Specular=%f,%f,%f,%f\n", 
			  specular[0], specular[1], specular[2], specular[3]);
	fprintf(f, "Emissive=%f,%f,%f,%f\n", 
			  emissive[0], emissive[1], emissive[2], emissive[3]);
	fprintf(f, "Shininess=%f\n", shininess);

	fclose(f);

	return 0;
}


int FreyjaModel::loadModel(const char *filename)
{
	int err = freyjaImportModel(filename); 

	if (freyjaGetCurrentSkeleton() == INDEX_INVALID &&
	    freyjaGetSkeletonCount() > 0)
	{
		freyjaCurrentSkeleton(0);
	}

	updateSkeletalUI();		
 
	if (err)
	{
		freyja_print("ERROR: File '%s' not found or unknown format\n", filename);
		freyja_print("ERROR CODE %i\n", err);
	}
	else
	{
		freyjaModelClampTexCoords(0); // Only support 1 model per edit atm

		unsigned int i, w, h, bpp, type, count;
		unsigned char *image = 0x0;


		count = freyjaGetTexturePoolCount();
	 
		/* Texture image was stored as raw buffer */
		for (i = 0; i < count; ++i)
		{
			freyjaGetTextureImage(i, &w, &h, &bpp, &type, &image);

			if (!image)
				continue;

			switch (type)
			{
			case RGBA_32:
				loadTextureBuffer(image, w, h, 32, Texture::RGBA);
				break;

			case RGB_24:
				loadTextureBuffer(image, w, h, 24, Texture::RGB);
				break;

			case INDEXED_8:
				loadTextureBuffer(image, w, h, 8, Texture::INDEXED);
				break;

			default:
				freyja_print("%s> ERROR: Unsupported texture colormap %d",
							"FreyjaModel::loadModel", type);
			}

			freyjaTextureDelete(i); // hhhmmm
		}
	}

	return err;
}


int FreyjaModel::saveModel(const char *filename)
{
	unsigned int l, s, i;
	int ret = -1;
	char ext[32];


  
	if (!filename)
		return -1000;

	l = strlen(filename);
  
	for (s = l; s > 0; s--)
	{
		if (filename[s] == '.')
			break;
	}

	if (!s) /* No extention given, so make it '.ja' */
	{
		char buffer[4096];
		snprintf(buffer, 4095, "%s.ja", filename);
		buffer[4095] = 0;
		ret = freyjaExportModel(buffer, "ja");
	}
	else
	{
		if (s == 0 || (l - s) > 30)
			return -100;

		s++;

		memset(ext, 0, 32);

		for (i = 0; s < l; s++, i++)
		{
			ext[i] = filename[s];
		}

		ret = freyjaExportModel(filename, ext);
	}

	if (ret)
	{
		freyja_print("Unknown file export extention: '%s', try using '.ja'", 
					ext);
		return ret;
	}
  
	return 0;
}




bool FreyjaModel::isCurrentBoneAllocated()
{
	return freyjaIsBoneAllocated(getCurrentBone());
}
 

////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


/* Note this doesn't handle transformed bones! */
index_t FreyjaModel::getNearestBoneIndexInPlane(vec_t x, vec_t y, freyja_plane_t plane)
{
	uint32 i, count, best, current, skeleton;
	uint32 xx = 0, yy = 1;
	vec_t dist = 0.0;
	vec_t closest = 99999.0;
	vec3_t xyz;


	skeleton = freyjaGetCurrentSkeleton();
	count = freyjaGetSkeletonBoneCount(skeleton);

	if (count == 0)
	{
		return 0x0;
	}


	/* Avoid an extra looping branch operation by wasting stack space! */
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;

	case PLANE_XZ:
		xx = 0;
		yy = 2;
		break;

	case PLANE_ZY: 
		xx = 2;
		yy = 1;    
		break;
	}

	for (i = 0; i < count; ++i)
	{
		current = freyjaGetSkeletonBoneIndex(skeleton, i);

		if (!freyjaIsBoneAllocated(current))
			continue;

		freyjaGetBoneTranslation3fv(current, xyz);

		dist = helDist3v((Vector3d(x, y, 0)).mVec,
						 (Vector3d(xyz[xx], xyz[yy], 0)).mVec);

		if (i == 0 || dist < closest)
		{
			best = current;
			closest = dist;
		}
	}

	if (!freyjaIsBoneAllocated(best))
		return INDEX_INVALID;

	return best;
}


int32 FreyjaModel::getCurrentModel()
{
	return 0;  // limit of old backend
}


int32 FreyjaModel::getNearestMeshIndexInPlane(vec_t x, vec_t y,
											  freyja_plane_t plane)
{
	Vector3d v = Vector3d(x, y, 0), u;
	vec_t dist, closest;
	int xx = 0, yy = 1;
	uint32 i, count;
	int32 best = -1, meshIndex;
	vec3_t center;


	count = freyjaGetModelMeshCount(getCurrentModel());

	if (count < 1)
	{
		return -1;
	}

	/* Avoid additional looping branch using more stack space */
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;

	case PLANE_XZ: 
		xx = 0;
		yy = 2;
		break;

	case PLANE_ZY: 
		xx = 2;
		yy = 1;    
		break;
	}
	
	for (i = 0; i < count; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(getCurrentModel(), i);

		if (!freyjaIsMeshAllocated(meshIndex))
			continue;

		//	printf("%i <-- %i\n", meshIndex, i);

		freyjaGetMeshPosition(meshIndex, center);

		u = Vector3d(center[xx], center[yy], 0);
		dist = helDist3v(v.mVec, u.mVec);

		if (best == -1 || dist < closest)
		{
			best = meshIndex;
			closest = dist;
		}
	}

	return best;
}


int32 FreyjaModel::getNearestVertexIndexInPlane(vec_t x, vec_t y, 
												freyja_plane_t plane)
{
	Vector3d pos;
	vec3_t xyz;
	vec_t dist = 0.0;
	vec_t closest = 0.0;
	int32 best = -1;
	int32 meshIndex, vertexIndex, xx, yy;
	uint32 i, count;


	meshIndex = getCurrentMesh();
	count = freyjaGetMeshVertexGroupVertexCount(meshIndex, 0);
	//freyjaGetMeshVertexCount(meshIndex);

	if (count == 0)
	{
		freyja_print("!Current mesh[%i] empty, so no vertices to search...\n",
					 meshIndex);
		return -1;
	}

	/* Avoid extra branch in loop by wasting stack space */
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;

	case PLANE_XZ: 
		xx = 0;
		yy = 2;
		break;

	case PLANE_ZY: 
		xx = 2;
		yy = 1;    
		break;
	}

	// extra
	pos.mVec[0] = 0;
	pos.mVec[1] = 0;
	pos.mVec[2] = 0;

	pos.mVec[xx] = x;
	pos.mVec[yy] = y;

	for (i = 0; i < count; ++i)
	{
		vertexIndex = freyjaGetMeshVertexGroupVertexIndex(meshIndex, 0, i);
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);

		if (!freyjaIsVertexAllocated(vertexIndex))
			continue;

		/* FIXME: This doesn't take transformed vertices into account */
		freyjaGetVertexXYZ3fv(vertexIndex, xyz);

		dist = helDist3v(pos.mVec,
						(Vector3d(xyz[xx], xyz[yy], 0)).mVec);

		if (best == -1 || dist < closest)
		{
			best = vertexIndex;
			closest = dist;
		}
	}

	return best;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

