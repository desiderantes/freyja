/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ABI to expose Mesh class and utils to C.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.07.09:
 * Mongoose - Created, Split from Bone.cpp
 ==========================================================================*/

#include <math.h>
#include "Mesh.h"
#include "MeshABI.h"
//#include "FreyjaPluginABI.h"

using namespace freyja;


////////////////////////////////////////////////////////////////////////
// 0.9.5 C++ ABI
////////////////////////////////////////////////////////////////////////

Vector<Mesh *> gFreyjaMeshes;


Mesh *freyjaGetMeshClass(index_t meshUID)
{
	Mesh **array = gFreyjaMeshes.getVectorArray();

	if ( meshUID < gFreyjaMeshes.size() )
	{
		return array[meshUID];
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////////
// 0.9.5 ABI
////////////////////////////////////////////////////////////////////////

char freyjaIsMeshAllocated(index_t mesh)
{
	return ( freyjaGetMeshClass(mesh) != NULL );
}


uint32 freyjaGetMeshFlags(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->GetFlags();
	}

	return 0x0;
}


index_t freyjaMeshCreate()
{
	Mesh **array = gFreyjaMeshes.getVectorArray();
	Mesh *mesh = new Mesh();
	index_t idx = gFreyjaMeshes.size();

	for ( uint32 i = 0; i < idx; ++i )
	{
		if ( array[i] == NULL )
		{
			array[i] = mesh;
			return i;
		}
	}

	gFreyjaMeshes.pushBack(mesh);
	return idx;
}


void freyjaMeshDelete(index_t mesh)
{
	Mesh **array = gFreyjaMeshes.getVectorArray();

	if ( mesh < gFreyjaMeshes.size() && array[mesh] != NULL )
	{
		delete array[mesh];
		array[mesh] = NULL;
	}
}


index_t freyjaMeshPolygonCreate(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		return m->CreateFace();
	}

	return INDEX_INVALID;
}


void freyjaMeshPolygonDelete(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->DeleteFace(polygonIndex);
	}
}


uint32 freyjaGetMeshPolygonVertexCount(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mIndices.size();
		}
	}

	return 0;
}


index_t freyjaGetMeshPolygonVertexIndex(index_t mesh, index_t polygon, 
										uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mIndices[element];
		}
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshPolygonTexCoordCount(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mTexCoordIndices.size();
		}
	}

	return 0;
}


index_t freyjaGetMeshPolygonTexCoordIndex(index_t mesh, index_t polygon, 
										  uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mTexCoordIndices[element];
		}
	}

	return INDEX_INVALID;
}


void freyjaMeshPolygonMaterial(index_t mesh, index_t polygon, index_t material)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			f->mMaterial = material;
		}
	}	
}


index_t freyjaGetMeshPolygonMaterial(index_t mesh, index_t polygon)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			return f->mMaterial;
		}
	}	

	return 0;
}


void freyjaMeshPolygonAddTexCoord1i(index_t mesh, index_t polygon, 
									index_t texcoord)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Face *f = m->GetFace(polygon);

		if (f)
		{
			f->mFlags |= Face::fPolyMappedTexCoords;
			f->mTexCoordIndices.pushBack(texcoord);
		}
	}
}


void freyjaMeshPolygonAddVertex1i(index_t mesh, index_t polygon, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	ASSERT_MSG(m != NULL, "Mesh[%i] == NULL", mesh);

	if ( m != NULL )
	{
		Face *face = m->GetFace(polygon);
		Vertex *v = m->GetVertex(vertex);

		ASSERT_MSG(face != NULL, "Mesh[%i].Polygon[%i] == NULL", mesh, polygon);
		ASSERT_MSG(v != NULL, "vertex == NULL");

		if ( face != NULL && v != NULL )
		{
			for ( uint32 i = 0; i < face->mIndices.size(); ++i )
			{
				if (face->mIndices[i] == vertex)
				{
					MARK_MSGF("%s, Tried to insert duplicate vertex into face[%i].  %i  %i", __func__, polygon, face->mIndices[i], vertex);
					return;
				}
			}

			face->mIndices.pushBack(vertex);
		}
	}
}


