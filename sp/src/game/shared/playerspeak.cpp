
#include "cbase.h"
#include "playerspeak.h"

//class CSoundEmitterSystem;
extern CSoundEmitterSystem g_SoundEmitterSystem;

void UTIL_PlayerSpeak( const char *soundname, float soundtime )
{
	
#ifdef CLIENT_DLL
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
#else
	CBasePlayer *player = UTIL_GetLocalPlayer();
#endif
	

	if ( player )
	{

#ifdef CLIENT_DLL
		float flDuration = 0.0f;
		CPASAttenuationFilter filter(player, soundname);
		EmitSound_t params;
		params.m_pSoundName = soundname;
		params.m_flSoundTime = 0.0f;
		params.m_pflSoundDuration = &flDuration;
		params.m_bWarnOnDirectWaveReference = true;

		g_SoundEmitterSystem.EmitSound( filter, player->entindex(), params );
#else
		//char szCommand[MAX_SOUND_NAME+15] = {0};
		//Q_snprintf( szCommand, MAX_SOUND_NAME+15, "playerspeak %s", soundname );
		//Msg( "%s\n", szCommand );
		// The line below works, but it is still a trick.
		engine->ClientCommand( engine->PEntityOfEntIndex( player->entindex() ), "playerspeak %s", soundname );
#endif
		
	}
}