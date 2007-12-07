/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : MtkImage
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: This is the new mtkImage class for mtk
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-10-05:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#ifdef FREYJAIMAGE_PLUGINS
#   include "PluginABI.h"
#endif

#include "FreyjaImage.h"


#define FREYJAIMAGE_COLOR_WEIGHT_GREYSCALE 0


using namespace mstl;

extern Vector<mstl::String> gImagePluginDirectories;

FreyjaImage::FreyjaImage() :
	_image(NULL),
	_palette(NULL),
    _width(0),
    _height(0),
    _original_width(0),
    _original_height(0),
    _color_mode(RGB_24),
	_image_bpp(0),
	_palette_bpp(0)
{
}


FreyjaImage::FreyjaImage(const FreyjaImage &img) :
	_image(NULL),
	_palette(NULL),
    _width(img._width),
    _height(img._height),
    _original_width(img._original_width),
    _original_height(img._original_height),
    _color_mode(img._color_mode),
	_image_bpp(img._image_bpp),
	_palette_bpp(img._palette_bpp)
{
	if (img._image)
	{
		unsigned int size = img._width * img._height * _image_bpp;
		_image = new unsigned char[size];
		memcpy(_image, img._image, size);
	}

	if (img._palette)
	{
		// FIXME: Wow, the palette system looks broken in here
		// no time to fix it in dev branch needing more important feature first
		unsigned int size = 768;// img._width * img._height * _palette_bpp;
		_palette = new unsigned char[size];
		memcpy(_palette, img._palette, size);
	}
}


FreyjaImage::~FreyjaImage()
{
	erase();
}


void FreyjaImage::erase()
{
   if (_image)
      delete [] _image;
	
   if (_palette)
      delete [] _palette;
	
   _width = 0;
   _height = 0;  
   _original_width = 0;
   _original_height = 0;
   _palette_bpp = 0;
}


int FreyjaImage::getWidth()
{
	return _width;
}


int FreyjaImage::getHeight()
{
	return _height;
}


int FreyjaImage::getOriginalWidth()
{
  return _original_width;
}


int FreyjaImage::getOriginalHeight()
{
	return _original_height;
}


void FreyjaImage::setColorMode(colormode_t mode)
{
	if (!_image || _width < 1 || _height < 1)
		return;

	if ( mode == RGB_24 )
	{
		switch ( _color_mode )
		{
		case INDEXED_8: // If this is called to covert 'indexed_8' it's really greyscale.
			{
				unsigned char* pixmap = new unsigned char[ _width * _height * 3 ];
		
				for ( unsigned int i = 0, n = _width * _height; i < n; ++i )
				{
					const unsigned int idx = i * 3;
#if FREYJAIMAGE_COLOR_WEIGHT_GREYSCALE
					/* Convert greyscale using 'obsolete' color weights.
					 * I forgot why this filter was ever used, so it's disabled by default. */ 
					const unsigned char pixel = _image[i];
					pixmap[idx  ] = ( pixel >> 2 ); // ~ pixel * 0.30f;
					pixmap[idx+1] = ( pixel >> 1 ); // ~ pixel * 0.59f;
					pixmap[idx+2] = ( pixel >> 3 ); // ~ pixel * 0.11f;
#else
					pixmap[idx] = pixmap[idx+1] = pixmap[idx+2] = _image[i];
#endif
				}

				delete [] _image;
				_image = pixmap;
				_color_mode = RGB_24;
			}
			break;

		case RGB_24:
			break;

		case RGBA_32:
			{
				unsigned char* image = new unsigned char[_width * _height * 3];
			
				for (unsigned int i = 0, size = _width * _height; i < size; ++i)
				{
					const unsigned int idx = i*3;
					const unsigned int idx2 = i*4;
					image[idx  ] = _image[idx2  ];
					image[idx+1] = _image[idx2+1];
					image[idx+2] = _image[idx2+2];
					// Alpha componet not used in RGB_24.
				}

				delete [] _image;
				_image = image;
				_color_mode = RGB_24;
			}
			break;
		
		default:
			;

		}
	}

}


