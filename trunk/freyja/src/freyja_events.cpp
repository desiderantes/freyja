/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2001 Terry 'Mongoose' Hendrix II
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.01:
 * Mongoose - All platforms use registered callbacks now, removed old code
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#include "config.h"

#include <string.h>

#include <freyja/freyja.h>
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/PixelBuffer.h>

#include <mstl/SystemIO.h>

#include <mgtk/mgtk_events.h>
//#include <mgtk/mgtk_linker.h>
//#include <mgtk/mgtk_lua.h>
#include <mgtk/mgtk_tree.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ConfirmationDialog.h>
#include <mgtk/QueryDialog.h>

#include "FreyjaRender.h"
//#include "FreyjaControl.h"
#include "Control.h"
#include "MaterialControl.h"
#include "Plugins.h"
#include "freyja3d_scenegraph.h"
#include "freyja_events.h"

void freyja3d_misc_events_attach();


using namespace freyja3d;


int freyja3d_get_event_id(const char* symbol)
{
	return Control::GetEventIdByName( symbol );
}


void freyja3d_record_saved_model(const char* filename)
{
	//FreyjaControl::GetInstance()->RecordSavedModel(filename);
}


void freyja_event_info_dialog( const char* icon, const char* message )
{
	mgtk_create_info_dialog( icon, message );
}


void freyjaQueryCallbackHandler(unsigned int size, freyja_query_t *array)
{
	QueryDialog d;
	d.mName = "freyjaQueryCallbackHandler"; 
	d.mDialogIcon = "gtk-question"; 
	d.mInformationMessage = "Query Dialog"; 
	d.mCancelIcon = "gtk-cancel"; 
	d.mCancelText = "Cancel"; 
	d.mAcceptIcon = "gtk-ok";
	d.mAcceptText = "Answer"; 

	bool any = false;

	for (uint32 i = 0; i < size; ++i)
	{
		const char *type = array[i].type;
		const char *symbol = array[i].symbol;
		void *data = array[i].ptr;

		String s = type;

		if (s == "int")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<int> v(symbol, s.c_str(), *(int *)data);
			d.mInts.push_back(v);
			any = true;
		}
		else if (s == "float")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<float> v(symbol, s.c_str(), *(float *)data);
			d.mFloats.push_back(v);
			any = true;
		}
		else
		{
			FREYJA_INFOMSG(false, "FIXME: Unhandled query '%s' '%s' %p\n", 
						   type, symbol, data);
		}
	}


	// If the user chooses to set values, set them.
	if ( any && d.Execute() )
	{
		for (uint32 i = 0; i < size; ++i)
		{
			String s = array[i].type;

			if (s == "int")
			{
				(*(int *)array[i].ptr) = d.GetInt( array[i].symbol );
			}
			else if (s == "float")
			{
				(*(float *)array[i].ptr) = d.GetFloat( array[i].symbol );
			}
			else
			{
				// Not handled
			}
		}	
	}
}


void freyja_handle_application_window_close()
{
	// There is some bug with some Gtk+ builds that allow this function
	// to be called again before this exits.  
	// Also relates to the Cancel doesn't cancel bug.
	//FreyjaControl::GetInstance()->Shutdown();
	bool exiting = true;

	if ( gScene->GetModified( ) && !mgtk::ExecuteConfirmationDialog("ExitWarningDialog") )
	{
		exiting = false;
	}

	if ( exiting )
	{
		freyja3d_shutdown( );
	}
}


