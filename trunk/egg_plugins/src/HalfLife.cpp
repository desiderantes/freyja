/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : HalfLife
 * Comments: This is the HalfLife model class.
 *
 *           See file COPYING for license details.
 *
 *           Load based on code from Quest by ?? 
 *
 *           HalfLife (c) 1998, Valve LLC. All rights reserved.            
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2000-07-22:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <string.h>

#include "HalfLife.h"


HalfLife::HalfLife()
{
	memset(&mHeader, 0, sizeof(studiohdr_t));

	mBodyParts = 0x0;
	mModels = 0x0;
	mMeshes = 0x0;
	mBones = 0x0;
	mVertices = 0x0;
	mFaces = 0x0;
	mTextures = 0x0;
	mImages = 0x0;

	mMeshCount = 0;
	mVertexCount = 0;
	mBoneCount = 0;
	mModelCount = 0;
	mBodyPartCount = 0;
	mTextureCount = 0;
}


HalfLife::~HalfLife()
{
	unsigned int i;


	if (mTextures)
		delete [] mTextures;

	if (mImages)
	{
		for (i = 0; i < mTextureCount; ++i)
		{
			if (mImages[i].image)
				delete [] mImages[i].image;
		}

		delete [] mImages;
	}
}


int HalfLife::load(const char *filename)
{
	FILE *f;
	unsigned int i, j, k; //, count, offset;


	f = fopen(filename, "rb");

	if (!f)
	{
		printf("HalfLife::load> Couldn't load file.\n");
		perror(filename);

		return -1;
	}

	fread(&mHeader, 1, sizeof(studiohdr_t), f);

	if (mHeader.id != 0x54534449)
	{
		printf("Not a valid halflife model\n");
		fclose(f);
		return -2;
	}

	printf("Halflife {\n");
	printf("\tversion = 0x%x\n", mHeader.id);
	printf("\tname = '%s'\n", mHeader.name);

	printf(" eyepos <%g %g %g>\n min <%g %g %g>\n max <%g %g %g>\n",
		   mHeader.eyeposition[0], mHeader.eyeposition[1],
		   mHeader.eyeposition[2],
		   mHeader.min[0], mHeader.min[1], mHeader.min[2],
		   mHeader.max[0], mHeader.max[1], mHeader.max[2]);

	printf(" bbmin <%g %g %g>\n bbmax <%g %g %g>\n",
		   mHeader.bbmin[0], mHeader.bbmin[1], mHeader.bbmin[2],
		   mHeader.bbmax[0], mHeader.bbmax[1], mHeader.bbmax[2]);

	printf(" flags %i\n", mHeader.flags);
	printf(" bones %i\n boneindex %i\n", mHeader.numbones, mHeader.boneindex);
	printf(" numseqgroups %i \n", mHeader.numseqgroups);
	printf(" seqgroupindex %i\n", mHeader.seqgroupindex);
	printf(" bodyparts %i\n bodypartindex %i\n}\n\n",
		   mHeader.numbodyparts, mHeader.bodypartindex);


	/* Load Textures */
	if (mHeader.textureindex > 0 && mHeader.numtextures <= MAXSTUDIOSKINS)
	{
		byte palette[768]; // 256 RGB colors
		byte *image = 0x0;
		unsigned int size;
		byte pixel;


		mTextureCount = mHeader.numtextures;
		mTextures = new mstudio_texture_t[mTextureCount];
		mImages = new hl_image_t[mTextureCount];

		fseek(f, mHeader.textureindex, SEEK_SET);
		fread(mTextures, mTextureCount, sizeof(mstudio_texture_t), f);
    
		for (i = 0; i < mTextureCount; ++i)
		{
			printf("Texture[%i] %ix%i index= %i\n", 
				   i, 
				   mTextures[i].width, mTextures[i].height,
				   mTextures[i].index);

			if (mTextures[i].width < 1 || mTextures[i].height < 1)
			{
				mImages[i].image = 0x0;
				mImages[i].w = 0;
				mImages[i].h = 0;
				continue;
			}

			size = mImages[i].w * mImages[i].h;

			mImages[i].w = mTextures[i].width;
			mImages[i].h = mTextures[i].height;
			mImages[i].image = new byte[size * 3];

			image = new byte[size];

			fseek(f, mTextures[i].index, SEEK_SET);
			fread(image, size, 1, f);

			fseek(f, mTextures[i].index + size, SEEK_SET);
			fread(palette, 1, 768, f);

			/* Convert indexed image to RGB image using palette */
			for (j = 0; j < size; ++j)
			{
				pixel = image[j];

				mImages[i].image[j*3 + 0] = palette[pixel*3 + 0];
				mImages[i].image[j*3 + 1] = palette[pixel*3 + 1];
				mImages[i].image[j*3 + 2] = palette[pixel*3 + 2];
			}

			delete [] image;
		}

		printf("\n");
	}

	///////////////////////////////////////////////////////

	/* Load Bodyparts */
	mstudio_bodyparts_t bodyPart;
	mstudio_model_t model;
	mstudio_mesh_t mesh;
	mstudio_trivert_t vertex;
	unsigned int size;

	mBodyPartCount = mHeader.numbodyparts;
	mBodyParts = new hl_bodypart_t[mBodyPartCount];
	
	printf("Reading %d bodyparts\n\n", mHeader.numbodyparts);

	for (i = 0; i < mBodyPartCount; ++i)
	{
		size = sizeof(mstudio_bodyparts_t);

		fseek(f, mHeader.bodypartindex + i * size, SEEK_SET);
		fread(&bodyPart, 1, size, f);

		printf("mBodyParts[%i].name = '%s'\n", i, bodyPart.name);

		fseek(f, bodyPart.modelindex, SEEK_SET);
		fread(&model, 1, sizeof(mstudio_model_t), f);

		if (model.numverts < 0)
		{
			mBodyParts[i].meshCount = 0;
			mBodyParts[i].meshes = 0x0;
			mBodyParts[i].groupCount = 0;
			mBodyParts[i].groups = 0x0;
			mBodyParts[i].vertexCount = 0;
			mBodyParts[i].vertices = 0x0;
			mBodyParts[i].normalCount = 0;
			mBodyParts[i].normals = 0x0;
			continue;
		}

		mBodyParts[i].meshCount = model.nummesh;
		mBodyParts[i].meshes = new hl_mesh_t[mBodyParts[i].meshCount];
		mBodyParts[i].groupCount = model.numgroups;
		mBodyParts[i].groups = new hl_group_t[mBodyParts[i].groupCount];
		mBodyParts[i].vertexCount = model.numverts;
		mBodyParts[i].vertices = new vec3_t[mBodyParts[i].vertexCount];
		mBodyParts[i].normalCount = model.numnorms;
		mBodyParts[i].normals = new vec3_t[mBodyParts[i].normalCount];

		fseek(f, model.vertindex, SEEK_SET);
		fread(mBodyParts[i].vertices, sizeof(vec3_t), mBodyParts[i].vertexCount, f);

		/*
		for (j = 0; j < mBodyParts[i].vertexCount; ++j)
		{
			size = sizeof(vec_t);

			fread(&mBodyParts[i].vertices[j][0], size, 1, f);
			fread(&mBodyParts[i].vertices[j][1], size, 1, f);
			fread(&mBodyParts[i].vertices[j][2], size, 1, f);
		}
		*/

		short cmd;
		unsigned int aa, bb, a, b, c;

		for (j = 0; j < mBodyParts[i].meshCount; ++j)
		{
			size = sizeof(mstudio_mesh_t);

			fseek(f, model.meshindex + j * size, SEEK_SET);
			fread(&mesh, size, 1, f);

			mBodyParts[i].meshes[j].faceCount = mesh.numtris;
			mBodyParts[i].meshes[j].faces = new hl_face_t[mBodyParts[i].meshes[j].faceCount];
			mBodyParts[i].meshes[j].normalCount = mesh.numnorms;
			mBodyParts[i].meshes[j].material = mesh.skinref;

			fseek(f, mesh.triindex, SEEK_SET);

			size = sizeof(mstudio_trivert_t);

			/* Face loader: triangle fans and strips */
			for (k = 0; cmd && k < mBodyParts[i].meshes[j].faceCount; )
			{
				fread(&cmd, 1, 2, f);

				if (cmd < 0)
				{
					cmd =- cmd;
					aa = bb = k;

					fread(&mBodyParts[i].meshes[j].faces[k].vertex[0], 
						  size, 1, f);
					fread(&mBodyParts[i].meshes[j].faces[k].vertex[1], 
						  size, 1, f);

					a = mBodyParts[i].meshes[j].faces[k].vertex[0].vertindex;
					b = mBodyParts[i].meshes[j].faces[k].vertex[1].vertindex;

					for (cmd -= 3; cmd > 0; --cmd, ++k)
					{
						fread(&mBodyParts[i].meshes[j].faces[k].vertex[2], 
							  size, 1, f);

						c=mBodyParts[i].meshes[j].faces[k].vertex[2].vertindex;

						mBodyParts[i].meshes[j].faces[k].vertex[0].vertindex =
						a;
						mBodyParts[i].meshes[j].faces[k].vertex[1].vertindex =
						b;
						mBodyParts[i].meshes[j].faces[k].vertex[2].vertindex =
						c;

						bb = k;
						b = c;
					}
				}
				else
				{
					aa = bb = k;

					fread(&mBodyParts[i].meshes[j].faces[k].vertex[0], 
						  size, 1, f);
					fread(&mBodyParts[i].meshes[j].faces[k].vertex[1], 
						  size, 1, f);						
					
					a = mBodyParts[i].meshes[j].faces[k].vertex[0].vertindex;
					b = mBodyParts[i].meshes[j].faces[k].vertex[1].vertindex;

					for (cmd -= 3; cmd > 0; cmd--, k++)
					{	
						fread(&mBodyParts[i].meshes[j].faces[k].vertex[2], 
							  size, 1, f);

						c=mBodyParts[i].meshes[j].faces[k].vertex[2].vertindex;

						mBodyParts[i].meshes[j].faces[k].vertex[0].vertindex =
						a;
						mBodyParts[i].meshes[j].faces[k].vertex[1].vertindex =
						b;
						mBodyParts[i].meshes[j].faces[k].vertex[2].vertindex =
						c;

						aa = bb;
						bb = k;						
						a = b;
						b = c;
					}
				}
			}
		}
	}

	mBoneCount = mHeader.numbones;
 	mBones = new mstudio_bone_t[mBoneCount];


#ifdef OBSOLETE
	for (i = 0; i < mHeader.numbodyparts; ++i)
	{

		for (j = 0; j < _mdl[i].nummesh; j++)
		{

			////////////////////FIXME???
			if (mTextures[_mesh[i][j].skinref].flags & STUDIO_NF_CHROME)
			{
				short cmd;
				short a,b,c;
				int aa, bb;
	    

				k = 0;

				fseek(f, _mesh[i][j].triindex, SEEK_SET);
				fread(&cmd, 1, 2, f);

				// Trifan loader        
				while (cmd)
				{
					if (cmd < 0)
					{
						cmd =- cmd;
						fread(&a, 1, 2, f);
						fread(&_tris[i][j][k].norm[0], 1, 2, f);
						fread(&_tris[i][j][k].st[0], 1, 4, f);

						aa = k;

						fread(&b, 1, 2, f);
						fread(&_tris[i][j][k].norm[1], 1, 2, f);
						fread(&_tris[i][j][k].st[1], 1, 4, f);

						bb = k;

						for (cmd -= 2; cmd > 0; cmd--, k++)
						{
#ifdef UNIT_TEST_HALFLIFE
							printf("^ cmd %i\n", cmd);
#endif

							fread(&c, 1, 2, f);
							fread(&_tris[i][j][k].norm[2], 1, 2, f);
							fread(&_tris[i][j][k].st[2], 1, 4, f);
   
							_tris[i][j][k].tri[0] = a;
							_tris[i][j][k].tri[1] = b;
							_tris[i][j][k].tri[2] = c;

							_tris[i][j][k].st[0][0] = _tris[i][j][aa].st[0][0];
							_tris[i][j][k].st[0][1] = _tris[i][j][aa].st[0][1];
							_tris[i][j][k].st[1][0] = _tris[i][j][bb].st[1][0];
							_tris[i][j][k].st[1][1] = _tris[i][j][bb].st[1][1];

#ifdef UNIT_TEST_HALFLIFE
							printf("triangle[%i][%i][%i] {\n  verts %i, %i, %i\n", 
								   i, j, k, a, b, c);
							printf("  norms %i, %i, %i\n", 
								   _tris[i][j][k].norm[0],
								   _tris[i][j][k].norm[1],
								   _tris[i][j][k].norm[2]);
							printf("  st %i,%i %i,%i %i,%i\n}\n\n",
								   _tris[i][j][k].st[0][0],
								   _tris[i][j][k].st[0][1],
								   _tris[i][j][k].st[1][0],
								   _tris[i][j][k].st[1][1],
								   _tris[i][j][k].st[2][0],
								   _tris[i][j][k].st[2][1]);
#endif

							bb = k;
							b = c;
						}
					}
					else
					{
						fread(&a, 1, 2, f);
						fread(&_tris[i][j][k].norm[0], 1, 2, f);
						fread(&_tris[i][j][k].st[0], 1, 4, f);

						aa = k;

						fread(&b, 1, 2, f);
						fread(&_tris[i][j][k].norm[1], 1, 2, f);
						fread(&_tris[i][j][k].st[1], 1, 4, f);
	    
						bb = k;

						for (cmd -= 2; cmd > 0; cmd--, k++)
						{
#ifdef UNIT_TEST_HALFLIFE
							printf("V cmd %i\n", cmd);
#endif

							fread(&c, 1, 2, f);
							fread(&_tris[i][j][k].norm[2], 1, 2, f);
							fread(&_tris[i][j][k].st[2], 1, 4, f);
   
							_tris[i][j][k].tri[0] = a;
							_tris[i][j][k].tri[1] = b;
							_tris[i][j][k].tri[2] = c;
	      
							_tris[i][j][k].st[0][0] = _tris[i][j][aa].st[0][0];
							_tris[i][j][k].st[0][1] = _tris[i][j][aa].st[0][1];
							_tris[i][j][k].st[1][0] = _tris[i][j][bb].st[1][0];
							_tris[i][j][k].st[1][1] = _tris[i][j][bb].st[1][1];

#ifdef UNIT_TEST_HALFLIFE
							printf("triangle[%i][%i][%i] {\n  verts %i, %i, %i\n", 
								   i, j, k, a, b, c);
							printf("  norms %i, %i, %i\n", 
								   _tris[i][j][k].norm[0],
								   _tris[i][j][k].norm[1],
								   _tris[i][j][k].norm[2]);
							printf("  st %i,%i %i,%i %i,%i\n}\n\n",
								   _tris[i][j][k].st[0][0],
								   _tris[i][j][k].st[0][1],
								   _tris[i][j][k].st[1][0],
								   _tris[i][j][k].st[1][1],
								   _tris[i][j][k].st[2][0],
								   _tris[i][j][k].st[2][1]);
#endif	      

							aa = bb;
							bb = k;

							a = b;
							b = c;
						}
					}
	  
#ifdef UNIT_TEST_HALFLIFE
					printf("\n");
#endif
	  
					fread(&cmd,1,2,f);
				}
			}

			// just to be sure
			if (_mesh[i][j].numtris != k)
			{
				_mesh[i][j].numtris = k;  
				printf("HalfLife::Load> ERROR: mesh triangle load fubar\n");
				return -2;
			}
		}
	}
#endif

	return 0;
}

