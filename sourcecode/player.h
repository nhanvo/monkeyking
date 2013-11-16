/////////////////////////////////////////////////////////////////
#ifndef _PLAYER_H_
#define _PLAYER_H_
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djengine.h>
#include <djapp.h>
#include <dj2d.h>
#include <djrenderdevice.h>
#include <djrender.h>
#include <djtagfile.h>
#include <djmesh.h>
#include <djuinode.h>
#include <djosdatastorage.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "level.h"
/////////////////////////////////////////////////////////////////
class StickGold;
class LevelScene;
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

	virtual void SetPosition(const DJVector2 &vPos) { m_vPos = vPos; }
	virtual const DJVector2 &GetPosition() const { return m_vPos; }	

	virtual DJRECT* MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const;
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
#define ANIM_PLAYER_COUNT 1
class Player : public Entity
{
	enum 
	{
		STATE_NONE,
		STATE_CUT,

		STATE_COUNT
	};
protected:
	DJString			m_sID;	
	DJ2DSprite*			m_pSprite;
	DJ2DAnimation*		m_pAnimation;
	djuint32			m_uState;	
	StickGold*			m_pStickGold;
public:
	Player();
	~Player();

	djbool Init(DJString id, DJVector2 vpos,DJVector2 vSize, DJString strAnimFile, LevelScene* pLevelScene);
	djbool Update(djfloat fDeltaTime);
	void   Paint();
	void Term();
	void Reset();
	// Get stick gold
	StickGold* GetStickGold() const {return m_pStickGold;}
	// Touch begin callback
	djint32		OnTouchBegin( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch move callback
	djint32		OnTouchMove( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch end callback
	djint32		OnTouchEnd( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch cancel callback
	djint32		OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY );
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MonkeyCivilians : public Entity
{
public:
	// state of monkey civilians
	enum
	{
		STATE_MC_STANDING,
		STATE_MC_WALKING,
		STATE_MC_TREAMBING,
		STATE_MC_JUMP,
		STATE_MC_HITJUMP,
		STATE_MC_GROUND,

		STATE_COUNT
	};

	enum 
	{
		ANIM_01 = 0,
		ANIM_02,
		ANIM_03,
		ANIM_04,
		ANIM_05,
		ANIM_06
	};
protected:
	DJRECT				m_rectBoxHit;
	DJString			m_sID;
	DJ2DSkeletonNode*	m_pSkeletonNode;
	djuint32			m_uState;
	djfloat				m_fTimeToJump;
public:
	MonkeyCivilians();
	~MonkeyCivilians();

	djbool Init(DJString id, DJVector2 vpos, djint32 nState);
	djbool OnHit(const DJRECT& box);
	void SetState(djuint32 uState) {m_uState = uState;}
	void Update(djfloat fDeltaTime);
	void Term();
	virtual void Move(const DJVector2 &vDelta);
	virtual void Fall(const DJVector2 &vDelta);
	virtual void Jump(const DJVector2 &vDelta);		
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// class 
class StickGold : public Entity
{
public:
	enum
	{
		STATE_SG_STAND,
		STATE_SG_ZOOMIN,
	};
protected:
	DJRECT				m_rectBoxHit;
	DJ2DSkeletonNode*	m_pSkeletonNode;	
	djuint				m_uState;
	djbool				m_bEnableTouch;
	djfloat				m_fTimeChangeAnim;
public:
	StickGold();
	~StickGold();

	djbool Init(DJVector2 vPos, DJString strAtlastFile, DJString strAnimName);
	DJRECT GetRectBoxHit() const {return m_rectBoxHit;}
	void Update(float fDeltaTime);
	void Term();	

	// Touch begin callback
	djint32		OnTouchBegin( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch move callback
	djint32		OnTouchMove( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch end callback
	djint32		OnTouchEnd( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch cancel callback
	djint32		OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY );
};
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif //_PLAYER_H_