void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color = { r, g, b, a };

	/* Color event listener */
	if (ResourceEvent::listen(id - ePluginEventBase, color, 4))
		return;

	switch (id)
	{
	case eColorMaterialAmbient: 
		freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialDiffuse:
		freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialSpecular:
		freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialEmissive:
		freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorLightAmbient: 
		freyjaLightAmbient(freyjaGetCurrentLight(), color);
		mgtk_event_set_color(eColorLightAmbient, r, g, b, a);
		break;

	case eColorLightDiffuse:
		freyjaLightDiffuse(freyjaGetCurrentLight(), color);
		mgtk_event_set_color(eColorLightDiffuse, r, g, b, a);
		break;

	case eColorLightSpecular:
		freyjaLightSpecular(freyjaGetCurrentLight(), color);
		mgtk_event_set_color(eColorLightSpecular, r, g, b, a);
		break;

	case eColorBackground:
		memcpy(FreyjaRender::mColorBackground, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorBackground, r, g, b, a);
		break;

	case eColorGrid:
		memcpy(FreyjaRender::mColorGridSeperator, color, sizeof(vec4_t));
		memcpy(FreyjaRender::mColorGridLine, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorGrid, r, g, b, a);
		break;

	case eColorVertex:
		memcpy(FreyjaRender::mColorVertex, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorVertex, r, g, b, a);
		break;

	case eColorVertexHighlight:
		memcpy(FreyjaRender::mColorVertexHighlight, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorVertexHighlight, r, g, b, a);
		break;

	case eColorMesh:
		memcpy(FreyjaRender::mColorWireframe, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorMesh, r, g, b, a);
		break;

	case eColorMeshHighlight:
		memcpy(FreyjaRender::mColorWireframeHighlight, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorMeshHighlight, r, g, b, a);
		break;

	case eColorBone:
		memcpy(FreyjaRender::mColorBone, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorBone, r, g, b, a);
		break;

	case eColorBoneHighlight:
		memcpy(FreyjaRender::mColorBoneHighlight, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorBoneHighlight, r, g, b, a);
		break;

	case eColorJoint:
		memcpy(FreyjaRender::mColorJoint, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorJoint, r, g, b, a);
		break;

	case eColorJointHighlight:
		memcpy(FreyjaRender::mColorJointHighlight, color, sizeof(vec4_t));
		mgtk_event_set_color(eColorJointHighlight, r, g, b, a);
		break;

	default:
		return;
	}

	mgtk_event_gl_refresh();
}


bool freyja3d_save_user_preferences( )
{
	String filename = freyja_rc_map_string( FREYJA_USER_PREF_FILE );
	SystemIO::TextFileWriter w;
	MSTL_MSG("\tSaving '%s'...", filename.c_str());	

	if (!w.Open(filename.c_str()))
		return false;

	w.Print("-- Custom colors\n");

	for (uint32 i = 0; i < 13; ++i)
	{
		String s;
		float r, g, b, a;
		int id;
	 
		switch (i)
		{
		case 0:
			s = "eColorBackground";
			id = eColorBackground;
			break;

		case 1:
			s = "eColorGrid";
			id = eColorGrid;
			break;

		case 2:
			s = "eColorMesh";
			id = eColorMesh;
			break;

		case 3:
			s = "eColorMeshHighlight";
			id = eColorMeshHighlight;
			break;

		case 4:
			s = "eColorVertex";
			id = eColorVertex;
			break;

		case 5:
			s = "eColorVertexHighlight";
			id = eColorVertexHighlight;
			break;

		case 6:
			s = "eColorBone";
			id = eColorBone;
			break;

		case 7:
			s = "eColorBoneHighlight";
			id = eColorBoneHighlight;
			break;

		case 8:
			s = "eColorLightAmbient";
			id = eColorLightAmbient;
			break;

		case 9:
			s = "eColorLightDiffuse";
			id = eColorLightDiffuse;
			break;

		case 10:
			s = "eColorLightSpecular";
			id = eColorLightSpecular;
			break;

		case 11:
			s = "eColorJoint";
			id = eColorJoint;
			break;

		case 12:
			s = "eColorJointHighlight";
			id = eColorJointHighlight;
			break;

		default:
			continue;
		}

		mgtk_event_get_color(id, &r, &g, &b, &a);
		w.Print("mgtk_color_set( \"%s\", %f, %f, %f, %f )\n", s.c_str(), r, g, b, a);
	}

	w.Print("\n");

	{
		w.Print("-- Custom boolean values\n");
		FreyjaRender* render = FreyjaRender::GetInstance( );


		const char* format = "mgtk_boolean_set( \"%s\", %i )\n";
		int n = (render->GetFlags() & FreyjaRender::fGrid) ? 1 : 0;
		w.Print(format, "eRenderGrid",n);

		n = (render->GetFlags() & FreyjaRender::fFace) ? 1 : 0;
		w.Print(format, "eRenderFace", n);

		n = (render->GetFlags() & FreyjaRender::fBoundingVolumes) ? 1 : 0;
		w.Print(format, "eRenderBbox", n);

		n = (render->GetFlags() & FreyjaRender::fPoints) ? 1 : 0;
		w.Print(format, "eRenderVertex", n);

		n = (render->GetFlags() & FreyjaRender::fWireframe) ? 1 : 0;
		w.Print(format, "eRenderWireframe", n);

		n = (render->GetFlags() & FreyjaRender::fLighting) ? 1 : 0;
		w.Print(format, "eRenderLighting", n);

		n = (render->GetFlags() & FreyjaRender::fMaterial) ? 1 : 0;
		w.Print(format, "eRenderMaterial", n);

		n = (render->GetFlags() & FreyjaRender::fDrawPickRay) ? 1 : 0;
		w.Print(format, "eRenderPickRay", n);

		n = (render->GetFlags() & FreyjaRender::fBones) ? 1 : 0;
		w.Print(format, "eRenderSkeleton", n);

		n = (render->GetFlags() & FreyjaRender::fBones2) ? 1 : 0;
		w.Print(format, "eRenderSkeleton2", n);

		n = (render->GetFlags() & FreyjaRender::fBones3) ? 1 : 0;
		w.Print(format, "eRenderSkeleton3", n);

		n = (render->GetFlags() & FreyjaRender::fBoneName) ? 1 : 0;
		w.Print(format, "eRenderBoneName", n);

		n = (render->GetFlags() & FreyjaRender::fSkeletalVertexBlending) ? 1 : 0;
		w.Print(format, "eSkeletalDeform", n);
	}
	
	return true;
}


