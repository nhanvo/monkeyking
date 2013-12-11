/////////////////////////////////////////////////////////////////
//
// hudmenu.cpp
// hud menu class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djlocalizer.h>
#include <djfont.h>
#include <djgamesounds.h>
#include <dj2dutil.h>
#include <djanalytics.h>
#include <djservice.h>
#include <djstore.h>
#include <djuinode.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "menuhud.h"
#include "monkeyking.h"


/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

extern djbool g_bGamePause;

/////////////////////////////////////////////////////////////////
DJHUDUINode::DJHUDUINode()
{
	//SetClickable(DJTRUE);
}

///

DJHUDUINode::~DJHUDUINode()
{
}

///

void DJHUDUINode::InstanciateNode(DJUINode *pNode)
{
	DJPageUINode::InstanciateNode(pNode);
}

///

void DJHUDUINode::CopyNode(DJUINode *pNode)
{
	DJPageUINode::CopyNode(pNode);
}

///

djint32 DJHUDUINode::Build()
{
	djint32 nRet = DJPageUINode::Build();     
	return nRet;
}

///

void DJHUDUINode::OnPaint(const DJ2DRenderContext &rc)
{
	if (!IsNodeVisible())
		return;
	DJVector2 vPos = GetPosition();
	if (IsNodeEnabled()) // Transition in
	{
		vPos += djStepToDesiredVector2(vPos, DJVector2(0.0f), 1000.0f)*0.1f;	 
	}
	else // Transition out
	{
		vPos += djStepToDesiredVector2(vPos, DJVector2(0.0f, -200.0f), 1000.0f)*0.1f;
	} 
	SetPosition(vPos);
	//Paint
	DJPageUINode::OnPaint(rc);
}

///

void DJHUDUINode::OnShowNode(djbool bShow, djbool bParentChanged)
{
	DJPageUINode::OnShowNode(bShow, bParentChanged);
	SetPosition(DJVector2(0.0f, -200.0f));
}

///

djbool DJHUDUINode::OnTick()
{
	djbool bRet = DJPageUINode::OnTick();
	return bRet;
}

///

djbool DJHUDUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJPageUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJPageUINode::OnTouchBegin(nDevice, nID, x, y);
	return bRet;
}
///

djint32 DJHUDUINode::OnTouchMove( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJPageUINode::OnTouchMove(nDevice, nID, x, y);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJPageUINode::OnTouchEnd(nDevice, nID, x, y);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJPageUINode::OnTouchCancel(nDevice, nID, x, y);
	return nRet;
}

///

djbool DJHUDUINode::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	if (ev.m_uEventID == pTheUI->EVENTID_ON_CLICKED)
	{
		if (ev.m_uStateID == pTheUI->GetStateID("CLICK_PAUSE"))
		{
			g_bGamePause = !g_bGamePause;
			if(g_bGamePause)
			{
				pTheSoundDevice->PauseMusic();
			}
			else
			{
				pTheSoundDevice->ResumeMusic();
			}
		}	
		else if (ev.m_uStateID == pTheUI->GetStateID("CLICK_EXIT"))
		{
			pTheSoundDevice->StopMusic();
			((DJMonkeyKingApplication*)pTheApp)->GotoGameState(GS_UNLOAD_LEVEL);
		}
	}
	return DJFALSE;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
