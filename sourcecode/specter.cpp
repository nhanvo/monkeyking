/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djsounddevice.h>
#include <spine/Bone.h>
#include <spine/Skin.h>
#include <spine/Skeleton.h>
#include <spine/Attachment.h>

/////////////////////////////////////////////////////////////////
// Game includes
#include "specter.h"
#include "util.h"
#include "musichandler.h"
#include "debug.h"
#include "monkeyking.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START(); 
/////////////////////////////////////////////////////////////////
extern djfloat g_fGameTimeScale;
extern Player* g_pPlayer; 
extern LevelManager	*g_pLevelManager;
/////////////////////////////////////////////////////////////////
// Ray ghost
const char* lsz_AnimCircleName[RaysGhost::ANIM_CIRCLE_COUNT] = 
{
	"animation_0",
	"animation_1",
	"animation_2",
	"animation_3",
	"animation_4",
	"animation_5",
	"animation_6",
	"animation_7",
};

DJRECT	g_arrRectCircleBullet[RaysGhost::ANIM_CIRCLE_COUNT] = 
{
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
};

// animation name on sprites/light_effect.json file
const char* lsz_AnimCircleFastName[RaysGhost::ANIM_CIRCLE_FAST_COUNT] = 
{
	"animation_0",
	"animation_1",
	"animation_2",
	"animation_3",
	"animation_4",
	"animation_5",
	"animation_6",
	"animation_7",
};

DJRECT	g_arrRectCircleFastBullet[RaysGhost::ANIM_CIRCLE_FAST_COUNT] = 
{
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
};	

// animation name on sprites/spiral_effect.json file
const char* lsz_AnimSpiralName[RaysGhost::ANIM_SPIRAL_COUNT] = 
{
	"animation_spiral_one",
	"animation_spiral_two",
	"animation_spiral_three",
	"animation_spiral_four",
	"animation_spiral_five",
	"animation_spiral_six",
	"animation_spiral_seven",
	"animation_spiral_eight",
}; 

DJRECT	g_arrRectCircleSpiralBullet[RaysGhost::ANIM_CIRCLE_FAST_COUNT] = 
{
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
	DJRECT(0,0,0,0),
};

const char* g_szAtlastCircleAnimFile		= "sprites/circle_effect";
const char* g_szAtlastCircleFastAnimFile	= "sprites/circle_effect_fast";
const char* g_szAtlastSpiralAnimFile		= "sprites/spiral_effect";

const char* g_szCircleSlotName				= "circle_effect";
const char* g_szCircleFastSlotName			= "circle";
const char* g_szSpiralSlotName				= "circle";

const char* g_szCircleBone					= "root";
const char* g_szCircleFastBone				= "all";
const char* g_szSpiralBone					= "all";

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Centipede
const char* g_szAtlastCentipedeAnimFile		= "sprites/centipede";

/////////////////////////////////////////////////////////////////

RaysGhost::RaysGhost() : m_uState(STATE_RG_STAND), m_nBeatTimeID(0), m_uMaxAnim(0), 
						 m_uType(0), m_strAtlastFile(""), m_nID(0), m_pRectHitBox(NULL),
						 m_strSlotName(""), m_strBoneName("")
{ 	
	for(djuint i = 0; i < ANIM_CIRCLE_COUNT; i++)
	{
		m_bCircleEffectHit[i] = DJFALSE;
	}
	for(djuint i = 0; i < ANIM_CIRCLE_FAST_COUNT; i++)
	{
		m_bCircleFastEffectHit[i] = DJFALSE;
	}
	for(djuint i = 0; i < ANIM_SPIRAL_COUNT; i++)
	{
		m_bSpiralEffectHit[i] = DJFALSE;
	}
}

///

RaysGhost::~RaysGhost()
{
	Term();
}

///