//FIXME: Add support for 32bit!
int FreyjaImage::setPalette(unsigned char *buffer, int bpp)
{
  if (buffer && buffer != _palette && 
      (bpp == 8 || bpp == 16 || bpp == 24))
  {
    if (_palette)
      delete [] _palette;

    _palette_bpp = bpp;

    _palette = new unsigned char[768];
    memcpy(_palette, buffer, 768);
      
    return 0;
  }

  printf("FreyjaImage::LoadPaletteBuffer()> Assertion Error bpp not {8,16,24}\n");
  return -1;
}


int FreyjaImage::loadIndexedPixmap(unsigned char *image, int width, int height)
{
  int i, size, index;


  if (!image || /* !_palette || */ !width || !height || image == _image)
  {
    printf("FreyjaImage::LoadIndexedBuffer> Assertion failed\n");
    return -1;
  }

  if (_image)
    delete [] _image;

  size = width * height;

  _width = _original_width = width;
  _height = _original_height = height;


  if (!_palette) // 2007.06.16 -- wow, this old ass code sucks, let's allow 8bit here
  {
	  _image = new unsigned char[size];
	  memcpy(_image, image, size);
	  _color_mode = INDEXED_8;
	  return 0;
  }

  _image = new unsigned char[size * 3];
  _color_mode = RGB_24;


  // Greyscale -- man, first update since 1998  =)
  if (!_palette)
  {
	  for (i = 0; i < size; ++i)
	  {
		  index = image[i];

		  // Let's not bother with correction for now
		  // greyscale = 0.30 * red + 0.59 * green + 0.11 * blue

		  _image[i*3] = index;
		  _image[i*3+1] = index;
		  _image[i*3+2] = index;
	  }

	  return 0;
  }


  for (i = 0; i < size; ++i)
  {
    index = image[i];

    if (index * 3 + 2 > 768)
	{
      printf("Index outside of palette!\n");
	}
    else
    {
      _image[i*3] = _palette[index*3];
      _image[i*3+1] = _palette[index*3+1];
      _image[i*3+2] = _palette[index*3+2];
    }
  }

  return 0;
}


void FreyjaImage::brightenPalette(float p)
{
  int i;


  if (p <= 0.0 || p > 0.999)
  {
    printf("FreyjaImage::BrightenPalette> %f isn't a postive percentage\n", p);
    return;
  }

  if (_palette)
  {
    for (i = 0; i < 255; ++i)
    {
      _palette[i*3] += (unsigned char)(_palette[i*3] * p);
      _palette[i*3+1] += (unsigned char)(_palette[i*3+1] * p);
      _palette[i*3+2] += (unsigned char)(_palette[i*3+2] * p);
    }    
  }
}


void FreyjaImage::getImage(unsigned char **buffer)
{
	*buffer = NULL;

	if (!_image)
	{
		printf("Image::ImageBuffer> No image allocated\n");
		return;
	}
	
	switch (_color_mode)
	{
	case RGB_24:
		*buffer = new unsigned char[_width*_height*3];
		memcpy(*buffer, _image, _width*_height*3);
		break;

	case RGBA_32:
		*buffer = new unsigned char[_width*_height*4];
		memcpy(*buffer, _image, _width*_height*4);
		break;

	case INDEXED_8:
		*buffer = new unsigned char[_width*_height];
		memcpy(*buffer, _image, _width*_height);
		break;
	}
}


void FreyjaImage::getPalette(unsigned char **buffer)
{
  *buffer = NULL;

  if (!_palette)
  {
    printf("Image::PaletteBuffer> No palette allocated\n");
    return;
  }

  *buffer = new unsigned char[768];
  memcpy(*buffer, _palette, 768);
}


