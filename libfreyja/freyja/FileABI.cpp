/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: This is the File C ABI, which is great for Lua and Python use.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.24:
 * Mongoose - Created.
 ==========================================================================*/

#include "FileABI.h"


///////////////////////////////////////////////////////////////////////
// freyjaFile 0.10.0 ABI
//
///////////////////////////////////////////////////////////////////////

freyja_ptr
freyjaFileOpen( const char* filename, 
				const char* mode ) 
{
#warning FIXME
	return NULL;
}


void 
freyjaFileClose( freyja_ptr file )
{
#warning FIXME
}
