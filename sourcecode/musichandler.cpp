//////////////////////////////////////////////////////////////////
//
// musichandler.cpp
// Music handler class
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
#include <djosthread.h>

/////////////////////////////////////////////////////////////////
#include "musichandler.h"

/////////////////////////////////////////////////////////////////
DJ_FILE_START();
///////////////////////////////////////////////////////////////// 
MusicLayerHandler theMusicLayerHandler;

/////////////////////////////////////////////////////////////////
MusicLayerHandler::MusicLayerHandler()
{
	m_uSampleRate		= 44100;
	m_uBitsPerSample	= 16;
	m_uChannels			= 2;
	m_pMarkListener		= NULL;
	m_pLayerListener	= NULL;
}

///

MusicLayerHandler::~MusicLayerHandler()
{
}

///

djbool MusicLayerHandler::Init(djint32 nLayers)
{ 
	return DJTRUE;
}

///

void MusicLayerHandler::Term()
{
}

///

void MusicLayerHandler::PlaySegment(djuint32 uLayer, MusicSegment* pSegment, djint nLoops, djbool bReplaceQueue, djbool bStopExisting)
{
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Global musichandler object
DJMusicHandler theMusicHandler;
/////////////////////////////////////////////////////////////////

volatile static djbool l_bMusicThreadRun = DJFALSE;
volatile static djbool l_bMusicThreadTerm = DJFALSE;

/////////////////////////////////////////////////////////////////
DJMusicHandler::DJMusicHandler()
{
	m_fVolumeFactor			= 1.0f;
	m_fGlobalMusicVolume	= 1.0f;
	m_bPendingLoop			= DJFALSE;
	m_bNextLoop				= DJFALSE;
	m_bCurrLoop				= DJFALSE;
	m_fFadeFactor			= 1.0f;
	m_hThreadCS				= 0;
	m_hUpdateThread			= DJ_INVALID_HANDLE;
	l_bMusicThreadRun		= DJFALSE;
	l_bMusicThreadTerm		= DJFALSE;
	m_fDesiredLowerFactor	= 1.0f;
	m_fLowerFactor			= 1.0f;
}

///

DJMusicHandler::~DJMusicHandler()
{
}

///

static void* MusicUpdateThread(void* pParam)
{
	while (l_bMusicThreadRun)
	{
		theMusicHandler.OnUpdate();

		if (theDeviceManager.IsMinimized() || theDeviceManager.IsHidden())
			djThreadSleep(250);
		else
			djThreadSleep(16);
	}
	l_bMusicThreadRun = DJFALSE;
	l_bMusicThreadTerm = DJTRUE;
	return NULL;
}


djbool DJMusicHandler::Init()
{
	if (l_bMusicThreadRun)
		return DJTRUE;

	m_fDesiredLowerFactor	= 1.0f;
	m_fLowerFactor			= 1.0f;
	m_fVolumeFactor			= 1.0f;
	m_fGlobalMusicVolume	= 1.0f;
	m_bPendingLoop			= DJFALSE;
	m_bNextLoop				= DJFALSE;
	m_bCurrLoop				= DJFALSE;
	m_sPendingMusic			= "";
	m_sNextMusic			= "";
	m_sCurrMusic			= "";
	m_fFadeFactor			= 1.0f;
	djCreateCriticalSection(&m_hThreadCS);
	l_bMusicThreadRun			= DJTRUE;
	m_hUpdateThread = djThreadCreate( 128*1024, MusicUpdateThread, this, NULL);
	return DJTRUE;
}

///

djbool DJMusicHandler::Term()
{
	if (!l_bMusicThreadRun)
		return DJTRUE;

	l_bMusicThreadTerm = DJFALSE;
	l_bMusicThreadRun = DJFALSE;
	int nCount = 100;
	while (!l_bMusicThreadTerm)
	{
		djThreadSleep(20);
		if (--nCount < 0)
		{
			DJWarning("Failed to terminate musich handler update thread!");
			break;
		}
	}
	m_hUpdateThread = DJ_INVALID_HANDLE;
	pTheSoundDevice->StopMusic();
	pTheSoundDevice->StopAllSounds();
	m_sPendingMusic	= "";
	m_sNextMusic	= "";
	m_sCurrMusic	= "";
	djDestroyCriticalSection(m_hThreadCS);
	m_hThreadCS = 0;
	return DJTRUE;
}

///

djbool DJMusicHandler::IsPlaying(const char* szMusic)
{ 
	return m_sCurrMusic == szMusic; 
}

///

djbool DJMusicHandler::IsMusicFading()
{
	return m_sPendingMusic.GetLength()>0 || m_sNextMusic.GetLength()>0; 
}

///

float DJMusicHandler::GetCurrentVolumeFactor() const
{ 
	return m_fVolumeFactor; 
}

///

float DJMusicHandler::GetGlobalMusicVolume() const
{ 
	return m_fGlobalMusicVolume; 
}

///

void DJMusicHandler::SetGlobalMusicVolume(float fVol)
{ 
	fVol = djClampFloat(fVol, 0.0f, 1.0f); 
	m_fGlobalMusicVolume = fVol; 
}

///

void DJMusicHandler::PlayMusic(const char* szMusic, djbool bLoop, float fFadeFactor)
{
#ifndef NO_MUSIC
	if (szMusic == NULL || *szMusic == 0)
		return;
	djEnterCriticalSection(m_hThreadCS);
	m_sPendingMusic	= szMusic;
	m_bPendingLoop	= bLoop;
	m_fFadeFactor	= fFadeFactor;
	djLeaveCriticalSection(m_hThreadCS);
#endif // NO_MUSIC
}

///

void DJMusicHandler::StopMusic(float fFadeFactor)
{
	djEnterCriticalSection(m_hThreadCS);
	m_bPendingLoop	= DJFALSE;
	m_bNextLoop		= DJFALSE;
	m_bCurrLoop		= DJFALSE;
	m_sPendingMusic	= "";
	m_sNextMusic	= "";
	m_sCurrMusic	= "";
	m_fFadeFactor	= fFadeFactor;
	djLeaveCriticalSection(m_hThreadCS);
}

///

void DJMusicHandler::OnUpdate()
{
	djEnterCriticalSection(m_hThreadCS);
	djbool bFadeOut = DJFALSE;

	if (m_fLowerFactor < m_fDesiredLowerFactor)
		m_fLowerFactor += djStepToDesiredValue(m_fLowerFactor, m_fDesiredLowerFactor, pTheApp->GetDeltaFrameTime());
	else
		m_fLowerFactor += djStepToDesiredValue(m_fLowerFactor, m_fDesiredLowerFactor, pTheApp->GetDeltaFrameTime())*0.9f;

	// Start fading to new pending music?
	if (m_sPendingMusic.GetLength()!=0 && m_sNextMusic.GetLength()==0)
	{
		m_sNextMusic	= m_sPendingMusic;
		m_bNextLoop		= m_bPendingLoop;
		m_sPendingMusic	= "";
		m_bPendingLoop	= DJFALSE;
		bFadeOut		= DJTRUE;
	}
	// Do we have new music set?
	if (m_sNextMusic.GetLength() && !theDeviceManager.IsMinimized() && !theDeviceManager.IsHidden() && pTheSoundDevice->IsActive())
	{
		if (m_fVolumeFactor <= DJ_EPSf)
		{ 
			// Time to stop current music and start the new one
			pTheSoundDevice->StopMusic();
			//if (!g_bIntroMode)
				pTheSoundDevice->PlayMusic(m_sNextMusic);
			m_fVolumeFactor = 1.0f;
			m_sCurrMusic	= m_sNextMusic;
			m_bCurrLoop		= m_bNextLoop;
			m_sNextMusic	= "";
			m_bNextLoop		= DJFALSE;
			bFadeOut		= DJFALSE;
		}
		else
		{
			// Still waiting for fadeout to complete
			bFadeOut	= DJTRUE;
		}
	}
	// If the current music is an empty string we want to fade out completely
	if (!m_sCurrMusic.GetLength())
	{
		bFadeOut = DJTRUE;
		if (m_fVolumeFactor <= DJ_EPSf)
		{
			pTheSoundDevice->StopMusic();
		}
	}
	// Fade in or out depending on state
	if (bFadeOut)
	{
		m_fVolumeFactor += djStepToDesiredValue(m_fVolumeFactor, 0.0f, pTheApp->GetDeltaFrameTime() * m_fFadeFactor);
	}
	else
	{
		m_fVolumeFactor += djStepToDesiredValue(m_fVolumeFactor, 1.0f, pTheApp->GetDeltaFrameTime() * m_fFadeFactor);
	}
	// Set the music volume using the global music volume and the fade factor
	pTheSoundDevice->SetMusicVolume(m_fLowerFactor * m_fVolumeFactor * m_fGlobalMusicVolume);

	if (!pTheSoundDevice->IsMusicPlaying() && !pTheSoundDevice->IsMusicPaused() && pTheSoundDevice->IsActive())
	{
		// Loop time?
		if (!m_sNextMusic.GetLength() && !m_sPendingMusic.GetLength() && m_sCurrMusic.GetLength())
		{
			if (m_bCurrLoop)
			{
				if (/*!g_bIntroMode &&*/ !pTheSoundDevice->PlayMusic(m_sCurrMusic))
				{
					m_sCurrMusic	= "";
					m_bCurrLoop		= DJFALSE;
				}
				else
				{
					pTheSoundDevice->SetMusicVolume(m_fLowerFactor * m_fVolumeFactor * m_fGlobalMusicVolume);
				}
			}
		}
		else
		{
			// If a song has ended and we want to transition, we might as well set the fade to 0.0 so the next or pending song will start quicker
			m_fVolumeFactor = 0.0f;
		}
	}
	djLeaveCriticalSection(m_hThreadCS);
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////