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
#include "menulevelselect.h"
#include "monkeyking.h"
#include "level.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

extern LevelManager*		g_pLevelManager;
extern djuint32				g_nScreenWidth;
extern djuint32				g_nScreenHeight;
extern djuint32             g_nHUDWidth;
extern djuint32             g_nHUDHeight;

/////////////////////////////////////////////////////////////////
DJLevelUINode::DJLevelUINode()
{
	m_uLevelID = 0;
}

///

DJLevelUINode::~DJLevelUINode()
{
}

///

void DJLevelUINode::Update()
{
}

///

void DJLevelUINode::OnPaint(const DJ2DRenderContext &rc)
{
	DJUINode::OnPaint(rc);	
}

///

void DJLevelUINode::InstanciateNode(DJUINode *pNode)
{
	DJLevelUINode *pObject = (DJLevelUINode*)pNode;
	pObject->m_uLevelID = m_uLevelID;
	DJUINode::InstanciateNode(pNode);
}

///

void DJLevelUINode::CopyNode(DJUINode *pNode)
{
	DJLevelUINode *pObject = (DJLevelUINode*)pNode;
	pObject->m_uLevelID = m_uLevelID;
	DJUINode::CopyNode(pNode);
}

///

djbool DJLevelUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);

	if (bRet)
	{
		DJTagLine* pLine = pDir->GetTag("LEVEL");
		if (!pLine || !pLine->GetArgDword(0, m_uLevelID))
		{
			DJWarning("Failed to parse LEVEL tag from level list item node file!");
			bRet = DJFALSE;
		}
	}

	return bRet;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJLevelSelectMenuPageUINode::DJLevelSelectMenuPageUINode()
{
	for (int q=0; q<LEVEL_COUNT; q++)
		m_pLevelButtons[q] = NULL;
	SetClickable();
}

///

DJLevelSelectMenuPageUINode::~DJLevelSelectMenuPageUINode()
{
}

///

void DJLevelSelectMenuPageUINode::InstanciateNode(DJUINode *pNode)
{
	DJLevelSelectMenuPageUINode *pObject = (DJLevelSelectMenuPageUINode*)pNode;
	DJPageUINode::InstanciateNode(pNode);
}

///

void DJLevelSelectMenuPageUINode::CopyNode(DJUINode *pNode)
{
	DJLevelSelectMenuPageUINode *pObject = (DJLevelSelectMenuPageUINode*)pNode;
	DJPageUINode::CopyNode(pNode);
}

///

/// Build node virtual call
djint32 DJLevelSelectMenuPageUINode::Build()
{
	djint32 nRet = DJPageUINode::Build(); 
	return nRet;
}

///

void DJLevelSelectMenuPageUINode::OnPaint(const DJ2DRenderContext &rc)
{
	if (IsNodeHidden())
		return;

	DJPageUINode::OnPaint(rc);
}

///

void DJLevelSelectMenuPageUINode::Update()
{
}

///

void DJLevelSelectMenuPageUINode::OnShowNode(djbool bShow, djbool bParentChanged)
{
	//SetPosition(DJVector2(0,768));
	DJPageUINode::OnShowNode(bShow, bParentChanged);
	if (bShow)
	{
		
	}
}

///

djbool DJLevelSelectMenuPageUINode::LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance)
{
	djbool bRet = DJPageUINode::LoadFromTagFile(tagfile, pDir, bFirstInstance);

	return bRet;
}

///

djbool DJLevelSelectMenuPageUINode::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	if (ev.m_uEventID == pTheUI->EVENTID_ON_CLICKED)
	{
	}
	return DJFALSE;
}

///

void DJLevelSelectMenuPageUINode::UnlockAllLevels()
{ 
}

///

djint32 DJLevelSelectMenuPageUINode::OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y )
{
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnTouchMove( djint32 nDevice, djint32 nID, float x, float y )
{
#if defined(DEMO_VERSION) && (!defined(_FINALVER) || defined(_DEBUGMENU))
	if (x<32 && y>pTheUI->GetUIHeight()-32)
	{
		return 1;
	}
#endif
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y )
{
#if defined(DEMO_VERSION) && (!defined(_FINALVER) || defined(_DEBUGMENU))
	if (x<32 && y>pTheUI->GetUIHeight()-32)
	{
		UnlockAllLevels();
		return 1;
	}
#endif
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnKeyDown( djint32 nKey )
{
	if (nKey == DJKEY_ESC)
	{
		return 1;
	}
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnKeyUp( djint32 nKey )
{
	if (nKey == DJKEY_ESC)
	{
		return 1;
	}
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnButtonDown( djint32 nKey )
{
	return 0;
}

///

djint32 DJLevelSelectMenuPageUINode::OnButtonUp( djint32 nKey )
{
	if (nKey == DJ_GAMEPADKEY_BACK || nKey == DJ_GAMEPADKEY_B)
	{
		return 1;
	}
	if (nKey == DJ_GAMEPADKEY_START || nKey == DJ_GAMEPADKEY_A)
	{
		return 1;
	}
#ifndef _FINALVER
	if (nKey == DJ_GAMEPADKEY_D)
	{
		UnlockAllLevels();
		return 1;
	}
#endif
	return 0;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

