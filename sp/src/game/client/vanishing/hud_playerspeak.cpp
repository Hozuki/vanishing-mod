#include "cbase.h"
#include "hud_playerspeak.h"
#include "hud_macros.h"
#include "playerspeak.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT(CHudPlayerSpeak);
DECLARE_HUD_MESSAGE(CHudPlayerSpeak, HudSentenceRequested);

extern CBasePlayer *GetLocalPlayer();

CHudPlayerSpeak::CHudPlayerSpeak(const char *pszElementName)
	: CHudElement(pszElementName), BaseClass(NULL, "HudPlayerSpeak") {
}

void CHudPlayerSpeak::Init() {
	HOOK_HUD_MESSAGE(CHudPlayerSpeak, HudSentenceRequested);
}

void CHudPlayerSpeak::MsgFunc_HudSentenceRequested(bf_read &msg) const {
	bool overflow;
	char *soundname = msg.ReadAndAllocateString(&overflow);
	const float volume = msg.ReadFloat();
	const float pitch = msg.ReadLong();

	if (overflow) {
		Warning("%s: read string overflow!\n", __FUNCTION__);
	}

	UTIL_PlayerSpeak(soundname, volume, pitch);

	free(soundname);
}
