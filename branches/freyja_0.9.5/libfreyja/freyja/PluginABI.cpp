/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the Freyja plugin ABI/API.
 * 
 *           
 *-- History ------------------------------------------------ 
 *
 * 2004.12.17: (v0.0.9)
 * Mongoose -  Created, FreyjaPlugin ABI interface defination refinement
 *             based on FreyjaPlugin and FreyjaFSM.
 *
 *             This common interface makes it possible to share ABI 
 *             compatiblity for binary C/C++ plugins.
 *             ( Chimera and Umbra branches merged )
 *
 ==========================================================================*/

#define USING_FREYJA_CPP_ABI

#include <math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "MaterialABI.h"
#include "ModelABI.h"
#include "MeshABI.h"
#include "BoneABI.h"
#include "SkeletonABI.h"
#include "PluginABI.h"
#include "PythonABI.h"
#include "Skeleton.h"
#include "Plugin.h"

mstl::Vector<freyja::PluginDesc *> gFreyjaPlugins;
mstl::Vector<mstl::String> gPluginDirectories;
mstl::Vector<mstl::String> gImagePluginDirectories;
int32 gCurrentFreyjaPlugin = -1;


//////////////////////////////////////////////////////////////////////
// Plugin C++ ABI
//////////////////////////////////////////////////////////////////////

mstl::Vector<freyja::PluginDesc *> &freyjaGetPluginDescriptions()
{
	return gFreyjaPlugins;
}


mstl::Vector<mstl::String> &freyjaGetPluginDirectories()
{
	return gPluginDirectories;
}


freyja::PluginDesc *freyjaGetPluginClassByFilename(const char *name)
{
	long i, l;

	if (!name || !name[0])
		return 0x0;

	l = strlen(name);

	for (i = gFreyjaPlugins.begin(); i < (long)gFreyjaPlugins.end(); ++i)
	{
		if (gFreyjaPlugins[i] && 
			gFreyjaPlugins[i]->mFilename.c_str() && 
			gFreyjaPlugins[i]->mFilename.c_str()[0])
		{
			if (!strncmp(gFreyjaPlugins[i]->mFilename.c_str(), name, l))
			{
				return gFreyjaPlugins[i];
			}
		}
	}

	return 0x0;
}


freyja::PluginDesc *freyjaGetPluginClassByIndex(long pluginIndex)
{
	if (pluginIndex > 0 && pluginIndex < (long)gFreyjaPlugins.end())
	{
		return gFreyjaPlugins[pluginIndex];
	}

	return 0x0;
}


//////////////////////////////////////////////////////////////////////
// Plugin 0.9.5 ABI
//////////////////////////////////////////////////////////////////////

void freyjaPluginDirectoriesInit()
{
#ifdef WIN32
	freyjaPluginAddDirectory("plugins/model");
	gImagePluginDirectories.push_back(mstl::String("plugins/image"));
#else
	String s = getenv("HOME");
	s += "/.freyja/plugins/model";
   	freyjaPluginAddDirectory(s.c_str());

	s = getenv("HOME");
	s += "/.freyja/plugins/image";
	gImagePluginDirectories.push_back(s);

	//freyjaPluginAddDirectory("/usr/lib/freyja_0.9.5/modules/model");
	//freyjaPluginAddDirectory("/usr/local/lib/freyja_0.9.5/modules/model");
	//freyjaPluginAddDirectory("/usr/share/freyja/plugins/model");
#endif
}


void freyjaPluginAddDirectory(const char *dir)
{
	if (!dir || !dir[0] || !SystemIO::File::IsDirectory(dir))
		return;

	uint32 i;
	foreach(gPluginDirectories, i)
	{
		if (!strcmp(gPluginDirectories[i].c_str(), dir))
			return;
	}

	gPluginDirectories.pushBack(dir);
}


void freyjaPluginFilename1s(const char *filename)
{

	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->SetFilename(filename);
}


int qSort_FreyjaPluginDesc(const void *a, const void *b)
{
	if (a == NULL)
		return 1;

	if (b == NULL)
		return -1;

	freyja::PluginDesc &objA = *( (freyja::PluginDesc *)a );
	freyja::PluginDesc &objB = *( (freyja::PluginDesc *)b );

	return objA.mDescription.Strcmp(objB.mDescription.c_str());
}


