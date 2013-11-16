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
#include "ball.h"
#include "player.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////
class MonkeyCivilians;
class LevelBackground;
class LevelScene
{
public:
	struct BallData
	{
		djint32		id;
		DJVector2	vpos;
		DJVector2	vSize;
		DJString	strSprite;
	};

	struct MonkeyCiviliansData
	{
		DJString	id;
		DJVector2	vpos;
		DJVector2	vsize;
		djint32		nState;
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
protected:
	// Scene name
	DJString m_name;

	// Scene root
	DJ2DNode* m_pSceneRoot;

	// Sprite 
	DJ2DSprite *m_pSprite;
	// Skeleton	

	DJVector2 m_vCameraPos;

	// Scene camera rotation (radians)
	float m_fCameraRotation;

	// Scene camera zoom factor
	float m_fCameraZoom;

	//Ball data
	BallData	m_BallData;

	// Player data
	PlayerData	m_PlayerData;

	// Monkey civilians data
	DJLinkedList<MonkeyCiviliansData>	m_listMonkeyCiviliansData;

	// Stick gold data
	StickGoldData		m_StickGoldData;

	// background image
	DJString m_sBackgroundConfig;

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

	// Get ball data
	BallData GetBallData() const {return m_BallData;}

	// Get player data
	PlayerData GetPlayerData() const {return m_PlayerData;}

	// Get list data of monkey civilians
	DJLinkedList<MonkeyCiviliansData>& GetMonkeyCiviliansData() {return m_listMonkeyCiviliansData;}

	// Get data for stickgold 
	StickGoldData GetStickGoldData()const {return m_StickGoldData;}
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

	// List ball
	DJLinkedList<Ball> m_Ball;

	// List monkey civinians
	DJLinkedList<MonkeyCivilians> m_listMonkeyCivians;

	// Current scene
	LevelScene*			m_pCurrentScene;

	//Level background
	LevelBackground*	m_pLevelBackground;

	// Current city
	djint32				m_nCurrentCity;

public:
	// Constructor
	Level();
	// Destructor
	~Level();

	// Initialize level
	djbool Init(const char* szLevelFile);
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