void freyjaMeshTransform3fv(index_t mesh, 
							freyja_transform_action_t action, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m == NULL)
		return;

	switch (action)
	{
	case fRotate:
		{
			Matrix t, r, t2, mat;
			// Rotate about bounding volume center instead of origin
			t.translate(m->GetBoundingVolumeCenter().mVec);
			r.rotate(xyz);
			t2.translate((-m->GetBoundingVolumeCenter()).mVec);
			//mat = t*r*t2;
			//mesh->TransformVertices(mat);

			// FIXME: Fix the damn matrix backend to avoid such expensive
			//        processing here ( only want to transform once )
			m->TransformVertices(t2);
			m->TransformVertices(r);
			m->TransformVertices(t);

			// Transform normals by inverted rotation to stay correct
			Matrix nr;
			nr.rotate(xyz);
			nr.invert();
			m->TransformNormals(nr);
		}
		break;

	case fScale:
		{
			Matrix s;
			s.scale(xyz);
			m->TransformVertices(s);
		}
		break;

	case fTranslate:
		{
			m->Translate(Vec3(xyz));
		}
		break;
		
	default:
		BUG_ME("Case '%i' Not Implemented", action);
	}
}


index_t freyjaMeshVertexCreate3fv(index_t mesh, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		vec3_t uvw = {0.5f, 0.5f, 0.0f};
		vec3_t nxyz = {0.0f, 1.0f, 0.0f};

		return m->CreateVertex(xyz, uvw, nxyz);
	}

	return INDEX_INVALID;
}


void freyjaMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			m->SetVertexArrayPos(v->mVertexIndex, xyz);
		}
	}
}


index_t freyjaMeshTexCoordCreate2f(index_t meshIndex, vec_t u, vec_t v)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		vec3_t uvw = {u, v, 0.0f };
		return mesh->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


index_t freyjaMeshTexCoordCreate3f(index_t meshIndex, vec_t u, vec_t v, vec_t w)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		vec3_t uvw = {u, v, w };
		return mesh->CreateTexCoord(uvw);
	}

	return INDEX_INVALID;
}


void freyjaMeshVertexWeight(index_t mesh, index_t vertex, 
							index_t bone, vec_t weight)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		m->AddWeight(vertex, weight, bone);
	}
}


index_t freyjaMeshVertexWeld(index_t mesh, index_t a, index_t b)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		if (m->WeldVertices(a, b))
			return a;
	}	

	return INDEX_INVALID;
}


index_t freyjaMeshVertexTrackNew(index_t mesh, vec_t duration, vec_t rate)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		index_t track = m->NewVertexAnimTrack();
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		t.SetRate((rate < 15.0f) ? 15.0f : rate);
		t.SetDuration((duration < 10.0f) ? 10.0f : duration);

		return track;
	}

	return INDEX_INVALID;
}


index_t freyjaMeshVertexKeyFrameNew(index_t mesh, index_t track, vec_t time)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		index_t key = t.NewKeyframe(time);
		VertexAnimKeyFrame *k = t.GetKeyframe(key);
		
		if (k)
		{
			k->ArrayResize(m->GetVertexCount());
			return key;
		}
	}

	return INDEX_INVALID;
}


void freyjaMeshVertexKeyFrame3f(index_t mesh, index_t track, index_t key,
								uint32 vert, vec_t x, vec_t y, vec_t z)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m )
	{
		VertexAnimTrack &t = m->GetVertexAnimTrack(track);
		VertexAnimKeyFrame *k = t.GetKeyframe(key);

		if (k)
		{
			//MSTL_ASSERTMSG(false, "Debug me");
			k->SetPos(vert, Vec3(x, y, z));
		}
	}
}


void freyjaMeshPolygonSplit(index_t meshIndex, index_t polygonIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);
	
	if (mesh)
	{
		mesh->SplitFace(polygonIndex);
	}
}


