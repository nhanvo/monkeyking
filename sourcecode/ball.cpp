/////////////////////////////////////////////////////////////////
// Engine Includes

/////////////////////////////////////////////////////////////////
// Game Includes
#include "ball.h"
#include "gamedef.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
Ball::Ball()
{
	m_vPosition = DJVector2(0.0f);
	m_pSprite = NULL;
}

///

Ball::~Ball()
{
	Term();
}

///

djbool Ball::Init(djint32 id, DJVector2 vPos,DJVector2 vSize, DJString strFileName)
{
	m_ID = id;
	m_vPosition = vPos;
	
	// Create sheet
	DJ2DGraphicsSheet* pSheet = DJ_NEW(DJ2DGraphicsSheet);
	if(!pSheet->Init(strFileName))
	{
		DJError("Fail to load file sheet for ball")
		return DJFALSE;
	}
	DJVector2 vHotpot = DJVector2(vSize.e[0]/2, vSize.e[1]/2);
	pSheet->AutoGenFrames(0, 0, vSize.e[0], vSize.e[1], 1, 1, &vHotpot);
	theSpriteEngine.AddGraphicsSheet(pSheet);

	// Create a sprite node
	m_pSprite = DJ_NEW(DJ2DSprite);
	m_pSprite->SetFilter(DJ2DSprite::filterLinear);
	m_pSprite->SetPosition(vPos);
	m_pSprite->SetCurrentFrame(pSheet->GetFrame(0));

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

void Ball::Update(djfloat fDeltaTime)
{
	
}

///

void Ball::Term()
{
	if(m_pSprite)
	{
		theSpriteEngine.RemoveActiveNode(m_pSprite);
		DJ_SAFE_DELETE(m_pSprite);
	}
}

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
