/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 *
 *-- TODO --------------------------------------------------- 
 *
 * + Remove Egg dependence ( planes ) 
 *
 * + 3d events to Model facade
 *
 * + Command pattern event refactoring
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.14:
 * Mongoose - Documentation update and Freyja coding guidelines ( finally )
 *
 * 2000.09.10:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_

#include <freyja/FreyjaPrinter.h>
#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <mgtk/Resource.h>

#include "freyja_events.h"
#include "FreyjaRender.h"
#include "Texture.h"
#include "Freyja3dCursor.h"


class RecentFilesControl
{
public:

	RecentFilesControl() //:
	//	mResourceFilename("recent_files")
	{
	}


	void AddRecentFilename(const char *filename)
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends filename to recently opened file list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	{
		bool found = false;
		uint32 idx;

		if (!filename || !filename[0] || 
			!SystemIO::File::DoesFileExist(filename))
		{
			return;
		}

		for (uint32 i = mRecentFiles.begin(), n = mRecentFiles.end(); i < n; ++i)
		{
			if (strcmp(filename, mRecentFiles[i].GetCString()) == 0)
			{
				idx = i;
				found = true;
				break;
			}
		}


		/* 'Boost' this file to top slot, push others down one */
		if (found)
		{
			// Already at top, no change to menu
			if (idx == 0)
				return;

			String swap, old;
			uint32 n = mRecentFiles.end();
			swap = mRecentFiles[0];
			mRecentFiles[0] = mRecentFiles[idx];

			for (uint32 i = 1; i < n; ++i)
			{
				if (i > idx)
				{
					break;
				}

				old = mRecentFiles[i];
				mRecentFiles[i] = swap;
				swap = old;
			}
		}
		else  /* Add new file to top slot, push others down one */
		{
			String swap, old;
			String insert(filename);

			// Bubble up hack
			if (mRecentFiles.end() >= mRecentFileLimit)
			{
				swap = insert;
				uint32 n = mRecentFiles.end();
				for (uint32 i = mRecentFiles.begin(); i < n; ++i)
				{				
					old = mRecentFiles[i];
					mRecentFiles[i] = swap;
					swap = old;
				}
			}
			else
			{
				mRecentFiles.pushBack(insert);
			}
		}

		/* Rebuild menu in order of mRecentFiles */
		uint32 menuId = Resource::mInstance->getIntByName("eRecentFiles");
		freyja_remove_all_items_to_menu(menuId);
		
		uint32 n = mRecentFiles.end();
		for (uint32 i = mRecentFiles.begin(); i < n; ++i)
		{
			mgtk_append_item_to_menu2i(menuId, mRecentFiles[i].GetCString(), menuId, i);
		}


		/* Save recent_files to disk */
		SystemIO::TextFileWriter w;

		if (w.Open(freyja_rc_map_string("recent_files").GetCString()))
		{
			String swap;
			uint32 n = mRecentFiles.end();
			for (uint32 i = mRecentFiles.begin(); i < n; ++i)
			{
				swap = mRecentFiles[i];
			
				if (!swap.Empty())
				{
					w.Print("%s\n", swap.GetCString());
				}
			}

			w.Close();
		}
	}


	bool HandleEvent(uint32 value)
	{
		if (value < mRecentFiles.size())
		{
			BUG_ME("FIXME");
			//mFileDialogMode = FREYJA_MODE_LOAD_MODEL;
			//handleFilename(mRecentFiles[value]);
			return true;
		}

		return false;
	}


	bool LoadResource()
	{
		/* Recent files persistance */
		SystemIO::TextFileReader r;
		String filename = freyja_rc_map_string("recent_files");

		if (r.Open(filename.GetCString()))
		{
			for (uint32 j = 0; j < mRecentFileLimit && !r.IsEndOfFile(); ++j)
			{
				const char *sym = r.ParseSymbol();
				AddRecentFilename(sym);
			}
		
			r.Close();
			return true;
		}

		return false;
	}

	const static uint32 mRecentFileLimit = 7;