void FreyjaImage::getIndexedImage(unsigned char **buffer)
{
  int size = _width * _height;
  float rgb[3];
  int i;


  *buffer = NULL;

  if (!_image || !_palette)
  {
    printf("FreyjaImage::IndexedImage> No image or palette allocated\n");
    return;
  }

  *buffer = new unsigned char[size];

  for (i = 0; i < size; ++i)
  {
    rgb[0] = _image[i*3]/255.0;
    rgb[1] = _image[i*3+1]/255.0;
    rgb[2] = _image[i*3+2]/255.0;

    (*buffer)[i] = matchPaletteColor(rgb);
  }
}

void FreyjaImage::flipVertical()
{
  int i, j;
  unsigned char *swap_row = NULL;


  if (!_image)
    return;

  switch(_color_mode)
  {
  case INDEXED_8:
    // We don't store images as indexed, they're computed
    break;
  case RGB_24:
    swap_row = new unsigned char[_width*3];

    for (i = 0, j = _height-1; i < _height/2; ++i, --j)
    {
      memcpy(swap_row, &_image[i*_width*3], _width*3);
      memcpy(&_image[i*_width*3], &_image[j*_width*3], _width*3);
      memcpy(&_image[j*_width*3], swap_row, _width*3);
    }

    delete [] swap_row;
    break;
  case RGBA_32:
    swap_row = new unsigned char[_width*4];

    for (i = 0, j = _height-1; i < _height/2; ++i, --j)
    {
      memcpy(swap_row, &_image[i*_width*4], _width*4);
      memcpy(&_image[i*_width*4], &_image[j*_width*4], _width*4);
      memcpy(&_image[j*_width*4], swap_row, _width*4);
    }

    delete [] swap_row;
    break;
  }
}


int FreyjaImage::loadPixmap(unsigned char *image, 
								 unsigned int w, unsigned int h,
								 colormode_t mode)
{
	if (!image || !w || !h || image == _image)
      return -1;
	
	if (_image)
	{
		delete [] _image;
		_image = 0x0;
	}

	switch (mode)
	{
	case INDEXED_8:
		return loadIndexedPixmap(image, w, h);
		break;
		
	case RGB_24:
		_color_mode = RGB_24;
		_width = _original_width = w;
		_height = _original_height = h;
		_image = new unsigned char[w * h * 3];
		memcpy(_image, image, w * h * 3);
		return 0;
		break;
		
	case RGBA_32:
		_color_mode = RGBA_32;
		_width = _original_width = w;
		_height = _original_height = h;		
		_image = new unsigned char[w * h * 4];
		memcpy(_image, image, w * h * 4);
		return 0;
		break;
	}
	
	return -1;
}


// Palette ////////////////////////////////////////////

// Search palette for matching ( or closest ) rgb color
// Colors *must be 0.0 to 1.0 form!
//
// Note if you have a small palette this may 'wash' to black
// quickly after running through a few times ( 0.0 is default )
unsigned int FreyjaImage::matchPaletteColor(color3_t rgb)
{
  color3_t color;
  float best_weight, current_weight;
  int i, best;


  if (!_palette)
  {
    printf("FreyjaImage::MatchColor> ERROR no palette loaded\n");
    return 0;
  }

  best_weight = current_weight = 0.0;
  
  for (best = 0, i = 0; i < 256; i++)
  {
    // Convert from 0-255 to 0.0-1.0 color scale
    getPaletteColor(i, color);
    //color[0] = _palette[i*3]/255.0;
    //color[1] = _palette[i*3+1]/255.0;
    //color[2] = _palette[i*3+2]/255.0;

    // FIXME: Should be 0.0 (far off) to 1.0 (exact match)
    current_weight = 1.0 - ((fabs(rgb[0] - color[0]) + 
			     fabs(rgb[1] - color[1]) + 
			     fabs(rgb[2] - color[2])) / 3.0);

    if (current_weight > best_weight)
    {
      best_weight = current_weight;
      best = i;

      if (best_weight == 1.0)
	i = 1024; // break;
    }
  }

  return best;
}

