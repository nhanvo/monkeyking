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
#include <djsounddevice.h>
#include <djostime.h>
#include <djcamera.h>
#include <djmesh.h>
#include <djfont.h>
#include <dj2dutil.h>
#include <djeffectresourcemanager.h>
#include <djlocalizer.h>
#include <djgamesounds.h>
#include <djvideo.h>
#include <djdevicemanager.h>
#include <djservice.h>
#include <djscene.h>
#include <djosthread.h>
#include <djconsole.h>
#include <djanalytics.h>
#include <djuinode.h>

/////////////////////////////////////////////////////////////////
// Game Includes
#include "monkeyking.h"
#include "level.h"
#include "musichandler.h"
#include "camera.h"
#include "player.h"
#include "debug.h"
#include "musichandler.h"
#include "menulevelselect.h"
#include "menumain.h"

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
ENUMERATE_INTERFACE(LevelUINode)
ENUMERATE_INTERFACE(LevelSelectMenuPageUINode)
ENUMERATE_INTERFACE(MainMenuPageUINode)
END_ENUMERATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_CREATE_INTERFACE()
CREATE_INTERFACE(MonkeyKingApplication)
CREATE_INTERFACE(LevelUINode)
CREATE_INTERFACE(LevelSelectMenuPageUINode)
CREATE_INTERFACE(MainMenuPageUINode)
END_CREATE_INTERFACE()
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
BEGIN_REGISTER_SYMBOL(MonkeyKing)
REGISTER_INTERFACE(LevelUINode)
REGISTER_INTERFACE(LevelSelectMenuPageUINode)
REGISTER_INTERFACE(MonkeyKingApplication)
REGISTER_INTERFACE(MainMenuPageUINode)
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
djint32					g_SceneStart			= SCENE_CENTIPEDE_SPECTER;	

djint32 g_nControls		= CONTROLS_TOUCH;

