/////////////////////////////////////////////////////////////////
#ifndef _UTIL_H_
#define _UTIL_H_
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2d.h>
#include <djconsole.h>
#include <djmatrix2D.h>
#include <dj2dutil.h>
#include <djlink.h>
/////////////////////////////////////////////////////////////////
// Game Includes

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

///

struct RectVertex
{
	float x,y,z;
	DJColor c;
	float u,v;
};

static const djuint32 RectVertex_DescCount = 3;

static DJVertexBuffer::DJVERTEX_DESC RectVertex_Descs[3] =
{
	DJVertexBuffer::DJVERTEX_DESC(0, DJVertexBuffer::typeFloat3, "pos_attr", INDEX_POS_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(12, DJVertexBuffer::typeFloat4, "col_attr", INDEX_COL_ATTR, 0),
	DJVertexBuffer::DJVERTEX_DESC(28, DJVertexBuffer::typeFloat2, "uv_attr", INDEX_UV_ATTR, 0),
};

///

void FillQuadVertices(RectVertex *pVertices,
					  const DJVector2 &vTopLeft,
					  const DJVector2 &vSize,
					  const DJColor &color,
					  djfloat fZ = 0.0f,
					  const DJMatrix &tm = DJMatrix::Identity(), 
					  const DJVector2 &vTopLeftUV = DJVector2(0.0f),
					  const DJVector2 &vBottomRightUV = DJVector2(1.0f));

///

DJVector2 const GetSizeFromSpine(const char *strName,const DJ2DSkeletonNode* pNode);

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif //_UTIL_H_