djbool RaysGhost::Init(djint32 id, djint32 nType, DJVector2 vPos, djint32 nBeatTime)
{
	m_nID			= id;
	m_vPos			= vPos;
	m_vOrgPos		= m_vPos;
	m_nBeatTimeID	= nBeatTime; 
	m_uType			= nType;
	if(m_uType >= TYPE_RG_COUNT)
	{
		DJWarning("Ray ghost line %d is TYPE faild",id);
		DJAssert(m_uType < TYPE_RG_COUNT);
	}
	
	// circle effect 
	if(m_uType == TYPE_RG_CIRCLE)
	{
		if(m_nID % 2 == 0)
		{
			m_strAtlastFile = g_szAtlastCircleAnimFile;
			m_uMaxAnim		= ANIM_CIRCLE_COUNT;
			m_pRectHitBox	= &g_arrRectCircleBullet[0];
			m_strSlotName	= g_szCircleSlotName;
			m_strBoneName	= g_szCircleBone;
		}
		else
		{
			m_strAtlastFile = g_szAtlastCircleFastAnimFile;
			m_uMaxAnim		= ANIM_CIRCLE_FAST_COUNT;
			m_pRectHitBox	= &g_arrRectCircleFastBullet[0];
			m_strSlotName	= g_szCircleFastSlotName;
			m_strBoneName	= g_szCircleFastBone;
		}
	}
	else if(m_uType == TYPE_RG_SPIRAL)
	{
		/*m_strAtlastFile = g_szAtlastSpiralAnimFile;
		m_uMaxAnim = ANIM_SPIRAL_COUNT;*/
		m_strAtlastFile = g_szAtlastCircleFastAnimFile;
		m_uMaxAnim		= ANIM_CIRCLE_FAST_COUNT;
		m_pRectHitBox	= &g_arrRectCircleSpiralBullet[0];
		m_strSlotName	= g_szSpiralSlotName;
		m_strBoneName	= g_szSpiralBone;
	}

	// Init skeleton node
	djuint32 uID = 0;
	while((uID < m_uMaxAnim))
	{
		SkeletonAnimNode* pNode = DJ_NEW(SkeletonAnimNode);
		pNode->s_pSkeletonNode = DJ_NEW(DJ2DSkeletonNode);
		pNode->s_pSkeletonNode->SetPosition(m_vPos);			
		pNode->s_pSkeletonNode->Create(m_strAtlastFile);		
		theSpriteEngine.AddActiveNode(pNode->s_pSkeletonNode);
		theSpriteEngine.AddNode(pNode->s_pSkeletonNode, LAYER_SPRITES);	

		if(m_uType == TYPE_RG_CIRCLE)
		{
			if(m_nID % 2 == 0)
			{
				g_arrRectCircleBullet[uID] = CalculateBordingBox(pNode->s_pSkeletonNode); 				
			}
			else 
			{
				g_arrRectCircleFastBullet[uID] = CalculateBordingBox(pNode->s_pSkeletonNode);
			}
		}
		else if(m_uType == TYPE_RG_SPIRAL)
		{
			if(!pNode->s_pSkeletonNode->SetSkin("circle_bigbig"))
			{
				DJAssert(DJFALSE);
			}
			g_arrRectCircleSpiralBullet[uID] = CalculateBordingBox(pNode->s_pSkeletonNode);
		}

		// Set time duration
		pNode->s_fTimeDuration = GetTimeDurationOfAnimation(pNode->s_pSkeletonNode);
		DJAssert(pNode->s_fTimeDuration != 0);

		// Set time finish anim 
		pNode->s_fTimeFinishAnim = 0.0f;
		
		// Set ID for animation
		pNode->s_uIDAnim = uID;
		m_listSkeletonAnimNode.AddLast(pNode);
		uID++;
	}	

	return DJTRUE;
}

///

void RaysGhost::Reset()
{
	if(m_uState == STATE_RG_FINISH_SHOOT)
	{
		m_uState = STATE_RG_STAND; 	
	}
	if(m_uState == STATE_RG_SHOOT)
	{
		DJAssert(DJFALSE);
	}
}

///