// UI view port
DJViewport				g_UIViewport;
djuint32				g_nHUDWidth				= 0;
djuint32				g_nHUDHeight			= 0;
float					g_fScreenScaler			= 1.0f;
float					g_fAspectScaler			= 1.0f;


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
Camera*						g_pCamera				= NULL;
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
	m_fDeltaFrameTime	= 0.0f;
	m_fDeltaAppTime		= 0.0f;
	m_fAppTimeScale		= 1.0f;
	m_fAppTime			= 0.0f;
	m_fFrameTime		= 0.0f;
	m_dwFrameCount		= 0;

	// allocate memory for menu 
	djMemSet(m_pMenus, 0, sizeof(m_pMenus));

	m_bGameIsLoaded		= DJFALSE;
	m_bGotoPauseMenu	= DJFALSE;

	m_nGameState		= GS_PRELOAD;
	m_nLastGameState	= GS_PRELOAD;
	m_fStateTimer		= 0.0f;
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
	
	// Get size of screen and put in global variables
	pTheRenderDevice->GetScreenSize(g_nScreenWidth, g_nScreenHeight);

	djuint32 rsw, rsh;
	pTheRenderDevice->GetScreenSize(rsw, rsh);
	pTheRenderDevice->SetViewport(DJViewport(rsw/2-g_nScreenWidth/2, rsh/2 - g_nScreenHeight/2, g_nScreenWidth, g_nScreenHeight));
	g_fScreenScaler = DJ_MIN((float) g_nScreenWidth/1024.0f, (float)g_nScreenHeight/768.0f);
	g_fAspectScaler = ((float)g_nScreenWidth/(float)g_nScreenHeight)/(1024.0f/768.0f);
	if(g_fAspectScaler < 1.0f)
		g_fAspectScaler = 1.0f;

	if(g_nScreenWidth == g_nScreenHeight)
	{
		g_UIViewport.SetViewport(0,0,720,720);
		g_nHUDWidth = 720;
		g_nHUDHeight = (djuint32)((float)g_nScreenHeight/(float)g_nScreenWidth*(float)g_nHUDWidth);
	}
	else if(g_nScreenWidth <= 480)
	{
		g_UIViewport.SetViewport(0,0,800,(djint32)(520.0f*((float)(g_nScreenHeight)/(float)(g_nScreenWidth))/ ((float)UI_DESIGN_HEIGHT/(float)UI_DESIGN_WIDTH)));
		g_nHUDWidth		= 800;
		g_nHUDHeight	= 800*g_nScreenHeight/g_nScreenWidth;
	}
	else
	{
		float fh = (((float)(g_nScreenHeight)/(float)(g_nScreenWidth))/((float)UI_DESIGN_HEIGHT/(float)UI_DESIGN_WIDTH));
		float fw = 1.0f;
		if (fh < 1.3356521739130434782608695652174f)
		{
			fw = 1.3356521739130434782608695652174f/fh;
			fh = 1.3356521739130434782608695652174f;
		}
		g_UIViewport.SetViewport(0,0,(djint32)(UI_DESIGN_WIDTH*fw),(djint32)(UI_DESIGN_HEIGHT*fh));
		g_nHUDWidth		= DJ_MIN(g_nScreenWidth,1024);
		g_nHUDHeight	= (djuint32)((float)g_nScreenHeight/(float)g_nScreenWidth*(float)g_nHUDWidth);

		if (g_nScreenWidth==960 && g_nScreenHeight==544)
		{
			g_UIViewport.SetViewport(0,0,(djint32)(700.0f*960.0f/544.0f),700);
			g_nHUDWidth		= (djuint32)((float)UI_DESIGN_HEIGHT*960.0f/544.0f);
			g_nHUDHeight	= UI_DESIGN_HEIGHT;
		}
		else if (g_nScreenWidth==1024 && g_nScreenHeight==600)
		{
			g_UIViewport.SetViewport(0,0,1194,700);
		}
		else if (g_nScreenWidth==854 && g_nScreenHeight==480)
		{
			g_UIViewport.SetViewport(0,0,1244,700);
			g_nHUDWidth		= (djuint32)((float)UI_DESIGN_HEIGHT*854.0f/480.0f);
			g_nHUDHeight	= UI_DESIGN_HEIGHT;
		}
		else if (g_nScreenWidth==800 && g_nScreenHeight==480)
		{
			g_UIViewport.SetViewport(0,0,1167,700);
			g_nHUDWidth		= (djuint32)((float)UI_DESIGN_HEIGHT*800.0f/480.0f);
			g_nHUDHeight	= UI_DESIGN_HEIGHT;
		}
		else if (g_nScreenWidth==1136 && g_nScreenHeight==640)
		{
			g_UIViewport.SetViewport(0,0,1242,700);
		}
		else if (g_nScreenWidth==1280 && g_nScreenHeight==768)
		{
			g_UIViewport.SetViewport(0,0,1166,700);
		}
		else if (g_nScreenWidth==1280 && g_nScreenHeight==720)
		{
			g_UIViewport.SetViewport(0,0,1244,700);
		}
		else if (g_nScreenWidth==960 && g_nScreenHeight==640)
		{
			g_UIViewport.SetViewport(0,0,(djint32)(700.0f*960.0f/640.0f),700);
			//g_UIViewport.SetViewport(0,0,1244,700);
		}
		/*else if (g_nScreenWidth==1024 && g_nScreenHeight==600)
		{
			g_UIViewport.SetViewport(0,0,1194,700);
		}*/

		/*if (g_UIViewport.GetHeight() < (float)UI_DESIGN_HEIGHT * (550.0f/480.0f))
		{
			g_UIViewport.SetHeight(djRoundToInt((float)UI_DESIGN_WIDTH * (550.0f/480.0f)));
			g_UIViewport.SetWidth(djRoundToInt(((float)UI_DESIGN_WIDTH*((float)(g_nScreenWidth)/(float)(g_nScreenHeight))/((float)UI_DESIGN_WIDTH/550.0f))));
		}*/
	}
	theLocalizer.SetUseUnknownString(DJFALSE);

	// UI init
	pTheUI = DJ_NEW(DJUI);
	pTheUI->Init();
	pTheUI->SetMultitouchEnabled(DJFALSE);
	pTheUI->SetUIWidth(g_UIViewport.GetWidth());
	pTheUI->SetUIHeight(g_UIViewport.GetHeight());
	pTheUI->RegisterEventListener(this);

	// Randomize random seed by using system time
	djRandomSetSeed( (djuint32)(djGetSystemTimeFloat() * 65536.0f) );

	
	// Initialize sprite engine with the layers we want
	if (!theSpriteEngine.Init(LAYER_COUNT))
	{
		DJError("Failed to initialize the sprite engine!");
		return DJ_FAILURE;
	}

	// Effect resource manager 
	theEffectResourceManager.Init();

	// menu text	
	theLocalizer.AddTagFile("text/eng/menu.text");

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

	/* ::TODO
	theMusicHandler.SetGlobalMusicVolume(theUserStorage.m_storage.m_fMusicVolume);
	pTheSoundDevice->ChangeVolumeGroup(0, theUserStorage.m_storage.m_fSoundVolume);
	pTheSoundDevice->ChangeVolumeGroup(31, theUserStorage.m_storage.m_fMenuVolume);
	pTheSoundDevice->ChangeVolumeGroup(1, theUserStorage.m_storage.m_fVoiceVolume);
	g_nControls = theUserStorage.m_storage.m_nControls;
	g_fControlSensitivity = theUserStorage.m_storage.m_fControlSensitivity*0.5f+0.5f;
	*/	 

	// Animate the cursor for loading
	LoadFinish();

	m_bInitialized = DJTRUE;

	return DJ_SUCCESS;
}

