/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djsounddevice.h>
#include <spine/Bone.h>
#include <spine/Skin.h>
#include <spine/Skeleton.h>

/////////////////////////////////////////////////////////////////
// Game includes
#include "specter.h"
#include "util.h"
#include "musichandler.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START(); 
/////////////////////////////////////////////////////////////////

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

// animation name on sprites/light_effect.json file
const char* lsz_AnimCircleFastName[RaysGhost::ANIM_CIRCLE_FAST_COUNT] = 
{
	"animation_0",
	"animation_00",
	"animation_1",
	"animation_11", 
	"animation_2",
	"animation_22",
	"animation_3",
	"animation_33",
	"animation_4",
	"animation_44",
	"animation_5",
	"animation_55",
	"animation_6",
	"animation_66",
	"animation_7",
	"animation_77",
};

// animation name on sprites/light_effect.json file
const char* lsz_AnimLightName[RaysGhost::ANIM_LIGHT_COUNT] = 
{
	"animation_0",
	"animation_1",
	"animation_2",
	"animation_3",
	"animation_4",
	"animation_5", 
	"animation_6",
	"animation_7",
	"animation_8",
	"animation_9",
	"animation_10",
	"animation_11", 
	"animation_12",
	"animation_13",
	"animation_14",
	"animation_15",
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

const char* g_szAtlastCircleAnimFile		= "sprites/circle_effect";
const char* g_szAtlastCircleFastAnimFile	= "sprites/circle_effect_fast";
const char* g_szAtlastLightAnimFile			= "sprites/light_effect";
const char* g_szAtlastSpiralAnimFile		= "sprites/spiral_effect";
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Centipede
const char* g_szAtlastCentipedeAnimFile		= "sprites/centipede";

/////////////////////////////////////////////////////////////////

RaysGhost::RaysGhost() : m_uState(STATE_RG_STAND), m_nBeatTimeID(0), m_uMaxAnim(0), m_uType(0), m_strAtlastFile(""), m_nID(0)
{ 	
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
			m_uMaxAnim	= ANIM_CIRCLE_COUNT;
		}
		else
		{
			m_strAtlastFile = g_szAtlastCircleFastAnimFile;
			m_uMaxAnim	= ANIM_CIRCLE_FAST_COUNT;
		}
	}
	else if(m_uType == TYPE_RG_LIGHT)
	{
		m_strAtlastFile =   g_szAtlastLightAnimFile;
		m_uMaxAnim = ANIM_LIGHT_COUNT;
	}
	else if(m_uType == TYPE_RG_SPIRAL)
	{
		/*m_strAtlastFile = g_szAtlastSpiralAnimFile;
		m_uMaxAnim = ANIM_SPIRAL_COUNT;*/
		m_strAtlastFile = g_szAtlastCircleFastAnimFile;
		m_uMaxAnim = ANIM_CIRCLE_FAST_COUNT;
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
						pNode->s_pSkeletonNode->SetSkin("circle_small");
						pNode->s_pSkeletonNode->SetAnimation(lsz_AnimCircleFastName[pNode->s_uIDAnim], DJTRUE);
					}
				}
				else if(m_uType == TYPE_RG_LIGHT)
				{
					pNode->s_pSkeletonNode->SetAnimation(lsz_AnimLightName[pNode->s_uIDAnim], DJTRUE);	
				}
				else if(m_uType == TYPE_RG_SPIRAL)
				{
					pNode->s_pSkeletonNode->SetSkin("circle_bigbig");
					pNode->s_pSkeletonNode->SetAnimation(lsz_AnimCircleFastName[pNode->s_uIDAnim], DJTRUE);	
				}
			}
			pNode->s_fTimeFinishAnim += pTheApp->GetDeltaAppTime();
			if(pNode->s_fTimeFinishAnim >= pNode->s_fTimeDuration)
			{
				m_uState = STATE_RG_STAND;
				pNode->s_pSkeletonNode->ClearAnimation();
				pNode->s_fTimeFinishAnim = 0.0f;
			}
		}
		/// ::TODO Update position of RG[i]
	}
}

///

void RaysGhost::Term()
{
	
}

///

DJRECT* RaysGhost::MakeBox(DJRECT *pRect, const DJVector2 &vPos, djint32 nWidth, djint32 nHeight) const
{
	DJRECT* rect = DJ_NEW(DJRECT(0, 0, 0, 0));
	return rect;
}

/////////////////////////////////////////////////////////////////
// Centipede

Centipede::Centipede():m_pSkeletonNode(NULL), m_nBeatsTime(-1)
{

}

/// Destructor

Centipede::~Centipede()
{
	Term();
}

/// Init object

djbool Centipede::Init(djint32 uType, DJVector2 vPos, djint32 nBeatsTime)
{
	m_nType = uType;
	m_vPos = vPos;

	// init skeleton
	m_pSkeletonNode = DJ_NEW(DJ2DSkeletonNode);
	m_pSkeletonNode->SetPosition(m_vPos);			
	m_pSkeletonNode->Create(g_szAtlastCentipedeAnimFile);	
	m_pSkeletonNode->SetAnimation("animation", DJTRUE);
	theSpriteEngine.AddActiveNode(m_pSkeletonNode);
	theSpriteEngine.AddNode(m_pSkeletonNode, LAYER_SPRITES);
	return DJTRUE;
}

/// Update object

void Centipede::Update(djfloat fDeltaTime)
{
	//////////////////////////////////////////////////////////////////////////////
	// update ray ghost
	DJLinkedListIter<RaysGhost> iter(m_listRayGhost);
	RaysGhost* p;
	djint32 i = 0;
	while(p = iter.GetStep())
	{
		i++;
	}
	DJLinkedListIter<RaysGhost> iterRayGhost(m_listRayGhost);
	DJLinkedListIter<BeatsTime> iterBeatsTime(m_listBeatsTime);
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
			BeatsTime *pBT;
			while((pBT = iterBeatsTime.GetStep()))
			{
				if(pRG->GetBeatsTimeID() == pBT->GetID())
				{
					//DJInfo("%d", pTheSoundDevice->GetMusicPos());
					if(pTheSoundDevice->GetMusicPos() >= pBT->GetBeatsTime().e[0] && 
						pTheSoundDevice->GetMusicPos() <= pBT->GetBeatsTime().e[1])
					{
						pRG->SetState(RaysGhost::STATE_RG_SHOOT);
					}
				}
			}
			pRG->Update(fDeltaTime);	
		}
	}
	//////////////////////////////////////////////////////////////////////////////
}

/// Term object

void Centipede::Term()
{

}

/// Init rayghost

void Centipede::SetListRayGhost(DJLinkedList<RaysGhost> listRayGhost)
{
	m_listRayGhost.CopyTo(listRayGhost);
}

/// Init beats time

void Centipede::SetListBeatsTime(DJLinkedList<BeatsTime> listBeatsTime)
{
	m_listBeatsTime.CopyTo(listBeatsTime);
}

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////