////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_MODEL_PLUGINS

#include <freyja8/EggPlugin.h>


extern "C" {

	int freyja_model__halflife_check(char *filename);
	int freyja_model__halflife_import(char *filename);
}



int freyja_model__halflife_check(char *filename)
{
	FILE *f;
	char header[32];


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("mdl_check> fopen failed");
		return -1;
	}

	fread(&header, 32, 1, f);      
	fclose(f);

	header[4] = 0;

	// HalfLife
	if (strcmp(header, "IDST") == 0)
	{
		return 0;
	}
	return -2;
}


int freyja_model__halflife_import(char *filename)
{
	HalfLife hl;
	Map<unsigned int, unsigned int> trans;
	unsigned int i, b, f, m, vert;
	float u, v, w, h;
	short s, t;


	if (hl.load(filename) < 0)
	{
		return -1;
	}

	printf("Processing HalfLife textures: ");

	for (i = 10; i < hl.mTextureCount; ++i)
	{
		if (hl.mImages[i].image && hl.mImages[i].w > 0 && hl.mImages[i].h > 0)
		{
			eggTextureStoreBuffer(hl.mImages[i].image, 3, 
								  hl.mImages[i].w, hl.mImages[i].h,
								  EGG_COLORMODE_RGB);
		}
	}

	printf("Processing HalfLife bodyparts:\n");

	for (b = 0; b < hl.mBodyPartCount; ++b)
	{
		eggBegin(FREYJA_MESH);
		eggBegin(FREYJA_GROUP);	
		trans.Clear();

		for (i = 0; i < hl.mBodyParts[b].vertexCount; ++i)
		{
			vert = eggVertexStore3f(hl.mBodyParts[b].vertices[i*3][0],
									hl.mBodyParts[b].vertices[i*3][1], 
									hl.mBodyParts[b].vertices[i*3][2]);
			
			// Mongoose 2002.02.09, Generates id translator list
			trans.Add(i, vert);	
		}

		eggEnd(); // FREYJA_GROUP

		for (m = 0; m < hl.mBodyParts[b].meshCount; ++m)
		{
			for (f = 0; f < hl.mBodyParts[b].meshes[m].faceCount; ++f)
			{
				// Start a new polygon
				eggBegin(FREYJA_POLYGON);

				// Store vertices by true id, using translation table
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[0].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[1].vertindex]);
				eggVertex1i(trans[hl.mBodyParts[b].meshes[m].faces[f].vertex[2].vertindex]);


				/* Generate, Store, and link UVs to polygon */
				if (hl.mImages) // should only be null while fixing this plugin
				{
					w = (float)hl.mImages[hl.mBodyParts[b].meshes[m].material].w;
					h = (float)hl.mImages[hl.mBodyParts[b].meshes[m].material].h;

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[0].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[0].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[1].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[1].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					s = hl.mBodyParts[b].meshes[m].faces[f].vertex[2].s;
					t = hl.mBodyParts[b].meshes[m].faces[f].vertex[2].t;
					u = s / w;
					v = t / h;
					eggTexCoord1i(eggTexCoordStore2f(u, v));

					eggTexture1i(hl.mBodyParts[b].meshes[m].material);

					eggEnd(); // FREYJA_POLYGON
				}
			}
		}

		eggEnd(); // FREYJA_MESH
	}

	return 0;
}


