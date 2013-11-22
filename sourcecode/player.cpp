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
#include <djgamesounds.h>
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
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

extern djuint32 g_nScreenHeight;
extern djuint32 g_nScreenWidth;
extern Camera *g_pCamera;
extern DJFont*		g_pFont;
struct Bone;
extern Player* g_pPlayer;

/////////////////////////////////////////////////////////////////
// Begin class Player
Player::Player()
{
	m_vPos = DJVector2(0.0f);
	m_vAcceleration = DJVector2(0.0f);
	m_vVelocity = DJVector2(0.0f); 	
	m_uState = STATE_NONE;	
}

///

Player::~Player()
{
	Term();
}

///

djbool Player::Init(DJString id, DJVector2 vpos, DJVector2 vSize, DJString strAnimFile)
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
// Begin StickGold class
///
/// Define 
///
const char* STR_STICKGOLD_SLOTNAME = "stick_gold";
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
	m_fTimeDuration		= 0.0f;
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
	m_pSkeletonNode->SetAnimationTimeScale(1.5f);
	theSpriteEngine.AddActiveNode(m_pSkeletonNode);
	theSpriteEngine.AddNode(m_pSkeletonNode, LAYER_SPRITES);

	// set size for object
	m_vSize = GetSizeFromSpine(STR_STICKGOLD_SLOTNAME, m_pSkeletonNode);
	m_vOrgSize = m_vSize;													
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());

	// set total time(duration) for one animation
	m_fTimeDuration = GetTimeDurationOfAnimation(m_pSkeletonNode);

	return DJTRUE;
}

///
/// Update 
///
void StickGold::Update(djfloat fDeltaTime)
{
	spBone* bone = m_pSkeletonNode->FindBone(STR_STICKGOLD_BONENAME);
	DJAssert(bone != NULL);
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
			}
			m_fTimeChangeAnim += pTheApp->GetDeltaAppTime();
			if(m_fTimeChangeAnim > m_fTimeDuration/2.0f)
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
