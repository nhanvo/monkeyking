/////////////////////////////////////////////////////////////////
//
// level.h
// Level class
//
/////////////////////////////////////////////////////////////////

#ifndef _LEVEL_H_
#define _LEVEL_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
#include <dj2dutil.h>
/////////////////////////////////////////////////////////////////
// Game Includes
#include "gamedef.h"
#include "player.h"
#include "monkeycilivians.h"
#include "specter.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
//class RaysGhost;
class Centipede;
class Specter;
class StickGold;
class MonkeyCivilians;
class LevelBackground;
class Level;

class LevelScene
{
public:
	struct MonkeyCiviliansData
	{
		djint32		id;
		DJVector2	vpos;
		djint32		nBeatsTimeGroup;
	};

	struct PlayerData
	{
		DJVector2	vPos;	
	};

	struct StickGoldData
	{
		DJVector2 vPos;
		DJVector2 vSize;
		DJString  strAtlastFile;
		DJString  strAnimationName;
	};

	struct BeatsData
	{
		djint32		nID;
		djint32		uCountNumber;
		DJVector2	vTimesDistance;
	};

	struct RaysGhostData
	{
		djint32		nID;
		djint32		nType;
		DJVector2	vPosition;
		djint32		nBeatsTimeGroup;
	};

	struct SpecterData
	{
		djint32		nType;
		DJVector2	vPosition;
	};

	enum 
	{
		// Level 01
		SCENE_01 = 0,
		SCENE_02,
		SCENE_03,
		SCENE_COUNT
	};
protected:
	// Scene ID
	DJString	m_sID;
		
	// Scene name
	DJString	m_name;

	// Scene root
	DJ2DNode*	m_pSceneRoot;

	// Sprite 
	DJ2DSprite *m_pSprite;

	// Camera position	
	DJVector2	m_vCameraPos;

	// Scene camera rotation (radians)
	float		m_fCameraRotation;

	// Scene camera zoom factor
	float		m_fCameraZoom;

	// End music value
	float		m_fEndMusic;

	// Player data
	PlayerData	m_PlayerData;
	djbool						m_bPlayer;

	// Beats data 
	djbool						m_bBeatsTime;
	DJLinkedList<BeatsData>		m_listBeatsData;

	// background image
	DJString m_sBackgroundConfig;

	// Rays ghost data
	djbool							m_bRayGhost;
	DJLinkedList<RaysGhostData>		m_listRayGhostData;

	// Stick Gold
	djbool			m_bStickGold;
	StickGoldData	m_StickGoldData;

	// Monkey civilians data
	djbool								m_bMonkeyCilivians;
	DJLinkedList<MonkeyCiviliansData>	m_listMonkeyCiviliansData;

	// Specter 
	djbool								m_bSpecter;
	DJLinkedList<SpecterData>			m_listSpecterData;

public:
	// Constructor
	LevelScene();
	// Destructor
	~LevelScene();

	// Initialize scene
	djbool Init(DJTagFile& file, DJTagDir* pDir);
	
	// Terminate scene
	void Term();

	// Get camera position
	DJVector2 GetCameraPosition() const {return m_vCameraPos;}

	// Get scene root
	DJ2DNode* GetSceneRoot() {return m_pSceneRoot;}

	//Get background image
	DJString GetBgFileConfig() const {return m_sBackgroundConfig;}

	//Get sprite
	DJ2DSprite* GetSprite() const {return m_pSprite;}

	// Get player data
	PlayerData GetPlayerData() const {return m_PlayerData;}

	// Get list data of monkey civilians
	DJLinkedList<MonkeyCiviliansData>& GetMonkeyCiviliansData() {return m_listMonkeyCiviliansData;}
	djbool	IsMonKeyCilivians() const {return m_bMonkeyCilivians;}

	// Get data for stickgold 
	StickGoldData GetStickGoldData()const {return m_StickGoldData;}
	djbool	IsStickGold()	const {return m_bStickGold;}

	// Get data for beats time
	DJLinkedList<BeatsData>& GetBeatsData() {return m_listBeatsData;}
	djbool  IsBeatsTime()	const {return m_bBeatsTime;}

	// Get data for rays ghost
	DJLinkedList<RaysGhostData>& GetRaysGhostData() {return m_listRayGhostData;}
	djbool	IsRayGhost()	const {return m_bRayGhost;}

	// Get specter
	DJLinkedList<SpecterData>& GetSpecterData() {return m_listSpecterData;}
	djbool IsSpecter() const {return m_bSpecter;}

	// Get ID
	DJString GetID() const {return m_sID;}

	// Get end music value
	djfloat GetEndMusic() const {return m_fEndMusic;}
};
/////////////////////////////////////////////////////////////////  

