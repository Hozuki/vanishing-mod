//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================


#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"

#ifdef GAME_DLL

#define ACHIEVEMENT_VAN_FINISH_CHAPTER_1 501
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_2 502
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_3 503
#define ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRPAS 504
#define ACHIEVEMENT_VAN_SIX_ROLLERMINE_TAKE_DOWN 505
#define ACHIEVEMENT_VAN_ALL_GUNSHIPS_NO_MISTAKE 506
#define ACHIEVEMENT_VAN_THREE_BARNACLES 507
#define ACHIEVEMENT_VAN_BATTERY_PUZZLE 508
#define ACHIEVEMENT_VAN_KILL_FINAL_ZOMBIE 509
#define ACHIEVEMENT_VAN_LOCATE_LAMBDA_CACHES 510

#define ACHIEVEMENT_VAN_SHARE_DARKNESS 511
#define ACHIEVEMENT_VAN_COMING_THROUGH 512
#define ACHIEVEMENT_VAN_WATERMELON 513
#define ACHIEVEMENT_VAN_FREE_ZOMBIES 514
#define ACHIEVEMENT_VAN_PACIFIST_AND_RESISTANCE 515

#define ACHIEVEMENT_VAN_DJ_FINANCIAL_FREEDOM 551
#define ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF 552
#define ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING 553
#define ACHIEVEMENT_VAN_DJ_STINGY 554
#define ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE 555
#define ACHIEVEMENT_VAN_DJ_DIE_HARD 556
#define ACHIEVEMENT_VAN_DJ_MERCY_RULE 557
#define ACHIEVEMENT_VAN_DJ_MISER 558

// achievements which are won by a map event firing once
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_1, "VAN_FINISH_CHAPTER_1", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_2, "VAN_FINISH_CHAPTER_2", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_3, "VAN_FINISH_CHAPTER_3", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRPAS, "VAN_PRECISE_OPERATION", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_SIX_ROLLERMINE_TAKE_DOWN, "VAN_MASTER_OF_PARABOLA", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_ALL_GUNSHIPS_NO_MISTAKE, "VAN_BETTER_THAN_FREEMAN", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_THREE_BARNACLES, "VAN_BON_APPETITE", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_BATTERY_PUZZLE, "VAN_RIDE_THE_WAVES", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_KILL_FINAL_ZOMBIE, "VAN_COUNT_AGAIN", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_LOCATE_LAMBDA_CACHES, "VAN_GEIGER_COUNTER", 10);

DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_SHARE_DARKNESS, "VAN_SHARE_DARKNESS", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_COMING_THROUGH, "VAN_COMING_THROUGH", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_WATERMELON, "VAN_WATERMELON_YOU_SHALL_HAVE", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_FREE_ZOMBIES, "VAN_SHHHH", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_PACIFIST_AND_RESISTANCE, "VAN_MIRRORS_EDGE", 20);

DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_FINANCIAL_FREEDOM, "VAN_DJ_FINANCIAL_FREEDOM", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF, "VAN_DJ_EXPENSES_PAY_OFF", 15);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING, "VAN_DJ_SEEMS_PROMISING", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_STINGY, "VAN_DJ_STINGY", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE, "VAN_DJ_PRIDE_OF_THE_COMBINE", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_DIE_HARD, "VAN_DJ_DIE_HARD", 15);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_MERCY_RULE, "VAN_DJ_MERCY_RULE", 20);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_MISER, "VAN_DJ_MISER", 20);

#endif // GAME_DLL