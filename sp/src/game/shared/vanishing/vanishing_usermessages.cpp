#include "cbase.h"
#include "usermessages.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void RegisterUserMessages_Vanishing() {
	// hud_target.h
	usermessages->Register("HudTargetStateUpdated", 20);
	usermessages->Register("HudTargetHeaderUpdated", -1);
	// hud_playerspeak.h
	usermessages->Register("HudSentenceRequested", -1);
}
