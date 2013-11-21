/////////////////////////////////////////////////////////////////
#ifndef _ENTITY_H_
#define _ENTITY_H_
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djengine.h>

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
class Entity
{
protected:
	DJVector2 m_vPos;
	DJVector2 m_vOrgPos;
	DJVector2 m_vSize;
	DJVector2 m_vOrgSize;
	DJVector2 m_vVelocity;
	DJVector2 m_vAcceleration;
public:
	Entity(){}
	virtual ~Entity()	{}
public:
	// Forces the entity to move in a direction
	virtual void Move(const DJVector2 &vDelta);

	// Something the entity is standing on is falling
	virtual void Fall(const DJVector2 &vDelta);

	// jump
	virtual void Jump(const DJVector2 &vDelta);

	void SetPosition(const DJVector2 &vPos) { m_vPos = vPos; }
	const DJVector2 &GetPosition() const { return m_vPos; }	

	virtual DJRECT* MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const;
};

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif //_ENTITY_H_ 