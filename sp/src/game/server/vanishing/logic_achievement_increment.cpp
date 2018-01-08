#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CAchievementHelper {
	public:
	static void Increment(CBaseAchievement *pAchievement, int iCount) {
		if (pAchievement) {
			pAchievement->IncrementCount(iCount);
		}
	}
};

class CLogicAchievementIncrement : public CLogicalEntity {

	DECLARE_CLASS(CLogicAchievementIncrement, CLogicalEntity);

	public:

	CLogicAchievementIncrement();

	// Inputs
	void InputIncrement(inputdata_t &inputdata);
	void InputIncrementBy(inputdata_t &inputdata);

	DECLARE_DATADESC();

	private:

	static void IncrementAchievement(const char *pszAchievementName, int iCount);

	string_t m_szAchievementName;
	int m_iCount;

};

LINK_ENTITY_TO_CLASS(logic_achievement_increment, CLogicAchievementIncrement);

BEGIN_DATADESC(CLogicAchievementIncrement)

DEFINE_KEYFIELD(m_szAchievementName, FIELD_STRING, "Achievement"),
DEFINE_KEYFIELD(m_iCount, FIELD_INTEGER, "Count"),

// Inputs
DEFINE_INPUTFUNC(FIELD_VOID, "Increment", InputIncrement),
DEFINE_INPUTFUNC(FIELD_VOID, "IncrementBy", InputIncrementBy),

END_DATADESC()

CLogicAchievementIncrement::CLogicAchievementIncrement()
	: m_szAchievementName(NULL_STRING), m_iCount(0) {
}

void CLogicAchievementIncrement::InputIncrement(inputdata_t &inputdata) {
	const char *pszAchievementName = STRING(m_szAchievementName);
	IncrementAchievement(pszAchievementName, m_iCount);
}

void CLogicAchievementIncrement::InputIncrementBy(inputdata_t &inputdata) {
	const char *pszAchievementName = STRING(m_szAchievementName);
	const int count = inputdata.value.Int();
	IncrementAchievement(pszAchievementName, count);
}


void CLogicAchievementIncrement::IncrementAchievement(const char *pszAchievementName, int iCount) {
	if (!pszAchievementName || !pszAchievementName[0]) {
		return;
	}

	if (iCount <= 0) {
		return;
	}

	CAchievementMgr *mgr = dynamic_cast<CAchievementMgr *>(engine->GetAchievementMgr());
	const bool bAchDbg = !!cc_achievement_debug.GetInt();

	if (!mgr) {
		if (bAchDbg) {
			Warning("%s: failed to get achievement manager\n", __FUNCTION__);
		}
		return;
	}

	CBaseAchievement *achievement = mgr->GetAchievementByName(pszAchievementName);

	if (!achievement) {
		if (bAchDbg) {
			Warning("%s: achievement '%s' is not found\n", __FUNCTION__, pszAchievementName);
		}
		return;
	}

	CAchievementHelper::Increment(achievement, iCount);
}

