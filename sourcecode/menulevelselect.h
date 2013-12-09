			  /////////////////////////////////////////////////////////////////
//
// menulevelselect.h
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

#ifndef _LEVELSELECTMENU_H_
#define _LEVELSELECTMENU_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djuinode.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "gamedef.h"
//#include "mainmenu.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJLevelUINode : public DJButtonUINode
{
	IMPLEMENT_INTERFACE(LevelUINode, ButtonUINode)

protected:
	// Level ID
	djuint32	m_uLevelID;

public:
	/// Constructor
	DJLevelUINode();
	/// Destructor
	virtual ~DJLevelUINode();

	/// Paint (virtual function implementation)
	virtual void OnPaint(const DJ2DRenderContext &rc);

	/// Get Level ID
	djuint32 GetLevelID() const		{ return m_uLevelID; }
	/// Set Level ID
	void SetLevelID(djuint32 uID)	{ m_uLevelID = uID; }

	/// Update everything required (called whenever level ID changes or lock status)
	void Update();

	/////////////////////////////////////////////////////////////////
	// Instancing / Copying

	/// Create Instance (virtual)
	virtual DJUINode* CreateInstance()				{ return DJ_NEW(DJLevelUINode); }

	/// Instanciate Node (virtual)
	virtual void InstanciateNode(DJUINode *pNode);
	/// Copy Node (virtual)
	virtual void CopyNode(DJUINode *pNode);

	/// Virtual overload to create node from tagfile (remember to call superclass versions to load everything!)
	virtual djbool LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance);

};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJLevelSelectMenuPageUINode : public DJPageUINode
{
	IMPLEMENT_INTERFACE(LevelSelectMenuPageUINode, PageUINode)

public:
	/*enum
	{
		STATE_INITIAL,
		STATE_TRANSITION,
		STATE_DIFFICULTY,
		STATE_TRANSITION_BACK,
	};*/

protected:
	DJLevelUINode*	m_pLevelButtons[LEVEL_COUNT];

public:
	/// Constructor
	DJLevelSelectMenuPageUINode();
	/// Destructor
	virtual ~DJLevelSelectMenuPageUINode();

	/// Paint (virtual function implementation)
	virtual void OnPaint(const DJ2DRenderContext &rc);
	/// Called when the node has been shown or hid (virtual function)
	virtual void OnShowNode(djbool bShow, djbool bParentChanged);

	/////////////////////////////////////////////////////////////////
	// Instancing / Copying

	/// Create Instance (virtual)
	virtual DJUINode* CreateInstance()				{ return DJ_NEW(DJLevelSelectMenuPageUINode); }
	/// Instanciate Node (virtual)
	virtual void InstanciateNode(DJUINode *pNode);
	/// Copy Node (virtual)
	virtual void CopyNode(DJUINode *pNode);

	// Update content
	void Update();

	/// Build node virtual call
	virtual djint32 Build();

	/// Virtual overload to create node from tagfile (remember to call superclass versions to load everything!)
	virtual djbool LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance);

	/// On UI event callback
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);

	/// Cheat to unlock all levels
	void UnlockAllLevels();

	/// Touch end callback
	virtual djint32 OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchMove( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y );
	/// Button down callback
	virtual djint32 OnButtonDown( djint32 nKey );
	/// Button up callback
	virtual djint32 OnButtonUp( djint32 nKey );
	/// Key down callback
	virtual djint32 OnKeyDown( djint32 nKey );
	/// Key up callback
	virtual djint32 OnKeyUp( djint32 nKey );
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _LEVELSELECTMENU_H_