	Vector<String> mRecentFiles;            /* Recently loaded model files */	
};


class FreyjaControl
{
 public:

	/* Each of these control schemes will have it's own control class later */
	typedef enum {        
		eScheme_UV = 0,
		eScheme_Model,
		eScheme_Material
		
	} ControlScheme;


	typedef enum {
		tNone = 0,
		tPoint,
		tFace,
		tMesh,
		tBone,
		tKeyFrame,
		tSkeleton,
		tScene,
		tSelectedVertices,
		tSelectedFaces,
		tSelectedBones,
		tSelectedMeshes
	} object_type_t;


	typedef enum {
		fNone                = 0,
		fPolyMappedTexCoords = 1,
		fDeformBoneVertices  = 2,
		fLoadTextureInSlot   = 4,
		fLoadMaterialInSlot  = 8

	} options_t;


	/* Old event format enums */
	typedef enum {                              /* Editor event modes */
		modeNone = 1,
		MESH_MOVE_CENTER,
		POINT_ADD_MODE,
		POINT_DEL_MODE,
		POLYGON_ADD_MODE,
		POLYGON_DEL_MODE,
		BONE_CONNECT_MODE,
		BONE_DISCONNECT_MODE,
		BONE_ADD_MODE,
		TAG_MOVE_CENTER,
		TEXEL_COMBINE,
		VERTEX_COMBINE,
		VERTEX_BBOX_SELECT_MODE,
		POLYGON_SELECT_MODE,
		
		/* Generic transforms */
		modeMove,
		modeRotate,
		modeScale,
		modeSelect,
		modeUnselect
		
	} EventMode;


	class FreyjaControlPrinter : public FreyjaPrinter
	{
	public:

		virtual void errorArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}