void RaysGhost::Update(djfloat fDeltaTime)
{
	if(m_uState == STATE_RG_SHOOT)
	{
		DJLinkedListIter<SkeletonAnimNode> iter(m_listSkeletonAnimNode);
		SkeletonAnimNode* pNode;
		while((pNode = iter.GetStep()))
		{
			if(!pNode->s_pSkeletonNode->IsAnyAnimationRunning())
			{
				if(m_uType == TYPE_RG_CIRCLE)
				{
					if(m_nID % 2 == 0)
					{
						pNode->s_pSkeletonNode->SetAnimation(lsz_AnimCircleName[pNode->s_uIDAnim], DJTRUE);							
					}
					else
					{
						pNode->s_pSkeletonNode->SetAnimation(lsz_AnimCircleFastName[pNode->s_uIDAnim], DJTRUE);	
					}
				}
				else if(m_uType == TYPE_RG_SPIRAL)
				{
					pNode->s_pSkeletonNode->SetAnimation(lsz_AnimCircleFastName[pNode->s_uIDAnim], DJTRUE);	
				} 				
			}
			
			m_vOrgPos = m_vPos;
			pNode->s_pSkeletonNode->SetPosition(m_vPos);			

			pNode->s_fTimeFinishAnim += pTheApp->GetDeltaAppTime();
			if(pNode->s_fTimeFinishAnim >= pNode->s_fTimeDuration)
			{
				m_uState = STATE_RG_FINISH_SHOOT;
				pNode->s_pSkeletonNode->ClearAnimation();
				pNode->s_fTimeFinishAnim = 0.0f;
			}

			//
			spBone* bone = pNode->s_pSkeletonNode->FindBone(m_strBoneName);
			if(bone)
			{
				DJVector2 vPos = m_vOrgPos;
				vPos.e[1] += bone->y;
				vPos.e[0] += bone->x;

				DJRECT rect = DJRECT(vPos.e[0], vPos.e[1], m_vSize.x(), m_vSize.y());

				if(m_uType == TYPE_RG_CIRCLE)
				{
					if(m_nID %2 == 0)
					{
						g_arrRectCircleBullet[pNode->s_uIDAnim] = rect;	
					}
					else
					{
						g_arrRectCircleFastBullet[pNode->s_uIDAnim] = rect;
					}
				}
				else if(m_uType == TYPE_RG_SPIRAL)
				{
					g_arrRectCircleSpiralBullet[pNode->s_uIDAnim] = rect;
				}
			}
		} 

		if(m_uType == TYPE_RG_CIRCLE)
		{
			if(m_nID % 2 == 0)
			{
				m_pRectHitBox = &g_arrRectCircleBullet[0];
				for(djint32 i = 0; i < ANIM_CIRCLE_COUNT; i++)
				{
					if(!m_bCircleEffectHit[i])
					{
						if(OnHit(m_pRectHitBox))
						{
							/// TODO:: Processs if hit with player
							DJInfo("HIT CIRCLE %d", m_nID);
							m_bCircleEffectHit[i] = DJTRUE;
						}
					}
					#ifdef _DEV
						DJRECT box;	
						DJVector2 vPosHit = DJVector2(m_pRectHitBox->nX, m_pRectHitBox->nY);
						MakeBox(&box,vPosHit,m_pRectHitBox->nW, m_pRectHitBox->nH);
						theBoundingBoxCollection.QueueBoundingBox(box);
					#endif //_DEV
					m_pRectHitBox ++;
				}
			}
			else 
			{
				m_pRectHitBox = &g_arrRectCircleFastBullet[0];
				for(djint32 i = 0; i < ANIM_CIRCLE_FAST_COUNT; i++)
				{
					if(!m_bCircleFastEffectHit[i])
					{
						if(OnHit(m_pRectHitBox))
						{
							/// TODO:: Processs if hit with player
							DJInfo("HIT FAST CIRCLE %d", m_nID);
							m_bCircleFastEffectHit[i] = DJTRUE;
						}
					}
					#ifdef _DEV
						DJRECT box;	
						DJVector2 vPosHit = DJVector2(m_pRectHitBox->nX, m_pRectHitBox->nY);
						MakeBox(&box,vPosHit,m_pRectHitBox->nW, m_pRectHitBox->nH);
						theBoundingBoxCollection.QueueBoundingBox(box);
					#endif //_DEV
					m_pRectHitBox ++;
				}				
			}
		}
		else if(m_uType == TYPE_RG_SPIRAL)
		{
			m_pRectHitBox =	&g_arrRectCircleSpiralBullet[0];
			for(djint32	i =	0; i < ANIM_CIRCLE_FAST_COUNT; i++)
			{
				if(!m_bSpiralEffectHit[i])
				{
					if(OnHit(m_pRectHitBox))
					{
						///	TODO:: Processs	if hit with	player
						DJInfo("HIT SPIRAL %d", m_nID);
						m_bSpiralEffectHit[i] = DJTRUE;
					}
				}
				#ifdef _DEV
					DJRECT box;	
					DJVector2 vPosHit = DJVector2(m_pRectHitBox->nX, m_pRectHitBox->nY);
					MakeBox(&box,vPosHit,m_pRectHitBox->nW, m_pRectHitBox->nH);
					theBoundingBoxCollection.QueueBoundingBox(box);
				#endif //_DEV
				m_pRectHitBox ++;
			}
		}		 
	}
}

