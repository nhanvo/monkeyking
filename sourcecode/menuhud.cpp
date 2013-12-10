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
}

///

DJHUDUINode::~DJHUDUINode()
{
}

///

void DJHUDUINode::InstanciateNode(DJUINode *pNode)
{
	DJUINode::InstanciateNode(pNode);
}

///

void DJHUDUINode::CopyNode(DJUINode *pNode)
{
	DJUINode::CopyNode(pNode);
}

///

djint32 DJHUDUINode::Build()
{
	djint32 nRet = DJUINode::Build();     
	return nRet;
}

///

void DJHUDUINode::OnPaint(const DJ2DRenderContext &rc)
{
	if (!IsNodeVisible())
		return;
	if (IsNodeEnabled()) // Transition in
	{
		
	}
	else // Transition out
	{
		
	}  	
	//Paint
	DJUINode::OnPaint(rc);
}

///

void DJHUDUINode::OnShowNode(djbool bShow, djbool bParentChanged)
{
	DJUINode::OnShowNode(bShow, bParentChanged);
	SetPosition(DJVector2(0.0f));
}

///

djbool DJHUDUINode::OnTick()
{
	djbool bRet = DJUINode::OnTick();
	return bRet;
}

///

djbool DJHUDUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJUINode::OnTouchBegin(nDevice, nID, x, y);
	return bRet;
}
///

djint32 DJHUDUINode::OnTouchMove( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJUINode::OnTouchMove(nDevice, nID, x, y);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y )
{
	djbool bRet = DJUINode::OnTouchEnd(nDevice, nID, x, y);
	return bRet;
}

///

djint32 DJHUDUINode::OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y )
{
	djint32 nRet = DJUINode::OnTouchCancel(nDevice, nID, x, y);
	return nRet;
}

///

djbool DJHUDUINode::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	if (ev.m_uEventID == pTheUI->EVENTID_ON_CLICKED)
	{
		if (ev.m_uStateID == pTheUI->GetStateID("CLICK_PAUSE"))
		{
			g_bGamePause = DJTRUE;	
		}
	}
	return DJFALSE;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