///

djbool DJMonkeyKingApplication::OnLoad()
{
	DJTrace("%s()", __FUNCTION__);

	if (m_bGameIsLoaded)
	{
		return DJTRUE;
	}

	/// ::TODO - Load menu system background animation cutscene
	
	/// ::TODO - Load simple filled rectangle drawing material

	// Create camera
	g_pCamera = DJ_NEW (Camera);
	if(g_pCamera == NULL)
	{
		DJError("Failed to create camera (out of memory)!");
		return DJFALSE;
	}

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

	// Load menu for menu system
	m_pMenus[MENU_MAIN] = (DJUINode*)theResourceManager.GetResource("menumain", DJResource::TYPE_UINODE);
	if (m_pMenus[MENU_MAIN])
	{
		m_pMenus[MENU_MAIN]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[MENU_MAIN]);
		m_pMenus[MENU_MAIN]->AddToParent(pTheUI->GetRootNode());
	}

	m_pMenus[MENU_LEVEL_SELECT] = (DJUINode*)theResourceManager.GetResource("levelselect", DJResource::TYPE_UINODE);
	if (m_pMenus[MENU_LEVEL_SELECT])
	{
		m_pMenus[MENU_LEVEL_SELECT]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[MENU_LEVEL_SELECT]);
		m_pMenus[MENU_LEVEL_SELECT]->AddToParent(pTheUI->GetRootNode());
	}

	m_pMenus[MENU_GAMEOVER] = (DJUINode*)theResourceManager.GetResource("game_over", DJResource::TYPE_UINODE);
	if (m_pMenus[MENU_GAMEOVER])
	{
		m_pMenus[MENU_GAMEOVER]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[MENU_GAMEOVER]);
		m_pMenus[MENU_GAMEOVER]->AddToParent(pTheUI->GetRootNode());
	}

	m_pMenus[MENU_DEBUG] = (DJUINode*)theResourceManager.GetResource("debug", DJResource::TYPE_UINODE);
	if (m_pMenus[MENU_DEBUG])
	{
		m_pMenus[MENU_DEBUG]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[MENU_DEBUG]);
		m_pMenus[MENU_DEBUG]->AddToParent(pTheUI->GetRootNode());
	}

	m_pMenus[MENU_INGAME] = (DJUINode*)theResourceManager.GetResource("ingame", DJResource::TYPE_UINODE);
	if (m_pMenus[MENU_INGAME])
	{
		m_pMenus[MENU_INGAME]->ShowNode(DJFALSE);
		pTheUI->GetRootNode()->GetChildList().AddLast(m_pMenus[MENU_INGAME]);
		m_pMenus[MENU_INGAME]->AddToParent(pTheUI->GetRootNode());
	}

	m_bGameIsLoaded = DJTRUE;
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

	
	pTheUI->UnregisterEventListener(this);
	pTheUI->Term();
	DJ_SAFE_DELETE(pTheUI);

	// Terminate DJEngine gamesound helper function
	theMusicHandler.Term();
	djGameSoundsTerm();

	m_bGameIsLoaded = DJFALSE;
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