///

void RaysGhost::Term()
{
	m_listSkeletonAnimNode.Clear();	
}

///
djbool RaysGhost::OnHit(const DJRECT* pRect)
{
	// hit
	if(((pRect->nX + pRect->nW) > m_rectTarget.nX) && ((pRect->nX + pRect->nW) < (m_rectTarget.nX + m_rectTarget.nW)))
	{
		if((pRect->nY > m_rectTarget.nY) && (pRect->nY + pRect->nH) < (m_rectTarget.nY + m_rectTarget.nH))
		{
			return DJTRUE;
		}
	}
	return DJFALSE;
}

///

DJRECT* RaysGhost::MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const
{
	pRect->nX = djTruncToInt(vPos.e[0] - nWidth/2.0f);
	pRect->nY = djTruncToInt(vPos.e[1] - nHeight/2.0f);
	pRect->nW = nWidth;
	pRect->nH = nHeight;
	return pRect;
}

///

DJRECT RaysGhost::CalculateBordingBox(const DJ2DSkeletonNode* pNode)
{
	DJRECT rect = DJRECT(0.0f,0.0f,0.0f,0.0f);	
	spBone* pBone = pNode->FindBone(m_strBoneName);
	DJVector2 vPosBullet = DJVector2(0.0f);
	m_vPos.e[0] += pBone->x;
	m_vPos.e[1] += pBone->y;

	DJVector2 vSize = GetSizeFromSpine(m_strSlotName, pNode);
	vSize.e[0] *= pBone->scaleX;
	vSize.e[1] *= pBone->scaleY;
	m_vSize = vSize;
	m_vOrgSize = m_vSize;

	rect = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());
	return rect;
}

/////////////////////////////////////////////////////////////////
// Centipede
const char*	g_szAnimCentipede[Centipede::STATE_CEP_SHOOP_WEAPON_COUNT] = 
{
	"shoot_weapon_1",
	"shoot_weapon_2",
	"shoot_weapon_3",
	"shoot_weapon_4",
};

const char* STR_CENTIPEDE_SLOTNAME = "centipede";

Centipede::Centipede():m_pSkeletonNode(NULL), m_uState(STATE_CEP_SHOOT_WEAPON_1)
{
	m_fTimeAnimActive	= 0.0f;
	m_fTimeDuration		= 0.0f;
	m_fTimeChangeState[0]	= m_fTimeChangeState[1] = m_fTimeChangeState[2] = 0.0f;
	m_bFinishState[0]		= m_bFinishState[1] = m_bFinishState[2] = DJFALSE;
	m_rectBoxHit			= DJRECT(0,0,0,0);
	m_rectTargetHitBox		= DJRECT(0,0,0,0);
}

/// Destructor

Centipede::~Centipede()
{
	Term();
}

/// Init object

