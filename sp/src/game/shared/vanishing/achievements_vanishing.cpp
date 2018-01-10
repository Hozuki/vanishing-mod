#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"

#ifdef GAME_DLL

#define ACHIEVEMENT_VAN_FINISH_CHAPTER_1 501
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_2 502
#define ACHIEVEMENT_VAN_FINISH_CHAPTER_3 503
#define ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRAPS 504
#define ACHIEVEMENT_VAN_MASTER_OF_PARABOLA 505
#define ACHIEVEMENT_VAN_BETTER_THAN_FREEMAN 506
#define ACHIEVEMENT_VAN_BON_APPETITE 507
#define ACHIEVEMENT_VAN_RIDE_THE_WAVES 508
#define ACHIEVEMENT_VAN_KILL_FINAL_ZOMBIE 509
#define ACHIEVEMENT_VAN_LOCATE_LAMBDA_CACHES 510

#define ACHIEVEMENT_VAN_SHARE_DARKNESS 511
#define ACHIEVEMENT_VAN_COMING_THROUGH 512
#define ACHIEVEMENT_VAN_WATERMELON 513
#define ACHIEVEMENT_VAN_FREE_ZOMBIES 514
#define ACHIEVEMENT_VAN_PACIFIST_AND_RESISTANCE 515

#define ACHIEVEMENT_VAN_CHOSE_REBELS 516
#define ACHIEVEMENT_VAN_CHOSE_COMBINES 517

#define ACHIEVEMENT_VAN_DJ_FINANCIAL_FREEDOM 551
#define ACHIEVEMENT_VAN_DJ_WORTH_IT 552
#define ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF 553
#define ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING 554
#define ACHIEVEMENT_VAN_DJ_STINGY 555
#define ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE 556
#define ACHIEVEMENT_VAN_DJ_DIE_HARD 557
#define ACHIEVEMENT_VAN_DJ_MISER 558

#define ACHIEVEMENT_VAN_DJ_GUARDIAN_ANGEL 559

#define SPAWNFLAG_ZOMBIE_SOUL_LOST (65536)
#define SPAWNFLAG_ZOMBIE_FINAL_ZOMBIE (131072)

class CAchievementVanishingMasterOfParabola : public CFailableAchievement {

	DECLARE_CLASS(CAchievementVanishingMasterOfParabola, CFailableAchievement);

	public:

	CAchievementVanishingMasterOfParabola() {
		m_iSpawnedCount = 0;
	}

	void Init() override {
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(1);
		SetMapNameFilter("v2_base_01");
	}

	void ListenForEvents() override {
		ListenForGameEvent("van_rollermine_spawned");
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

	protected:

	void FireGameEvent_Internal(IGameEvent *event) override {
		if (Q_strcmp(event->GetName(), "van_rollermine_spawned")) {
			return;
		}

		++m_iSpawnedCount;

#ifdef _DEBUG
		Log("%s: current spawn count = %d.\n", __FUNCTION__, m_iSpawnedCount);
#endif
	}

	private:

	int m_iSpawnedCount;

};

// TODO: not verified.
class CAchievementVanishingBetterThanFreeman : public CFailableAchievement {

	DECLARE_CLASS(CAchievementVanishingBetterThanFreeman, CFailableAchievement);

	public:

	void Init() override {
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(5);
	}

	void ListenForEvents() override {
		ListenForGameEvent("van_rpg_gunship_missed");
	}

	const char *GetActivationEventName() override {
		return "VAN_BETTER_THAN_FREEMAN_START";
	}

	const char *GetEvaluationEventName() override {
		return "VAN_BETTER_THAN_FREEMAN_END";
	}

#ifndef _DEBUG
	bool ShouldShowProgressNotification() override {
		return false;
	}
#endif

	protected:

	void FireGameEvent_Internal(IGameEvent *event) override {
		const char *pEventName = event->GetName();

		if (Q_strcmp(pEventName, "van_rpg_gunship_missed")) {
			return;
		}

		SetFailed();
	}

};

class CAchievementVanishingBonAppetite : public CBaseAchievement {
	DECLARE_CLASS(CAchievementVanishingBonAppetite, CBaseAchievement);
	public:
	CAchievementVanishingBonAppetite()
		: m_iKilled(0), m_pLastInflictor(nullptr) {
	}
	void Init() override {
		m_iKilled = 0;
		m_pLastInflictor = nullptr;
		SetGoal(1);
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetInflictorFilter("npc_grenade_frag");
	}
	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_PLAYER) {
			m_iKilled = 0;
			return;
		}
		if (pVictim->Classify() != CLASS_BARNACLE) {
			m_iKilled = 0;
			return;
		}
		if (pInflictor != m_pLastInflictor) {
			m_pLastInflictor = pInflictor;
			m_iKilled = 1;
			return;
		}
		++m_iKilled;
		if (m_iKilled == 3) {
			IncrementCount();
		}
	}
	private:
	int m_iKilled;
	CBaseEntity *m_pLastInflictor;
};

class CAchievementVanishingCountAgain : public CBaseAchievement {
	DECLARE_CLASS(CAchievementVanishingCountAgain, CBaseAchievement);
	public:
	void Init() override {
		SetGoal(1);
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetMapNameFilter("v5_choice_02");
	}
	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_PLAYER) {
			return;
		}

		if (pVictim->Classify() != CLASS_ZOMBIE) {
			return;
		}

		if (pVictim->GetSpawnFlags() & SPAWNFLAG_ZOMBIE_FINAL_ZOMBIE) {
			IncrementCount();
		}
	}
};

class CAchievementVanishingGeigerCounter : public CBaseAchievement {

	DECLARE_CLASS(CAchievementVanishingGeigerCounter, CBaseAchievement);

	public:

	void Init() override {
		const char *szComponents[] = {
			"VAN_GEIGER_COUNTER_LAMBDA_01", "VAN_GEIGER_COUNTER_LAMBDA_02"
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = szComponents;
		m_iNumComponents = ARRAYSIZE(szComponents);
		SetComponentPrefix("VAN_GEIGER_COUNTER_LAMBDA");
		SetGameDirFilter("Vanishing"); // mind the capital
		SetGoal(m_iNumComponents);
	}

	//#ifndef _DEBUG
	//	bool ShouldShowProgressNotification() override {
	//		return false;
	//	}
	//#endif

};

class CZombieFriendlyAchievement : public CFailableAchievement {
	DECLARE_CLASS(CZombieFriendlyAchievement, CFailableAchievement);
	public:
	void Init() override {
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
	}
	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_PLAYER) {
			return;
		}
		if (pVictim->Classify() != CLASS_ZOMBIE && pVictim->Classify() != CLASS_HEADCRAB) {
			return;
		}
		SetFailed();
	}
};

class CAchievementVanishingShareDarkness : public CZombieFriendlyAchievement {
	DECLARE_CLASS(CAchievementVanishingShareDarkness, CZombieFriendlyAchievement);
	public:
	const char *GetActivationEventName() override {
		return "VAN_SHARE_DARKNESS_START";
	}
	const char *GetEvaluationEventName() override {
		return "VAN_SHARE_DARKNESS_END";
	}
};

class CAchievementVanishingComingThrough : public CZombieFriendlyAchievement {
	DECLARE_CLASS(CAchievementVanishingShareDarkness, CZombieFriendlyAchievement);
	public:
	const char *GetActivationEventName() override {
		return "VAN_COMING_THROUGH_START";
	}
	const char *GetEvaluationEventName() override {
		return "VAN_COMING_THROUGH_END";
	}
};

class CAchievementVanishingShhhh : public CBaseAchievement {

	DECLARE_CLASS(CAchievementVanishingShareDarkness, CBaseAchievement);

	public:

	void Init() override {
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGoal(3);
	}

	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_PLAYER) {
			return;
		}
		if (pVictim->Classify() != CLASS_ZOMBIE) {
			return;
		}

		if (pVictim->GetSpawnFlags() & SPAWNFLAG_ZOMBIE_SOUL_LOST) {
			IncrementCount();
		}
	}

#ifndef _DEBUG
	bool ShouldShowProgressNotification() override {
		return false;
	}
#endif

};

class CAchievementVanishingMirrorsEdge : public CFailableAchievement {

	DECLARE_CLASS(CAchievementVanishingMirrorsEdge, CFailableAchievement);

	public:

	void Init() override {
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
	}

	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_PLAYER) {
			return;
		}

		const Class_T victimClass = pVictim->Classify();

		switch (victimClass) {
		case CLASS_ANTLION:
		case CLASS_BARNACLE:
		case CLASS_BULLSEYE:
		case CLASS_COMBINE:
		case CLASS_HEADCRAB:
		case CLASS_MANHACK:
		case CLASS_METROPOLICE:
		case CLASS_SCANNER:
		case CLASS_STALKER:
		case CLASS_PROTOSNIPER:
		case CLASS_COMBINE_HUNTER:
			SetFailed();
			break;
		case CLASS_ZOMBIE:
			// Don't conflict with the achievement...
			if (!(pVictim->GetSpawnFlags() & SPAWNFLAG_ZOMBIE_SOUL_LOST)) {
				SetFailed();
			}
			break;
		case CLASS_COMBINE_GUNSHIP:
		default:
			break;
		}
	}

#ifndef _DEBUG
	bool ShouldShowProgressNotification() override {
		return false;
	}
#endif

	const char *GetActivationEventName() override {
		return "VAN_MIRRORS_EDGE_START";
	}

	const char *GetEvaluationEventName() override {
		return "VAN_MIRRORS_EDGE_END";
	}

};

class CDoomJudgementPointSpentAchievement : public CFailableAchievement {
	DECLARE_CLASS(CDoomJudgementPointSpentAchievement, CFailableAchievement);
	public:
	void Init() override {
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_WITH_GAME);
		SetMapNameFilter("lq_city21_after");
	}
	void ListenForEvents() override {
		ListenForGameEvent("van_dj_point_spent");
	}
	protected:
	void FireGameEvent_Internal(IGameEvent *event) override {
		const char *pszEventName = event->GetName();
		if (Q_strcmp(pszEventName, "van_dj_point_spent")) {
			return;
		}
		SetFailed();
	}
};

class CAchievementVanishingStingy : public CDoomJudgementPointSpentAchievement {
	DECLARE_CLASS(CAchievementVanishingStingy, CDoomJudgementPointSpentAchievement);
	public:
	const char *GetActivationEventName() override {
		return "VAN_DJ_STINGY_START";
	}
	const char *GetEvaluationEventName() override {
		return "VAN_DJ_STINGY_END";
	}
};

class CAchievementVanishingMiser : public CDoomJudgementPointSpentAchievement {
	DECLARE_CLASS(CAchievementVanishingMiser, CDoomJudgementPointSpentAchievement);
	public:
	const char *GetActivationEventName() override {
		return "VAN_DJ_MISER_START";
	}
	const char *GetEvaluationEventName() override {
		return "VAN_DJ_MISER_END";
	}
};

class CAchievementVanishingDJGuardianAngel : public CFailableAchievement {
	DECLARE_CLASS(CDoomJudgementPointSpentAchievement, CFailableAchievement);
	public:
	void Init() override {
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_SAVE_WITH_GAME);
		SetMapNameFilter("lq_city21_after");
	}
	const char *GetActivationEventName() override {
		return "VAN_DJ_GUARDIAN_ANGEL_START";
	}
	const char *GetEvaluationEventName() override {
		return "VAN_DJ_GUARDIAN_ANGEL_END";
	}
	void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event) override {
		if (pAttacker->Classify() != CLASS_ANTLION) {
			return;
		}
		if (pVictim->Classify() != CLASS_COMBINE) {
			return;
		}
		SetFailed();
	}
};

#define DECLARE_ACHIEVEMENT_HIDDEN(classname, achievementID, achievementName, iPointValue) \
	DECLARE_ACHIEVEMENT_(classname, achievementID, achievementName, NULL, iPointValue, true)