void freyjaMeshPolygonExtrudeQuad1f(index_t meshIndex, index_t polygonIndex, vec3_t v)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);
	if (mesh) 
		mesh->ExtrudeFace(polygonIndex, v);
}


index_t freyjaMeshTexCoordWeld(index_t mesh, index_t a, index_t b)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if ( m != NULL )
	{
		if (m->WeldTexCoords(a, b))
			return a;
	}	

	return INDEX_INVALID;
}


const char *freyjaGetMeshNameString(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		return mesh->GetName();
	}

	return 0x0;
}


void freyjaGetMeshName1s(index_t meshIndex, int32 lenght, char *name)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		strncpy(name, mesh->GetName(), (lenght < 32) ? lenght : 32);
		name[lenght-1] = 0;
	}
}


void freyjaGetMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->GetPosition(xyz);
	}
}


void freyjaMeshMaterial(index_t meshIndex, uint32 materialIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		for (uint32 i = 0, n = mesh->GetFaceCount(); i < n; ++i)
		{
			Face *f = mesh->GetFace(i);

			if (f)
			{
				f->mMaterial = materialIndex;
			}
		}

		mesh->SetMaterial(materialIndex);
	}
}


void freyjaMeshFrameCenter(index_t meshIndex, uint32 frame, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


void freyjaGetMeshFrameBoundingBox(index_t meshIndex, uint32 frame, 
									vec3_t min, vec3_t max)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetBBox(min, max);
	}
}



void freyjaMeshName1s(index_t meshIndex, const char *name)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetName(name);
	}
}


void freyjaMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (mesh)
	{
		mesh->SetPosition(xyz);
	}
}


uint32 freyjaGetMeshCount()
{
	return gFreyjaMeshes.size();
}


void freyjaGetMeshBoundingBox(index_t mesh, vec3_t min, vec3_t max)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		m->GetBBox(min, max);
	}
}


void freyjaGetMeshBoundingSphere(index_t mesh, vec3_t origin, vec_t &radius)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vec3 o = m->GetBoundingVolumeCenter();
		o.Get(origin);
		radius = m->GetBoundingVolumeRadius();
	}
}


uint32 freyjaGetMeshWeightCount(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	uint32 count = 0;

	if (m)
	{
		count = m->GetWeightCount();
	}

	return count;
}


void freyjaGetMeshWeight(index_t mesh, index_t weight,
						 index_t &vertex, index_t &bone, vec_t &weightv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Weight *w = m->GetWeight(weight);

		if (w)
		{
			vertex = w->mVertexIndex;
			bone = w->mBoneIndex;
			weightv = w->mWeight;
		}
	}
}


index_t freyjaGetMeshVertexPolygonRefIndex(index_t mesh, 
										   index_t vertex, uint32 element)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v && element < v->GetFaceRefs().size())
		{
			return v->GetFaceRefs()[element];
		}
	}

	return INDEX_INVALID;
}


uint32 freyjaGetMeshVertexPolygonRefCount(index_t mesh, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			return v->GetFaceRefs().size();
		}
	}

	return 0;
}


void freyjaGetMeshPosition(index_t meshIndex, vec3_t xyz)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		mesh->GetPosition(xyz);
	}
}


uint32 freyjaGetMeshPolygonCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		return mesh->GetFaceCount();
	}

	return 0;
}


void freyjaMeshPolygonGroup1u(index_t mesh, index_t face, uint32 group)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->SetFaceSmoothingGroup(face, group);
	}
}


uint32 freyjaGetMeshVertexCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		return mesh->GetVertexCount();
	}

	return 0;
}


uint32 freyjaGetMeshTexCoordCount(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex );

	if (mesh)
	{
		return mesh->GetTexCoordCount();
	}

	return 0;
}


void freyjaMeshPolygonClearFlag1u(index_t mesh, index_t face, byte flag)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->ClearFaceFlags(face, flag);
	}
}


void freyjaMeshPolygonSetFlag1u(index_t mesh, index_t face, byte flag)
{
	Mesh *m = freyjaGetMeshClass( mesh );

	if (m)
	{
		return m->SetFaceFlags(face, flag);
	}
}


