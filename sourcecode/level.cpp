//////////////////////////////////////////////////////////////////
//
// level.cpp
// Level class
//
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Engine Includes

/////////////////////////////////////////////////////////////////
// Game Includes
#include <djmodulemanager.h>
#include <djostime.h>
#include <dj2d.h>
#include <dj2dutil.h>
#include <djgamesounds.h>
#include <djfont.h>
#include <djdevicemanager.h>
#include <djrenderdevice.h>
#include <djcamera.h>
#include <djstring.h>
/////////////////////////////////////////////////////////////////
#include "level.h"
#include "monkeyking.h"
#include "monkeycilivians.h"
#include "util.h"
#include "musichandler.h"
/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
extern djuint32 g_nScreenWidth;
extern djuint32 g_nScreenHeight;
extern DJVector2 g_vScaleScreen;
extern DJCamera* g_pCamera;
extern Player* g_pPlayer;
extern djbool g_bRemoveOneLayerInBG;
extern DJColor			g_cModColor;
extern djfloat			g_fRestrictionBottom;
extern DJMusicHandler theMusicHandler;

const char* szLevelID[Level::LEVEL_COUNT] = 
{
	"LEVEL_01",
	"LEVEL_02",
	"LEVEL_03"
};

const char* szSceneID[LevelScene::SCENE_COUNT] = 
{
	"SCENE_01",
	"SCENE_02",
	"SCENE_03"
};

const char* g_szSceneMusic = "music/";

/////////////////////////////////////////////////////////////////
LevelScene::LevelScene()
{
	m_pSceneRoot		= NULL;
	m_pSprite			= NULL;
	m_vCameraPos		= DJVector2(0,0);
	m_fCameraRotation	= 0.0f;
	m_fCameraZoom		= 1.0f;
	m_name				= "";
	m_sID				= "";
	m_bStickGold		= DJFALSE;
	m_bMonkeyCilivians	= DJFALSE;
	m_bBeatsTime		= DJFALSE;
	m_bRayGhost			= DJFALSE;
	m_bPlayer			= DJFALSE;
}

///

LevelScene::~LevelScene()
{
	Term();
}

///

