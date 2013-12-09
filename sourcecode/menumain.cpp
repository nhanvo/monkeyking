/////////////////////////////////////////////////////////////////
//
// levelselectmenu.cpp
// Level select menu class
//
/////////////////////////////////////////////////////////////////
//
// Copyright (C) 2009 Daniel Jeppsson
// All Rights Reserved. These instructions, statements, computer
// programs, and/or related material (collectively, the "Source")
// contain unpublished information propietary to Daniel Jeppsson
// which is protected by US federal copyright law and by 
// international treaties. This Source may NOT be disclosed to 
// third parties, or be copied or duplicated, in whole or in 
// part, without the written consent of Daniel Jeppsson.
//
/////////////////////////////////////////////////////////////////
// 
// Author: NhanVo
// 
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djlocalizer.h>
#include <djfont.h>
#include <dj2dutil.h>

//#undef _FINALVER

/////////////////////////////////////////////////////////////////
// Game Includes
#include "menumain.h"
#include "monkeyking.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJMainMenuPageUINode::DJMainMenuPageUINode()
{
}

///

DJMainMenuPageUINode::~DJMainMenuPageUINode()
{
}

///

void DJMainMenuPageUINode::InstanciateNode(DJUINode *pNode)
{
	//DJMainMenuPageUINode *pObject = (DJMainMenuPageUINode*)pNode;
	DJPageUINode::InstanciateNode(pNode);
}

///

void DJMainMenuPageUINode::CopyNode(DJUINode *pNode)
{
	//DJMainMenuPageUINode *pObject = (DJMainMenuPageUINode*)pNode;
	DJPageUINode::CopyNode(pNode);
}

///

djint32 DJMainMenuPageUINode::Build()
{
	DJPageUINode::Build();
	return 0;
}


void DJMainMenuPageUINode::OnPaint(const DJ2DRenderContext &rc)
{
	DJVector2 vPos = GetPosition();
	vPos += djStepToDesiredVector2(vPos, DJVector2(0,0), 1000.0f) * 0.1f;
	SetPosition(vPos);
	DJPageUINode::OnPaint(rc);
}

///

void DJMainMenuPageUINode::OnShowNode(djbool bShow, djbool bParentChanged)
{
	DJPageUINode::OnShowNode(bShow, bParentChanged);
	if (bShow)
	{
		SetPosition(DJVector2(854,0));
	}
}

///

djbool DJMainMenuPageUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJPageUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);
	return bRet;
}

///

djbool DJMainMenuPageUINode::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	if (ev.m_uEventID == pTheUI->EVENTID_ON_CLICKED)
	{
		/*if (ev.m_uStateID == pTheUI->GetStateID("START"))
		{
			((DJTutorial4Application*)pTheApp)->GotoGameState(GS_LOAD_LEVEL);
			return DJTRUE;
		}
		if (ev.m_uStateID == pTheUI->GetStateID("CREDITS"))
		{
			((DJTutorial4Application*)pTheApp)->GotoGameState(GS_CREDITS);
			return DJTRUE;
		}
		if (ev.m_uStateID == pTheUI->GetStateID("QUIT"))
		{
			((DJTutorial4Application*)pTheApp)->m_bQuit = DJTRUE;
			return DJTRUE;
		}*/
	}
	return DJFALSE;
}

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////