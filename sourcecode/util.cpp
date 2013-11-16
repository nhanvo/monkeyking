/////////////////////////////////////////////////////////////////
//
// util.cpp
// Util helper class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2dutil.h>
#include <djfont.h>
#include <djosfile.h>		
#include <spine/Bone.h>
#include <spine/Skin.h>
#include <spine/RegionAttachment.h>
#include <spine/animation.h>
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Game Includes
#include "util.h"
#include "gamedef.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
float QUAD_DX[6] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
float QUAD_DY[6] = { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

void FillQuadVertices(RectVertex *pVertices,
					  const DJVector2 &vTopLeft,
					  const DJVector2 &vSize,
					  const DJColor &color,
					  float fZ,
					  const DJMatrix &tm,
					  const DJVector2 &vTopLeftUV,
					  const DJVector2 &vBottomRightUV)
{
	for (djint32 i = 0; i < 6; i++)
	{
		DJVector3 v(vTopLeft[0] + QUAD_DX[i] * vSize[0],
					vTopLeft[1] + QUAD_DY[i] * vSize[1],
					fZ);
		v = tm * v;
					
		pVertices[i].x = v[0];
		pVertices[i].y = v[1];
		pVertices[i].z = v[2];
		pVertices[i].c = color;
		pVertices[i].u = vTopLeftUV[0] + QUAD_DX[i] * (vBottomRightUV[0] - vTopLeftUV[0]);
		pVertices[i].v = vTopLeftUV[1] + QUAD_DY[i] * (vBottomRightUV[1] - vTopLeftUV[1]);
	}
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// All gloal function for spine action

/// 
///	Get width and height of root 
///
DJVector2 const GetSizeFromSpine(const char *strName,const DJ2DSkeletonNode* pNode)
{
	DJVector2 vSize;
	Slot* pSlot = pNode->FindSlot(strName);
	DJAssert(pSlot != NULL);
	if(pSlot)
	{
		RegionAttachment* patt = (RegionAttachment*)pSlot->attachment;
		vSize.e[0] = patt->width;
		vSize.e[1] = patt->height;			
	}
	return vSize;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////