djbool LevelScene::Init(DJTagFile& file, DJTagDir* pDir)
{
	if (m_pSceneRoot)
	{
		DJWarning("LevelScene::Init() called on a scene that has already been initialized!");
		Term();
	}

	DJAssert(m_pSprite == NULL);

	// Init LevelID for Scene
	DJTagLine* pLine;

	// Parse name
	m_name = pDir->m_attribute;

	// Parse nodes
	m_pSceneRoot = DJ_NEW(DJ2DNode);

	theSpriteEngine.AddNode(m_pSceneRoot, LAYER_BACKGROUND);

	DJLinkedListIter<DJTagLine> iter(pDir->m_lines);
	while ((pLine = iter.GetStep()))
	{
		// Stick gold
		if(pLine->m_name == "STICKGOLD")
		{
			if(!pLine->GetArgVector2(0, m_StickGoldData.vPos))
			{
				DJWarning("Invalid POSITION from STICKGOLD tag");
			}
			if(!pLine->GetArgString(1, m_StickGoldData.strAtlastFile))
			{
				DJWarning("Invalid SIZE from STICKGOLD tag");
			}
			if(!pLine->GetArgString(2, m_StickGoldData.strAnimationName))
			{
				DJWarning("Invalid ANIMATION_NAME from STICKGOLD tag");
			}
			m_bStickGold = DJTRUE;
		}						  
		// Monkey cilivians
		if(pLine->m_name == "MONKEY_CIVILIANS")
		{
			MonkeyCiviliansData	*pMonkeyCiviliansData =	DJ_NEW(MonkeyCiviliansData);
			if(!pLine->GetArgInt(0,pMonkeyCiviliansData->id))
			{
				DJWarning("Invalid ID from MONKEY_CIVILIANS	tag");
			}

			if(!pLine->GetArgVector2(1,pMonkeyCiviliansData->vpos))
			{
				DJWarning("Invalid POSITION	from MONKEY_CIVILIANS tag");
			}

			if(!pLine->GetArgInt(2,	pMonkeyCiviliansData->nBeatsTimeGroup))
			{
				DJWarning("Invalid <BEATS TIME GROUP> from MONKEY_CIVILIANS	tag");			
			}

			m_listMonkeyCiviliansData.AddLast(pMonkeyCiviliansData);
			m_bMonkeyCilivians = DJTRUE;
		} 

		// Player
		if(pLine->m_name == "PLAYER")
		{
			if(!pLine->GetArgVector2(0, m_PlayerData.vPos))
			{
				DJWarning("Invalid POSITION from PLAYER tag");
			}
			
			m_bPlayer = DJTRUE;
		}

		// Beats time
		if(pLine->m_name == "BEATSTIME")
		{
			BeatsData *pBeatsData = DJ_NEW(BeatsData);
			if(!pLine->GetArgInt(0, pBeatsData->nID))
			{
				DJWarning("Invalid ID from BEATSTIME tag");				
			}
			if(!pLine->GetArgInt(1, pBeatsData->uCountNumber))
			{
				DJWarning("Invalid NUMBER MONKEY from BEATSTIME tag");				
			}

			if(!pLine->GetArgVector2(2,pBeatsData->vTimesDistance))
			{
				DJWarning("Invalid 	(START,END) from BEATSTIME tag");								
			}
			m_listBeatsData.AddLast(pBeatsData);
			m_bBeatsTime = DJTRUE;
		}

		// Ray ghost
		if(pLine->m_name == "RAYS_GHOST")
		{
			RaysGhostData *pRayGhostData = DJ_NEW(RaysGhostData);
			if(!pLine->GetArgInt(0, pRayGhostData->nID))
			{
				DJWarning("Invalid 	ID from RAY_GHOST tag");									
			}
			if(!pLine->GetArgInt(1, pRayGhostData->nType))
			{
				DJWarning("Invalid TYPE from RAY_GHOST tag");								
			}
			if(!pLine->GetArgVector2(2, pRayGhostData->vPosition))
			{
				DJWarning("Invalid 	POSITION from RAY_GHOST tag");									
			}
			if(!pLine->GetArgInt(3, pRayGhostData->nBeatsTimeGroup))
			{
				DJWarning("Invalid 	BEATS TIME GROUP from RAY_GHOST tag");				
			}
			m_listRayGhostData.AddLast(pRayGhostData);
			m_bRayGhost = DJTRUE;
		}

		// Specter
		if(pLine->m_name == "SPECTER")
		{
			SpecterData *pSpecterData = DJ_NEW(SpecterData);
			if(!pLine->GetArgInt(0, pSpecterData->nType))
			{
				DJWarning("Invalid 	TYPE from SPECTER tag");
			}

			if(!pLine->GetArgVector2(1, pSpecterData->vPosition))
			{
				DJWarning("Invalid POSITION from SPECTER tag");
			}

			m_listSpecterData.AddLast(pSpecterData);
			m_bSpecter = DJTRUE;
		}	

		if(pLine->m_name == "SCENE_ID")
		{
			if (!pLine->GetArgString(0, m_sID))
			{
				DJWarning("Failed to parse ID in SCENE_ID!");
				DJAssert(pLine != NULL);
			} 			

			// Sure scene is exits
			DJAssert(m_sID != "");
		}
		else if (pLine->m_name == "BG_IMAGE")
		{
			DJString id;
			DJString strBGConfig;
			if (!pLine->GetArgString(0, id))
			{
				DJWarning("Failed to parse ID in BG_IMAGE!");
				DJAssert(DJFALSE);
			} 
			else
			{
				djint32 i = id.Compare(id, "BG_CONFIG"); 
				if(i == 0)
				{
					if(!pLine->GetArgString(1, m_sBackgroundConfig))
					{
						DJWarning("Failed to parse BG_CONFIG_NAME in BG_IMAGE!");
						DJAssert(DJFALSE);
					}
				}
			}
		}
		else if (pLine->m_name == "SPRITE")
		{
			DJString id;
			DJVector2 vPos;
			djint32	nLayer;
			DJVector2 vScale;
			DJString strAnimationFile;

			if(!pLine->GetArgString(0, id) || 
				!pLine->GetArgString(1,	strAnimationFile)||
				!pLine->GetArgVector2(2, vPos) ||
				!pLine->GetArgInt(3, nLayer)||
				!pLine->GetArgVector2(4, vScale))
				
			{
				DJWarning("Failed to parse SPRITE in level scene!");
			} 		

			// init	sprite
			m_pSprite =	DJ_NEW(DJ2DSprite);
			DJAssert(m_pSprite != NULL);
			// Make	the	sprite magnify with	a blur instead of pixelated
			m_pSprite->SetFilter(DJ2DSprite::filterLinear);
			// Set the sprite to the center	of the screen
			m_pSprite->SetPosition(vPos);
			// Set the sprite sorting value	to the Y position of the sprite
			m_pSprite->SetSortValue(m_pSprite->GetPosition()[1]);
			m_pSprite->SetScale(vScale);

			DJ2DAnimation* pAnim =	(DJ2DAnimation*)theResourceManager.GetResource(strAnimationFile, DJResource::TYPE_ANIMATION2D);
			m_pSprite->PlayAnimation(pAnim);		
		} 		
		else if (pLine->m_name == "CAMERA")
		{
			if (!pLine->GetArgVector2(0, m_vCameraPos))
			{
				DJWarning("Invalid CAMERA tag in level scene!");
			}
			if (pLine->GetArgFloat(1, m_fCameraRotation))
			{
				m_fCameraRotation =	DJRADIANS(m_fCameraRotation);
			}
			if (pLine->GetArgFloat(2, m_fCameraZoom))
			{
			}
		} 	   	
		
	} 

	float fScaleX = (float)g_nScreenWidth/1024.0f;
	float fScaleY = (float)g_nScreenHeight/768;
	float fScale = DJ_MAX(fScaleX,fScaleY);

	//m_pSceneRoot->SetScale(fScale,fScale);
	m_pSceneRoot->SetPosition(m_vCameraPos[0], m_vCameraPos[1]);
	//m_pSceneRoot->SetRotation(m_fCameraRotation);	  

	// Sure Player is exits
	DJAssert(m_bPlayer != NULL);

	return DJTRUE;
}  