int freyja_model__halflife_export(char *filename)
{
  printf("freyja_model__halflife_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);

  return -1;
}

// FIXME: Finish porting EggPlugin code
#ifdef FIXME
void FreyjaModel::HalfLifeImport(HalfLife *hl)
{
  Map<unsigned int> trans;
  egg_mesh_t *mesh;
  egg_tag_t *tag;
  egg_group_t *grp;
  egg_vertex_t *vert;
  MtkImage img;
  int b, t, m, v, i;
  float st[2];
  studiohdr_t *hl_header;
  vec3_t **hl_verts;
  mstudiomesh_t **hl_mesh;
  mstudiomodel_t *hl_mdl;
  trisy_keen_dood_t ***hl_tris;
  hlskin_t *hl_skin;
  char buffer[64];


  if (!_egg || !hl)
    return;

  hl_skin = hl.Skin();
  hl_header = hl.Header();
  hl_mesh = hl.Meshes();
  hl_mdl = hl.Mdls();
  hl_verts = hl.Vertices();
  hl_tris = hl.Tris();


  // FIXME: Texture code not supported yet ////////////////////////////////
#ifdef FIXME
  mtkTextureClear();
  mtkTextureSetNum(hl_header->numtextures);

  for (i = 0; i < hl_header->numtextures; i++)
  {
    mtkTextureLoadBuffer(hl_skin[i].image, hl_skin[i].w, hl_skin[i].h,
			 COLORMODE_RGB);

    if (_defaults & FL_DUMP_TEXTURE)
    {
      img.Load(hl_skin[i].image, hl_skin[i].w, hl_skin[i].h, COLORMODE_RGB);
      sprintf(buffer, "textures/hl-%i.tga", i);
      img.Save(freyja_rc_map(buffer), "tga");
    }
  }
#endif
  ///////////////////////////////////////////////////////////////////////


  for (b = 0; b < hl_header->numbodyparts; b++)
  {
    eggBegin(FREYJA_BONE_FRAME);
    eggBegin(FREYJA_BONE_TAG);

    eggTagCenter(0.0, 0.0, 0.0);

    trans.Clear();

    for (v = 0; v < hl_mdl[b].numverts; v++)
    {
      vert = _egg->VertexAdd(hl_verts[b][v][0], hl_verts[b][v][1], 
			     hl_verts[b][v][2]);

      
      trans.Add(v, vert->id);
    }

    for (m = 0; m < hl_mdl[b].nummesh; m++)
    {
      mesh = _egg->MeshNew();
      _egg->MeshAdd(mesh);
      _egg->TagAddMesh(tag, mesh->id);
      grp = _egg->GroupNew();
      _egg->GroupAdd(grp);
      mesh->group.Add(grp->id);

      for (t = 0; t < hl_mesh[b][m].numtris; t++)
      {
	// Clear the dictionary/indexed lists
	vertex.Clear();
	texel.Clear();

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[0]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[0]]);

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[1]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[1]]);

        if (grp->vertex.SearchKey(trans[hl_tris[b][m][t].tri[2]]) == UINT_MAX)
          grp->vertex.Add(trans[hl_tris[b][m][t].tri[2]]);

	// Store vertices by true id, using translation table
	vertex.Add(trans[hl_tris[b][m][t].tri[0]]);
	vertex.Add(trans[hl_tris[b][m][t].tri[1]]);
	vertex.Add(trans[hl_tris[b][m][t].tri[2]]);

	st[0] = hl_tris[b][m][t].st[0][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[0][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	st[0] = hl_tris[b][m][t].st[1][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[1][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	st[0] = hl_tris[b][m][t].st[2][0] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].w;
	st[1] = hl_tris[b][m][t].st[2][1] / 
	  (float)hl_skin[hl_mesh[b][m].skinref].h;
	texel.Add(_egg->TexelAdd(st[0], st[1]));
	//printf("st = %f %f\n", st[0], st[1]);

	mesh->polygon.Add(_egg->PolygonAdd(&vertex, &texel, 
					   hl_mesh[b][m].skinref));
      }
    }
  }

  return 0;
}
#endif

#endif

#ifdef UNIT_TEST_HALFLIFE
int main(int argc, char *argv[])
{
	HalfLife hl;


	printf("[HalfLife class test]\n");

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!hl.Load(argv[2]))
				printf("main: Load reports success.\n");
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			printf("Not Implemented\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			printf("Not Implemented\n");
		}
		else
			printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
				   argv[0]);
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
			   argv[0]);
	}
}
#endif
