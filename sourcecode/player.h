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