/////////////////////////////////////////////////////////////////
class LevelState
{
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class Level
{ 
public:
	enum
	{
		LEVEL_01,
		LEVEL_02,
		LEVEL_03,

		LEVEL_COUNT
	};
protected:
	// Level ID
	DJString						m_sID;
	
	// Level name 
	DJString						m_sLevelName;

	// music path
	DJString						m_sSceneMusic;

	// List of all scenes in level
	DJLinkedList<LevelScene>		m_scenes;

	// Stick gold 
	StickGold*						m_pStickGold;

	// List monkey civinians
	DJLinkedList<MonkeyCivilians>	m_listMonkeyCivians;

	// List Specter
	DJLinkedList<Specter>			m_listSpecter;

	// List beatstime
	DJLinkedList<BeatsTime>			m_listBeatsTime;

	// Current scene
	LevelScene*						m_pCurrentScene;

	//Level background
	LevelBackground*				m_pLevelBackground;

	// Current city
	djint32							m_nCurrentCity;

	// Finish level
	djbool							m_bFinishScene;

public:
	// Constructor
	Level();
	// Destructor
	~Level();

	// Initialize level
	djbool Init(const char* szLevelFile, djint32 nSceneID);

	// Reset level
	void Reset();

	// Terminate
	void Term();

	// Update level
	void Update(float fDeltaTime);
	void PrePaint();

	// Get stick gold
	StickGold* GetStickGold() const {return m_pStickGold;}

	// Get current scene
	LevelScene* GetCurrentScene() const {return m_pCurrentScene;}
	void SetCurrentScene(LevelScene* pScene) {m_pCurrentScene = pScene;}

	// Set finish scene
	void SetFinishScene(djbool bFinish)	{if(!m_bFinishScene) m_bFinishScene = bFinish;}

	void InitSceneMusic();
};
///////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////// 
// Begin level manager
class LevelManager
{
protected:
	Level*			m_pCurrentLevel;
	djint32			m_nSceneID;

	// Level loaded
	djbool			m_bLevelLoaded;
public:
	LevelManager();
	~LevelManager();

	djbool Init();
	djbool Update(float fDeltaTime);
	void   Term();

	djbool LoadLevel(const char* szLevelFile);
	djbool UnloadLevel();
	Level*	GetCurrentLevel(){return m_pCurrentLevel;}

	// Get and Set scene id
	void SetSceneID(djint32 nSceneID)	{m_nSceneID = nSceneID;}
	djint32 GetSceneID() const {return m_nSceneID;}

	// Level is loaded
	djbool	IsLevelLoaded()		{return m_bLevelLoaded;}

};
// End level manager
///////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////// 
// Begin level background

/// define maximum level background in game
#define LEVEL_BG_MAX 5
class LevelBackground
{
protected:
	struct BGData
	{
		DJQuaternion	quUV;
		DJString		sMaterialName;
		DJVector2		vPos;
		DJVector2		vSize;
		DJVector2		vParalax;
		djfloat			fScale;

		DJVector2 GetUvTopLeft() const { return DJVector2(quUV.x(), quUV.y()); }
		DJVector2 GetUvSize() const { return DJVector2(quUV.z(), quUV.scalar()); }
		DJVector2 GetPosition() const
		{
			return vPos;
		}
		DJVector2 GetSize() const
		{
			return vSize;
		}
	};

	struct LevelBGData
	{
		BGData* pLevelBGs[LEVEL_BG_MAX];
		LevelBGData()
		{
			for(djuint32 i=0; i<LEVEL_BG_MAX; i++)
			{
				pLevelBGs[i] = NULL;
			}
		}
		~LevelBGData()
		{
			for(djuint32 i=0; i<LEVEL_BG_MAX; i++)
			{
				if(pLevelBGs[i] != NULL)
				{
					DJ_SAFE_DELETE(pLevelBGs[i]);
				}
			}
		}
	};

protected:
	LevelBGData m_levelBGData[CITY_COUNT];
	DJMaterial*	m_pMaterial[LEVEL_BG_MAX];
	DJColor			m_cCol;
	djuint			m_nCity;
	djbool			m_bActive;
public:
	//
	LevelBackground();
	~LevelBackground();

	// Init level background
	void Init(const char* strBGFileConfig);
	//
	void Term();
	//
	void OnPaint();
		
	void SetLevelBGUp(djint nCity);
	//
	void SetLevelBGDown()	{m_bActive = DJFALSE;}
	//
	void SetColor(const DJColor &col);
	//
	DJColor GetColor()	{ return m_cCol; }
	void Active(djbool bActive)	{m_bActive = bActive;}
};

// End level background
///////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif // _LEVEL_H_