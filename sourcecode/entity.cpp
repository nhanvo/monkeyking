/////////////////////////////////////////////////////////////////
// Game include
#include "entity.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Begin class Entity

///
void Entity::Move(const DJVector2 &vDelta)
{
	DJTrace("%s", __FUNCTION__);
	m_vPos += vDelta;
}

///

void Entity::Fall(const DJVector2 &vDelta)
{
	DJTrace("%s", __FUNCTION__);
	m_vPos += vDelta;
} 

///

void Entity::Jump(const DJVector2 &vDelta)
{
	DJTrace("%s", __FUNCTION__);	
}

DJRECT* Entity::MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const
{
	pRect->nX = djTruncToInt(vPos.e[0] - nWidth/2.0f);
	pRect->nY = djTruncToInt(vPos.e[1] - nHeight);
	pRect->nW = nWidth;
	pRect->nH = nHeight;
	return pRect;
}
// End class Entity
/////////////////////////////////////////////////////////////////