djbool Centipede::Init(djint32 uType, DJVector2 vPos)
{
	m_nType		= uType;
	m_vPos		= vPos;
	m_vOrgPos	= m_vPos;	 	

	// init skeleton
	m_pSkeletonNode = DJ_NEW(DJ2DSkeletonNode);
	m_pSkeletonNode->SetPosition(m_vPos);			
	m_pSkeletonNode->Create(g_szAtlastCentipedeAnimFile);		
	theSpriteEngine.AddActiveNode(m_pSkeletonNode);
	theSpriteEngine.AddNode(m_pSkeletonNode, LAYER_SPRITES);

	// Set time change animation
	m_fTimeDuration = GetTimeDurationOfAnimation(m_pSkeletonNode);

	// Set time change state
	if(m_listBeatsTime.GetLength() != 0)
	{
		djuint nCnt = 1;
		for(djint32 i = 0; i < STATE_CEP_SHOOP_WEAPON_COUNT - 1; i++)
		{
			djint32 nIdx = djRoundToInt(m_listBeatsTime.GetLength()*nCnt / STATE_CEP_SHOOP_WEAPON_COUNT);
			m_fTimeChangeState[i] = m_listBeatsTime.GetByIndex(nIdx)->GetBeatsTime().e[0];	
			nCnt++;
		}
	}

	// Init size
	m_vSize = GetSizeFromSpine(STR_CENTIPEDE_SLOTNAME, m_pSkeletonNode);

	// Init rect box hit
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());

	return DJTRUE;
}

/// Update object