void FreyjaImage::getPaletteColor(unsigned int i, color3_t rgb)
{
  if (i > 0 && i < 256 && rgb && _palette)
  {
    rgb[0] = (float)_palette[i*3]/255.0;
    rgb[1] = (float)_palette[i*3+1]/255.0;
    rgb[2] = (float)_palette[i*3+2]/255.0;
  }
}	


int FreyjaImage::loadImage(const char *filename)
{
#ifdef FREYJAIMAGE_PLUGINS
	SystemIO::FileReader reader;
	int (*import_img)(char *filename, unsigned char **image,
					  unsigned int *width, unsigned int *height, 
					  char *type);
	bool done = false;
	const char *module_filename;
	void *handle;
	unsigned char *image = 0x0;
	unsigned int width = 0, height = 0;
	char type = 0;


	if (!reader.DoesFileExist(filename))
	{
		print("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	print("[FreyjaImage plugin system invoked]");

	// It IS very funny to see a C++ foreach implementation along side code that
	// was used with software renderers years ago  =)
	unsigned int i;
	foreach (gImagePluginDirectories, i)
	{
		if (!reader.OpenDir(gImagePluginDirectories[i].c_str()))
		{
			printError("Couldn't access image plugin directory");
			continue;
		}

	while (!done && (module_filename = reader.GetNextDirectoryListing()))
	{
		if (reader.IsDirectory(module_filename))
			continue;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			print("Module '%s' opened.", module_filename);
    
			import_img = (int (*)(char *filename, unsigned char **image,
										 unsigned int *width, unsigned int *height, 
										 char *type))freyjaModuleImportFunction(handle, "import_image");

			if (import_img == NULL)  
			{
				freyjaModuleUnload(handle);
				continue;
			}

			done = !(*import_img)((char*)filename, &image, &width, &height, &type);

			freyjaModuleUnload(handle);
		}
	}

	reader.CloseDir();

	print("[FreyjaPlugin module loader sleeps now]\n");

	if (done && image && width > 0 && height > 0)
	{
		print("%p - %ux%u %ibpp", image, width, height, type*8);

		switch (type)
		{
		case 1:
			loadPixmap(image, width, height, INDEXED_8);
			delete [] image;
			return 0; /* Success! */			
			break;

		case 3:
			loadPixmap(image, width, height, RGB_24);
			delete [] image;
			return 0; /* Success! */
			break;

		case 4:
			loadPixmap(image, width, height, RGBA_32);
			delete [] image;
			return 0; /* Success! */
			break;

		default:
			;
		}
	}
	}
#else
	print("FreyjaImage: This build was compiled w/o plugin support");
#endif

	return -1;
}


int FreyjaImage::loadPaletteLMP(const char *filename)
{
  FILE *f;
	

  f = fopen(filename, "rb");

  if (f)
  {
    if (!_palette)
      _palette = new unsigned char[768];

    fread(_palette, 1, 768, f);
    fclose(f);
    return 0;
  }

  return -1;
}

int FreyjaImage::loadPaletteMTK(const char *filename)
{
  FILE *f;
  int i;
  unsigned int rgb[3];


  if (!filename)
    return -1;

  f = fopen(filename, "r");

  if (f)
  {
    if (_palette)
      delete [] _palette;

    _palette = new unsigned char[768];

    for (i = 0; i < 768; i+=3)
    {
      fscanf(f, "%u %u %u\n", 
	     &rgb[0],
	     &rgb[1],
	     &rgb[2]);

      _palette[i]   = rgb[0];
      _palette[i+1] = rgb[1];
      _palette[i+2] = rgb[2];
    }
    
    fclose(f);
    return 0;
  }

  return 1;
}


int FreyjaImage::saveImage(const char *filename, const char *module_name)
{
#ifdef FREYJAIMAGE_PLUGINS
	SystemIO::FileReader reader;
	char symbol[256];
	int (*export_img)(char *filename, unsigned char *image,
					  unsigned int width, unsigned int height, 
					  char type);
	bool done = false;
	const char *module_filename;
	void *handle;


	print("[FreyjaImage plugin system invoked]");

	unsigned int i;
	foreach (gImagePluginDirectories, i)
	{
		if (!reader.OpenDir(gImagePluginDirectories[i].c_str()))
		{
			//printError("Couldn't access image plugin directory '%s'",
			//		   gImagePluginDirectories[i].c_str());
			continue;
		}

	while (!done && (module_filename = reader.GetNextDirectoryListing()))
	{
		if (reader.IsDirectory(module_filename))
			continue;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			print("Module '%s' opened.", module_filename);

			snprintf(symbol, 256, "freyja_image_export__%s", module_name);

			export_img = (int (*)(char *filename, unsigned char *image,
								  unsigned int width, unsigned int height, 
								  char type))freyjaModuleImportFunction(handle, symbol);

			if (export_img == NULL)  
			{
				freyjaModuleUnload(handle);
				continue;
			}
			
			done = !(*export_img)((char*)filename, 
								  _image, _width, _height, 
								  ((_color_mode == RGBA_32) ? 4 :
								   (_color_mode == RGB_24) ? 3 : 1));

			freyjaModuleUnload(handle);
		}
	}

	reader.CloseDir();

	}

	print("[FreyjaPlugin module loader sleeps now]\n");

	if (done)
		return 0;
#else
	print("FreyjaImage: This build was compiled w/o plugin support");
#endif

	return -1;
}