		virtual void messageArgs(char *format, va_list *args)
		{
			freyja_print_args(format, args);
		}
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaControl(Resource *r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaControl object is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : FreyjaControl object is allocated
	 * Post : FreyjaControl object is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Properties
	////////////////////////////////////////////////////////////

	ControlScheme GetControlScheme() { return mControlScheme; }
	void SetControlScheme(ControlScheme scheme) { mControlScheme = scheme; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Alters the way input is handled for this control
	 ------------------------------------------------------*/

	uint32 GetFaceEdgeCount() { return mFaceEdgeCount; }
	void SetFaceEdgeCount(uint32 i) { if (i>2) mFaceEdgeCount = i; }
	uint32 mFaceEdgeCount;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns face 'edge count' for draw poly from 
	 *        points interface.  ( An odd, yet popular feature. ) 
	 ------------------------------------------------------*/

	const Vec3 &GetSceneTranslation() { return mSceneTrans; }
	void SetSceneTranslation(const Vec3 &v) { mSceneTrans = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns xyz offset of the scene
	 ------------------------------------------------------*/
	
	uint32 GetSelectedAnimation() { return mSelectedAnimation; }
	void SetSelectedAnimation(uint32 i) { mSelectedAnimation = i; }
	uint32 mSelectedAnimation;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected animation index
	 ------------------------------------------------------*/

	uint32 GetSelectedBone() { return mSelectedBone; }
	void SetSelectedBone(uint32 i) 
	{ if (i < freyjaGetCount(FREYJA_BONE)) mSelectedBone = i; }
	uint32 mSelectedBone;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected bone index
	 ------------------------------------------------------*/

	uint32 GetSelectedFace() { return mSelectedFace; }
	void SetSelectedFace(uint32 i) 
	{ if (i < freyjaGetCount(FREYJA_POLYGON)) mSelectedFace = i; }
	uint32 mSelectedFace;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected face index
	 ------------------------------------------------------*/

	uint32 GetSelectedKeyFrame() { return mSelectedKeyFrame; }
	void SetSelectedKeyFrame(uint32 i) { mSelectedKeyFrame = i; }
	uint32 mSelectedKeyFrame;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected keyframe index
	 ------------------------------------------------------*/

	uint32 GetSelectedMesh() { return mSelectedMesh; }
	void SetSelectedMesh(uint32 i) 
	{
		if (i < freyjaGetCount(FREYJA_MESH) && freyjaIsMeshAllocated(i))
			mSelectedMesh = i;
	}
	uint32 mSelectedMesh;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected mesh index
	 ------------------------------------------------------*/

	uint32 GetSelectedSkeleton() { return mSelectedSkeleton; }
	void SetSelectedSkeleton(uint32 i) 
	{ if (i < freyjaGetCount(FREYJA_SKELETON)) mSelectedSkeleton = i; }
	uint32 mSelectedSkeleton;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected skeleton index
	 ------------------------------------------------------*/

	uint32 GetSelectedTexCoord() { return mSelectedTexCoord; }
	void SetSelectedTexCoord(uint32 i) 
	{ if (i < freyjaGetCount(FREYJA_TEXCOORD)) mSelectedTexCoord = i; }
	uint32 mSelectedTexCoord;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected texcoord index
	 ------------------------------------------------------*/

	uint32 GetSelectedTexture() { return mSelectedTexture; }
	void SetSelectedTexture(uint32 i) { mSelectedTexture = i; }
	uint32 mSelectedTexture;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected texture index
	 ------------------------------------------------------*/

	uint32 GetSelectedVertex() { return mSelectedVertex; }
	void SetSelectedVertex(uint32 i) 
	{ if (i < freyjaGetCount(FREYJA_VERTEX)) mSelectedVertex = i; }
	uint32  mSelectedVertex;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns currently selected vertex
	 ------------------------------------------------------*/

	freyja_plane_t GetSelectedView() { return mSelectedView; }
	void SetSelectedView(freyja_plane_t p) { mSelectedView = p; }
	freyja_plane_t  mSelectedView;
	/*------------------------------------------------------
	 * Pre  : - TEMP USE ONLY -
	 * Post : Returns currently selected view
	 ------------------------------------------------------*/

	vec_t GetZoom();
	void SetZoom(vec_t zoom);
	/*------------------------------------------------------
	 * Pre  : Set() <zoom> is a number greater than 0.0
	 * Post : Returns current viewing zoom of scene
	 ------------------------------------------------------*/
	


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void AdjustMouseXYForViewports(vec_t &x, vec_t &y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Maps actual mouse x, y to veiwport's local x, y.
	 *        As if the viewport was the same size as 
	 *        entire context.
	 ------------------------------------------------------*/

	Freyja3dCursor &GetCursor() { return mCursor; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool SaveModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Writes model to disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool SaveMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Writes material to disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	String ObjectTypeToString(object_type_t t);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns String for transform type
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////
	
	void ActionModelModified(FreyjaState *s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : New event 'helper' for undo/redo.
	 *
	 *        Call this after every modification outside the
	 *        normal state system.  Should be obsoleted later.
	 *
	 *        Currently not fully implemented.
	 ------------------------------------------------------*/

	void AddRecentFilename(const char *filename) { BUG_ME("FIXME"); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends filename to recently opened file list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void CastPickRay(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Casts the pick array given a screen x, y.
	 *        Accounts for view mode ( front, top, side, free )
	 ------------------------------------------------------*/

	bool event(int event, unsigned int value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *mControlScheme
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.22:
	 * Mongoose - Replaces 2000 handleEvent method
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool event(int event, vec_t value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool event(int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 200X.XX.XX: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool handleEvent(int event, int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process event input, such as from a menu
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void handleFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid and mode was set
	 * Post : Executes a file operation depending on mode
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void handleTextEvent(int event, const char *text);
	/*------------------------------------------------------
	 * Pre  : text is valid
	 * Post : Processes event using text given
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool LoadTextureBuffer(byte *image, uint32 width, uint32 height, 
						  uint32 bpp, Texture::ColorMode type);
	/*------------------------------------------------------
	 * Pre  : Don't pass in invalid/mismatched data
	 * Post : Loads a texture given a pixel buffer
	 ------------------------------------------------------*/

	bool mouseEvent(int btn, int state, int mod, int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input
	 --------------------------------------------*/
	
	bool motionEvent(int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 --------------------------------------------*/
	
	bool Key(unsigned int key, int x, int y, int modifer_keys);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process alphanumeric keyboard 
	 *            input
	 --------------------------------------------*/

	bool LoadModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads model from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	bool LoadMaterial(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	void LoadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the Resource file from disk and sets cvars
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/

	bool LoadTexture(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Reads material from disk
	 * Post : Returns true if sucessful
	 ------------------------------------------------------*/

	void Display()
	{
		ASSERT_MSG(mRender, "FreyjaRender Singleton not allocated");
		mRender->display();
	}


	void HandleResize(uint32 width, uint32 height)
	{
		ASSERT_MSG(mRender, "FreyjaRender Singleton not allocated");
		mRender->resizeContext(width, height);
	}


	void CursorStatePush() { mCursor.Push(); }
	
	
	void CursorStatePop() { mCursor.Pop(); }

	bool ToggleFlag(options_t flag) { mFlags ^= flag; return mFlags & flag; }


	static FreyjaControl *mInstance;


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	void getPickRay(vec_t mouseX, vec_t mouseY, 
					double *rayOrigin, double *rayVector);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pick ray in object cooridnates
	 ------------------------------------------------------*/

	void getScreenToWorldOBSOLETE(float *x, float *y);
	/*------------------------------------------------------
	 * Pre  : Emulates the old ScreenToWorld() method behavior
	 * Post : This function will be removed after the Model rewrite
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.15: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getWorldFromScreen(float *x, float *y, float *z);
	/*------------------------------------------------------
	 * Pre  : X and Y are the mouse position
	 *
	 * Post : Converts x,y on screen to x,y,z in world
	 *        for whichever plane was last selected
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25: 
	 * Mongoose - Created from ScreenToWorld from GooseEgg
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void addObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	// Collection of some hasty merging 
	bool SaveKeyFrame(char const *filename)
	{
		char filename2[512];
		snprintf(filename2, 512, "%s-keyframe.smd", filename);
		return SaveModel(filename2);
	}

	// FIXME: Temp here to let this work during rewrite so it could be uploaded to public svn to fix revision corruption in public svn
	void InitTexture()
	{
		unsigned char rgba[4] = {255, 255, 255, 255};
		mTexture.reset();
		mTexture.setMaxTextureCount(64);
		mTexture.setFlag(Texture::fUseMipmaps);
		mTexture.loadColorTexture(rgba, 32, 32);
		mTextureId = 1;
	}

	int32 mTextureId;

	void MeshMove(vec_t xx, vec_t yy);

	void SetFaceMaterial(index_t faceIndex, index_t material);

	void CreatePolyMappedUVMap(int i) {BUG_ME("Not implemented in this build");}

	void CursorMove(float xx, float yy);

	void GetBoneRotation(float *x, float *y, float *z)
	{
		vec3_t xyz;

		freyjaGetBoneRotationEuler3fv(GetSelectedBone(), xyz);
		*x = HEL_RAD_TO_DEG(xyz[0]);
		*y = HEL_RAD_TO_DEG(xyz[1]);
		*z = HEL_RAD_TO_DEG(xyz[2]);
		freyja_print("%f %f %f", xyz[0], xyz[1], xyz[2]);
	}

	void Transform(int mode, freyja_transform_action_t action, 
				   float x, float y, float z) { BUG_ME("Not implemented"); }
	/*------------------------------------------------------
	 * Pre  : mode is {FRAME, MESH, SCENE, BONE, etc}
	 *        action is { fTranslate, fRotate, fScale, etc }
	 *        x, y, z are in degrees or units
	 *
	 * Post : Transform is performed
	 *
	 * Notes: FRAME  : Transform current frame of current mesh
	 *        MESH   : Transform current mesh
	 *        SCENE  : Transform entire scene
	 *        BONETAG: Transform current bone tag
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void UpdateSkeletalUI() { UpdateSkeletonUI_Callback(GetSelectedSkeleton()); }

	const char *GetBoneName(unsigned int boneIndex)
	{
		return (freyjaIsBoneAllocated(boneIndex)) ? freyjaGetBoneNameString(boneIndex) : 0x0;
	}

	void SetBoneName(unsigned int boneIndex, const char *name)
	{
		if (freyjaIsBoneAllocated(boneIndex) && name && name[1])
		{
			freyjaBoneName(boneIndex, name);
			freyja_print("bone[%i].name = '%s'", boneIndex, name);
			UpdateSkeletalUI();
		}
	}

	void PrintInfo()
	{
		freyja_print("%d bones, %d meshes, %d polygons, %d vertices",
					 freyjaGetCount(FREYJA_BONE), 
					 freyjaGetCount(FREYJA_MESH), 
					 freyjaGetCount(FREYJA_POLYGON), 
					 freyjaGetCount(FREYJA_VERTEX));
	}

	void SetBoneRotation(float x, float y, float z)
	{
		vec3_t xyz = {HEL_DEG_TO_RAD(x), HEL_DEG_TO_RAD(y), HEL_DEG_TO_RAD(z)};
		freyjaBoneRotateEuler3fv(GetSelectedBone(), xyz);
	}

	void SetMaterialForSelectedFaces(uint32 material);

	void Clear()
	{
		freyjaModelClear(0); // Only 1 edit model supported in this bulid
		mCursor.Reset();
		mCleared = true;
	}

	void DeleteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool copySelectedObject();
	bool pasteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void moveObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void rotateObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void SelectCursorAxis(vec_t vx, vec_t vy);
	/*------------------------------------------------------
	 * Pre  : vx, vy are mouse coords adjusted for viewports
	 * Post : Sets selected cursor 'axis' X Y Z
	 *
	 ------------------------------------------------------*/

	void SelectObject(vec_t mouseX, vec_t mouseY);
	void UnselectObject(vec_t mouseX, vec_t mouseY);
	//void SelectObject(vec_t x, vec_t y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void scaleObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void MotionEdit(int x, int y, freyja_plane_t plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 *            in edit mode
	 --------------------------------------------*/
	
	bool MouseEdit(int btn, int state, int mod, int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse input in edit mode
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created  ( Updated comment style )
	 ------------------------------------------------------*/


	uint32 mFlags;                          /* Option bitflags */

	object_type_t mObjectMode;              /* Current object type to edit */

	Texture mTexture;                       /* Collection of Texture utils */

	Vector<int32> mUVMap;                   /* 'Texture faces' grouping */

	FreyjaControlPrinter mPrinter;          /* Reroute text msg from backend */

	String mResourceFilename;	            /* Resource file for control */

	String mCurrentlyOpenFilename;          /* What file is loaded now? */

	Vec3 mSceneTrans;                       /* Offset of scene in 3 space */

	Freyja3dCursor mCursor;                 /* Special mouse input handler */

	Resource *mResource;                    /* Resource system */
	
	FreyjaRender *mRender;                  /* OpenGL renderer */

	unsigned int mLastEvent, mLastCommand;  /* The last command pair recieved*/

	unsigned int mMouseButton, mModKey;     /* Cached mouse button event */

	int mMouseState;                        /* Cached mouse state event */

	ControlScheme mControlScheme;           /* Current control scheme in use */

	EventMode mEventMode;                   /* Mode of generic event handler */
	
	int mUVMouseState;                      /* Mouse state on texture canvas */
	
	int mXYZMouseState;                     /* Mouse state on modeler canvas */

	unsigned int mFileDialogMode;

	bool mFullScreen;

	bool mCleared;

	bool mAllowBoneNameUpdate;

	float mGenMeshHeight;
	unsigned long mGenMeshCount;
	unsigned long mGenMeshSegements;
};

void FreyjaControlEventsAttach();

#endif