void Centipede::Update(djfloat fDeltaTime)
{
	//////////////////////////////////////////////////////////////////////////////
	// update centipede
	spBone* pBone = m_pSkeletonNode->FindBone("centipede");
	switch(m_uState)
	{
		case STATE_CEP_SHOOT_WEAPON_1: 		
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation(g_szAnimCentipede[STATE_CEP_SHOOT_WEAPON_1], DJTRUE);				
			}

			if(pTheSoundDevice->GetMusicPos() != -1)
			{
				if(!m_bFinishState[0] && pTheSoundDevice->GetMusicPos() >= m_fTimeChangeState[0])
				{
					m_pSkeletonNode->ClearAnimation();
					m_fTimeAnimActive = 0.0f;
					m_uState = STATE_CEP_SHOOT_WEAPON_2;	
				}
				else if(!m_bFinishState[1] && pTheSoundDevice->GetMusicPos() >= m_fTimeChangeState[1])
				{
					m_pSkeletonNode->ClearAnimation();
					m_fTimeAnimActive = 0.0f;
					m_uState = STATE_CEP_SHOOT_WEAPON_3;
				}
				else if(!m_bFinishState[2] && pTheSoundDevice->GetMusicPos() >= m_fTimeChangeState[2])
				{
					m_pSkeletonNode->ClearAnimation();
					m_fTimeAnimActive = 0.0f;
					m_uState = STATE_CEP_SHOOT_WEAPON_4;
				}  				
			} 			
		}
		break;

		case STATE_CEP_SHOOT_WEAPON_2:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation(g_szAnimCentipede[STATE_CEP_SHOOT_WEAPON_2], DJTRUE);				
			} 
			if(pBone)
			{
				DJVector2 vPos;
				vPos = m_vOrgPos;
				vPos.e[0] += djFloatAbs(pBone->x);
				vPos.e[1] += djFloatAbs(pBone->y);	 
				m_vPos = vPos;		
			}
			m_fTimeAnimActive += fDeltaTime;			
			if(m_fTimeAnimActive >= m_fTimeDuration)
			{
				m_pSkeletonNode->ClearAnimation();
				m_fTimeAnimActive = 0.0f;
 				m_bFinishState[0] = DJTRUE;
				m_uState = STATE_CEP_SHOOT_WEAPON_1;				
				m_vOrgPos = m_vPos;
				m_pSkeletonNode->SetPosition(m_vPos);
			}
		}
		break;

		case STATE_CEP_SHOOT_WEAPON_3:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation(g_szAnimCentipede[STATE_CEP_SHOOT_WEAPON_3], DJTRUE);				
			}
			if(pBone)
			{
				DJVector2 vPos;
				vPos = m_vOrgPos;
				vPos.e[0] -= djFloatAbs(pBone->x);
				vPos.e[1] -= djFloatAbs(pBone->y);	 
				m_vPos = vPos;		
			}
			m_fTimeAnimActive += fDeltaTime;			
			if(m_fTimeAnimActive >= m_fTimeDuration)
			{
				m_pSkeletonNode->ClearAnimation();
				m_fTimeAnimActive = 0.0f;
				m_bFinishState[1] = DJTRUE;
				m_uState = STATE_CEP_SHOOT_WEAPON_1;	
				m_vOrgPos = m_vPos;
				m_pSkeletonNode->SetPosition(m_vPos);
			}
		}
		break;

		case STATE_CEP_SHOOT_WEAPON_4:
		{
			if(!m_pSkeletonNode->IsAnyAnimationRunning())
			{
				m_pSkeletonNode->SetAnimation(g_szAnimCentipede[STATE_CEP_SHOOT_WEAPON_4], DJTRUE);				
			}
			if(pBone)
			{
				DJVector2 vPos;
				vPos = m_vOrgPos;
				vPos.e[0] += djFloatAbs(pBone->x);
				vPos.e[1] -= djFloatAbs(pBone->y);	 
				m_vPos = vPos;	
			}
			m_fTimeAnimActive += fDeltaTime;			
			if(m_fTimeAnimActive >= m_fTimeDuration)
			{
				m_pSkeletonNode->ClearAnimation();
				m_fTimeAnimActive = 0.0f;
				m_bFinishState[2] = DJTRUE;
				m_uState = STATE_CEP_SHOOT_WEAPON_1;	
				m_vOrgPos = m_vPos;
				m_pSkeletonNode->SetPosition(m_vPos);
			}
		}
		break;
	};	

	// update rect box hit
	m_rectBoxHit = DJRECT(m_vPos.x(), m_vPos.y(), m_vSize.x(), m_vSize.y());

	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// update ray ghost
	DJLinkedListIter<RaysGhost> iterRayGhost(m_listRayGhost);	
	RaysGhost *pRG;
	while((pRG = iterRayGhost.GetStep()))
	{
		if(pRG->GetBeatsTimeID() == -1)
		{
			DJWarning("Ray ghost invalid beats time");		
			DJAssert((pRG->GetBeatsTimeID() != -1));
		}
		else
		{
			DJLinkedListIter<BeatsTime> iterBeatsTime(m_listBeatsTime);
			BeatsTime *pBT;
			while((pBT = iterBeatsTime.GetStep()))
			{
				if(pRG->GetBeatsTimeID() == pBT->GetID())
				{
					if(pTheSoundDevice->GetMusicPos() >= pBT->GetBeatsTime().e[0] && 
					   pTheSoundDevice->GetMusicPos() <= pBT->GetBeatsTime().e[1])
					{
						pRG->SetState(RaysGhost::STATE_RG_SHOOT); 	
					}
				}
			}
			pRG->SetPosition(m_vPos);
			pRG->SetTargetHitBox(g_pPlayer->GetHitBox());
			pRG->Update(fDeltaTime);   
			if(pRG == m_listRayGhost.GetLast() && pRG->GetState() == RaysGhost::STATE_RG_FINISH_SHOOT)
			{
				if(m_uState == STATE_CEP_SHOOT_WEAPON_1)
				{
					g_pLevelManager->GetCurrentLevel()->Reset();
					g_pLevelManager->GetCurrentLevel()->SetFinishScene(DJTRUE);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////
}

/// Term object

void Centipede::Term()
{
	if(m_pSkeletonNode)
	{
		theSpriteEngine.RemoveNode(m_pSkeletonNode);
		theSpriteEngine.RemoveActiveNode(m_pSkeletonNode);
		DJ_SAFE_DELETE(m_pSkeletonNode);
	}

	m_listRayGhost.Clear();
	m_listBeatsTime.Clear();
}

/// Init rayghost

void Centipede::SetListRayGhost(DJLinkedList<RaysGhost> &listRayGhost)
{
	listRayGhost.CopyTo(m_listRayGhost);
}

/// Init beats time

void Centipede::SetListBeatsTime(DJLinkedList<BeatsTime> &listBeatsTime)
{
	listBeatsTime.CopyTo(m_listBeatsTime);
}

/// Reset

void Centipede::Reset()
{
	// Centipede 	
	for(djuint32 i = 0; i <= STATE_CEP_SHOOP_WEAPON_COUNT-1; i++)
	{
		m_bFinishState[i] = DJFALSE;
	}

	// Rayghost
	DJLinkedListIter<RaysGhost> iterRayGhost(m_listRayGhost);	
	RaysGhost *pRG;
	while((pRG = iterRayGhost.GetStep()))
	{
		pRG->Reset();
	}
}

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////