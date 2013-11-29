/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djengine.h>
#include <djapp.h>
#include <dj2d.h>
#include <djrenderdevice.h>
#include <djrender.h>
#include <djtagfile.h>
#include <djmesh.h>
#include <djsounddevice.h>
#include <djgamesounds.h>
#include <djfont.h>
#include <dj2dutil.h>
#include <spine/Bone.h>
#include <spine/Skin.h>
#include <spine/RegionAttachment.h>
#include <spine/animation.h>
#include <spine/animationstate.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "monkeycilivians.h"
#include "level.h"
#include "monkeyking.h"
#include "camera.h"
#include "musichandler.h"
#include "debug.h"
#include "util.h"
#include "datatemp.h"
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_START()
/////////////////////////////////////////////////////////////////

extern djuint32 g_nScreenHeight;
extern djuint32 g_nScreenWidth;
extern Camera *g_pCamera;
extern DJFont*		g_pFont;
struct Bone;
extern Player* g_pPlayer;

/////////////////////////////////////////////////////////////////
// Begin class Monkey civilians

/////////////////////////////////////////////////////////////////
// Animation path of all monkey civilians

/////////////////////////////////////////////////////////////////

const char* STR_MONKEYCIV_SLOTNAME = "monkey_jump";
///
/// Constructor 
///
MonkeyCivilians::MonkeyCivilians()
{
	m_nID			= -1;	 
	m_vSize			= DJVector2(0.0f, 0.0f);
	m_vOrgSize		= DJVector2(0.0f, 0.0f);
	m_vOrgPos		= DJVector2(0.0f, 0.0f);
	m_vPos			= DJVector2(0.0f, 0.0f);
	m_pSkeletonNode = NULL;
	m_uState		= STATE_MC_STANDING;
	m_rectBoxHit	= DJRECT(0,0,0,0);
	m_fTimeToJump	= 0.0f;
	m_nIDBeatsTime	= -1;
	m_bEnableJump	= DJFALSE;
}
///
///	Destructor
///
MonkeyCivilians::~MonkeyCivilians()
{
	Term();
}
///
/// Initialize all value for object
///
djbool MonkeyCivilians::Init(djint32 id, DJVector2 vpos, djint32 nBeatsTime)
{
	m_nID = id;
	m_vPos = vpos;
	m_vOrgPos = vpos;
	m_nIDBeatsTime = nBeatsTime;
	// Init skeleton animation
	m_pSkeletonNode = DJ_NEW(DJ2DSkeletonNode);
	m_pSkeletonNode->SetPosition(vpos);			
	m_pSkeletonNode->Create("sprites/monkey_jump");
	theSpriteEngine.AddActiveNode(m_pSkeletonNode);
	theSpriteEngine.AddNode(m_pSkeletonNode, LAYER_SPRITES); 	

	// get size for monkey cilivians
	m_vSize = GetSizeFromSpine(STR_MONKEYCIV_SLOTNAME, m_pSkeletonNode);
	m_vOrgSize = m_vSize;
	// initialize box hit
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y()); 

	m_fTimeDurations = GetTimeDurationOfAnimation(m_pSkeletonNode);
	DJAssert(m_fTimeDurations != 0.0f);

    return DJFALSE;
}
///
/// Update object with applications delta time 
///
void MonkeyCivilians::Update(djfloat fDeltaTime,const DJRECT& box)
{
	// update position of object
	spBone* bone = m_pSkeletonNode->FindBone("root");  	
	DJVector2 vPos = m_vOrgPos;
	vPos.e[0] += bone->x;
	vPos.e[1] -= bone->y;
	m_vPos = vPos;
	
	DJVector2 vScale = DJVector2(bone->scaleX, bone->scaleY);
	DJVector2 vSize = m_vOrgSize;
	vSize = vSize & vScale;
	m_vSize = vSize;
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());
#ifdef _DEV	 
	DJRECT boxDev;
	MakeBox(&boxDev, vPos, m_vSize.e[0], m_vSize.e[1]);
	theBoundingBoxCollection.QueueBoundingBox(boxDev);
	//DJInfo("%d, %d", m_rectBoxHit.nX, m_rectBoxHit.nY);
