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
#include <djsounddevice.h>
#include <djfont.h>
#include <dj2dutil.h>
#include <spine/Bone.h>
#include <spine/Skin.h>
#include <spine/RegionAttachment.h>
#include <spine/animation.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "player.h"
#include "level.h"
#include "monkeyking.h"
#include "camera.h"
#include "musichandler.h"
#include "debug.h"
#include "util.h"
/////////////////////////////////////////////////////////////////
extern djuint32 g_nScreenHeight;
extern djuint32 g_nScreenWidth;
extern Camera *g_pCamera;
extern DJFont*		g_pFont;
struct Bone;
extern Player* g_pPlayer;

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

/////////////////////////////////////////////////////////////////
// Begin class Player
Player::Player()
{
	m_vPos = DJVector2(0.0f);
	m_vAcceleration = DJVector2(0.0f);
	m_vVelocity = DJVector2(0.0f); 	
	m_uState = STATE_NONE;		
	m_pStickGold = NULL;
}

///

Player::~Player()
{
	Term();
}

///

djbool Player::Init(DJString id, DJVector2 vpos, DJVector2 vSize, DJString strAnimFile, LevelScene* pLevelScene)
{
	m_sID = id;
	m_vPos = vpos;
	m_vSize = vSize;
	// init sprite
	m_pSprite = DJ_NEW(DJ2DSprite);
	m_pSprite->SetFilter(DJ2DSprite::filterLinear);
	m_pSprite->SetPosition(vpos);
	m_pSprite->SetSortValue(m_pSprite->GetPosition()[1]);
	m_pSprite->SetScale(DJVector2(1.0f, 1.0f));

	m_pAnimation =	(DJ2DAnimation*)theResourceManager.GetResource(strAnimFile, DJResource::TYPE_ANIMATION2D);
	if(m_pAnimation)
	{
		m_pSprite->PlayAnimation(m_pAnimation);
	}
	
	// Add sprrite to Sprite Engine
	if(!m_pSprite)
	{
		DJAssert(m_pSprite != NULL)	;
		return DJFALSE;
	}
	else
	{
		theSpriteEngine.AddActiveNode(m_pSprite);
		theSpriteEngine.AddNode(m_pSprite, LAYER_SPRITES);
	}

	// Init stick gold
	LevelScene::StickGoldData pSGD = pLevelScene->GetStickGoldData();
	m_pStickGold = DJ_NEW(StickGold);
	m_pStickGold->Init(pSGD.vPos, pSGD.strAtlastFile, pSGD.strAnimationName);
	return DJTRUE;	
}

///
void Player::Paint()
{/*
	if(m_bTouch)
	{
		g_pFont->DrawString(
			"Boom", 
			DJVector3((djfloat)g_nScreenWidth/2,(djfloat)g_nScreenHeight/2,0), 
			DJVector2(32,32), 
			0xFFFFFFFF);
	}*/
}

///

djbool Player::Update(djfloat fDeltaTime)
{			
#ifdef _DEV
	DJRECT box;	
	//MakeBox(&box,m_vPos, 
	theBoundingBoxCollection.QueueBoundingBox(box);
#endif //_DEV
	m_pStickGold->Update(fDeltaTime);
	return DJTRUE;
}

///

void Player::Term()
{
	if(m_pSprite)
	{
		theSpriteEngine.RemoveActiveNode(m_pSprite);
		DJ_SAFE_DELETE(m_pSprite);		
	}	
}

///

void Player::Reset()
{

}

///

djint32 Player::OnTouchBegin( djint32 nDevice, djint32 nID, float fX, float fY )
{
	m_pStickGold->OnTouchBegin(nDevice, nID, fX, fY);	
	return 0;
}

///

//DJRECT* Player::MakeBox(DJRECT *pRect, const DJVector2 &vBottom, djint32 nWidth, djint32 nHeight) const
//{
//	DJRECT* rect = Entity::MakeBox(pRect, vBottom, nWidth, nHeight);
//	return rect;
//}

// End class Player
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Begin class Monkey civilians

/////////////////////////////////////////////////////////////////
// Animation path of all monkey civilians

/////////////////////////////////////////////////////////////////
const char* l_szMonkeyCiviliansAnim[MAX_MONKEY_CIVILIANS] = 
{
	"monkey_civilians_01",
	"monkey_civilians_02",
	"monkey_civilians_03"
};