void freyjaMeshGenerateVertexNormals(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (!mesh)
		return;

	BUG_ME("Function local vertex refs implementation used.");
	Vector<Vec3> faceNormals;
	Vec3 a, b, c, normal;

    for (uint32 v = 0, vn = mesh->GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = mesh->GetVertex(v);

		if (vertex)
			vertex->GetTmpRefs().clear();
	}

	/* Compute face normals */
	for (uint32 f = 0, fn = mesh->GetFaceCount(); f < fn; ++f)
	{
		Face *face = mesh->GetFace(f);

		if (!face)
		{
			faceNormals.pushBack(normal);  // For alignment purposes
			continue;
		}

		for (uint32 v = 0, vn = face->mIndices.size(); v < vn; ++v)
		{
			Vertex *vertex = mesh->GetVertex(face->mIndices[v]);
			
			if (vertex)
				vertex->GetTmpRefs().pushBack(f);
		}

		mesh->GetVertexPos(face->mIndices[0], a.mVec);
		mesh->GetVertexPos(face->mIndices[1], b.mVec);
		mesh->GetVertexPos(face->mIndices[2], c.mVec);

		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(a - b, c - b);
		normal.normalize();
		faceNormals.pushBack(normal);
		//DEBUG_MSGF("face %i. %f %f %f\n", f, normal.mVec[0], normal.mVec[1], normal.mVec[2]);
	}


	/* Compute vertex normals */
    for (uint32 v = 0, vn = mesh->GetVertexCount(); v < vn; ++v)
    {
		Vertex *vertex = mesh->GetVertex(v);

		if (!vertex)
			continue;

		normal.zero();

		//ASSERT_MSG(vertex->mPolyRefIndices.size(), "WTF vertex[%i] with no faces...", v);

		if (vertex->GetTmpRefs().size() == 0)
		{
			//DEBUG_MSGF("WTF vertex[%i] with no faces...", v);
			continue;
		}

		for (uint32 j = 0, jn = vertex->GetTmpRefs().size(); j < jn; ++j)
		{
			if (vertex->GetTmpRefs()[j] == INDEX_INVALID)
			{
				freyjaPrintError("freyjaGenerateMeshVertexNormals> ERROR Bad face reference\n");
				continue;
			}

			normal += faceNormals[vertex->GetTmpRefs()[j]];
		}


		normal.normalize();

		//DEBUG_MSGF("vert %i. %f %f %f\n", v, normal.mVec[0], normal.mVec[1], normal.mVec[2]);

		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		mesh->SetNormal(v, normal.mVec);
    }
}


void freyjaMeshNormalFlip(index_t meshIndex)
{
	Mesh *mesh = freyjaGetMeshClass(meshIndex);

	if (!mesh)
		return;

	Vec3 normal;

    for (uint32 n = 0, nn = mesh->GetNormalCount(); n < nn; ++n)
    {
		// FIXME: Doesn't use vertex normal remap ( which isn't used yet )
		mesh->GetNormal(n, normal.mVec);
		normal = -normal;
		mesh->SetNormal(n, normal.mVec);
	}
}


void freyjaGetMeshVertexPos3fv(index_t mesh, index_t vertex, vec3_t xyz)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		m->GetVertexPos(vertex, xyz);
	}
}


index_t freyjaGetMeshVertexTexCoord(index_t mesh, index_t vertex)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		Vertex *v = m->GetVertex(vertex);

		if (v)
		{
			return v->mTexCoordIndex;
		}
	}

	return INDEX_INVALID;
}


void freyjaMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		vec3_t uvw = { uv[0], uv[1], 0.0f };
		m->SetTexCoord(texcoord, uvw);
	}
}


void freyjaGetMeshTexCoord2fv(index_t mesh, index_t texcoord, vec2_t uv)
{
	Mesh *m = freyjaGetMeshClass(mesh);

	if (m)
	{
		vec3_t uvw = { uv[0], uv[1], 0.0f };
		m->GetTexCoord(texcoord, uvw);
	}
}


