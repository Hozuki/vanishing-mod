#include "cbase.h"

#if DEBUG
#include "achievementmgr.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CC_Reset_Achievements() {
	if (!steamapicontext) {
		Warning("Cannot find Steam API context.");
		return;
	}

	ISteamUserStats *stats = steamapicontext->SteamUserStats();

	if (!stats) {
		Warning("Cannot access Steam user stats interface.");
		return;
	}

	const uint32 numAchievements = stats->GetNumAchievements();

	Log("Counting %u achievements.", numAchievements);

	for (uint32 i = 0; i < numAchievements; ++i) {
		const char *achName = stats->GetAchievementName(i);
		const bool b = stats->ClearAchievement(achName);

		if (b) {
			Log("Clearing achievement \"%s\" : successful", achName);
		} else {
			Warning("Clearing achievement \"%s\" : FAILED", achName);
		}
	}
}

static ConCommand reset_achievements("reset_achievements", CC_Reset_Achievements, "Resets all achievements.", FCVAR_SPONLY | FCVAR_DEVELOPMENTONLY);

#endif