///

void LevelScene::Term()
{
	DJ_SAFE_DELETE(m_pSceneRoot);
	if(m_pSprite)
	{
		// Remove sprite from active list
		theSpriteEngine.RemoveActiveNode(m_pSprite);
		// Delete the sprite
		DJ_DELETE(m_pSprite);
	}
	m_vCameraPos		= DJVector2(0,0);
	m_fCameraRotation	= 0.0f;
	m_fCameraZoom		= 1.0f;
	m_name				= "";
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
Level::Level()
{
	m_pCurrentScene = NULL;
	m_pLevelBackground = NULL;
	m_nCurrentCity	= 0;
	m_bFinishLevel	= DJFALSE;
	m_sLevelName	= "";
	m_sID			= "";
	m_sSceneMusic	= "";
	m_pStickGold	= NULL;
}

///

Level::~Level()
{
	Term();
}

///

djbool Level::Init(const char* szLevelFile, djint32 nSceneID)
{
	DJTagFile tf;
	if (tf.Load(szLevelFile) != DJ_SUCCESS)
	{
		DJWarning("Failed to load or parse level!");
		return DJFALSE;
	}
	
	DJTagLine* pLine;
		
	tf.GetTag("LEVEL_DESC");
	tf.GetTag("VIRTUAL_SCREEN_SIZE");

	// Level ID
	pLine = tf.GetTag("LEVEL_ID");
	if(!pLine->GetArgString(0, m_sID))
	{
		DJAssert(DJFALSE);
	}

	// Sure level id is exits
	DJAssert(m_sID != "");

	// Level name
	pLine = tf.GetTag("LEVEL_NAME");
	if(!pLine->GetArgString(0, m_sLevelName))
	{
		DJWarning("Failed toload LEVEL_NAME fron LEVEL");
		DJAssert(0);
	}

	DJLinkedListIter<DJTagDir> iter(tf.m_root.m_dirs);
	DJTagDir* pDir;
	while ((pDir = iter.GetStep()))
	{
		if (pDir->m_name == "SCENE")
		{
			// pDir->m_attribute;
			LevelScene* pScene = DJ_NEW(LevelScene);
			if (!pScene->Init(tf, pDir))
			{
				DJWarning("Failed to initialize scene!");
			}
			else
			{
				m_scenes.AddLast(pScene);
			}
		}
	}

	// Set current scene
	m_pCurrentScene = m_scenes.GetByIndex(nSceneID); 

	// Init music for scene
	InitSceneMusic();

	// Current scene 
	if(m_pCurrentScene)
	{
		// New player 
		g_pPlayer = DJ_NEW(Player);
		if(g_pPlayer == NULL)
		{
			DJError("Failed to create Player (out of memory)!");
			return DJFALSE;
		}
		
		// Init player
		g_pPlayer->Init(m_pCurrentScene->GetPlayerData().vPos);	

		/////////////////////////////////////////////////////////////////////////////
		// Init Stick Gold
		if(m_pCurrentScene->IsStickGold())
		{  			
			LevelScene::StickGoldData pSG = m_pCurrentScene->GetStickGoldData();
			m_pStickGold = DJ_NEW(StickGold);
			m_pStickGold->Init(pSG.vPos, pSG.strAtlastFile, pSG.strAnimationName);
		}
		/////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////
		// init monkey cilivians
		if(m_pCurrentScene->IsMonKeyCilivians())
		{
			DJLinkedListIter<LevelScene::MonkeyCiviliansData> iter( m_pCurrentScene->GetMonkeyCiviliansData());
			LevelScene::MonkeyCiviliansData *pMCD;
			while((pMCD = iter.GetStep()))
			{
				MonkeyCivilians *pMC = DJ_NEW(MonkeyCivilians);
				pMC->Init(pMCD->id, pMCD->vpos, pMCD->nBeatsTimeGroup);
				m_listMonkeyCivians.AddLast(pMC);
			}
		}
		/////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////
		// init beats time
		if(m_pCurrentScene->IsBeatsTime())
		{
			DJLinkedListIter<LevelScene::BeatsData> iter1( m_pCurrentScene->GetBeatsData());
			LevelScene::BeatsData *pBD;
			while((pBD = iter1.GetStep()))
			{
				BeatsTime *pBT = DJ_NEW(BeatsTime);
				pBT->Init(pBD->nID, pBD->uCountNumber, pBD->vTimesDistance);
				m_listBeatsTime.AddLast(pBT);
			}
		}


		/////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////
		// init centipede specter

		// Ray ghost
		DJLinkedList<RaysGhost> listRayGhost;
		if(m_pCurrentScene->IsRayGhost())
		{
			DJLinkedListIter<LevelScene::RaysGhostData> iterRGD(m_pCurrentScene->GetRaysGhostData());
			LevelScene::RaysGhostData *pRGD;
			while((pRGD = iterRGD.GetStep()))
			{
				RaysGhost *pRG = DJ_NEW(RaysGhost);
				pRG->Init(pRGD->nID, pRGD->nType, pRGD->vPosition, pRGD->nBeatsTimeGroup);
				listRayGhost.AddLast(pRG);
			}
		}

		// Centipede
		if(m_pCurrentScene->IsSpecter())
		{
			DJLinkedListIter<LevelScene::SpecterData> iterSD(m_pCurrentScene->GetSpecterData());
			LevelScene::SpecterData *pSD;
			while((pSD = iterSD.GetStep()))
			{
				Centipede *pSPE; 
				switch(pSD->nType)
				{
					case Specter::SPECTER_CENTIPEDE:
					{
						pSPE = DJ_NEW(Centipede);  		

						DJAssert(listRayGhost.GetLength() != 0);
						pSPE->SetListRayGhost(listRayGhost); 

						DJAssert(m_listBeatsTime.GetLength() != 0);
						pSPE->SetListBeatsTime(m_listBeatsTime);

						((Centipede*)pSPE)->Init(pSD->nType, pSD->vPosition);
					}
					break;
				}				
				m_listSpecter.AddLast(pSPE);  
			}
		} 		
		/////////////////////////////////////////////////////////////////////////////		
	}

	// Random current city
	djint32 nCity = djRandomGetDword() % CITY_COUNT;
	if(nCity == m_nCurrentCity)
		m_nCurrentCity = (m_nCurrentCity + 1) % CITY_COUNT;
	else
		m_nCurrentCity = nCity;		

	// Level background
	m_pLevelBackground = DJ_NEW(LevelBackground);
	m_pLevelBackground->Init(m_pCurrentScene->GetBgFileConfig());
	m_pLevelBackground->SetLevelBGUp(m_nCurrentCity);
	m_pLevelBackground->Active(DJTRUE);

	//g_pCamera->SetupOrthoCamera(m_pCurrentScene->GetCameraPosition().e[0], m_pCurrentScene->GetCameraPosition().e[1], g_nScreenWidth, g_nScreenHeight, 0.0f, 0.0f);
	//g_pCamera->SetLocalTransform(DJMatrix::Translate(DJVector3(0, 0, 0)));
	//g_pCamera->UpdateTransforms();
	Reset();

	return DJTRUE;
}

///

void Level::Term()
{
	m_scenes.Clear(DJTRUE);
	m_pCurrentScene = NULL;
	//DJ_SAFE_DELETE(m_pLevelBackground);
}

///
static float fTimeDelay = 0.0f;

void Level::Update(float fDeltaTime)
{ 
	if(m_bFinishLevel)
	{	
		if(theMusicHandler.IsPlaying(m_sSceneMusic))
		{
			theMusicHandler.StopMusic(0.5f);
		}
		if(fTimeDelay <= 2.0f)
		{
			fTimeDelay += pTheApp->GetDeltaAppTime();			
		}   
		else
		{
			m_bFinishLevel = DJFALSE;
		}
		return;
	}
	else
	{  
		fTimeDelay = 0.0f;
		if(!theMusicHandler.IsPlaying(m_sSceneMusic))
		{
			theMusicHandler.PlayMusic(m_sSceneMusic);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	// update player
	g_pPlayer->Update(fDeltaTime);
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// update stick gold
	if(m_pCurrentScene->IsStickGold() && !m_bFinishLevel)
	{
		m_pStickGold->Update(fDeltaTime);
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// update monkey cilivians
	if(m_pCurrentScene->IsMonKeyCilivians())
	{
		DJLinkedListIter<MonkeyCivilians> iter(m_listMonkeyCivians);
		MonkeyCivilians *pMC;  
		while((pMC = iter.GetStep()))
		{
			if(pMC->GetIDBeatsTime() == -1)
			{
				DJWarning("Monkey cilivians %d invalid beats time", pMC->GetID());		
				DJAssert((pMC->GetIDBeatsTime() != -1));
			}
			else
			{
				DJLinkedListIter<BeatsTime> iter2(m_listBeatsTime);
				BeatsTime *pBT;
				while((pBT = iter2.GetStep()))
				{
					if(pMC->GetIDBeatsTime() == pBT->GetID())
					{
						if(pTheSoundDevice->GetMusicPos() >= pBT->GetBeatsTime().e[0] && 
							pTheSoundDevice->GetMusicPos() <= pBT->GetBeatsTime().e[1])
						{
		 					pMC->EnableJump();
						}
					}
				}
				pMC->Update(fDeltaTime, m_pStickGold->GetRectBoxHit());	

				// Reset level if monkey cilivians end is finish jump
				if(pMC == m_listMonkeyCivians.GetLast() &&
				   (m_listMonkeyCivians.GetLast()->GetState() == MonkeyCivilians::STATE_MC_DIE ||
				   m_listMonkeyCivians.GetLast()->GetState() == MonkeyCivilians::STATE_MC_FINISH))
				{
					m_bFinishLevel = DJTRUE;			
					Reset();			
				}
			}
			
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// update specter
	DJLinkedListIter<Centipede> iterSpecter(m_listSpecter);
	Specter* pSPEC;
	while((pSPEC = iterSpecter.GetStep()))
	{
		if(pSPEC->GetType() == Specter::SPECTER_CENTIPEDE)
		{
			((Centipede*)pSPEC)->Update(fDeltaTime);
		}
		
	};
	//////////////////////////////////////////////////////////////////////////////


	g_fRestrictionBottom = 1024.0f;
}

///

void Level::PrePaint()
{
	//m_pLevelBackground->OnPaint();
}

///

void Level::Reset()
{
	DJLinkedListIter<MonkeyCivilians> iter(m_listMonkeyCivians);
	MonkeyCivilians *pMC;  
	while((pMC = iter.GetStep()))
	{
		pMC->Reset();  
	} 	
}

///

///
void Level::InitSceneMusic()
{
	// Scene ID	
	if(djStringCompare(m_sID, szLevelID[Level::LEVEL_01])== 0)
	{
		if(djStringCompare(m_pCurrentScene->GetID(), szSceneID[LevelScene::SCENE_01]) == 0)
		{
			m_sSceneMusic = DJString("%s", g_szSceneMusic) + DJString("level%d_scene%d.mp3", Level::LEVEL_01, LevelScene::SCENE_01);						
		}
		else if(djStringCompare(m_pCurrentScene->GetID(), szSceneID[LevelScene::SCENE_02]) == 0)
		{
			m_sSceneMusic = DJString("%s", g_szSceneMusic) + DJString("level%d_scene%d.mp3", Level::LEVEL_01 , LevelScene::SCENE_02);						
		}

	}
	else if(djStringCompare(m_pCurrentScene->GetID(), szLevelID[Level::LEVEL_02])== 0)
	{
		
	}
	else if(djStringCompare(m_pCurrentScene->GetID(), szLevelID[Level::LEVEL_03])== 0)
	{
		
	}
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
LevelManager::LevelManager()
{
	m_pCurrentLevel = NULL;	
	m_nSceneID = 0;
}

///

LevelManager::~LevelManager()
{
    Term();
}

///

djbool LevelManager::Init()
{
	DJTrace("%s", __FUNCTION__); 
	return DJTRUE;
}

///

djbool LevelManager::Update(float fDeltaTime)
{
	DJTrace("%s()", __FUNCTION__);
	if(m_pCurrentLevel == NULL)
		return DJFALSE;

	m_pCurrentLevel->Update(fDeltaTime);
	return DJTRUE;
}

///

void LevelManager::Term()
{
	DJ_SAFE_DELETE(m_pCurrentLevel);
}

///

djbool LevelManager::LoadLevel(const char* szLevelFile)
{
	m_pCurrentLevel  = DJ_NEW(Level);
	if(m_pCurrentLevel == NULL || !m_pCurrentLevel->Init(szLevelFile, m_nSceneID))
	{
		DJError("Failed to load level!");
		DJ_SAFE_DELETE(m_pCurrentLevel);
		return DJFALSE;
	}
	return DJTRUE;
}

///

djbool LevelManager::UnloadLevel()
{
	return DJTRUE;
}

///

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
LevelBackground::LevelBackground()
{
	DJTrace("%s", __FUNCTION__);
	for(djuint32 i = 0; i<=LEVEL_BG_MAX; i++)
	{
		m_pMaterial[i] = NULL;
	} 
	m_nCity = 0;
	m_cCol = DJColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_bActive = DJFALSE;
}

///

LevelBackground::~LevelBackground()
{
	DJTrace("%s()", __FUNCTION__);
	Term();
}

///

void LevelBackground::Init(const char* strBGFileConfig)
{
	DJTrace("%s()", __FUNCTION__);
	djMemSet(m_pMaterial, 0,sizeof(DJMaterial*)*LEVEL_BG_MAX);
	DJTagFile file;
	file.Load(strBGFileConfig);
	DJTagDir* pCityDir;
	DJTagDir* pBGDir;
	DJTagLine* pLine;	
   	djint32 nCity = 0;

	pCityDir = file.GetDir("CITY");		
	while((pCityDir) && (nCity < CITY_COUNT))
	{
		pBGDir = pCityDir->GetDir("BACKGROUND");
		djint32 nBGNumber = 0;
		while((pBGDir) && (nBGNumber < LEVEL_BG_MAX))
		{
			BGData* pBGData = DJ_NEW(BGData);
			pLine = pBGDir->GetTag("MATERIAL");
			if(!pLine || !pLine->GetArgString(0, pBGData->sMaterialName))
			{
				DJError("Failed to load material file name.");
			}

			pLine = pBGDir->GetTag("UV");
			if(!pLine || !pLine->GetArgQuaternion(0,pBGData->quUV,DJFALSE))
			{
				DJError("Failed to load quaternion uv.");
			}

			pLine = pBGDir->GetTag("SCALE");
			if(!pLine || !pLine->GetArgFloat(0, pBGData->fScale))
			{
				DJError("Failed to load scale.");
			}

			pLine = pBGDir->GetTag("POSITION");
			if(!pLine || !pLine->GetArgVector2(0, pBGData->vPos))
			{
				DJError("Failed to position vector.");
			}

			pLine = pBGDir->GetTag("PARALAX");
			if(!pLine || !pLine->GetArgVector2(0, pBGData->vParalax))
			{
				DJError("Failed to paralax vector.");
			} 

			pBGData->vSize *= pBGData->fScale;
			pBGData->vPos = DJVector2(0.0f,(pBGData->vPos[1]+30.0f) /** fScale*/ + (float)g_nScreenHeight * (pBGData->vParalax[1]+1.0f) - pBGData->vSize[1]);
			m_levelBGData[nCity].pLevelBGs[nBGNumber] = pBGData;
			pBGDir = file.GetNextDir(pBGDir);
			nBGNumber++;
		}
		pCityDir = file.GetNextDir(pCityDir);
		nCity++;
	}
}

///

void LevelBackground::Term()
{
	DJTrace("%s()", __FUNCTION__);
	for(djuint i=0; i<=LEVEL_BG_MAX; i++)
	{
		if(m_pMaterial)
			DJ_SAFE_RELEASE(m_pMaterial[i]);
	}
}

///

void LevelBackground::OnPaint()
{
	DJTrace("%s()", __FUNCTION__);
	if(!m_bActive)
	return;

	DJMaterial *prevMaterial = NULL;
	for(djuint32 i = 0; i<LEVEL_BG_MAX; i++)
	{
		BGData* pBgData = m_levelBGData[m_nCity].pLevelBGs[i];
		if(pBgData == NULL)
			break;
		pTheRenderDevice->SetWorldTransform(DJMatrix::Identity());
		if(prevMaterial != m_pMaterial[i])
		{
			pTheRenderDevice->SetMaterial(m_pMaterial[i]);
			pTheRenderDevice->UpdateUniforms(m_pMaterial[i]->GetMaterialMode(0)->GetLocalUniformList());
			prevMaterial = m_pMaterial[i];
		}
		RectVertex vtx[6];

		DJColor color = theSpriteEngine.GetLayer(LAYER_BACKGROUND)->GetModColor();
		color = m_cCol * color * g_cModColor;
		DJVector2 vTexTopLeft;
		//size
		DJVector2 boxSize = pBgData->GetSize();
		//paralax
		DJVector2 vTexParalax = DJVector2(pBgData->vParalax[0], 0.0f);
		DJVector2 vPosParalax = DJVector2(0.0f, pBgData->vParalax[1]);

		//vTexTopLeft = g_pCamera->GetTransform(vTexParalax) * pBgData->GetUvTopLeft();
		vTexTopLeft = vTexTopLeft & DJVector2(1.0f/(float)boxSize[0], 1.0f);
		vTexTopLeft = DJVector2(
								djCeiling(vTexTopLeft[0]) - vTexTopLeft[0],
								vTexTopLeft[1] - djFloor(vTexTopLeft[1])
								);
		
		FillQuadVertices(
			vtx, 
			/*(g_pCamera->GetTransform(vPosParalax) **/ pBgData->GetPosition()/* + m_vTransition*/, 
			pBgData->GetSize(), 
			color, 
			0.0f, 
			DJMatrix::Identity(), 
			vTexTopLeft, 
			vTexTopLeft + pBgData->GetUvSize());
		
		pTheRenderDevice->RenderPrimitiveRaw(DJRenderDevice::ptTriangleList,
											 2, 
											 vtx, 
											 RectVertex_DescCount, 
											 RectVertex_Descs, 
											 sizeof(RectVertex));
		} 	
}

///

void LevelBackground::SetLevelBGUp(djint nCity)
{
	DJTrace("%s()", __FUNCTION__);
	m_nCity = nCity;
	djint32 i;
	for(i = 0; i < LEVEL_BG_MAX; i++)
	{
		if(m_levelBGData[m_nCity].pLevelBGs[i] == NULL)
			break;
		if(m_pMaterial[i] != NULL)
		{
			//DJInfo("Releasing background material");
			DJ_SAFE_RELEASE(m_pMaterial[i]);
			m_pMaterial[i] = NULL;
		}
		DJMaterial* pMat = (DJMaterial*)theResourceManager.GetResource((const char*)m_levelBGData[m_nCity].pLevelBGs[i]->sMaterialName, DJResource::TYPE_MATERIAL);
		DJAssert(pMat != NULL);
		//if(pMat == NULL)
		//{
		//	DJError("Failed to load bg material");
		//}
		if(m_pMaterial[i] == NULL)
			m_pMaterial[i] = pMat;
		//else if((pMat != NULL) && (m_pMaterial[i] != pMat))
		//{
		//	DJ_SAFE_RELEASE(m_pMaterial[i]);
		//	m_pMaterial[i] = pMat;
		//	m_pMaterial[i]->SetReplacement(pMat);
		//}
		//m_pMaterial[i] = (DJMaterial*)theResourceManager.GetResource((const char*)m_levelBGData[m_nCity].pLevelBGs[i]->sMaterialName, DJResource::TYPE_MATERIAL);
	}

	for(;i<LEVEL_BG_MAX; i++)
	{
		DJ_SAFE_RELEASE(m_pMaterial[i]);
		m_pMaterial[i] = NULL;
	}

	m_cCol = DJColor(1.0f,1.0f,1.0f,1.0f);
	m_bActive = DJFALSE;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////