int FreyjaImage::savePaletteMTK(const char *filename)
{
  FILE *f;
  int i;


  if (!_palette || !filename)
    return -1;

  f = fopen(filename, "w");

  if (f)
  {
    for (i = 0; i < 768; i+=3)
    {
      fprintf(f, "%i %i %i\n", 
	      (int)_palette[i], 
	      (int)_palette[i+1], 
	      (int)_palette[i+2]);
    }
    
    fclose(f);
    return 0;
  }

  return 1;
}

FreyjaImage::colormode_t FreyjaImage::getColorMode()
{
  return _color_mode;
}


void FreyjaImage::scaleImage()
{
	scaleImage(1024, 1024);
}


/* This code based off on gluScaleImage()  */
void FreyjaImage::scaleImage(unsigned int maxW, unsigned int maxH)
{
   int i, j, k;
   float* tempin;
   float* tempout;
   float sx, sy;
   int components = 3;
   unsigned char *image;
	
	
   if (!_image || !_width || !_height)
		return;

	if (_color_mode == RGBA_32)
	{
		components = 4;
	}

   _height = getNextPower(_height);
   _width = getNextPower(_width);
	
   if (_height > (int)maxH) 
		_height = maxH;

   if (_width > (int)maxW) 
		_width = maxW;

	
   // Check to see if scaling is needed
   if (_height == _original_height && _width == _original_width) 
		return;
	
   image = new unsigned char[_height * _width * components];
	
   tempin = new float[_original_width * _original_height * 
							 components * sizeof(float)];
   tempout = new float[_width * _height * components * sizeof(float)];
	
   if (!tempout || !tempin)
   {
		if (tempout)
			delete [] tempout;
		
		if (tempin)
			delete [] tempin;
		
		printf("Oh shit out of memory!\n");
		return;
   }

   // Copy user data to float format.
   for (i = 0; i < _original_height * _original_width * components; i++) 
   {
		tempin[i] = (float)_image[i];
   }
	
   // Determine which filter to use by checking ratios.
   if (_width > 1) 
   {
		sx = (float)(_original_width - 1) / (float)(_width - 1);
   } 
   else 
   {
		sx = (float)(_original_width - 1);
   }
	
   if (_height > 1)
   {
		sy = (float)(_original_height - 1) / (float) (_height - 1);
   } 
   else 
   {
		sy = (float)(_original_height - 1);
   }
	
   if (sx < 1.0 && sy < 1.0) 
   {
		/* Magnify both width and height:  use weighted sample of 4 pixels */
		int i0, i1, j0, j1;
		float alpha, beta;
		float* src00;
		float* src01;
		float* src10;
		float* src11;
		float s1, s2;
		float* dst;
		
		for(i = 0; i < _height; i++) 
		{
			i0 = (int)(i * sy);
			i1 = i0 + 1;
			
			if (i1 >= _original_height) 
			{
				i1 = _original_height - 1;
			}
			
			alpha = i * sy - i0;
			
			for (j = 0; j < _width; j++) 
			{
				j0 = (int) (j * sx);
				j1 = j0 + 1;
				
				if (j1 >= _original_width) 
				{
					j1 = _original_width - 1;
				}
				
				beta = j * sx - j0;
				
				/* Compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
				src00 = tempin + (i0 * _original_width + j0) * components;
				src01 = tempin + (i0 * _original_width + j1) * components;
				src10 = tempin + (i1 * _original_width + j0) * components;
				src11 = tempin + (i1 * _original_width + j1) * components;

				dst = tempout + (i * _width + j) * components;
				
				for (k = 0; k < components; k++) 
				{
					s1 = *src00++ * (1.0 - beta) + *src01++ * beta;
					s2 = *src10++ * (1.0 - beta) + *src11++ * beta;
					*dst++ = s1 * (1.0 - alpha) + s2 * alpha;
				} 
			}
		}     
   }
   else 
   {
		/* Shrink width and/or height:  use an unweighted box filter */
		int i0, i1;
		int j0, j1;
		int ii, jj;
		float sum;
		float* dst;
		
		for (i = 0; i < _height; i++) 
		{
			i0 = (int) (i * sy);
			i1 = i0 + 1;
			
			if (i1 >= _original_height) 
			{
				i1 = _original_height - 1;
			}
			
			for (j = 0; j < _width; j++) 
			{
				j0 = (int) (j * sx);
				j1 = j0 + 1;
				
				if (j1 >= _original_width) 
				{
					j1 = _original_width - 1;
				}
				
				dst = tempout + (i * _width + j) * components;
				
				/* Compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
				for (k = 0; k < components; k++) 
				{
					sum = 0.0;
					
					for (ii = i0; ii <= i1; ii++) 
					{
						for (jj = j0; jj <= j1; jj++) 
						{
							sum += *(tempin + (ii * _original_width + jj) 
										* components + k);
						}
					}
					
					sum /= ( j1 - j0 + 1 ) * ( i1 - i0 + 1 );
					*dst++ = sum;
				}
			} 
		}
   }
	
   // Copy to our results.
   for( i = 0; i < _height * _width * components; i++ ) 
   {
		image[i] = (unsigned char) tempout[i];
   }
	
   // Delete our temp buffers.
   delete[] tempin;
   delete[] tempout;
   delete[] _image;
	
   _image = image;
}

int FreyjaImage::getNextPower(int seed)
{
  int i;

  for(i = 1; i < seed; i *= 2) 
    ;

  return i;
}


void FreyjaImage::printError(char *format, va_list *args)
{

	fprintf(stdout, "FreyjaImage> ");
	vfprintf(stdout, format, *args);
	fprintf(stdout, "\n");
}


void FreyjaImage::printMessage(char *format, va_list *args)
{
	fprintf(stderr, "FreyjaImage> ");
	vfprintf(stderr, format, *args);
	fprintf(stderr, "\n");
}


void FreyjaImage::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	printMessage(s, &args);
	va_end(args);
}


void FreyjaImage::printError(char *s, ...)
{
	va_list args;
	
	va_start(args, s);
	printError(s, &args);
	va_end(args);
}
