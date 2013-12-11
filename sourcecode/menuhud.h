/////////////////////////////////////////////////////////////////
//
// hudmenu.h
// Hud menu class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

#ifndef __HUD_MENU_H__
#define __HUD_MENU_H__

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djuinode.h>

/////////////////////////////////////////////////////////////////
// Game Includes

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJHUDUINode : public DJPageUINode
{
	IMPLEMENT_INTERFACE(HUDUINode, PageUINode)
public:
	/// Constructor
	DJHUDUINode();
	/// Destructor
	virtual ~DJHUDUINode();

	/// Paint (virtual function implementation)
	virtual void OnPaint(const DJ2DRenderContext &rc);
	/// Called when the node has been shown or hid (virtual function)
	virtual void OnShowNode(djbool bShow, djbool bParentChanged);
	/// Called every frame if registered
	virtual djbool OnTick();

	/////////////////////////////////////////////////////////////////
	// Instancing / Copying

	/// Create Instance (virtual)
	virtual DJUINode* CreateInstance()				{ return DJ_NEW(DJHUDUINode); }
	/// Instanciate Node (virtual)
	virtual void InstanciateNode(DJUINode *pNode);
	/// Copy Node (virtual)
	virtual void CopyNode(DJUINode *pNode);

	/// Build node virtual call
	virtual djint32 Build();

	/// Virtual overload to create node from tagfile (remember to call superclass versions to load everything!)
	virtual djbool LoadFromTagFile(DJTagFile &tagfile, DJTagDir *pDir, djbool bFirstInstance);

	/// Touch begin callback
	virtual djint32 OnTouchBegin( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch move callback
	virtual djint32 OnTouchMove( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchEnd( djint32 nDevice, djint32 nID, float x, float y );
	/// Touch end callback
	virtual djint32 OnTouchCancel( djint32 nDevice, djint32 nID, float x, float y );
	/// On UI event callback
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // __HUD_MENU_H__