const char* STR_MONKEYCIV_SLOTNAME = "monkey_jump";
///
/// Constructor 
///
MonkeyCivilians::MonkeyCivilians()
{
	m_sID = "";	 
	m_vSize = DJVector2(0.0f, 0.0f);
	m_vOrgSize = DJVector2(0.0f, 0.0f);
	m_vOrgPos = DJVector2(0.0f, 0.0f);
	m_vPos = DJVector2(0.0f, 0.0f);
	m_pSkeletonNode = NULL;
	m_uState = STATE_MC_STANDING;
	m_rectBoxHit = DJRECT(0,0,0,0);
	m_fTimeToJump = 0.0f;
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
djbool MonkeyCivilians::Init(DJString id, DJVector2 vpos, djint32 nState)
{
	m_sID = id;
	m_vPos = vpos;
	m_vOrgPos = vpos;
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

    return DJFALSE;
}
///
/// Update object with applications delta time 
///
void MonkeyCivilians::Update(djfloat fDeltaTime)
{
	// update position of object
	Bone* bone = m_pSkeletonNode->FindBone("root");
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
	DJRECT box;
	MakeBox(&box, vPos, m_vSize.e[0], m_vSize.e[1]);
	theBoundingBoxCollection.QueueBoundingBox(box);
	DJInfo("%d, %d", m_rectBoxHit.nX, m_rectBoxHit.nY);
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
			m_fTimeToJump += pTheApp->GetDeltaAppTime();
			if(m_fTimeToJump >= 2.0f)
			{
				m_uState = STATE_MC_JUMP;
				m_fTimeToJump = 0.0f;
				m_pSkeletonNode->ClearAnimation();
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
			if(m_fTimeToJump >= 0.6666)
			{
				if(OnHit(g_pPlayer->GetStickGold()->GetRectBoxHit()))
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
		}
		break;

		case STATE_MC_TREAMBING:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("monkey_stand", DJTRUE);
			}
		}
		break;
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
		DJInfo("Hit:%d", (box.nY - box.nH) - nPointDetectY);
#endif
		m_pSkeletonNode->ClearAnimation();
		m_fTimeToJump = 0.0f;
		return DJTRUE;		
	}
	return DJFALSE;
}
//DJRECT* MonkeyCivilians::MakeBox(DJRECT *pRect, const DJVector2 &vBottom, djint32 nWidth, djint32 nHeight) const
//{
//	DJRECT* rect = Entity::MakeBox(pRect, vBottom, nWidth, nHeight);
//	return rect;
//}
// End class Monkey civilians
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Begin StickGold class
///
/// Define 
///
const char* STR_STICKGOLD_SLOTNAME = "stickgold";
const char* STR_STICKGOLD_BONENAME = "root";
///
/// Construction
///
StickGold::StickGold()
{
	m_pSkeletonNode		= NULL;
	m_vOrgPos			= DJVector2(0.0f);
	m_vPos				= DJVector2(0.0f);
	m_vSize				= DJVector2(0.0f);
	m_vOrgSize			= DJVector2(0.0f);
	m_rectBoxHit		= DJRECT(0,0,0,0);
	m_uState			= STATE_SG_STAND;
	m_bEnableTouch		= DJTRUE;
	m_fTimeChangeAnim	= 0.0f;
}

///
/// Destruction
///
StickGold::~StickGold()
{
	Term();
}

///
///	Initialize
///
djbool StickGold::Init(DJVector2 vPos, DJString strAtlastFile, DJString strAnimName)
{
	m_vPos		= vPos;
	m_vOrgPos	= vPos;
	
	m_pSkeletonNode = DJ_NEW(DJ2DSkeletonNode);
	m_pSkeletonNode->SetPosition(vPos);
	m_pSkeletonNode->SetScale(DJVector2(1.0f, 1.0f));
	m_pSkeletonNode->Create(strAtlastFile);
	m_pSkeletonNode->SetAnimation(strAnimName, DJTRUE);
	m_pSkeletonNode->SetMix("stand", "zoom_in", 0.1f);
	m_pSkeletonNode->GetAnimationTimeScale();
	theSpriteEngine.AddActiveNode(m_pSkeletonNode);
	theSpriteEngine.AddNode(m_pSkeletonNode, LAYER_SPRITES);

	// set size for object
	m_vSize = GetSizeFromSpine(STR_STICKGOLD_SLOTNAME, m_pSkeletonNode);
	m_vOrgSize = m_vSize;													
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());

	return DJTRUE;
}

///
/// Update 
///
void StickGold::Update(djfloat fDeltaTime)
{
	Bone* bone = m_pSkeletonNode->FindBone(STR_STICKGOLD_BONENAME);
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
	DJRECT box;
	MakeBox(&box,  m_vPos, m_vSize.e[0], m_vSize.e[1]);
	theBoundingBoxCollection.QueueBoundingBox(box);
#endif //_DEV
	
	switch(m_uState)
	{
		case STATE_SG_STAND:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("stand", DJTRUE);		   
			}
		}
		break;

		case STATE_SG_ZOOMIN:
		{  	
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation("zoom_in", DJTRUE);				
				m_pSkeletonNode->SetAnimationTimeScale(0.6666f);
			}
			m_fTimeChangeAnim += pTheApp->GetDeltaAppTime();
			if(m_fTimeChangeAnim > m_pSkeletonNode->GetAnimationTimeScale())
			{
			   m_uState = STATE_SG_STAND;
			   m_fTimeChangeAnim = 0.0f;
			   m_pSkeletonNode->ClearAnimation();
			}			
		}
		break;
	} 
}

///
/// Term
///
void StickGold::Term()
{

}  

///

djint32	StickGold::OnTouchBegin( djint32 nDevice, djint32 nID, float fX, float fY )
{
	if(!m_bEnableTouch)
		return 0;

	if(m_uState != STATE_SG_ZOOMIN)
	{
		m_pSkeletonNode->ClearAnimation();
		m_uState = STATE_SG_ZOOMIN;
	}
	return 0;
}

///

djint32 StickGold::	OnTouchMove( djint32 nDevice, djint32 nID, float fX, float fY )
{
	return 0;
}

///

djint32	StickGold::OnTouchEnd( djint32 nDevice, djint32 nID, float fX, float fY )
{
	return 0;
}

///

djint32	StickGold::OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY )
{
	return 0;
}
// End StickGold class
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