djuint32 DJMonkeyKingApplication::PopMenu()
{
	djuint32 uCurrentMenu = GetCurrentMenu();
	djuint32 uMenu	= GetLastMenu();
	djuint32 uRet = m_menuStack.Pop();

	// make menu want to go to is not MENU_RATE_APP or MENU_MB_YESNO
	//while((uMenu == MENU_RATE_APP) || (uMenu == MENU_MB_YESNO))
	//{
	//	uMenu = GetLastMenu();
	//	m_menuStack.Pop();
	//}

	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	if (uMenu != MENU_INVALID)
	{
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}

	return uRet;
}

///

void DJMonkeyKingApplication::GotoMenu(djuint32 uMenu, djbool bReplaceStack, djbool bReplaceLast)
{
	djuint32 uCurrentMenu = GetCurrentMenu();

	if (bReplaceStack)
	{
		m_menuStack.SetCurrentIndex(0);
	}
	else if (bReplaceLast)
	{
		m_menuStack.Pop();
	}

	m_menuStack.Push(uMenu);
	//m_uLastMenu = m_uCurrentMenu;
	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	//m_uCurrentMenu = uMenu;
	if (uMenu != MENU_INVALID)
	{
		if (m_pMenus[uMenu]->IsNodeVisible())
		{
			m_pMenus[uMenu]->ShowNode(DJFALSE);
		}
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}
}

///

// Check if requested menu is already on stack
djbool DJMonkeyKingApplication::IsMenuOnStack(djuint32 uMenu)
{
	for (int q=0; q<m_menuStack.GetCurrentIndex(); q++)
	{
		if (m_menuStack[q] == uMenu)
		{
			return DJTRUE;
		}
	}
	return DJFALSE;
}

///

// Go back on stack to requested menu
void DJMonkeyKingApplication::GoBackToMenu(djuint32 uMenu)
{
	djuint32 uCurrentMenu = GetCurrentMenu();

	djbool bFound = DJFALSE;
	for (int q=m_menuStack.GetCurrentIndex()-1; q>=0; q--)
	{
		if (m_menuStack[q] == uMenu)
		{
			m_menuStack.SetCurrentIndex(q+1);
			bFound = DJTRUE;
			break;
		}
	}
	if (!bFound)
	{
		m_menuStack.SetCurrentIndex(0);
		m_menuStack.Push(uMenu);
	}

	if (uCurrentMenu != MENU_INVALID)
		m_pMenus[uCurrentMenu]->EnableNode(DJFALSE);
	if (uMenu != MENU_INVALID)
	{
		if (m_pMenus[uMenu]->IsNodeVisible())
		{
			m_pMenus[uMenu]->ShowNode(DJFALSE);
		}
		m_pMenus[uMenu]->EnableNode(DJTRUE);
		m_pMenus[uMenu]->ShowNode(DJTRUE);
	}
}

///