///////////////////////////////////////////////////////////////////////
// Freyja wrappers
///////////////////////////////////////////////////////////////////////

const char *freyja_get_resource_path_callback()
{
	static String s;
	s = freyja_get_resource_path(); // In case it's changed refresh here.
	return s.c_str();
}


// FIXME remove duplicates
void freyja_handle_key_press(int key, int mod)
{
	freyja_print("mgtk_handle_key_press(%d, %d) not handled", key, mod);
}


void freyja_handle_command2i(int event, int command)
{
	freyja_event2i(event, command);
}


void freyja_handle_event1u(int event, unsigned int value)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	// Removed legacy uint event codepath, kept old menu event fallback
	if (!ResourceEvent::listen(event - ePluginEventBase, value))
	{
		// Old style 2 ID menu events
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("!Event(%i, %i) dropped.", eEvent, event);
	}
}


void freyja_handle_event1f(int event, float value)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	//if (!FreyjaControl::mInstance->event(event, value))
	if (!ResourceEvent::listen(event - ePluginEventBase, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void freyja_handle_gldisplay()
{
	FREYJA_ASSERTMSG(FreyjaRender::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->Display();
}


void freyja_handle_glresize(unsigned int width, unsigned int height)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->ResizeContext(width, height);
}


void freyja_handle_text_array(int event, unsigned int count, char **text)
{
	if (count != 2)
		return;

	if (!ResourceEvent::listen(event - ResourceEvent::eBaseEvent, 
							   text[0], text[1]))
	{
		// Not handled
	}	
}


void freyja_handle_text(int event, char *text)
{
	if (text == NULL || text[0] == 0)
		return;

	if (!ResourceEvent::listen(event - ePluginEventBase, text))
	{
		//if (!FreyjaControl::mInstance->handleTextEvent(event, text))
		freyja_print("%s(%i, '%s'): Unhandled event.", __func__, event, text);
	}
}


void freyja_callback_get_image_data_rgb24(const char *filename, 
										  unsigned char **image, 
										  int *width, int *height)
{
	*image = NULL;
	*width = 0;
	*height = 0;

	freyja::PixelBuffer* pixbuf = freyja::PixelBuffer::Create( filename );

	if ( pixbuf )
	{
		pixbuf->ConvertPixelFormat( freyja::PixelBuffer::RGB_24bpp );
		pixbuf->Scale(256, 256);
		*image = pixbuf->CopyPixmap();
		*width = pixbuf->GetWidth();
		*height = pixbuf->GetHeight();
	}

	delete pixbuf;
}


void freyja_handle_command(int command)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!ResourceEvent::listen(command - 10000 /*ePluginEventBase*/))
		freyja_print("!Event(%d): Unhandled event.", command);
}


