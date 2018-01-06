#include "cbase.h"
#include "playerspeak.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CBasePlayer *GetLocalPlayer();

CON_COMMAND_F(playerspeak, "Emit specific sound.\n\tArguments: <sound name> <volume> <pitch>\n", FCVAR_GAMEDLL) {
	if (args.ArgC() != (1 + 3)) {
		Msg("Usage:\n	playerspeak <sound name> <volume> <pitch>\n");
		return;
	}

	const char *soundname = args.Arg(1);
	const float volume = static_cast<float>(Q_atof(args.Arg(2)));
	const int pitch = Q_atoi(args.Arg(3));

	UTIL_PlayerSpeak(soundname, volume, pitch);
}

void UTIL_PlayerSpeak(const char *soundname, float volume, int pitch, float soundtime) {
	CBasePlayer *player = GetLocalPlayer();

	if (!player) {
		Warning("%s: local player is not found.\n", __FUNCTION__);
		return;
	}

	CPASAttenuationFilter filter(player);
	filter.MakeReliable();

	float flSoundDuration;
	EmitSound_t ep;
	ep.m_nChannel = CHAN_STATIC;
	ep.m_pSoundName = soundname;
	ep.m_flVolume = volume;
	ep.m_flSoundTime = soundtime;
	ep.m_SoundLevel = SNDLVL_NORM;
	ep.m_nPitch = pitch;
	ep.m_pflSoundDuration = &flSoundDuration;

	CBaseEntity::EmitSound(filter, player->entindex(), ep);

	if (flSoundDuration < FLT_EPSILON) {
		Warning("%s: sound file '%s' not found.\n", __FUNCTION__, soundname);
	}
}