djbool DJMonkeyKingApplication::GotoGameState(djint nState)
{
	djbool bRet = (m_nGameState != nState);
	// deactive old state
	switch(m_nGameState)
	{
		case GS_LOAD_LEVEL:
		case GS_INGAME:
		case GS_LOAD_GAME:
		case GS_MENU:
		case GS_UNLOAD_LEVEL:
		case GS_GAMEOVER:
		{
			if(m_pMenus[GetCurrentMenu()])
			{
				m_pMenus[GetCurrentMenu()]->EnableNode(DJFALSE);				
			}
		}	
		break;
	};
	if(pTheUI)
	{
		pTheUI->SetUIWidth(g_UIViewport.GetWidth());
		pTheUI->SetUIHeight(g_UIViewport.GetHeight());
	}

	// active new state
	switch(nState)
	{
		case GS_LOAD_LEVEL:
		case GS_LOAD_GAME:
		case GS_UNLOAD_LEVEL:
		{
			/// TODO:: Load game 
		}
		break;

		case GS_MENU:
		{
			if(IsMenuOnStack(MENU_MAIN) && (GetCurrentMenu() != MENU_MAIN))
			{
				m_pMenus[MENU_MAIN]->EnableNode(DJFALSE);
				if(m_pMenus[MENU_MAIN]->IsNodeVisible())
				{
					m_pMenus[MENU_MAIN]->ShowNode(DJFALSE);
				}
				m_pMenus[GetCurrentMenu()]->ShowNode(DJTRUE);
				m_pMenus[GetCurrentMenu()]->EnableNode(DJTRUE);
			}
		}
		break;

		case GS_INGAME_MENU:
		{
			GotoMenu(MENU_INGAME);
			m_pMenus[GetCurrentMenu()]->ShowNode(DJTRUE);
			m_pMenus[GetCurrentMenu()]->EnableNode(DJTRUE);
		}
		break;

		case GS_GAMEOVER:
		{
			pTheUI->SetUIWidth(g_nHUDWidth);
			pTheUI->SetUIHeight(g_nHUDHeight);
			GotoMenu(MENU_GAMEOVER);
			m_pMenus[GetCurrentMenu()]->ShowNode(DJTRUE);
			m_pMenus[GetCurrentMenu()]->EnableNode(DJTRUE);
		}
		break;

		case GS_INGAME:
		{
			pTheUI->SetUIWidth(g_nHUDWidth);
			pTheUI->SetUIHeight(g_nHUDHeight);
			GotoMenu(MENU_HUD);
			m_pMenus[GetCurrentMenu()]->ShowNode(DJTRUE);
			m_pMenus[GetCurrentMenu()]->EnableNode(DJTRUE);
		}
		break;
	}

	m_nLastGameState = m_nGameState;
	m_nGameState = nState;
	return bRet;
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

	pTheUI->OnUpdate();	 

	switch(m_nGameState)
	{
		case GS_PRELOAD:
#ifdef USE_PRELOGO
			GotoGameState(GS_PRELOGO);
#else    // USE_PRELOGO
			//GotoGameState(GS_LOGO);
			GotoGameState(GS_LOAD_GAME);
#endif	 // USE_PRELOGO
		break;

		case GS_PRELOGO:
			UpdatePreLogo();
		break;

		case GS_LOGO:
			UpdateLogo();
		break;

		case GS_LOAD_GAME:
			UpdateLoadGame();
		break;

		case GS_LOAD_LEVEL:
			UpdateLoadLevel();
		break;

		case GS_UNLOAD_LEVEL:
			UpdateUnloadLevel();
		break;

		case GS_MENU:
		case GS_INGAME_MENU:
			UpdateMenu();
		break;

		case GS_GAMEOVER:
		case GS_INGAME:
			UpdateIngame();
		break;

	}	
}

///

djbool DJMonkeyKingApplication::OnPaint()
{
	DJTrace(__FUNCTION__"()");
	djuint32 rsw, rsh;
	pTheRenderDevice->GetScreenSize(rsw, rsh);
	pTheRenderDevice->SetViewport(DJViewport(rsw/2-g_nScreenWidth/2, rsh/2 - g_nScreenHeight/2, g_nScreenWidth, g_nScreenHeight));

	if(m_bQuit)
	{
		pTheRenderDevice->SetClearColor(DJColor(0.,0,0,1));
		pTheRenderDevice->Clear(DJRenderDevice::flagClearAll);
		return DJTRUE;
	}

	pTheRenderDevice->SetModColor(DJColor(1,1,1,1));
	switch(m_nGameState)
	{
		case GS_PRELOAD:
		break;
		case GS_PRELOGO:
			PaintPreLogo();
		break;
		case GS_LOGO:
			PaintLogo();
		break;
		case GS_LOAD_GAME:
			PaintLoadGame();
		break;
		case GS_LOAD_LEVEL:
		case GS_UNLOAD_LEVEL:
			PaintLoadLevel();
		break;
		case GS_MENU:
			PaintMenu();
		break;
		case GS_INGAME_MENU:
			PaintMenu();
		break;
		case GS_GAMEOVER:
		case GS_INGAME:
			PaintIngame();
		break;	
	};		
	return DJTRUE;
}

///

enum
{
	TOUCHOWNER_INVALID = -1,
	TOUCHOWNER_APP,
	TOUCHOWNER_PLAYER,
};

