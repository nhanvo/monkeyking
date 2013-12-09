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

#ifndef _MENUMAIN_H_
#define _MENUMAIN_H_

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
class DJMainMenuPageUINode : public DJPageUINode
{
	IMPLEMENT_INTERFACE(PageUINode, UINode)

public:
	/// Constructor
	DJMainMenuPageUINode();
	/// Destructor
	virtual ~DJMainMenuPageUINode();

	/// Build node virtual call
	virtual djint32 Build();

	/// Paint (virtual function implementation)
	virtual void OnPaint(const DJ2DRenderContext &rc);
	/// Show node (virtual function implementation)	
	virtual void OnShowNode(djbool bShow, djbool bParentChanged);

	/////////////////////////////////////////////////////////////////
	// Instancing / Copying

	/// Create Instance (virtual)
	virtual DJUINode* CreateInstance()				{ return DJ_NEW(DJMainMenuPageUINode); }
	/// Instanciate Node (virtual)
	virtual void InstanciateNode(DJUINode *pNode);
	/// Copy Node (virtual)
	virtual void CopyNode(DJUINode *pNode);

	/// Virtual overload to create node from tagfile (remember to call superclass versions to load everything!)
	virtual djbool LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance);


	/// On UI event callback
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);
};

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _MENUMAIN_H_

