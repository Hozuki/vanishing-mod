
#ifndef _SOUND_EMITTER_SYSTEM_H_
#define _SOUND_EMITTER_SYSTEM_H_

#include "cbase.h"
#include <ctype.h>
#include <KeyValues.h>
#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "igamesystem.h"
#include "soundchars.h"
#include "filesystem.h"
#include "tier0/vprof.h"
#include "checksum_crc.h"
#include "tier0/icommandline.h"

#ifndef CLIENT_DLL
#include "envmicrophone.h"
#include "sceneentity.h"
#else
#include <vgui_controls/Controls.h>
#include <vgui/IVgui.h>
#include "hud_closecaption.h"
#define CRecipientFilter C_RecipientFilter
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CSoundEmitterSystem : public CBaseGameSystem
{
public:
	virtual char const *Name();
#if !defined( CLIENT_DLL )
private:
	bool			m_bLogPrecache;
	FileHandle_t	m_hPrecacheLogFile;
	CUtlSymbolTable m_PrecachedScriptSounds;
public:
	CSoundEmitterSystem( char const *pszName ) :
		m_bLogPrecache( false ),
		m_hPrecacheLogFile( FILESYSTEM_INVALID_HANDLE )
	{
	}

	void LogPrecache( char const *soundname );
	void StartLog();
	void FinishLog();
#else
	CSoundEmitterSystem( char const *name );
#endif

	// IServerSystem stuff
	virtual bool Init();
	virtual void Shutdown();
	virtual void TraceEmitSound( char const *fmt, ... );

	// Precache all wave files referenced in wave or rndwave keys
	virtual void LevelInitPreEntity();
	virtual void LevelInitPostEntity();
	virtual void LevelShutdownPostEntity();
		
	void InternalPrecacheWaves( int soundIndex );
	void InternalPrefetchWaves( int soundIndex );
	HSOUNDSCRIPTHANDLE PrecacheScriptSound( const char *soundname );
	void PrefetchScriptSound( const char *soundname );

public:

	void EmitSoundByHandle( IRecipientFilter& filter, int entindex, const EmitSound_t & ep, HSOUNDSCRIPTHANDLE& handle );
	void EmitSound( IRecipientFilter& filter, int entindex, const EmitSound_t & ep );
	void EmitCloseCaption( IRecipientFilter& filter, int entindex, bool fromplayer, char const *token, CUtlVector< Vector >& originlist, float duration, bool warnifmissing /*= false*/ );
	void EmitCloseCaption( IRecipientFilter& filter, int entindex, const CSoundParameters & params, const EmitSound_t & ep );
	void EmitAmbientSound( int entindex, const Vector& origin, const char *soundname, float flVolume, int iFlags, int iPitch, float soundtime /*= 0.0f*/, float *duration /*=NULL*/ );
	void StopSound( int entindex, const char *soundname );
	void StopSound( int iEntIndex, int iChannel, const char *pSample );
	void CSoundEmitterSystem::StopSoundByHandle( int entindex, const char *soundname, HSOUNDSCRIPTHANDLE& handle );
	void EmitAmbientSound( int entindex, const Vector &origin, const char *pSample, float volume, soundlevel_t soundlevel, int flags, int pitch, float soundtime /*= 0.0f*/, float *duration /*=NULL*/ );

};

static CSoundEmitterSystem g_SoundEmitterSystem( "CSoundEmitterSystem" );

#endif