void freyjaPluginsInit()
{
#ifdef FREYJA_PLUGINS
	SystemIO::FileReader reader;
	mstl::String module;
	freyja::PluginDesc plugin;
	const char *module_filename;
	void (*init)();
	void *handle;
	unsigned int i;


	gFreyjaPlugins.erase();

	freyjaPluginBegin();
	freyjaPluginDescription1s("All files (*.*)");
	freyjaPluginAddExtention1s("*.*");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	freyjaPluginBegin();
	freyjaPluginDescription1s("Freyja Model (*.ja)");
	freyjaPluginAddExtention1s("*.ja");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();



	freyjaPluginBegin();
	freyjaPluginDescription1s("Freyja Model (*.freyja)");
	freyjaPluginAddExtention1s("*.freyja");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_BLENDING |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginEnd();


	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while ((module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			if (!SystemIO::CheckModuleExt(module_filename))
			{
				if (SystemIO::CheckFilenameExt(module_filename, ".py"))
					freyjaPython1s(module_filename, "<symbol>InitPlugin</symbol>", "init;");
					
				continue;
			}

			freyjaPrintMessage("Module '%s' invoked.", module_filename);
			handle = freyjaModuleLoad(module_filename);

			if (handle)
			{
				init = (void (*)())freyjaModuleImportFunction(handle, "freyja_init");

				if (!init)
				{
					freyjaModuleUnload(handle);
					continue;
				}

				freyjaPluginBegin();
				freyjaPluginFilename1s(module_filename);
				(*init)();
				freyjaPluginEnd();

				freyjaModuleUnload(handle);
			}
		}

		reader.Close();
	}

#   if 0   // useless really  =)
	/* Sort the plugins for interface usage */
	gFreyjaPlugins.qSort(qSort_FreyjaPluginDesc);

	foreach (gFreyjaPlugins, i)
	{
		DEBUG_MSG("\t+ '%s', '%s'\n", 
				  gFreyjaPlugins[i]->mDescription.c_str(),
				  gFreyjaPlugins[i]->mExtention.c_str());
	}

#   endif

	gCurrentFreyjaPlugin = -1;
#endif
}


int32 freyjaImportModel(const char *filename)
{
	SystemIO::FileReader reader;

	if (!reader.DoesFileExist(filename))
	{
		freyjaPrintError("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	if (SystemIO::File::CompareFilenameExtention(filename, ".freyja") == 0)
	{
		SystemIO::TextFileReader tr; 

		if (tr.Open(filename))
		{
			tr.ParseSymbol(); // Freyja
			tr.ParseSymbol(); // 0.9.5

			tr.ParseSymbol(); // Version
			tr.ParseInteger(); // == 1

			tr.ParseSymbol(); // mMeshCount
			uint32 mMeshCount = tr.ParseInteger();

			tr.ParseSymbol(); // mBoneCount
			uint32 mBoneCount = tr.ParseInteger();

			tr.ParseSymbol(); // mSkeletonCount
			uint32 mSkeletonCount = tr.ParseInteger();

			tr.ParseSymbol(); // mMaterialCount
			uint32 mMaterialCount = tr.ParseInteger();

			uint32 count = mMaterialCount;
			if (count)
			{
				freyjaMaterialClearAll();
			}
			while (count > 0)
			{
				freyjaMaterialLoadChunkTextJA(tr);
				--count;
			}

			count = mMeshCount;
			while (count > 0)
			{
				freyjaMeshLoadChunkTextJA(tr);
				--count;
			}

			count = mBoneCount;
			while (count > 0)
			{
				freyjaBoneLoadChunkTextJA(tr);
				--count;
			}

			count = mSkeletonCount;
			while (count > 0)
			{
				index_t skel = freyjaSkeletonCreate();
				freyjaPrintMessage("> Reading in skeleton %i...", skel);
				if (Skeleton::GetSkeleton(skel))
					Skeleton::GetSkeleton(skel)->Serialize(tr);

				--count;
			}
		}
		return 0;
	}


	/* Check for native freyja JA format */
	if (freyjaCheckModel(filename) == 0)
	{
		if (freyjaLoadModel(filename) == 0)
			return 0;
		
		return -1;
	}


#ifdef FREYJA_PLUGINS
	bool loaded = false, done = false;
	const char *module_filename;
	int (*import)(char *filename);
	void *handle;
	unsigned int i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]");

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory[%d].", i);
			continue;
		}

		while (!done && (module_filename = reader.GetNextDirectoryListing()))
		{
			if (reader.IsDirectory(module_filename))
				continue;

			if (!SystemIO::CheckModuleExt(module_filename))
			{
				if (SystemIO::CheckFilenameExt(module_filename, ".py"))
					freyjaPython1s(module_filename, "<symbol>ImportModel</symbol>", filename);
					
				continue;
			}

			freyjaPrintMessage("Module '%s' invoked.", module_filename);

			if (!(handle = freyjaModuleLoad(module_filename)))
			{
				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				freyjaPrintMessage("Module '%s' opened.", module_filename);

#ifdef NO_DUPE_DL_SYM_HACK
				/* Mongoose 2004.11.01, 
				 * temp fix? */
				unsigned int l = strlen(module_filename);
				char tmp[128];
				module_filename[l-3] = 0;
				snprintf(tmp, 64, "%s_import_model", basename(module_filename));
				freyjaPrintMessage("Symbol '%s' import...", tmp);
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, tmp);
#else
				import = (int (*)(char *filename))freyjaModuleImportFunction(handle, "import_model");
#endif

				if (!import)  
				{
					freyjaModuleUnload(handle);
					continue;
				}


				freyja::PluginDesc *plug = freyjaGetPluginClassByFilename(module_filename); 

				if (plug)
					gCurrentFreyjaPlugin = plug->GetId(); 

				done = !(*import)((char*)filename);

				gCurrentFreyjaPlugin = -1;

				if (done)
				{
					loaded = true;
					freyjaPrintMessage("Module '%s' success.", module_filename);
				}

				freyjaModuleUnload(handle);
			}
		}

		reader.CloseDir();

		if (done)
		{
			break;
		}
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // success
#endif

	return -1;
}


