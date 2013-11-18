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

#include <djapp.h>
#include <dj2d.h>

#ifndef _MONKEYKINGAPP_H_
#define _MONKEYKINGAPP_H_

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJMonkeyKingApplication : public DJApplication
{
	IMPLEMENT_INTERFACE(MonkeyKingApplication, Application)

protected:
	djbool m_bInitialized;
	djbool m_bGameLoaded;

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
	// EVENT HANDLERS

	// Touch begin callback
	virtual djint32 OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y );
	// Touch move callback
	virtual djint32 OnTouchMove(djint32 nDevice, djint32 nID, float x, float y );
	// Touch end callback
	virtual djint32 OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y );
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
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

#endif // _MONKEYKING_H_
