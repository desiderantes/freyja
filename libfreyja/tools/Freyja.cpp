/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaUtil
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides a command line interface translation/test tool.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.08:
 * Mongoose - Created
 ==========================================================================*/

#include "Freyja.h"

void freyja__setPrinter(Printer *printer, bool freyjaManaged);


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaUtil::FreyjaUtil()
{
#ifdef unix
	char pluginDir[4096];
	char *env;


	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
	}

	snprintf(pluginDir, 4094, "%s/.freyja/plugins/", env);
	pluginDir[4095] = 0;
#else
	char *pluginDir = "C:\freyja\plugins";
#endif

	freyja__setPrinter(&mPrinter , false);
	freyjaSpawn();
}


FreyjaUtil::~FreyjaUtil()
{
	freyjaFree();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int FreyjaUtil::exportModel(const char *filename, const char *type)
{
	return -1;//freyjaExportModel(filename, type);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int FreyjaUtil::importModel(const char *filename)
{
	return freyjaLoadModel(filename); //freyjaImportModel(filename);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAUTIL
int runFreyjaUtilUnitTest(int argc, char *argv[])
{
	FreyjaUtil test;

	switch (argc)
	{
	case 4: // util file.grn 3ds file.3ds
		test.importModel(argv[1]);
		test.exportModel(argv[3], argv[2]);
		break;


	case 2:
		test.importModel(argv[1]);
		break;


	default:
		printf("Usage: %s import_model [type export_model]\n", argv[0]);
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaUtil class test]\n");

	return runFreyjaUtilUnitTest(argc, argv);
}
#endif
