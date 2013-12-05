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
#include "entity.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

class StickGold;
class LevelScene;

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
#define ANIM_PLAYER_COUNT 1
class Player : public Entity
{
public:
	enum 
	{
		STATE_STAND,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,

		STATE_COUNT
	};
protected:
	DJ2DSkeletonNode*			m_pSkeletonNode;	
	djuint32					m_uState;
	DJVector2					m_vTarget;
	djfloat						m_fTimeMove;
	djfloat						m_fTimeDuration;
	DJRECT						m_rectHitBox;
	djbool						m_bEnableTouch;

	//Touch
	djint						m_nTouchTrackID;
	DJVector2					m_vTrackInitialDelta;
	DJVector2					m_vFirstTrackedPos;
	DJVector2					m_vLastTrackedPos;
public:
	Player();
	~Player();

	djbool Init(DJVector2 vpos);
	void Move(DJVector2 vTarget);
	djbool Update(djfloat fDeltaTime);
	void   Paint();
	void Term();
	void Reset();
	
	// Touch begin callback
	djint32		OnTouchBegin( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch move callback
	djint32		OnTouchMove( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch end callback
	djint32		OnTouchEnd( djint32 nDevice, djint32 nID, float fX, float fY );
	// Touch cancel callback
	djint32		OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY );

	// Hit box
	DJRECT GetHitBox() const {return m_rectHitBox;}
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
	djfloat				m_fTimeDuration;
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