void freyjaMeshUVMapPlanar(index_t meshIndex)
{
	uint32 vertexCount = freyjaGetMeshVertexCount(meshIndex);
	uint32 idx;
	vec3_t xyz;
	vec2_t uv;
	vec_t s;

    for (uint32 i = 0; i < vertexCount; ++i)
    {
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		uv[0] = (xyz[0] > 0) ? xyz[0] : -xyz[0];
		s = 0.025;
  
		while (uv[0] > 1.0)
		{
			uv[0] *= s;
			s *= 0.01;
		}
  
		uv[1] = (xyz[1] > 0) ? xyz[1] : -xyz[1];
		s = 0.025;
		
		while (uv[1] > 1.0)
		{
			uv[1] *= s;
			s *= 0.01;
		}
		
		uv[0] = 1.0 - uv[0];
		uv[1] = 1.0 - uv[1];

		idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


index_t freyjaMeshCopy(index_t meshIndex)
{
	Mesh *m = freyjaGetMeshClass(meshIndex);
	
	if (m)
	{
		Mesh *meshCopy = new Mesh(*m);
		gFreyjaMeshes.pushBack(meshCopy);
		return gFreyjaMeshes.size() - 1;
	}

	return INDEX_INVALID;
}


void freyjaMeshUVMapSpherical(index_t meshIndex)
{
	int32 i, vertexCount;
	vec_t longitude, latitude;
	vec3_t xyz;
	vec3_t uv;

	Mesh *m = freyjaGetMeshClass(meshIndex);

	if (!m)
		return;

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = latitude;

		//freyjaVertexTexCoord2fv(vertexIndex, uv);
		index_t idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


void freyjaMeshUVMapCylindrical(index_t meshIndex)
{
	int32 i, j, vertexCount;
	vec_t longitude, latitude, ysize;
	vec3_t xyz;
	vec3_t min = {999999.0f, 999999.0f, 999999.0f};
	vec3_t max = {-999999.0f, -999999.0f, -999999.0f};
	vec2_t uv;

	vertexCount = freyjaGetMeshVertexCount(meshIndex);

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		for (j = 0; j < 3; ++j)
		{
			if (xyz[j] < min[j])
				min[j] = xyz[j];

			if (xyz[j] > max[j])
				max[j] = xyz[j];
		}
	}

	if (max[1] >= 0)
	{
		if (min[1] >= 0)
		{
			ysize = max[1] - min[1];
		}
		else
		{
			ysize = max[1] + -min[1];
		}
	}
	else
	{
		ysize = -max[1] + min[1];
	}

	if (ysize < 0.0001 && ysize > -0.0001)
		ysize = 1.0f;

    for (i = 0; i < vertexCount; ++i)
    {
		//vertexIndex = freyjaGetMeshVertexIndex(meshIndex, i);
		//freyjaGetVertexXYZ3fv(vertexIndex, xyz);
		freyjaGetMeshVertexPos3fv(meshIndex, i, xyz);

		longitude = atan2((float)-xyz[0], xyz[2]);
		latitude = atan(xyz[1] / sqrt(xyz[0]*xyz[0] + xyz[2]*xyz[2]));

		longitude = 1.0 - longitude / (HEL_2_PI);
		latitude = fabs(0.5 - latitude / HEL_PI);

		uv[0] = longitude - floor(longitude);
		uv[1] = xyz[1] / ysize;

		//freyjaVertexTexCoord2fv(vertexIndex, uv);
		index_t idx = freyjaGetMeshVertexTexCoord(meshIndex, i);
		freyjaMeshTexCoord2fv(meshIndex, idx, uv);
	}
}


#include "Bone.h"

void freyjaMeshUpdateBlendVertices(index_t mesh, index_t track, vec_t time)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m)
	{
		// Make sure we have enough blend vertices, and
		// this is 'smart' enough not to reallocate if we do
		m->SyncBlendVertices();
		vec_t *array = m->GetBlendVerticesArray();

		//if (!array)
		//	return;

		m->ResetBlendVertices();

		// Forget about 'cobbling' random skeletons with reused weights!
		//Skeleton *s = Skeleton::getSkeleton(skeleton);
		
		for (uint i = 0, n = m->GetWeightCount(); i < n; ++i)
		{
			Weight *w = m->GetWeight(i);

			if (!w)
				continue;

			Bone *b = Bone::GetBone(w->mBoneIndex);

			if (!b)
				continue;

			Vertex *v = m->GetVertex(w->mVertexIndex);

			if (!v)
				continue;

			BoneTrack &t = b->GetTrack(track);	
			Vec3 rot = t.GetRot(time);
			Vec3 loc = t.GetLoc(time);
			Vec3 p;
			m->GetVertexArrayPos(v->mVertexIndex, p.mVec);

#if 0
			Matrix world;
			world.translate(loc.mVec[0], loc.mVec[1], loc.mVec[2]);
			world.rotate(rot.mVec[0], rot.mVec[2], rot.mVec[1]); // R 0 2 1
			// If this was a 'normal' data stream.. this would work 
			Matrix combined = world * b->GetInverseBindPose();
			p = (combined * p) * w->mWeight;
#elif 1
			Matrix world;
			world.translate(loc.mVec[0], loc.mVec[1], loc.mVec[2]);
			world.rotate(rot.mVec[0], rot.mVec[1], rot.mVec[2]); // R 0 2 1
			Matrix local;
			loc = b->mTranslation;
			b->mRotation.getEulerAngles(rot.mVec, rot.mVec+2, rot.mVec+1);
			local.translate(loc.mVec[0], loc.mVec[1], loc.mVec[2]);
			local.rotate(rot.mVec[0], rot.mVec[1], rot.mVec[2]);
			
			local.invert();
			Matrix combined = local * world;
			p = (combined * p) * w->mWeight;
#else
			int bone = w->mBoneIndex;
			Vec3 u;

			while (bone > -1)
			{
				index_t parent = freyjaGetBoneParent(bone);
				Vec3 v(0,0,0);
				
				if (freyjaIsBoneAllocated(parent))
				{
					freyjaGetBoneWorldPos3fv(parent, v.mVec);
				}
				else
				{
					bone = -1;
				}
				
				Bone *b = Bone::GetBone(bone);
				
				if (b)
				{
					v = b->mRotation.rotate(v);
					v += b->mTranslation;
				}
			}

			Vec3 o;
			freyjaGetBoneWorldPos3fv(w->mBoneIndex, o.mVec);
			p = p - o;
			//v = b->mRotation.rotate(v);
			//v += b->mTranslation;

			p *= w->mWeight;
#endif

			array[v->mVertexIndex*3  ] += p.mVec[0]; 
			array[v->mVertexIndex*3+1] += p.mVec[1]; 
			array[v->mVertexIndex*3+2] += p.mVec[2]; 
		}
	}
}


