/////////////////////////////////////////////////////////////////
//
// musiclayerhandler.h
// Music layer handler class
//
/////////////////////////////////////////////////////////////////

#ifndef _MUSICLAYER_HANDLER_H_
#define _MUSICLAYER_HANDLER_H_

/////////////////////////////////////////////////////////////////
// Engine Includes
/////////////////////////////////////////////////////////////////
// Game Includes
/////////////////////////////////////////////////////////////////
DJ_FILE_START();
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicMarker
{
public:
	// Name of marker
	DJString m_sName;
	// ID of marker
	djuint32 m_uID;
	// Sample offset to marker from start of music segment
	djint32 m_nSampleOffset;
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicSegment
{
protected:
	// Music resource
	DJSound*					m_pSound;
	// Music markers
	DJLinkedList<MusicMarker>	m_markers;

public:
	// Get music resource
	DJSound* GetSoundResource() const		{ return m_pSound; }
	// Set music resource
	void SetSoundResource(DJSound* pSound)	{ m_pSound = pSound; }
	// Get marker list
	DJLinkedList<MusicMarker>& GetMarkers()	{ return m_markers; }
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicSegmentItem
{
protected:
	// Music segment to play
	MusicSegment*	m_pSegment;
	// Number of times to loop remaining (-1 == loop infinite, 0 == no more loops, 1 == one more loop, ...)
	djint32			m_nLoopCount;
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicLayer
{
protected:
	// Queued music segment items
	DJLinkedList<MusicSegmentItem>	m_queue;
	// Current segment item playing
	MusicSegmentItem*				m_pCurrent;
	// Current sample offset (within the current loop)
	djint32							m_nOffset;
	// Layer ID
	djuint32						m_uLayerID;

public:
	// Constructor
	MusicLayer();
	// Destructor
	~MusicLayer();

	// Step specified number of samples
	void Step(djint32 nStep);
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicLayerMarkListener
{
public:
	// Callback whenever a marker is reached
	virtual void OnMusicMarker(djuint32 uMarkerID, djuint32 uLayerID, float fOffset) = 0;
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicLayerListener
{
public:
	// Callback whenever a music layer is empty and has no more music segments to play
	virtual void OnMusicLayerFinished(djuint32 uLayerID) = 0;
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class MusicLayerHandler
{
protected:
	// Array of music layers
	DJArray<MusicLayer*> m_layers;

	// Samplerate used (all segments need to use the same sample rate)
	djuint32	m_uSampleRate;
	// Bits per sample (all segments need use the same number of bits per sample)
	djuint32	m_uBitsPerSample;
	// Channels used
	djuint32	m_uChannels;

	// Music marker listener
	MusicLayerMarkListener*	m_pMarkListener;
	// Music layer listener
	MusicLayerListener*		m_pLayerListener;

public:
	// Constructor
	MusicLayerHandler();
	// Destructor
	~MusicLayerHandler();

	// Initialize music handler
	djbool Init(djint32 nLayers);
	// Terminate music handler
	void Term();

	// Get number of layers in music handler
	djint32 GetNumberOfLayers() const { return m_layers.GetCurrentIndex(); }

	// 1. Play directly, ignore existing queue
	// 2. Play directly after current segment loop is done
	// 3. Play after whole current queue

	// Play segment
	void PlaySegment(djuint32 uLayer, MusicSegment* pSegment, djint nLoops = 1, djbool bReplaceQueue = DJFALSE, djbool bStopExisting = DJFALSE);
	// Stop all layers
	//void StopAll();
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
class DJMusicHandler
{
protected:
	// Currently playing music
	DJString	m_sCurrMusic;
	// Is music looping?
	djbool		m_bCurrLoop;
	// Next music to play
	DJString	m_sNextMusic;
	// Is next music looping?
	djbool		m_bNextLoop;
	// Pending music
	DJString	m_sPendingMusic;
	// Is pending music looping?
	djbool		m_bPendingLoop;
	// Volume factor
	float		m_fVolumeFactor;
	// Global music volume
	float		m_fGlobalMusicVolume;
	// Fade factor (fade in/out)
	float		m_fFadeFactor;
	// Desired lowering volume factor (used to lower music during voice acting)
	float		m_fDesiredLowerFactor;
	// Current lowering volume factor (used to lower music during voice acting)
	float		m_fLowerFactor;

	// Critical section to protect when using threaded updates
	djhandle		m_hThreadCS;
	// Update thread handle (so music can fade/transition during loading etc.)
	djhandle		m_hUpdateThread;

public:
	// Constructor
	DJMusicHandler();
	// Destructor
	~DJMusicHandler();

	// Initialize
	djbool Init();
	// Terminate
	djbool Term();

	// Play music
	void PlayMusic(const char* szMusic, djbool bLoop = DJTRUE, float fFadeFactor = 1.0f);
	// Stop music
	void StopMusic(float fFadeFactor = 1.0f);

	// Is playing?
	djbool	IsPlaying(const char* szMusic);
	// Is music fading?
	djbool	IsMusicFading();

	// Get current volume factor
	float	GetCurrentVolumeFactor() const;
	// Get global music volume
	float	GetGlobalMusicVolume() const;
	// Set global music volume
	void	SetGlobalMusicVolume(float fVol);

	// Set desired lowered volume factor (useful for smoothly lowering volume a little while conversations are ongoing for example)
	void	SetDesiredLowerFactor(float fFactor)	{ m_fDesiredLowerFactor = fFactor; }
	// Get current lowered volume factor
	float	GetLowerFactor() const					{ return m_fLowerFactor; }
	// Set desired and current lowered volume factor (will not be smooth if you call this)
	void	SetLowerFactor(float fFactor)			{ m_fDesiredLowerFactor = m_fLowerFactor = fFactor; }

	// Update
	void OnUpdate();
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
// Global musichandler object
extern DJMusicHandler theMusicHandler;
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
DJ_FILE_END();
/////////////////////////////////////////////////////////////////
#endif // _MUSICLAYER_HANDLER_H_