int32 freyjaExportModel(const char *filename, const char *type)
{
	if (!type || !filename)
		return -100;

	/* Check for native format or temp use of EGG here */
	if (strcmp(type, "ja") == 0)
	{
		return freyjaSaveModel(filename); // FIXME: true or false needed?
	}

	if (strcmp(type, "freyja") == 0)
	{
		SystemIO::TextFileWriter tw; 

		if (tw.Open(filename))
		{
			tw.Print("%s\n", FREYJA_API_VERSION);
			tw.Print("Version 1\n");

			uint32 mcount = freyjaGetMeshCount();
			uint32 mallocated = 0;
			for (uint32 i = 0; i < mcount; ++i)
			{
				if (freyjaGetMeshClass(i))
					++mallocated;
			}
				
			tw.Print("mMeshCount %u\n", mallocated);

			uint32 bcount = freyjaGetBoneCount();
			uint32 ballocated = 0;
			for (uint32 i = 0; i < bcount; ++i)
			{
				if (freyjaIsBoneAllocated(i))
					++ballocated;
			}
			
			tw.Print("mBoneCount %u\n", ballocated);

			tw.Print("mSkeletonCount %u\n", Skeleton::GetCount()); // FIXME

			uint32 matcount = freyjaGetMaterialCount();
			uint32 matallocated = 0;
			for (uint32 i = 0; i < matcount; ++i)
			{
				if (freyjaGetMaterialClass(i))
					++matallocated;
			}
			
			tw.Print("mMaterialCount %u\n", matallocated);


			// Classes
			for (uint32 i = 0; i < matcount; ++i)
			{
				if (freyjaGetMaterialClass(i))
					freyjaGetMaterialClass(i)->Serialize(tw);
			}

			for (uint32 i = 0; i < mcount; ++i)
			{
				if (freyjaGetMeshClass(i))
					freyjaMeshSaveChunkTextJA(tw, i);
			}

			for (uint32 i = 0; i < bcount; ++i)
			{
				if (freyjaIsBoneAllocated(i))
					freyjaBoneSaveChunkTextJA(tw, i);
			}

			for (uint32 i = 0; i < Skeleton::GetCount(); ++i)
			{
				freyjaPrintMessage("> Wrtitng skeleton %i...", i);
				if (Skeleton::GetSkeleton(i))
					Skeleton::GetSkeleton(i)->Serialize(tw);
			}
		}
		return 0;
	}


#ifdef FREYJA_PLUGINS
	SystemIO::FileReader reader;
	mstl::String module, symbol;
	bool saved = false;
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	unsigned long i;


	freyjaPrintMessage("[FreyjaPlugin module loader invoked]\n");

	name = (char*)type;

	/* Check for other format */
	for (i = gPluginDirectories.begin(); i < gPluginDirectories.end(); ++i)
	{
		if (!reader.OpenDir(gPluginDirectories[i].c_str()))
		{
			freyjaPrintError("Couldn't access plugin directory");
			continue;
		}

		module.Set("%s/%s%s", 
				   gPluginDirectories[i].c_str(), name, 
				   SystemIO::GetModuleExt());
		symbol.Set("freyja_model__%s_export", name);


		if (!(handle = freyjaModuleLoad(module.c_str())))
		{
			module.Set("%s/%s.py", gPluginDirectories[i].c_str(), name);
			freyjaPython1s(module.c_str(), "<symbol>ExportModel</symbol>", filename);
		}
		else
		{
			freyjaPrintMessage("\tModule '%s' opened.\n", module.c_str());
    
			export_mdl = (int (*)(char * filename))freyjaModuleImportFunction(handle, symbol.c_str());

			freyja::PluginDesc *plug = freyjaGetPluginClassByFilename(module.c_str()); 

			if (plug)
				gCurrentFreyjaPlugin = plug->GetId(); 

			if (export_mdl)
				saved = (!(*export_mdl)((char*)filename));

			gCurrentFreyjaPlugin = -1;

			freyjaModuleUnload(handle);
		}

		if (saved)
			break;
	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // success
#endif

	return -1;
}


int32 freyjaExportModelByModule(const char *filename, const char *module)
{
#ifdef FREYJA_PLUGINS
	freyja::PluginDesc *plugin = freyjaGetPluginClassByFilename(module);

	if (!plugin || !SystemIO::File::DoesFileExist(plugin->mFilename.c_str()))
	{
		freyjaPrintError("Module '%s' couldn't be found.", module);
		return -1;
	}
												
	String symbol = "freyja_model__";
	symbol += plugin->mName;
	symbol += "_export";

	freyjaPrintError("! *** %s", symbol.c_str());

	bool saved = false;
	int (*export_func)(char *filename);
	void *handle = freyjaModuleLoad(module);

	if (handle)
	{
		freyjaPrintMessage("\tModule '%s' opened.\n", module);

		export_func = (int (*)(char * filename))freyjaModuleImportFunction(handle, symbol.c_str());

		gCurrentFreyjaPlugin = plugin->GetId(); 

		if (export_func)
			saved = (!(*export_func)((char*)filename));

		gCurrentFreyjaPlugin = -1;

		freyjaModuleUnload(handle);
	}

	return saved ? 0 : -2;
#endif

	return -1;
}


void freyjaPluginName(uint32 pluginIndex, const char *name)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetName(name);
	}
}