vec_t *freyjaGetMeshBlendVertices(index_t mesh)
{

	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m) return m->GetBlendVerticesArray();

	return NULL;
}


void freyjaDebugMeshBlendVertices(index_t mesh)
{
	Mesh *m = freyjaGetMeshClass(mesh);
	
	if (m && m->GetBlendVerticesArray()) 
	{
		vec_t *array = m->GetBlendVerticesArray();
	
		for (uint i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			Vertex *v = m->GetVertex(i);

			if (!v)
				continue;
			
			Vec3 p;
			m->GetVertexArrayPos(v->mVertexIndex, p.mVec);

			printf("%i, mesh, %f, %f, %f\n", 
				   i, p.mVec[0], p.mVec[1], p.mVec[2]);
			printf("%i, blnd, %f, %f, %f\n", 
				   i, array[i*3], array[i*3+1], array[i*3+2]);
		}
	}
}


void freyjaMeshClampTexCoords(index_t meshIndex)
{
	uint32 count = freyjaGetMeshTexCoordCount(meshIndex);
	vec2_t uv;

	for (uint32 i = 0; i < count; ++i)
	{
		freyjaGetMeshTexCoord2fv(meshIndex, i, uv);

		for (int32 j = 1; j >= 0; --j)
		{
			if (uv[j] < 0.0f)
			{
				uv[j] = 0.0f;
			}
			else if (uv[j] > 1.0f)
			{
				uv[j] = 1.0f;
			}
		}
		
		freyjaMeshTexCoord2fv(meshIndex, i, uv);
	}
}