void freyja_set_dialog_visible(const char *name)
{
	int e = ResourceEvent::GetResourceIdBySymbol((char*)name);
	mgtk_event_dialog_visible_set(e, 1);	
}


void freyja_handle_resource_init(Resource &r)
{
	/* Attach singleton method listeners. */
	//FreyjaControl::GetInstance()->AttachMethodListeners( );
	FreyjaRender::AttachMethodListeners( );

	// Non-class listeners
	freyja3d_misc_events_attach();

	// Bind mlisp script functions to C/C++ functions.
	//r.RegisterFunction("funcname", freyja_rc_funcname);


	////////////////////////////////////////////////////////////////////
	// Old style events
	////////////////////////////////////////////////////////////////////

	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching symbol in C/C++ */

	// Event types used for flow control of event ids
	r.RegisterInt("eMode", eMode);
	r.RegisterInt("eEvent", eEvent);
	r.RegisterInt("eNop", eNop);
	r.RegisterInt("eNone", eNone);

	// Menus
	r.RegisterInt("ePluginMenu", ePluginMenu);
	r.RegisterInt("eBlendDestMenu", eBlendDestMenu);
	r.RegisterInt("eBlendSrcMenu", eBlendSrcMenu);
	r.RegisterInt("eObjectMenu", eObjectMenu);
	r.RegisterInt("eViewportModeMenu", eViewportModeMenu);
	r.RegisterInt("eTransformMenu", eTransformMenu);

	// Colors
	r.RegisterInt("eColorMaterialAmbient", eColorMaterialAmbient);
	r.RegisterInt("eColorMaterialDiffuse", eColorMaterialDiffuse);
	r.RegisterInt("eColorMaterialSpecular", eColorMaterialSpecular);
	r.RegisterInt("eColorMaterialEmissive", eColorMaterialEmissive);
	r.RegisterInt("eColorLightAmbient", eColorLightAmbient);
	r.RegisterInt("eColorLightDiffuse", eColorLightDiffuse);
	r.RegisterInt("eColorLightSpecular", eColorLightSpecular);
	r.RegisterInt("eColorBackground", eColorBackground);
	r.RegisterInt("eColorGrid", eColorGrid);
	r.RegisterInt("eColorMesh", eColorMesh);
	r.RegisterInt("eColorVertex", eColorVertex);
	r.RegisterInt("eColorVertexHighlight", eColorVertexHighlight);
	r.RegisterInt("eColorMeshHighlight", eColorMeshHighlight);
	r.RegisterInt("eColorBone", eColorBone);
	r.RegisterInt("eColorBoneHighlight", eColorBoneHighlight);
	r.RegisterInt("eColorJoint", eColorJoint);
	r.RegisterInt("eColorJointHighlight", eColorJointHighlight);
}