void freyjaPluginDescription(uint32 pluginIndex, const char *info_line)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetDescription(info_line);
	}
}


void freyjaPluginImportFlags(uint32 pluginIndex, int32 flags)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mImportFlags = flags;
	}
}


void freyjaPluginExportFlags(uint32 pluginIndex, int32 flags)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->mExportFlags = flags;
	}
}

void freyjaPluginExtention(uint32 pluginIndex, const char *ext)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginIndex);

	if (plugin)
	{
		plugin->SetExtention(ext);
	}
}


uint32 freyjaGetPluginCount()
{
	return gFreyjaPlugins.end();
}


void freyjaPluginShutdown()
{
	gFreyjaPlugins.erase();
}


///////////////////////////////////////////////////////////////////////
//  Plugin import/export iteraction
///////////////////////////////////////////////////////////////////////

void freyjaPluginBegin()
{
	freyja::PluginDesc *plugin = new freyja::PluginDesc();
	plugin->SetId(gFreyjaPlugins.size());
	gFreyjaPlugins.pushBack(plugin);
	
	gCurrentFreyjaPlugin = plugin->GetId();
}


void freyjaPluginName1s(const char *name)
{
	//OBS_CALL(freyjaPluginName1s);
	freyjaPluginName(gCurrentFreyjaPlugin, name);
}


void freyjaPluginDescription1s(const char *info_line)
{
	//OBS_CALL(freyjaPluginDescription1s);
	freyjaPluginDescription(gCurrentFreyjaPlugin, info_line);
}


void freyjaPluginAddExtention1s(const char *ext)
{
	//OBS_CALL(freyjaPluginAddExtention1s);
	freyjaPluginExtention(gCurrentFreyjaPlugin, ext);
}


void freyjaPluginImport1i(int32 flags)
{
	//OBS_CALL(freyjaPluginImport1i);
	freyjaPluginImportFlags(gCurrentFreyjaPlugin, flags);
}


void freyjaPluginExport1i(int32 flags)
{
	//OBS_CALL(freyjaPluginExport1i);
	freyjaPluginExportFlags(gCurrentFreyjaPlugin, flags);
}

//FIXME these need direct index functions too
void freyjaPluginArg1i(const char *name, int32 defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddIntArg(name, defaults);
}


void freyjaPluginArg1f(const char *name, float defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddFloatArg(name, defaults);
}


void freyjaPluginArg1s(const char *name, const char *defaults)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		plugin->AddStringArg(name, defaults);
}


void freyjaPluginEnd()
{
	// ATM this does nothing, just here for reserved use
}


int32 freyjaGetPluginId()
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(gCurrentFreyjaPlugin);

	if (plugin)
		return plugin->GetId();

	return -1;
}


int freyjaGetPluginArg1f(int32 pluginId, const char *name, float *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetFloatArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1i(int32 pluginId, const char *name, int32 *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetIntArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArg1s(int32 pluginId, const char *name, char **arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);

	if (plugin)
	{
		*arg =  plugin->GetStringArg(name);
		return 0;
	}

	return -1;
}


int freyjaGetPluginArgString(int32 pluginId, const char *name, 
							 int32 len, char *arg)
{
	freyja::PluginDesc *plugin = freyjaGetPluginClassByIndex(pluginId);
	char *s = 0x0;

	if (plugin)
		s = plugin->GetStringArg(name);

	if (!s || !s[0])
		return -1;

	strncpy(arg, s, len);

	return 0;
}

