#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"

#ifdef GAME_DLL

#define ACHIEVEMENT_VAN_FINISH_CHAPTER_1 501
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_2 502
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_3 503
#define ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRAPS 504
#define ACHIEVEMENT_VAN_MASTER_OF_PARABOLA 505
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
#define ACHIEVEMENT_VAN_DJ_WORTH_IT 552
#define ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF 553
#define ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING 554
#define ACHIEVEMENT_VAN_DJ_STINGY 555
#define ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE 556
#define ACHIEVEMENT_VAN_DJ_DIE_HARD 557
#define ACHIEVEMENT_VAN_DJ_MISER 558

class CAchievementVanishingMasterOfParabola : public CFailableAchievement {

	DECLARE_CLASS(CAchievementVanishingMasterOfParabola, CFailableAchievement);

	public:

	CAchievementVanishingMasterOfParabola() {
		m_iSpawnedCount = 0;
	}

	protected:

	void Init() override {
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		SetMapNameFilter("v2_base_01");
	}

	void ListenForEvents() override {
		ListenForGameEvent("van_rollermine_spawned");
	}

	void FireGameEvent_Internal(IGameEvent *event) override {
		if (Q_strcmp(event->GetName(), "van_rollermine_spawned")) {
			return;
		}

		++m_iSpawnedCount;

#ifdef _DEBUG
		Log("%s: current spawn count = %d.\n", __FUNCTION__, m_iSpawnedCount);
#endif
	}

	const char *GetActivationEventName() override {
		return "VAN_MASTER_OF_PARABOLA_START";
	}

	const char *GetEvaluationEventName() override {
		return "VAN_MASTER_OF_PARABOLA_END";
	}

	void PostRestoreSavedGame() override {
		// Reset the spawn count since we cannot (de)serialize during S/L.
		m_iSpawnedCount = 0;

		BaseClass::PostRestoreSavedGame();
	}

	void OnEvaluationEvent() override {
		// Notes:
		// In the map (.vmf) this is implemented by a counter. When the counter hits 5, the door to the next area opens.
		// So why is here "not equal" instead of "larger than"? Can there be tolerance?
		// It is designed to prevent save-load cheating, i.e. saving the game after each success, and loading after each failure.
		// So in PostRestoreSavedGame, the counter resets. If you had successful hits before but saved and reloaded the game, the counter will become 0.
		// And finally, the total count will be smaller than 5.
		// So although the description text says "no more than 5", we can only handle "exactly 5" due to the flaw of Source achievement S/L functions.
		// And sadly, no, no tolerance accepted.
		// This is also a failsafe for forgetting to put ModEvents.res into resources directory.
		if (m_iSpawnedCount != 5) {
			SetFailed();
		}

		BaseClass::OnEvaluationEvent();
	}

	private:

	int m_iSpawnedCount;

};

// achievements which are won by a map event firing once
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_1, "VAN_FINISH_CHAPTER_1", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_2, "VAN_FINISH_CHAPTER_2", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_3, "VAN_FINISH_CHAPTER_3", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRAPS, "VAN_PRECISE_OPERATION", 5);
DECLARE_ACHIEVEMENT(CAchievementVanishingMasterOfParabola, ACHIEVEMENT_VAN_MASTER_OF_PARABOLA, "VAN_MASTER_OF_PARABOLA", 10);
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
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_WORTH_IT, "VAN_DJ_WORTH_IT", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF, "VAN_DJ_EXPENSES_PAY_OFF", 15);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING, "VAN_DJ_SEEMS_PROMISING", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_STINGY, "VAN_DJ_STINGY", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE, "VAN_DJ_PRIDE_OF_THE_COMBINE", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_DIE_HARD, "VAN_DJ_DIE_HARD", 15);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_MISER, "VAN_DJ_MISER", 20);

#endif // GAME_DLL