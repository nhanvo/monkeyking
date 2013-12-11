/////////////////////////////////////////////////////////////////
#ifndef _SPECTER_H_
#define _SPECTER_H_
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
// Engine include
#include <djosdef.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "level.h"
#include "entity.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
class BeatsTime;
/////////////////////////////////////////////////////////////////
// Rayghost
class RaysGhost : public Entity
{
public:
	// circle effect
	enum
	{
		ANIM_CIRCLE_0,
		ANIM_CIRCLE_1,
		ANIM_CIRCLE_2,
		ANIM_CIRCLE_3,
		ANIM_CIRCLE_4,
		ANIM_CIRCLE_5,
		ANIM_CIRCLE_6,
		ANIM_CIRCLE_7,
		ANIM_CIRCLE_COUNT
	};

	// circle effect fast
	enum 
	{
		ANIM_CIRCLE_FAST_0,
		ANIM_CIRCLE_FAST_1,
		ANIM_CIRCLE_FAST_2,
		ANIM_CIRCLE_FAST_3,		
		ANIM_CIRCLE_FAST_4,
		ANIM_CIRCLE_FAST_5,
		ANIM_CIRCLE_FAST_6,
		ANIM_CIRCLE_FAST_7,
		
		ANIM_CIRCLE_FAST_COUNT
	}; 
	

	// spiral effect
	enum
	{
		ANIM_SPIRAL_ONE,
		ANIM_SPIRAL_TWO,
		ANIM_SPIRAL_THREE,
		ANIM_SPIRAL_FOUR,
		ANIM_SPIRAL_FIVE,
		ANIM_SPIRAL_SIX,
		ANIM_SPIRAL_SEVEN,
		ANIM_SPIRAL_EIGHT,
		
		ANIM_SPIRAL_COUNT
	};
	
	// type effect
	enum 
	{
		TYPE_RG_CIRCLE,
		TYPE_RG_LIGHT,
		TYPE_RG_SPIRAL,
		TYPE_RG_COUNT
	};

	// state of ray ghost
	enum
	{
		STATE_RG_STAND,
		STATE_RG_SHOOT,	
		STATE_RG_FINISH_SHOOT,
	};
protected:
	struct SkeletonAnimNode
	{
		djuint32			s_uIDAnim;
		DJ2DSkeletonNode*	s_pSkeletonNode;
		djfloat				s_fTimeFinishAnim;
	    djfloat				s_fTimeDuration;
		SkeletonAnimNode()
		{
		
		}
		~SkeletonAnimNode()
		{
			if(s_pSkeletonNode)
			{
				theSpriteEngine.RemoveNode(s_pSkeletonNode);
				theSpriteEngine.RemoveActiveNode(s_pSkeletonNode);
				DJ_SAFE_DELETE(s_pSkeletonNode);
			}
		}	
	};
	DJLinkedList<SkeletonAnimNode>	m_listSkeletonAnimNode;	
	DJRECT		*m_pRectHitBox;
	DJRECT		m_rectTarget;
	djint32		m_nID;
	djuint32	m_uState;
	djint32		m_nBeatTimeID;
	djuint32	m_uType;
	djuint32	m_uMaxAnim;
	DJString	m_strAtlastFile;
	DJString	m_strSlotName;
	DJString	m_strBoneName;
	djbool		m_bCircleEffectHit[ANIM_CIRCLE_COUNT];
	djbool		m_bCircleFastEffectHit[ANIM_CIRCLE_FAST_COUNT];
	djbool		m_bSpiralEffectHit[ANIM_SPIRAL_COUNT];
public:
	RaysGhost();
	~RaysGhost();

	djbool Init(djint32 id, djint32 nType, DJVector2 vPos, djint32 nBeatTime);
	void Update(djfloat fDeltaTime);
	void Term();	
	djbool OnHit(const DJRECT* pRect);
	void Reset();

	virtual DJRECT* MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const;

	// Get and set state
	void SetState(djuint32 uState) {m_uState = uState;}
	djuint32 GetState() const {return m_uState;}

