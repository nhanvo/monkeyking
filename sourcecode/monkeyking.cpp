/////////////////////////////////////////////////////////////////
//
// monkeyking.cpp
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

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <djmodulemanager.h>
#include <djostime.h>
#include <dj2d.h>
#include <dj2dutil.h>
#include <djgamesounds.h>
#include <djfont.h>
#include <djdevicemanager.h>
#include <djcamera.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "monkeyking.h"
#include "level.h"
#include "musichandler.h"
#include "camera.h"
#include "player.h"
#include "debug.h"
#include "musichandler.h"

/////////////////////////////////////////////////////////////////
// Name of the game module
const char *GetGameModuleName()
{
	return "MonkeyKing";
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Name of the game application
const char *GetGameObjectName()
{
	return "MonkeyKingApplication";
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
IMPORT_STATIC_MODULE(MonkeyKing)
IMPORT_STATIC_MODULE(OpenGLRender)
#if defined (_DJAIRPLAY)
IMPORT_STATIC_MODULE(AirSound)
#elif defined (_DJANDROID)
IMPORT_STATIC_MODULE(OpenALSound)
#else
IMPORT_STATIC_MODULE(OpenALSound)
#endif
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void DJSystem_AddStaticModules()
{
	ADD_STATIC_MODULE(OpenGLRender);
#if defined (_DJAIRPLAY)
	ADD_STATIC_MODULE(AirSound);
#elif defined (_DJANDROID)
	ADD_STATIC_MODULE(OpenALSound);
#else
	ADD_STATIC_MODULE(OpenALSound);
#endif
	ADD_STATIC_MODULE(MonkeyKing);
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_ENUMERATE_INTERFACE()
ENUMERATE_INTERFACE(MonkeyKingApplication)
END_ENUMERATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_CREATE_INTERFACE()
CREATE_INTERFACE(MonkeyKingApplication)
END_CREATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_REGISTER_SYMBOL(MonkeyKing)
REGISTER_INTERFACE(MonkeyKingApplication)
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
REGISTER_ENUMERATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
END_REGISTER_SYMBOL()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Globals
// Screen width (set in OnInit function)
djuint32				g_nScreenWidth			= 0;
// Screen height (set in OnInit function)
djuint32				g_nScreenHeight			= 0;
// Sheet of graphics for the flare sprite
DJ2DGraphicsSheet*		g_pFlareSheet			= NULL;
// A font to use for drawing text
DJFont*					g_pFont					= NULL;
// Mode color
DJColor					g_cModColor(1,1,1,1);

djfloat					g_fRestrictionBottom	= 0.0f;

// Scene is start
djint32					g_SceneStart			= 1;	
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern const char*		g_szApplicationName;
extern const djwchar*	g_szApplicationNameW;
extern const char*		g_szConfigName;
extern djuint32			g_uIconID;
extern djuint32			g_uCursorID;
extern const char*		g_szDTXPlatform;
extern djbool			g_bUseMSAA;
extern MusicLayerHandler theMusicLayerHandler;	
extern BoundingBoxCollection theBoundingBoxCollection;

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

extern const char*		g_szApplicationName;
extern const djwchar*	g_szApplicationNameW;
extern const char*		g_szConfigName;
extern djuint32			g_uIconID;
extern djuint32			g_uCursorID;
extern const char*		g_szDTXPlatform;
extern djbool			g_bUseAppiRater;
extern djuint32			g_uGlobalVertexBufferUsageFlags;
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJCamera*					g_pCamera				= NULL;
LevelManager*				g_pLevelManager			= NULL;
Player*						g_pPlayer; 
DJVector2					g_vScaleScreen			= DJVector2(1.0f);
float						g_fGameTimeScale		= 1.0f;

// only background layer(HD - No HD)
djbool						g_bRemoveOneLayerInBG = DJFALSE;
/////////////////////////////////////////////////////////////////

#if defined(_DJANDROID)
	extern djbool g_bRequireOBB;
#endif // _DJANDROID


/////////////////////////////////////////////////////////////////
extern void djUpdateCursor();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// This function will be called very early. Don't rely on ANY engine features to have been initialized. Only use to set global variables that need be set before the engine initializes.
#ifdef _DJWINDOWS
extern "C" __declspec(dllexport) void djPreInit()
#else // _DJWINDOWS
extern "C" void djPreInit()
#endif // _DJWINDOWS
{
	g_uGlobalVertexBufferUsageFlags = DJVertexBuffer::ufTripleBuffered;
	g_szApplicationName			= "Monkey King Rhythm";
	g_szApplicationNameW		= L"Monkey King Rhythm";
	g_szConfigName				= "monkeyking.cfg";
	g_uIconID					= 101;
	g_uCursorID					= 102;
	djUpdateCursor();

#if defined(_DJQNX)
	extern const char* g_szQNXAppID;
	g_szQNXAppID = FIXME!;
#endif // _DJQNX

#if defined(_DJANDROID)
	g_bRequireOBB = DJFALSE;
#endif // _DJANDROID

#if defined(_DJQNX) || defined(_DJANDROID)
	extern const char* g_szScoreloopGameID;
	extern const char* g_szScoreloopGameSecret;
	extern const char* g_szScoreloopGameVersion;
	extern const char* g_szScoreloopGameCurrency;

	g_szScoreloopGameID = "f09d8473-32e2-4c12-afd0-18c4fff46adf";
	g_szScoreloopGameSecret = "dEZrqlrprGq8HfpdFM9+lfXuz0lrsXZjwL+uayY0jvpueacZ8Dddwg==";
	g_szScoreloopGameVersion = "1.0";
	g_szScoreloopGameCurrency = "JLC";
#endif // _DJQNX
    
//#ifdef _APPOTA_STORE
//	g_szAppotaClientKey = "??";
//	g_szAppotaClientSecret = "??";
//
//	//Code of this channel is :"??" send to ??
//#endif

	switch (theDeviceManager.GetDevice())
	{
		case DJDeviceManager::DEVICE_IPAD1:
			g_bRemoveOneLayerInBG = DJTRUE;
		break;
		case DJDeviceManager::DEVICE_IPAD2:
		case DJDeviceManager::DEVICE_IPAD3:
		case DJDeviceManager::DEVICE_IPAD4:
		case DJDeviceManager::DEVICE_IPAD_MINI:
		case DJDeviceManager::DEVICE_IPAD_UNKNOWN:
			//g_szDTXPlatform = "ipad_hd";
		break;
		case DJDeviceManager::DEVICE_PLAYBOOK:
			//g_szDTXPlatform = "playbook";
		break;
		case DJDeviceManager::DEVICE_BLACKBERRY:
			//g_szDTXPlatform = "phone_hd_raw";
		break;
		case DJDeviceManager::DEVICE_IPHONE3GS:
			g_bRemoveOneLayerInBG = DJTRUE;
		break;
		case DJDeviceManager::DEVICE_IPHONE4:
		case DJDeviceManager::DEVICE_IPOD4:
			g_bRemoveOneLayerInBG = DJTRUE;
			break;
		case DJDeviceManager::DEVICE_IPHONE5:
		case DJDeviceManager::DEVICE_IPOD5:
		case DJDeviceManager::DEVICE_IPHONE_UNKNOWN:
		case DJDeviceManager::DEVICE_IPOD_UNKNOWN:
			//g_szDTXPlatform = "iphone_hd";
		break;

		// ....

		default:
			g_szDTXPlatform = NULL;
		break;
	}
#ifdef _DJIPHONE
	extern const char* g_szIOSAppID;
	// Need to fill this in to use application rating API
    g_szIOSAppID = FIXME;
#endif // _DJIPHONE
    
	if (theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPAD2 ||
		theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPAD_MINI ||
		//theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPAD3 ||
		//theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPAD_UNKNOWN ||
		//theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPHONE5 ||
		theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPHONE3GS //||
		//theDeviceManager.GetDevice() == DJDeviceManager::DEVICE_IPHONE_UNKNOWN
		)
	{
		g_bUseMSAA = DJTRUE;
	}
	g_bUseAppiRater = DJFALSE;
}

#ifdef _DJWINDOWS
extern "C" __declspec(dllexport) djbool djOnArchiveAdded(djhandle hFile, const char* szName)
#else // _DJWINDOWS
extern "C" djbool djOnArchiveAdded(djhandle hFile, const char* szName)
#endif // _DJWINDOWS
{
	DJInfo("djOnArchiveAdded (%s)", szName);
	return DJTRUE;
}

/////////////////////////////////////////////////////////////////
// This function will be called very early. Set important variables that needs to be initialized before the application is created.
#ifdef _DJWINDOWS
extern "C" __declspec(dllexport) void djInit()
#else // _DJWINDOWS
extern "C" void djInit()
#endif // _DJWINDOWS
{
	DJInfo("%s()", __FUNCTION__);
	djuint32 sw,sh;
	theDeviceManager.GetDisplaySize(&sw, &sh);

	switch (theDeviceManager.GetDevice())
	{
		case DJDeviceManager::DEVICE_IPAD1:
			g_szDTXPlatform = "ipad";
		break;
		case DJDeviceManager::DEVICE_IPAD2:
		case DJDeviceManager::DEVICE_IPAD3:
		case DJDeviceManager::DEVICE_IPAD4:
		case DJDeviceManager::DEVICE_IPAD_MINI:
		case DJDeviceManager::DEVICE_IPAD_UNKNOWN:
			g_szDTXPlatform = "ipad_hd";
		break;
		case DJDeviceManager::DEVICE_PLAYBOOK:
			g_szDTXPlatform = "playbook";
		break;
		case DJDeviceManager::DEVICE_BLACKBERRY:
			g_szDTXPlatform = "phone_hd_raw";
		break;
		case DJDeviceManager::DEVICE_IPHONE3GS:
			g_szDTXPlatform = "iphone";
		break;
		case DJDeviceManager::DEVICE_IPHONE4:
		case DJDeviceManager::DEVICE_IPOD4:
		case DJDeviceManager::DEVICE_IPHONE5:
		case DJDeviceManager::DEVICE_IPOD5:
		case DJDeviceManager::DEVICE_IPHONE_UNKNOWN:
		case DJDeviceManager::DEVICE_IPOD_UNKNOWN:
			g_szDTXPlatform = "iphone_hd";
		break;

		// ....

		default:
			g_szDTXPlatform = NULL;
		break;
	}

#ifdef _DJANDROID
	djbool bHD = DJFALSE;
	if (sw*sh>=1280*720)
		bHD = DJTRUE;
	/*if (theDeviceManager.IsTextureCompressionAvailable(DJDeviceManager::TEXTURE_COMPRESSION_PVR))
	{
		if (theDeviceManager.GetDeviceClass() == DJDeviceManager::DEVICECLASS_TABLET)
		{
			if (bHD)
				g_szDTXPlatform = "tablet_hd_pvrtc";
			else
				g_szDTXPlatform = "tablet_pvrtc";
		}
		else
		{
			if (bHD)
				g_szDTXPlatform = "phone_hd_pvrtc";
			else
				g_szDTXPlatform = "phone_pvrtc";
		}
	}
	else if (theDeviceManager.IsTextureCompressionAvailable(DJDeviceManager::TEXTURE_COMPRESSION_S3TC))
	{
		if (theDeviceManager.GetDeviceClass() == DJDeviceManager::DEVICECLASS_TABLET)
		{
			if (bHD)
				g_szDTXPlatform = "tablet_hd_dxt";
			else
				g_szDTXPlatform = "tablet_dxt";
		}
		else
		{
			if (bHD)
				g_szDTXPlatform = "phone_hd_dxt";
			else
				g_szDTXPlatform = "phone_dxt";
		}
	}
	else if (theDeviceManager.IsTextureCompressionAvailable(DJDeviceManager::TEXTURE_COMPRESSION_ATC))
	{
		if (theDeviceManager.GetDeviceClass() == DJDeviceManager::DEVICECLASS_TABLET)
		{
			if (bHD)
				g_szDTXPlatform = "tablet_hd_atitc";
			else
				g_szDTXPlatform = "tablet_atitc";
		}
		else
		{
			if (bHD)
				g_szDTXPlatform = "phone_hd_atitc";
			else
				g_szDTXPlatform = "phone_atitc";
		}
	}
	else*/
	{
		if (theDeviceManager.GetDeviceClass() == DJDeviceManager::DEVICECLASS_TABLET)
		{
			if (bHD)
				g_szDTXPlatform = "tablet_hd_raw";
			else
				g_szDTXPlatform = "tablet_raw";
		}
		else
		{
			if (bHD)
				g_szDTXPlatform = "phone_hd_raw";
			else
				g_szDTXPlatform = "phone_raw";
		}
	}
	DJInfo("DTX PLATFORM: %s", g_szDTXPlatform);
#endif // _DJANDROID
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void LoadStart()
{
	g_uCursorID = 103;
	djUpdateCursor();
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void LoadFinish()
{
	g_uCursorID = 102;
	djUpdateCursor();
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Application class
DJMonkeyKingApplication::DJMonkeyKingApplication()
{
	DJTrace("%s()", __FUNCTION__);
	m_bGameLoaded = DJFALSE;
	m_bInitialized = DJFALSE;
}

///

DJMonkeyKingApplication::~DJMonkeyKingApplication()
{
	DJTrace("%s()", __FUNCTION__);
}

///
//#include "djosthread.h"

djresult DJMonkeyKingApplication::OnInit()
{
	DJTrace("%s()", __FUNCTION__);

	if (m_bInitialized)
	{
		DJWarning("Trying to initialize the game multiple times!");
	}

	// Animate the cursor for loading
	LoadStart();

	//djThreadSleep(10000);

	// Set screen orientation depending on device
//#if defined(_DJIPHONE)
//	theSpriteEngine.SetScreenOrientation(1);
//#else
	//theSpriteEngine.SetScreenOrientation(0);
//#endif
	
	// Get size of screen and put in global variables
	pTheRenderDevice->GetScreenSize(g_nScreenWidth, g_nScreenHeight);

	// Randomize random seed by using system time
	djRandomSetSeed( (djuint32)(djGetSystemTimeFloat() * 65536.0f) );
	
	// Initialize sprite engine with the layers we want
	if (!theSpriteEngine.Init(LAYER_COUNT))
	{
		DJError("Failed to initialize the sprite engine!");
		return DJ_FAILURE;
	}

	// Load font
	g_pFont = (DJFont*)theResourceManager.GetResource( "font", DJResource::TYPE_FONT );
	if (g_pFont == NULL)
	{
		DJError("Failed to load the font!");
		return DJ_FAILURE;
	}

	// Create a flare sprite sheet
	g_pFlareSheet = DJ_NEW(DJ2DGraphicsSheet);
	if (!g_pFlareSheet->Init("sprites/bg.png"))
	{
		DJError("Failed to load flare sheet!");
		return DJ_FAILURE;
	}
	DJVector2 vHotspot = DJVector2((float)g_pFlareSheet->GetTexture()->GetWidth()/2, (float)g_pFlareSheet->GetTexture()->GetHeight()/2);
	g_pFlareSheet->AutoGenFrames(0, 0, -1, -1, 1, 1, &vHotspot);
	theSpriteEngine.AddGraphicsSheet(g_pFlareSheet);

	DJ2DLayer *pLayer = theSpriteEngine.GetLayer(LAYER_BACKGROUND);
    DJ2DGraphicsFrame *pFrame = g_pFlareSheet->GetFrame(0);
	pLayer->SetBackground(pFrame);
	//pLayer->SetScale((float)g_nScreenWidth/1024.0f, (float)g_nScreenHeight/512.0f);
	pLayer->SetPosition(vHotspot);

	if (!djGameSoundsInit(l_szSoundFiles, SOUND_COUNT))
	{
		DJError("Failed to load sound effects!");
		return DJ_FAILURE;
	}

	// Initialize music layer handler
	pTheSoundDevice->EnableThreadedUpdates(DJTRUE);
	theMusicHandler.Init(); 

	if (!theMusicLayerHandler.Init(1))
	{
		DJError("Failed to initialize music layer handler!");
		return DJ_FAILURE;
	}

	if (!OnLoad())
	{
		DJWarning("Failed to load game!");
		return DJ_FAILURE;
	}

	// Animate the cursor for loading
	LoadFinish();

	m_bInitialized = DJTRUE;

	return DJ_SUCCESS;
}

///

djbool DJMonkeyKingApplication::OnLoad()
{
	DJTrace("%s()", __FUNCTION__);

	if (m_bGameLoaded)
	{
		DJWarning("Trying to load the game multiple times!");
		return DJFALSE;
	}

	// Create camera
	//g_pCamera = DJ_NEW (DJCamera);
	//if(g_pCamera == NULL)
	//{
	//	DJError("Failed to create camera (out of memory)!");
	//	return DJFALSE;
	//}

	// Create new level manager
	g_pLevelManager = DJ_NEW ( LevelManager );
	if (!g_pLevelManager->Init())
	{
		DJError("Failed to initialize level manager!");
		return DJFALSE;
	}
	else
	{
		// Scene ID get from menu select scene
		g_pLevelManager->SetSceneID(g_SceneStart);
	}

	// init level
	if (g_pLevelManager->GetCurrentLevel() == NULL)
	{
		if (!g_pLevelManager->LoadLevel("levels/level01.level"))
		{
			DJError("Failed to load level!!!");
			return DJFALSE;
		}
	}

	m_bGameLoaded = DJTRUE;
#ifdef _DEV

		// Use to check collision
		theBoundingBoxCollection.Init();		
#endif //_DEV

	return DJTRUE;
}

///

void DJMonkeyKingApplication::OnTerm()
{
	DJTrace("%s()", __FUNCTION__);

	if (!m_bInitialized)
	{
		DJWarning("Trying to terminate the game multiple times!");
		return;
	}

	// Music layer handler
	theMusicLayerHandler.Term();

	// Release font
	DJ_SAFE_RELEASE(g_pFont);

	// No need to delete this since the sprite engine takes care of it (we added it above)
	g_pFlareSheet = NULL;

	// Terminate sprite engine
	theSpriteEngine.Term();

	// Terminate DJEngine gamesound helper function
	theMusicHandler.Term();
	djGameSoundsTerm();

	m_bGameLoaded = DJFALSE;
	m_bInitialized = DJFALSE;

	// delete camera
	DJ_SAFE_DELETE(g_pCamera);

	// safe delete levelmanager
	DJ_SAFE_DELETE(g_pLevelManager);

	// debug handler
#ifdef _DEV
	theBoundingBoxCollection.Term();
#endif //_DEV
}

///

void DJMonkeyKingApplication::OnUpdate()
{
	DJTrace("%s",__FUNCTION__); 

	SetAppTimeScale(g_fGameTimeScale);
	// Deactivate sound if we are minimized or hidden
	if (theDeviceManager.IsMinimized() || theDeviceManager.IsHidden())
	{
		if (pTheSoundDevice->IsActive())
		{
			pTheSoundDevice->Deactivate();
		}
	}
	else if (!pTheSoundDevice->IsActive())
	{
		pTheSoundDevice->Activate();
	} 
	
	// Update sprite engine
	theSpriteEngine.OnUpdate();

	// Update level 
	if (!g_pLevelManager->Update(m_fDeltaAppTime))
		return;
}

///

djbool DJMonkeyKingApplication::OnPaint()
{
	//DJTrace(__FUNCTION__"()");

	// Set the clear color
	pTheRenderDevice->SetClearColor(DJColor(1,0,0,0));
	// Clear the screen
	pTheRenderDevice->Clear(DJRenderDevice::flagClearAll);
	// Disable the depth buffer (no need for it in 2D games usually)
	pTheRenderDevice->EnableZBuffer(0);
	
	// Set render context
	DJ2DRenderContext rc;
	rc.m_uFlags = 0;
	rc.m_cModColor = DJColor(1,1,1,1);
	rc.m_cAddColor = DJColor(0,0,0,0);
	rc.m_mLayerTransform = DJMatrix2D::Identity();
	rc.m_pLayer = NULL;
	rc.m_mTransform = DJMatrix2D::Identity();

	// Render sprites and all layers
	theSpriteEngine.OnPaint(rc);

	// Setup screen for rendering text
	pTheRenderDevice->SetViewTransform(DJMatrix::Identity());
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)g_nScreenWidth,(float)g_nScreenHeight,0.0f,100.0f);
	g_pFont->DrawString("Monkey King Rhythm\nDemo Version", DJVector3(10,10,0), DJVector2(32,32), 0xFFFFFFFF);
	g_pPlayer->Paint();
	g_pLevelManager->GetCurrentLevel()->PrePaint();
	return DJTRUE;
}

///

djint32 DJMonkeyKingApplication::OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Begin: %d %.2f %.2f", nID, x, y);

	if (DJApplication::OnTouchBegin(nDevice, nID, x, y))
		return 1;
	g_pPlayer->OnTouchBegin(nDevice, nID,x,y);
	g_pLevelManager->GetCurrentLevel()->GetStickGold()->OnTouchBegin(nDevice, nID,x,y);
    
	return 0;
}

///

djint32 DJMonkeyKingApplication::OnTouchMove(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Move: %d %.2f %.2f", nDevice, nID, x, y);

	if (DJApplication::OnTouchMove(nDevice, nID, x,y))
		return 1;

	return 0;
}

///

djint32 DJMonkeyKingApplication::OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch End: %d %.2f %.2f",nDevice, nID, x, y);

	if (DJApplication::OnTouchEnd(nDevice, nID, x,y))
		return 1;

	return 0;
}

///

djint32 DJMonkeyKingApplication::OnAccelerate(float x, float y, float z)
{
	if (DJApplication::OnAccelerate(x,y,z))
		return 1;

	//DJInfo("%s %.2f %.2f %.2f", __FUNCTION__, x, y, z);
	return 0;
}

///

djint32 DJMonkeyKingApplication::OnButtonDown(djint32 nKey)
{
	DJInfo("Button Down: %d", nKey);

	if (DJApplication::OnButtonDown(nKey))
		return 1;

	return 0;
}

///

djint32 DJMonkeyKingApplication::OnButtonUp(djint32 nKey)
{
	DJInfo("Button Up: %d", nKey);

	if (DJApplication::OnButtonUp(nKey))
		return 1;

	return 0;
}

///
djint32 DJMonkeyKingApplication::OnKeyDown( djint32 nKey)
{
#ifdef _DEV
	if (nKey == DJKEY_B)
	{
		theBoundingBoxCollection.Toggle();
		return 1;
	}
	if(nKey == DJKEY_SPACE)
	{
		if(g_fGameTimeScale != 0.1f)
		{
			g_fGameTimeScale = 0.1f;
		}
		else
		{
			g_fGameTimeScale = 1.0f;
		}
	}
#endif //_DEV
}

///

void DJMonkeyKingApplication::OnMessage(djuint32 nMessage, djuint32 nParam1, djuint32 nParam2)
{
	//DJTrace(__FUNCTION__"()");
	DJApplication::OnMessage(nMessage, nParam1, nParam2);
}
// Application class
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