djint32 g_nTouchOwners[16] =  {
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
	TOUCHOWNER_INVALID,
};

djint32 DJMonkeyKingApplication::OnTouchBegin(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Begin: %d %.2f %.2f", nID, x, y);
	djint32 nRet = 0;
	djint32 sx,sy,sw,sh;
    pTheRenderDevice->GetViewportSize(sx,sy,sw,sh);
    x = ((float)x-(float)sx);
    y = ((float)y-(float)sy);
	
	if (g_nTouchOwners[nID] != TOUCHOWNER_INVALID)
	{
		OnTouchCancel(nDevice,nID, x, y);
	}
	g_nTouchOwners[nID] = TOUCHOWNER_INVALID;

	if(!nRet)
	{
		if(g_pPlayer)
		{
			nRet = g_pPlayer->OnTouchBegin(nDevice, nID,x,y);
			if(nRet)
				g_nTouchOwners[nID] == TOUCHOWNER_PLAYER;
		}
	}

	
	if(g_pLevelManager->GetCurrentLevel()->GetCurrentScene()->IsStickGold())
	{
		g_pLevelManager->GetCurrentLevel()->GetStickGold()->OnTouchBegin(nDevice, nID,x,y);
	}
    
	return 0;
}

///

djint32 DJMonkeyKingApplication::OnTouchMove(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch Move: %d %.2f %.2f", nDevice, nID, x, y);

	if(g_nTouchOwners[nID] == TOUCHOWNER_INVALID)
		return 0;
	djint32 nRet = 0;
	if(g_nTouchOwners[nID] == TOUCHOWNER_APP)
	{
		if(!nRet)
		{
			nRet = DJApplication::OnTouchMove(nDevice, nID, x, y);
		}
	}
	else if(g_nTouchOwners[nID] == TOUCHOWNER_PLAYER)
	{
		if(g_pPlayer)
		{
			nRet = g_pPlayer->OnTouchMove(nDevice, nID, x, y);
		}
	}

	return nRet;
}

///

djint32 DJMonkeyKingApplication::OnTouchEnd(djint32 nDevice, djint32 nID, float x, float y)
{
	DJInfo("Touch End: %d %.2f %.2f",nDevice, nID, x, y);

	djint32 nRet = 0;
	if(g_nTouchOwners[nID] == TOUCHOWNER_PLAYER)
	{
		if(g_pPlayer)
		{
			nRet = g_pPlayer->OnTouchEnd(nDevice, nID, x, y);
		}
	}
	g_nTouchOwners[nID] = TOUCHOWNER_INVALID;
	return nRet;
}

///