#endif 

	// Check state then change animation for monkey civilians
	switch(m_uState)
	{
		case STATE_MC_STANDING:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_stand", DJTRUE);
			}
			
			// Enable jump
			if(m_bEnableJump)
			{	
				SetState(STATE_MC_WALKING);
				m_pSkeletonNode->ClearAnimation();
			}
		}
		break;
		case STATE_MC_WALKING:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				djresult result;
				m_pSkeletonNode->SetAnimation("monkey_walk", DJTRUE);  				
			}
			m_fTimeToJump += pTheApp->GetDeltaAppTime();
			if(m_fTimeToJump >= m_fTimeDurations)
			{
				m_uState = STATE_MC_JUMP;
				m_pSkeletonNode->ClearAnimation();
				m_fTimeToJump = 0.0f;
			}
		}
		break;
		case STATE_MC_JUMP:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_jump", DJTRUE);
			}

			// Check hit
			m_fTimeToJump += pTheApp->GetDeltaAppTime();
			if(m_fTimeToJump >= m_fTimeDurations)
			{
				if(OnHit(box))
				{
					m_uState = STATE_MC_HITJUMP;
					m_pSkeletonNode->ClearAnimation();
					m_fTimeToJump = 0.0f;
				} 
				else
				{
					m_uState = STATE_MC_TREAMBING;
					m_pSkeletonNode->ClearAnimation();
					m_fTimeToJump = 0.0f;
				}
			}
		}
		break;

		case STATE_MC_HITJUMP:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_hitjump", DJTRUE);
			}

			// Finish hit jump
			m_fTimeToJump += pTheApp->GetDeltaAppTime();
			if(m_fTimeToJump >= m_fTimeDurations)
			{
				m_uState = STATE_MC_FINISH;
				m_fTimeToJump = 0.0f;
				m_pSkeletonNode->ClearAnimation();
			}
		}
		break;

		case STATE_MC_TREAMBING:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_treamble", DJTRUE);
			}

			// Finish treambing
			m_fTimeToJump += pTheApp->GetDeltaAppTime();
			if(m_fTimeToJump >=m_fTimeDurations)
			{
				m_uState = STATE_MC_DIE;																														   
				m_fTimeToJump = 0.0f;
				m_pSkeletonNode->ClearAnimation();
			}
		}
		break;

		case STATE_MC_DIE:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_die", DJTRUE);
			}
		}
		break;

		case STATE_MC_FINISH:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_finish", DJTRUE);
			}
		}
	};
}
///
/// Term all pointer
///
void MonkeyCivilians::Term()
{
}
///
///	Move object delta distance
///
void MonkeyCivilians::Move(const DJVector2 &vDelta)
{  	
}
///
///
///
void MonkeyCivilians::Fall(const DJVector2 &vDelta)
{		
}
///
///
///
void MonkeyCivilians::Jump(const DJVector2 &vDelta)
{

}  
///
///
///
djbool MonkeyCivilians::OnHit(const DJRECT &box)
{	
	djint32 nPointDetectY = m_rectBoxHit.nY + m_rectBoxHit.nH;
	if((box.nY - box.nH) - nPointDetectY <= 0)
	{
#ifdef _DEBUG
		//DJInfo("Hit:%d", (box.nY - box.nH) - nPointDetectY);
#endif
		m_pSkeletonNode->ClearAnimation();
		m_fTimeToJump = 0.0f;
		return DJTRUE;		
	}
	return DJFALSE;
}

///
///
///
void MonkeyCivilians::Reset()
{	 
	m_vPos = m_vOrgPos;
	m_pSkeletonNode->SetPosition(m_vOrgPos);
	m_pSkeletonNode->ClearAnimation();
	m_uState = STATE_MC_STANDING;
	m_bEnableJump = DJFALSE;
	m_fTimeToJump = 0.0f;	
}
// End class Monkey civilians
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Begin class BeatsTime
///
BeatsTime::BeatsTime(): m_nID(-1), m_nNumberOfMonkeyCilivians(-1), m_vBeatsTime(DJVector2(0.0f))
{
}

///
BeatsTime::~BeatsTime()
{
	Term();
}

///
djbool BeatsTime::Init(djint32 nID, djint32 nNumber, DJVector2 vBeats)
{
	m_nID = nID;
	m_nNumberOfMonkeyCilivians = nNumber;
	m_vBeatsTime = vBeats;
	return DJTRUE;
}

///
void BeatsTime::UpDate(djfloat fDeltaTime)
{
	
}

///
void BeatsTime::Term()
{

}
// End class BeatsTime
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END()
/////////////////////////////////////////////////////////////////