	// Get and Set beats time
	void SetBeatsTimeID(djint32 nBeatsTime) {m_nBeatTimeID = nBeatsTime;}
	djint32 GetBeatsTimeID() const {return m_nBeatTimeID;}

	// Get skeleton node
	DJLinkedList<SkeletonAnimNode>& GetSkeletonAnimNode() {return m_listSkeletonAnimNode;}

	// calculate bording box for rayghost to collision detect
	DJRECT CalculateBordingBox(const DJ2DSkeletonNode* pNode);

	// Set and get target hit box
	void SetTargetHitBox(DJRECT rect){m_rectTarget = rect;}
	DJRECT GetTargetHitBox() const {return m_rectTarget;}
};				   
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Interface for all specter
class Specter : public Entity
{
public:
	enum{
		SPECTER_CENTIPEDE	=	0,	// This is centipede specter on scene_1 level_1 

		SPECTER_COUNT
	};
protected:
	djint32		m_nType;

public:
	Specter(): m_nType(-1){};
	~Specter(){};

	// Set and get type of specter
	djint32 GetType() const {return m_nType;}	

	virtual djbool  Init(djint32 uType, DJVector2 vPos)=0;
	virtual void	Update(djfloat fDeltaTime)=0;
	virtual void	Term()=0;

	/////////////////////////////////////////////////////////////////
	// EVENT HANDLERS

	// Touch begin callback
	virtual djint32 OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y ){return 0;}
	// Touch move callback
	virtual djint32 OnTouchMove(djint32 nDevice, djint32 nID, float x, float y ){return 0;}
	// Touch end callback
	virtual djint32 OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y ){return 0;}
	// Accelerator event callback
	virtual djint32 OnAccelerate( float x, float y, float z ){return 0;}
	// Button Down
	virtual djint32 OnButtonDown(djint32 nKey){return 0;}
	// Button Up
	virtual djint32 OnButtonUp(djint32 nKey){return 0;}

	// Key down
	virtual djint32 OnKeyDown( djint32 nKey ){return 0;} 
	
};

class Centipede : public Specter
{
public:
	enum
	{
		STATE_CEP_SHOOT_WEAPON_1,
		STATE_CEP_SHOOT_WEAPON_2,
		STATE_CEP_SHOOT_WEAPON_3,
		STATE_CEP_SHOOT_WEAPON_4,

		STATE_CEP_SHOOP_WEAPON_COUNT
	};
protected:
	DJ2DSkeletonNode*			m_pSkeletonNode;  
	DJLinkedList<RaysGhost>		m_listRayGhost;
	DJLinkedList<BeatsTime>		m_listBeatsTime;
	djuint32					m_uState;
	djfloat						m_fTimeAnimActive;
	djfloat						m_fTimeDuration;
	djfloat						m_fTimeChangeState[STATE_CEP_SHOOP_WEAPON_COUNT-1];
	djbool						m_bFinishState[STATE_CEP_SHOOP_WEAPON_COUNT-1];
	DJRECT						m_rectBoxHit;
	DJRECT						m_rectTargetHitBox;

public:
	Centipede();
	~Centipede();

	virtual djbool  Init(djint32 uType, DJVector2 vPos);
	virtual void	Update(djfloat fDeltaTime);
	virtual void	Term();

	// Set rayghost for centipede
	void SetListRayGhost(DJLinkedList<RaysGhost> &lisRayGhost);
	DJLinkedList<RaysGhost>& GetListRayGhost() {return m_listRayGhost;}

	// Set beats time for rayghost and centipede
	void SetListBeatsTime(DJLinkedList<BeatsTime> &listBeatsTime);
	DJLinkedList<BeatsTime>& GetListBeatsTime() {return m_listBeatsTime;}

	// Set and get state
	void SetState(djuint32 uState)	{m_uState = uState;}
	djuint32 GetState() const {return m_uState;}	 

	// Reset
	void Reset();
};

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif  //_SPECTER_H_