#define DECLARE_DJ_ACC_ACH(id, name, points, acc, hideProgress) \
	class CVanishingAccumulativeAchievement##id : public CBaseAchievement { \
		DECLARE_CLASS(CVanishingAccumulativeAchievement##id, CBaseAchievement); \
		public: \
		CVanishingAccumulativeAchievement##id() = default; \
		void Init() override { \
			SetFlags(ACH_SAVE_WITH_GAME); \
			SetMapNameFilter("lq_city21_after"); \
			SetGoal(acc); \
		} \
		bool ShouldShowProgressNotification() override { \
			return !(hideProgress); \
		} \
	}; \
	DECLARE_ACHIEVEMENT(CVanishingAccumulativeAchievement##id, id, name, points);

// achievements which are won by a map event firing once
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_1, "VAN_FINISH_CHAPTER_1", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_2, "VAN_FINISH_CHAPTER_2", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_FINISH_CHAPTER_3, "VAN_FINISH_CHAPTER_3", 10);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_ESCAPE_TRIPWIRE_TRAPS, "VAN_PRECISE_OPERATION", 5);
DECLARE_ACHIEVEMENT(CAchievementVanishingMasterOfParabola, ACHIEVEMENT_VAN_MASTER_OF_PARABOLA, "VAN_MASTER_OF_PARABOLA", 10);
DECLARE_ACHIEVEMENT(CAchievementVanishingBetterThanFreeman, ACHIEVEMENT_VAN_BETTER_THAN_FREEMAN, "VAN_BETTER_THAN_FREEMAN", 10);
DECLARE_ACHIEVEMENT(CAchievementVanishingBonAppetite, ACHIEVEMENT_VAN_BON_APPETITE, "VAN_BON_APPETITE", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_RIDE_THE_WAVES, "VAN_RIDE_THE_WAVES", 5);
DECLARE_ACHIEVEMENT(CAchievementVanishingCountAgain, ACHIEVEMENT_VAN_KILL_FINAL_ZOMBIE, "VAN_COUNT_AGAIN", 5);
DECLARE_ACHIEVEMENT(CAchievementVanishingGeigerCounter, ACHIEVEMENT_VAN_LOCATE_LAMBDA_CACHES, "VAN_GEIGER_COUNTER", 10);

DECLARE_ACHIEVEMENT_HIDDEN(CAchievementVanishingShareDarkness, ACHIEVEMENT_VAN_SHARE_DARKNESS, "VAN_SHARE_DARKNESS", 10);
DECLARE_ACHIEVEMENT_HIDDEN(CAchievementVanishingComingThrough, ACHIEVEMENT_VAN_COMING_THROUGH, "VAN_COMING_THROUGH", 10);
// TODO
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_WATERMELON, "VAN_WATERMELON_YOU_SHALL_HAVE", 5);
DECLARE_ACHIEVEMENT_HIDDEN(CAchievementVanishingShhhh, ACHIEVEMENT_VAN_FREE_ZOMBIES, "VAN_SHHHH", 10);
DECLARE_ACHIEVEMENT_HIDDEN(CAchievementVanishingMirrorsEdge, ACHIEVEMENT_VAN_PACIFIST_AND_RESISTANCE, "VAN_MIRRORS_EDGE", 20);

DECLARE_DJ_ACC_ACH(ACHIEVEMENT_VAN_DJ_FINANCIAL_FREEDOM, "VAN_DJ_FINANCIAL_FREEDOM", 10, 200, false);
DECLARE_DJ_ACC_ACH(ACHIEVEMENT_VAN_DJ_WORTH_IT, "VAN_DJ_WORTH_IT", 10, 500, false);
DECLARE_DJ_ACC_ACH(ACHIEVEMENT_VAN_DJ_EXPENSES_PAY_OFF, "VAN_DJ_EXPENSES_PAY_OFF", 15, 1000, false);
DECLARE_DJ_ACC_ACH(ACHIEVEMENT_VAN_DJ_SEEMS_PROMISING, "VAN_DJ_SEEMS_PROMISING", 5, 5, true);
DECLARE_ACHIEVEMENT(CAchievementVanishingStingy, ACHIEVEMENT_VAN_DJ_STINGY, "VAN_DJ_STINGY", 10);
DECLARE_DJ_ACC_ACH(ACHIEVEMENT_VAN_DJ_PRIDE_OF_THE_COMBINE, "VAN_DJ_PRIDE_OF_THE_COMBINE", 10, 8, true);
DECLARE_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_VAN_DJ_DIE_HARD, "VAN_DJ_DIE_HARD", 15);
DECLARE_ACHIEVEMENT(CAchievementVanishingMiser, ACHIEVEMENT_VAN_DJ_MISER, "VAN_DJ_MISER", 25); // this is extra hard...

DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_CHOSE_REBELS, "VAN_CHOSE_REBELS", 5);
DECLARE_MAP_EVENT_ACHIEVEMENT_HIDDEN(ACHIEVEMENT_VAN_CHOSE_COMBINES, "VAN_CHOSE_COMBINES", 5);

DECLARE_ACHIEVEMENT(CAchievementVanishingDJGuardianAngel, ACHIEVEMENT_VAN_DJ_GUARDIAN_ANGEL, "VAN_DJ_GUARDIAN_ANGEL", 20);

#endif // GAME_DLL