void freyja_handle_resource_start()
{
	/* Mongoose 2002.02.02,  
	 * This is the backend entry called by the mgtk widget layer.
	 */

	/* Mongoose 2007.04.07,
	 * Here all the resources are setup, and the main interface starts. 
	 */

#warning FIXME Replace all the FreyjaControl usage.

	freyja_print("!@Freyja started...");

	/* Attempt to allocate singletons. */
	//FREYJA_ASSERTMSG(FreyjaControl::GetInstance() != NULL, "FreyjaControl allocation failure.  See '%s' for possible errors.", FREYJA_LOG_FILE );
	FREYJA_ASSERTMSG( FreyjaRender::GetInstance() != NULL, "FreyjaRender Singleton allocation failure.  See '%s' for possible errors.", FREYJA_LOG_FILE );
	if ( /*!FreyjaControl::GetInstance() ||*/ !FreyjaRender::GetInstance( ) )
	{
		SystemIO::Print("See '%s' for possible errors.\n", FREYJA_LOG_FILE );
		freyja3d_shutdown( );
		return;
	}

	//freyja_handle_resource_init(FreyjaControl::GetInstance()->GetResource());

	/* Build the user interface from scripts and load user preferences. */
	//FreyjaControl::GetInstance()->Init();
	freyja3d_plugin_init();
	freyja3d_plugin_application_widget_init();


	/* Load and init application plugins. */
	{
		String dir = freyja_rc_map_string( "plugins" );	
		freyja3d_plugin_application_init( dir.c_str() );
	}

	/* Setup scenegraph widget(s). */
	freyja3d_scenegraph_init();

	/* Setup material interface */
	MaterialControl::GetInstance()->RefreshInterface();

	/* Setup editor modes and drop-down menus */
#if 0
	mgtk_option_menu_value_set(eViewportModeMenu, 0);
	FreyjaControl::GetInstance()->EvModeModel();

	mgtk_option_menu_value_set(eTransformMenu, 1);
	FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tMesh);

	mgtk_option_menu_value_set(eObjectMenu, 0);
	FreyjaControl::GetInstance()->SetActionMode(FreyjaControl::aSelect);
#endif

	/* Set init window title, log welcome, and refresh OpenGL context */
	mgtk_application_window_title( BUILD_NAME );
	freyja_print("Welcome to Freyja %s, %s", VERSION, __DATE__);
	mgtk_event_gl_refresh();

	/* Mongoose 2002.02.23, Hook for exit() calls */
	//atexit( freyja3d_shutdown );
}


void freyja_append_eventid(char *symbol, int eventid)
{
	//FreyjaControl::GetInstance()->GetResource().RegisterInt(symbol, eventid);
}


void freyja_get_rc_path(char *s, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd);

	if (sz < len)
		return;
	
	snprintf(s, len, cwd);
	s[len] = 0;
#else
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	long len = strlen(env) + strlen("/.freyja/") + 1;

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/", env);
	s[len] = 0;
#endif
}


void freyja_get_share_path(char *s, long sz)
{
	if (sz < 64)
		return;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(s, strlen(cwd), cwd);
#else
	snprintf(s, strlen("/usr/share/freyja/"), "/usr/share/freyja/");
#endif
}


void freyja_get_rc_filename(char *s, const char *filename, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
	s[len] = 0;
#else
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	long len = strlen(env) + strlen("/.freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/%s", env, filename);
	s[len] = 0;
#endif
}


