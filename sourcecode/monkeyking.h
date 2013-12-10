/////////////////////////////////////////////////////////////////
//
// monkeyking.h
// Application class
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
// Author: Daniel Jeppsson
// 
/////////////////////////////////////////////////////////////////
#ifndef _MONKEYKINGAPP_H_
#define _MONKEYKINGAPP_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djengine.h>
#include <djapp.h>
#include <dj2d.h>
#include <djrenderdevice.h>
#include <djrender.h>
#include <djtagfile.h>
#include <djmesh.h>
#include <djuinode.h>
#include <djosdatastorage.h>
#include <djanimation.h>
#include <djnodeanimator.h>
#include <djskelanimator.h>
#include <djservice.h>

/////////////////////////////////////////////////////////////////
// Game Includes 

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Layer enum
enum
{
	LAYER_BACKGROUND = 0,
	LAYER_BACKGROUND_1,
	LAYER_BACKGROUND_2,
	LAYER_UNDERLAY,
	LAYER_SHOTS,
	LAYER_BACKGROUND_OVERLAY,
	LAYER_SPRITES,
	LAYER_EXPLOSIONS,
	LAYER_OVERLAY,
	LAYER_OVERLAY2,
	LAYER_MENU,
	LAYER_HUD,
	LAYER_DEBUG,
#ifdef _DEV
	LAYER_BOUNDING_BOX,
#endif //_DEV

	LAYER_COUNT,
};

/////////////////////////////////////////////////////////////////
// Game state
enum
{
	GS_PRELOAD,
	GS_PRELOGO,
	GS_LOGO,
	GS_LOAD_GAME,
	GS_MENU,
	GS_INGAME_MENU,
	GS_LOAD_LEVEL,
	GS_UNLOAD_LEVEL,
	GS_GAMEOVER,
	GS_INGAME,
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Menu
enum
{
	MENU_MAIN,
	MENU_INGAME,
	MENU_LEVEL_SELECT,
	MENU_GAMEOVER,
	MENU_HUD,
	MENU_DEBUG,
	MENU_COUNT,
	MENU_INVALID
};
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
class DJMonkeyKingApplication : public DJApplication,  public DJUIEventListener
{
	IMPLEMENT_INTERFACE(MonkeyKingApplication, Application)

protected:
	djbool m_bInitialized;
	
	// Current game state
	djint32				m_nGameState;
	
	// Last game state
	djint32				m_nLastGameState;
	
	// Game state changed
	djbool				m_bGameStateChanged;	
	
	// Timer in current game state
	float				m_fStateTimer;
	// Is game loaded
	djbool				m_bGameIsLoaded;
	
	DJArray<djuint32>	m_menuStack;

	// Menu nodes
	DJUINode*			m_pMenus[MENU_COUNT];

	DJUINode*			m_pMenuTest;

	// User wanted to go to pause menu	
	djbool				m_bGotoPauseMenu;
public:

	// Paint prelogo state
	void PaintPreLogo( );

	// Paint logo state
	void PaintLogo( );

	// Paint load game
	void PaintLoadGame( );

	// Paint load level
	void PaintLoadLevel( );

	// Paint menu state
	void PaintMenu( );

	// Paint ingame state
	void PaintIngame( );

	// Update prelogo
	void UpdatePreLogo();

	// Update logo
	void UpdateLogo();

	// Update load game
	void UpdateLoadGame();

	// Update load level 
	void UpdateLoadLevel();

	// Update unload level state
	void UpdateUnloadLevel();

	// Update menu state
	void UpdateMenu();

	// Update ingame state
	void UpdateIngame();

   	djbool OnLoadLevel();
	djbool OnUnLoadLevel();
public:

	/////////////////////////////////////////////////////////////////
	// CONSTRUCTION/DESTRUCTION

	// Constructor
	DJMonkeyKingApplication( );
	// Destructor
	virtual ~DJMonkeyKingApplication( );

	// Application initialization
	virtual djresult OnInit( );
	// Application termination
	virtual void OnTerm( );

	// Load application data
	djbool OnLoad();

	/////////////////////////////////////////////////////////////////
	// OPERATIONS

	// Called once per frame from main loop
	virtual void OnUpdate( );

	// Called once per frame to render the application
	virtual djbool OnPaint( );

	/////////////////////////////////////////////////////////////////
	// Function
	// Current game state
	djint32 GetGameState() const {return m_nGameState;}

	// Is menu on menu stack
	djbool IsMenuOnStack(djuint32 uMenu);

	// Goto menu
	void GotoMenu(djuint32 uMenu, djbool bReplaceStack = DJFALSE, djbool bReplaceLast = DJFALSE);
	
	// Get current menu ID
	djuint32 GetCurrentMenu()						{ if (m_menuStack.GetCurrentIndex()==0) return MENU_INVALID; return m_menuStack[m_menuStack.GetCurrentIndex()-1]; }
	// Get last menu ID
	djuint32 GetLastMenu()							{ if (m_menuStack.GetCurrentIndex()<=1) return MENU_INVALID; return m_menuStack[m_menuStack.GetCurrentIndex()-2]; }
	// Pop menu from stack and goto that menu
	djuint32 PopMenu();
	// Goto menu
	void GoBackToMenu(djuint32 uMenu);

	void SetGotoPauseMenu(djbool bSet = DJTRUE)		{ m_bGotoPauseMenu = bSet; }

	// Change game state
	djbool GotoGameState(djint nState);
	
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// EVENT HANDLERS

	// Touch begin callback
	virtual djint32 OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y );
	// Touch move callback
	virtual djint32 OnTouchMove(djint32 nDevice, djint32 nID, float x, float y );
	// Touch end callback
	virtual djint32 OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y );
	// Touch cancel
	virtual djint32	OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY );
	// Accelerator event callback
	virtual djint32 OnAccelerate( float x, float y, float z );
	// Button Down
	virtual djint32 OnButtonDown(djint32 nKey);
	// Button Up
	virtual djint32 OnButtonUp(djint32 nKey);

	// Key down
	virtual djint32 OnKeyDown( djint32 nKey );

	// Handle incoming system event
	virtual void OnMessage( djuint32 nMessage, djuint32 nParam1 = 0, djuint32 nParam2 = 0 );

	// UI event callback
	virtual djbool OnUIEvent(DJUINode *pNode, const DJUIEvent &ev);
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _MONKEYKING_H_