void freyjaMeshTesselateTriangles(index_t meshIndex)
{
	Vector<long> purge;
	int32 i, j, polygonCount, polygonIndex, vertexCount, vertexIndex;
	int32 a, b, c, d, ta, tb, tc, td, material;
	unsigned int ii;


	polygonCount = freyjaGetMeshPolygonCount(meshIndex);

	for (i = 0; i < polygonCount; ++i)
	{
		polygonIndex = i;//freyjaGetMeshPolygonIndex(meshIndex, i);
		material = freyjaGetMeshPolygonMaterial(meshIndex, polygonIndex);

		if (polygonIndex > -1)
		{
			vertexCount = freyjaGetMeshPolygonVertexCount(meshIndex, polygonIndex);

			if (vertexCount < 4)
				continue;
			
			if (vertexCount == 4)
			{
				/* 1. Get ABCD quad vertices */
				a = freyjaGetMeshPolygonVertexIndex(meshIndex, polygonIndex, 0);
				b = freyjaGetMeshPolygonVertexIndex(meshIndex, polygonIndex, 1);
				c = freyjaGetMeshPolygonVertexIndex(meshIndex, polygonIndex, 2);
				d = freyjaGetMeshPolygonVertexIndex(meshIndex, polygonIndex, 3);

				if (freyjaGetMeshPolygonTexCoordCount(meshIndex, polygonIndex))
				{
					ta = freyjaGetMeshPolygonTexCoordIndex(meshIndex, polygonIndex, 0);
					tb = freyjaGetMeshPolygonTexCoordIndex(meshIndex, polygonIndex, 1);
					tc = freyjaGetMeshPolygonTexCoordIndex(meshIndex, polygonIndex, 2);
					td = freyjaGetMeshPolygonTexCoordIndex(meshIndex, polygonIndex, 3);
				}

				// FIXME: Using gobal FSM and 0.9.x API

				/* 2. Make ABC ACD triangles */
				freyjaBegin(FREYJA_POLYGON);
				freyjaPolygonMaterial1i(material);
				freyjaPolygonVertex1i(a);
				freyjaPolygonVertex1i(b);
				freyjaPolygonVertex1i(c);

				if (freyjaGetMeshPolygonTexCoordCount(meshIndex, polygonIndex))
				{
					freyjaPolygonTexCoord1i(ta);
					freyjaPolygonTexCoord1i(tb);
					freyjaPolygonTexCoord1i(tc);
				}

				freyjaEnd();

				freyjaBegin(FREYJA_POLYGON);
				freyjaPolygonMaterial1i(material);
				freyjaPolygonVertex1i(a);
				freyjaPolygonVertex1i(c);
				freyjaPolygonVertex1i(d);

				if (freyjaGetMeshPolygonTexCoordCount(meshIndex, polygonIndex))
				{
					freyjaPolygonTexCoord1i(ta); // should dupe a?
					freyjaPolygonTexCoord1i(tc);
					freyjaPolygonTexCoord1i(td);
				}

				freyjaEnd();


				/* 3. Prepare to remove ABCD polygon and update references */
				purge.pushBack(polygonIndex);
			}
			else  // Hhhhmm... can of worms...  doesn't touch polygons atm
			{
				for (j = 0; j < vertexCount; ++j)
				{
					// 0 1 2, 0 2 3, ..
					freyjaPrintError("freyjaMeshTesselateTriangles> No Implementation due to lack of constraints on 5+ edges");
					vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, polygonIndex, j);
				}
			}
		}
	}

	for (ii = purge.begin(); ii < purge.end(); ++ii)
	{
		freyjaMeshPolygonDelete(meshIndex, purge[ii]);
	}
}