void freyja_get_share_filename(char *s, const char *filename, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
	s[len] = 0;
#else
	long len = strlen("/usr/share/freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "/usr/share/freyja/%s", filename);
	s[len] = 0;
#endif
}


char freyja_is_user_installed_data_old()
{
#ifndef WIN32
	/*
	SystemIO::FileReader r;
	char path[128];
	freyja_get_rc_path(path, 128);
	return (r.DoesFileExist(path));
	*/

	String s = freyja_rc_map_string( "version" );
	mstl::SystemIO::BufferedTextFileReader r;

	if ( !r.DoesFileExist( s.c_str() ) )
		return 0;

	r.Open( s.c_str() );
	unsigned int version = r.ParseInteger();
	r.Close();

	r.Open( "/usr/share/freyja/version" );
	unsigned int version2 = r.ParseInteger();
	r.Close();

	freyja_print( "Data version %i, %i", version, version2 );

	return ( version < version2 ) ? 0 : 1;
#endif

	return 1;
}


void freyja_install_user()
{
	SystemIO::FileReader r;
	SystemIO::FileWriter w;
	char rc[512];
	const char *filename;


	/* Backup scripts. */
	{
		char backup[512];
		freyja_get_rc_path( rc, 512 );
		freyja_get_rc_filename( backup, "backup", 512 );

		SystemIO::File::CreateDir( backup );

		if ( r.OpenDir( rc) )
		{
			while ( (filename = r.GetNextDirectoryListing()) )
			{
				if ( r.IsDirectory(filename) )
					continue;
				
				freyja_print( "cp '%s' '%s'", filename, backup );
				SystemIO::CopyFileToPath( filename, backup );
			}
		}	
	}

	char share[512];

	/* Copy script files, logs, prefs, etc. */
	freyja_get_rc_path(rc, 512);
	freyja_get_share_path(share, 512);

	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			freyja_print( "cp '%s' '%s'", filename, rc );
			SystemIO::CopyFileToPath(filename, rc);
		}
	}

	/* Copy icon files */
	freyja_get_rc_filename(rc, "icons/", 512);
	freyja_get_share_filename(share, "icons/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			freyja_print( "cp '%s' '%s'", filename, rc );
			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy material files */
	freyja_get_rc_filename(rc, "materials/", 512);
	freyja_get_share_filename(share, "materials/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy palettes files */
	freyja_get_rc_filename(rc, "palettes/", 512);
	freyja_get_share_filename(share, "palettes/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			freyja_print( "cp '%s' '%s'", filename, rc );
			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Misc */
	freyja_get_rc_filename(rc, "models/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "animations/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "textures/", 512);
	SystemIO::File::CreateDir(rc);

	//freyja_get_rc_filename(rc, "particles/", 512);
	//SystemIO::File::CreateDir(rc);

	/* Copy plugins */
	freyja_get_rc_filename(rc, "plugins/", 512);
	freyja_get_share_filename(share, "plugins/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if ( r.IsDirectory(filename) )
			{
				// Recurse one level and copy as well.
				while ( (filename = r.GetNextDirectoryListing()) )
				{
					freyja_print( "cp '%s' '%s'", filename, rc );
					SystemIO::CopyFileToPath(filename, rc);					
				}
			}
			else
			{
				freyja_print( "cp '%s' '%s'", filename, rc );
				SystemIO::CopyFileToPath(filename, rc);
			}
		}
	}
}


void freyja_handle_motion(int x, int y)
{
	//if (FreyjaControl::GetInstance())
	
	//FreyjaControl::GetInstance()->MotionEvent(x, y);
	
}


void freyja_handle_mouse(int button, int state, int mod, int x, int y)
{

	//if (FreyjaControl::GetInstance())
	
		//FreyjaControl::GetInstance()->MouseEvent(button, state, mod, x, y);
	
}


int freyja_event2i(int event, int cmd)
{
	if ( event != eNop && 
		 !ResourceEvent::listen(cmd - ePluginEventBase) )
	{
		freyja_print("%s(%i, %i): Event has no handler.",
					 __func__, event, cmd);
		return -1;
	}

	return 0;
}


void freyja_print(char *format, ...)
{
	if ( format && format[0] )
	{
		const unsigned int sz = 1024;
		char buf[sz];

		va_list args;
		va_start(args, format);
		int truncated = vsnprintf(buf, sz, format, args);
		buf[sz-1] = 0;
		va_end(args);

		/* More than 1k string was needed, so allocate one. */
		if ( truncated >= (int)sz )
		{
			unsigned int len = truncated + 1; // Doesn't include '\0'
			char* s = new char[ len ];

			va_start( args, format );
			vsnprintf( s, len, format, args );
			s[len-1] = '\0';
			va_end( args );

			ControlPrinter::Print( s );
			delete [] s;
		}
		else
		{
			ControlPrinter::Print( buf );
		}
	}
}





String freyja_get_resource_path()
{
	mstl::String s;

#if WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	
	s = cwd;
	s += "/";
#else
	char *env = getenv("HOME");
	
	if (!env || !env[0])
	{
		MSTL_MSG("ERROR: Bad HOME envronment\n");
		s = "/usr/share/freyja";
	}
	else
	{
		s = env;
	}
	
	s += "/.freyja/";
#endif

	return s;
}


String freyja_rc_map_string(const char *filename)
{
	String s = freyja_get_resource_path();
	s += filename;
	return s;
}


const char* freyja_rc_map(const char* basename)
{
	String s = freyja_get_resource_path();
	s += basename;
	return mstl::String::Strdup( s.c_str() );
}


void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (size < 1)
		return;

#if WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(dest, size, "%s/icons/%s", cwd, icon_name);
#else
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#endif

	dest[size-1] = 0;
}






