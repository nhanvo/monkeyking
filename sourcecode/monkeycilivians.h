/////////////////////////////////////////////////////////////////
#ifndef _MONKEYCILIVIANS_H_
#define _MONKEYCILIVIANS_H_
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
#include "entity.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Start monkey cilivians class
class MonkeyCivilians : public Entity
{
public:
	// state of monkey civilians
	enum
	{
		STATE_MC_STANDING,
		STATE_MC_JUMP,
		STATE_MC_HITJUMP,
		STATE_MC_TREAMBING,
		STATE_MC_DIE,	
		STATE_MC_FINISH,

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
	djint32				m_nID;
	DJ2DSkeletonNode*	m_pSkeletonNode;
	djuint32			m_uState;
	djfloat				m_fTimeToJump;
	djint32				m_nIDBeatsTime;
	djbool				m_bEnableJump;
public:
	MonkeyCivilians();
	~MonkeyCivilians();

	djbool Init(djint32 id, DJVector2 vpos, djint32 nBeatsTime);
	djbool OnHit(const DJRECT& box);
	void SetState(djuint32 uState) {m_uState = uState;}

	// Set and get beats time
	void SetIDBeatsTime(djuint32 nID) {m_nIDBeatsTime = nID;}
	djuint32 GetIDBeatsTime() const {return m_nIDBeatsTime;}

	// get ID
	djint32 GetID() const {return m_nID;}

	// Enable jump
	void EnableJump() {m_bEnableJump = DJTRUE;}
	void FinishJump() {m_bEnableJump = DJFALSE;}

	void Update(djfloat fDeltaTime);
	void Term();
	virtual void Move(const DJVector2 &vDelta);
	virtual void Fall(const DJVector2 &vDelta);
	virtual void Jump(const DJVector2 &vDelta);	
};
// End monkey cilivians class
/////////////////////////////////////////////////////////////////
class BeatsTime
{
protected:
	djint32		m_nID;
	djint32		m_nNumberOfMonkeyCilivians;
	DJVector2	m_vBeatsTime;

public:
	BeatsTime();
	~BeatsTime();

	djbool Init(djint32 nID, djint32 nNumber, DJVector2 vBeats);
	void UpDate(djfloat fDeltaTime);
	void Term();

	// Get ID
	djint32 GetID() const { return m_nID;}

	// Get Beats time
	DJVector2	GetBeatsTime() const {return m_vBeatsTime;}

	// Get number of monkey
	djint32 GetNumberMonkey() const {return m_nNumberOfMonkeyCilivians;}
};

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif //_MONKEYCILIVIANS_H_