djint32	DJMonkeyKingApplication::OnTouchCancel( djint32 nDevice, djint32 nID, float fX, float fY )
{
	DJAssert(nID >= 0 && nID < 16);
	if(g_nTouchOwners[nID] == TOUCHOWNER_INVALID)
		return 0;

	djint32 nRet = 0;
	if(g_nTouchOwners[nID] == TOUCHOWNER_PLAYER)
	{
		if(g_pPlayer)
		{
			nRet = g_pPlayer->OnTouchCancel(nDevice, nID, fX, fY);
		}
	}
	g_nTouchOwners[nID] = TOUCHOWNER_INVALID;
	return nRet;
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

///
djbool DJMonkeyKingApplication::OnUIEvent(DJUINode *pNode, const DJUIEvent &ev)
{
	return DJTRUE;
}

///

void DJMonkeyKingApplication::PaintPreLogo()
{

}

/// 

void DJMonkeyKingApplication::PaintLogo()
{

}

///

void DJMonkeyKingApplication::PaintLoadGame()
{

}

///

void DJMonkeyKingApplication::PaintLoadLevel()
{

}

///

void DJMonkeyKingApplication::PaintMenu()
{
	pTheRenderDevice->SetViewTransform(DJMatrix::Translate(DJVector3(0, 0, 0)));
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)UI_DESIGN_WIDTH,(float)UI_DESIGN_HEIGHT,0.0f,100.0f);

	static float fCol = 1.0f;
	//if ((GetCurrentMenu() == MENU_MB_YESNO && m_pMenus[MENU_MB_YESNO]->IsNodeEnabled()) ||
	//	(GetCurrentMenu() == MENU_RATE_APP && m_pMenus[MENU_RATE_APP]->IsNodeEnabled()))
	//	fCol += djStepToDesiredValue(fCol, 0.05f, 0.5f)*0.1f;
	//else
		fCol += djStepToDesiredValue(fCol, 1.0f, 0.5f)*0.1f;

	DJColor cCol(fCol,fCol,fCol,1);
	//PaintMenuBackground(DJTRUE, DJFALSE, &cCol);

	pTheRenderDevice->SetViewTransform(DJMatrix::Translate(DJVector3(0, 0, 0)));
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)UI_DESIGN_WIDTH,(float)UI_DESIGN_HEIGHT,0.0f,100.0f);
	//pTheRenderDevice->SetViewport(DJViewport(0,0,(float)UI_DESIGN_WIDTH,(float)UI_DESIGN_HEIGHT));

	DJ2DRenderContext rc;
	rc.m_uFlags = 0;
	rc.m_cModColor = DJColor(fCol,fCol,fCol,fCol);
	rc.m_cAddColor = DJColor(0,0,0,0);
	rc.m_mLayerTransform = DJMatrix2D::Identity();
	rc.m_pLayer = NULL;
	rc.m_mTransform = DJMatrix2D::Identity();

	//pTheRenderDevice->EnableZBuffer(0);
	//pTheRenderDevice->Clear(DJRenderDevice::flagClearZ);

	// Render debug
	pTheRenderDevice->SetViewTransform(DJMatrix::Translate(DJVector3(0, 0, 0)));
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)g_nScreenWidth,(float)g_nScreenHeight,0.0f,100.0f);
	rc.m_cModColor = DJColor(1,1,1,1);
	rc.m_pViewport = NULL;
	theSpriteEngine.OnPaint(rc);

	// Render UI
	pTheRenderDevice->SetViewTransform(DJMatrix::Translate(DJVector3(0, 0, 0)));
	pTheRenderDevice->SetPerspectiveOrtho(0,0,(float)g_UIViewport.GetWidth(),(float)g_UIViewport.GetHeight(),0.0f,100.0f);
	rc.m_pViewport = &g_UIViewport;
	pTheUI->OnPaint(rc);
	rc.m_pViewport = NULL;	 
}

/// 

void DJMonkeyKingApplication::PaintIngame()
{
	g_pFont->DrawString("Monkey King Rhythm\nDemo Version", DJVector3(10,10,0), DJVector2(32,32), 0xFFFFFFFF);
	g_pPlayer->Paint();
	g_pLevelManager->GetCurrentLevel()->PrePaint();
}

///

void DJMonkeyKingApplication::UpdatePreLogo()
{

}

///

void DJMonkeyKingApplication::UpdateLogo()
{
	GotoGameState(GS_LOAD_GAME);
}

///

void DJMonkeyKingApplication::UpdateLoadGame()
{
	if (!OnLoad())
	{
		DJWarning("Failed to load game!");		
	}
	GotoMenu(MENU_MAIN);
	GotoGameState(GS_MENU);
}

/// 
void DJMonkeyKingApplication::UpdateLoadLevel()
{
	// Load level
	if (g_pLevelManager->GetCurrentLevel() == NULL)
	{
		if (!g_pLevelManager->LoadLevel("levels/level01.level"))
		{
			DJWarning("Load level not finish");
			DJAssert(DJFALSE);
		}
		else
		{
			GotoGameState(GS_INGAME);
		}
	}
}

///
void DJMonkeyKingApplication::UpdateUnloadLevel()
{

}

///

void DJMonkeyKingApplication::UpdateMenu()
{
	/// ::TODO - Update menu background.
	/// Move between menus 
}

/// 

void DJMonkeyKingApplication::UpdateIngame()
{	
	// Update level 
	if (!g_pLevelManager->Update(m_fDeltaAppTime))
	{
		DJAssert(DJFALSE);
	}	
}

///

djbool DJMonkeyKingApplication::OnLoadLevel()
{
	return DJTRUE;
}

///

djbool DJMonkeyKingApplication::OnUnLoadLevel()
{
	return DJTRUE;
}

// Application class
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////

