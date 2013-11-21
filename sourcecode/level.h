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
class RaysGhost;
class MonkeyCivilians;
class LevelBackground;
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
		DJString	sID;
		DJVector2	vPos;
		DJVector2   vSize;
		DJString	strAnimFile;		
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
protected:
	// Scene ID
	DJString	m_nID;
	// Scene name
	DJString	m_name;

	// Scene root
	DJ2DNode*	m_pSceneRoot;

	// Sprite 
	DJ2DSprite *m_pSprite;
	// Skeleton	

	DJVector2 m_vCameraPos;

	// Scene camera rotation (radians)
	float m_fCameraRotation;

	// Scene camera zoom factor
	float m_fCameraZoom;

	// Player data
	PlayerData	m_PlayerData;

	// Monkey civilians data
	DJLinkedList<MonkeyCiviliansData>	m_listMonkeyCiviliansData;

	// Stick gold data
	StickGoldData		m_StickGoldData;

	// Beats data 
	DJLinkedList<BeatsData>		m_listBeatsData;

	// background image
	DJString m_sBackgroundConfig;

	// Rays ghost data
	DJLinkedList<RaysGhostData>		m_listRayGhostData;

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

	// Get data for stickgold 
	StickGoldData GetStickGoldData()const {return m_StickGoldData;}

	// Get data for beats time
	DJLinkedList<BeatsData>& GetBeatsData() {return m_listBeatsData;}

	// Get data for rays ghost
	DJLinkedList<RaysGhostData>& GetRaysGhostData() {return m_listRayGhostData;}
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
protected:
	// List of all scenes in level
	DJLinkedList<LevelScene> m_scenes;

	// List monkey civinians
	DJLinkedList<MonkeyCivilians> m_listMonkeyCivians;

	// List raysghost
	DJLinkedList<RaysGhost>		m_listRayGhost;

	// List beatstime
	DJLinkedList<BeatsTime> m_listBeatsTime;

	// Current scene
	LevelScene*			m_pCurrentScene;

	//Level background
	LevelBackground*	m_pLevelBackground;

	// Current city
	djint32				m_nCurrentCity;

	// Finish level
	djbool				m_bFinishLevel;

public:
	// Constructor
	Level();
	// Destructor
	~Level();

	// Initialize level
	djbool Init(const char* szLevelFile);

	// Reset level
	void Reset();

	// Terminate
	void Term();

	// Update level
	void Update(float fDeltaTime);
	void PrePaint();
};
///////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////// 
// Begin level manager
class LevelManager
{
protected:
	Level	*m_pCurrentLevel;
public:
	LevelManager();
	~LevelManager();

	djbool Init();
	djbool Update(float fDeltaTime);
	void   Term();

	djbool LoadLevel(const char* szLevelFile);
	djbool UnloadLevel();
	Level*	GetCurrentLevel(){return m_pCurrentLevel;}

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