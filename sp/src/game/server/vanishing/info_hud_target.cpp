#include "cbase.h"
#include "entityinput.h"
#include "recipientfilter.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CBasePlayer *GetLocalPlayer();

// Shows/hides a target on HUD.
class CInfoHudTarget : public CLogicalEntity {

	DECLARE_CLASS(CInfoHudTarget, CLogicalEntity);

	public:

	CInfoHudTarget();

	virtual void Spawn();

	// Inputs
	void InputShowTarget(inputdata_t &inputdata);
	void InputHideTarget(inputdata_t &inputdata);

	virtual void Think();
	static void TransmitStatus(int iIndex, bool fEnabled, const Vector &vTarget);
	static void TransmitHeader(int iIndex, const char *pszHeader);
	
	DECLARE_DATADESC();

	protected:

	string_t m_szHeader;
	int32 m_iChannel;
	bool m_fTargetShown;

};

LINK_ENTITY_TO_CLASS(info_hud_target, CInfoHudTarget);

BEGIN_DATADESC(CInfoHudTarget)

DEFINE_KEYFIELD(m_szHeader, FIELD_STRING, "Header"),
DEFINE_KEYFIELD(m_iChannel, FIELD_INTEGER, "Channel"),

DEFINE_FIELD(m_fTargetShown, FIELD_BOOLEAN),

// Inputs
DEFINE_INPUTFUNC(FIELD_VOID, "ShowTarget", InputShowTarget),
DEFINE_INPUTFUNC(FIELD_VOID, "HideTarget", InputHideTarget),

END_DATADESC()

#define CINFO_HUD_TARGET_THINK_INTERVAL (0.1f)

CInfoHudTarget::CInfoHudTarget()
	: m_szHeader(NULL_STRING), m_iChannel(0), m_fTargetShown(false) {
}

void CInfoHudTarget::Spawn() {
	SetNextThink(gpGlobals->curtime);
}

void CInfoHudTarget::InputShowTarget(inputdata_t &inputdata) {
	m_fTargetShown = true;
	TransmitHeader(m_iChannel, STRING(m_szHeader));
	const Vector pos = GetAbsOrigin();
	TransmitStatus(m_iChannel, true, pos);
}

void CInfoHudTarget::InputHideTarget(inputdata_t &inputdata) {
	m_fTargetShown = false;
	TransmitStatus(m_iChannel, false, Vector(0, 0, 0));
}

void CInfoHudTarget::Think() {
	SetNextThink(gpGlobals->curtime + CINFO_HUD_TARGET_THINK_INTERVAL);

	if (m_fTargetShown) {
		const Vector pos = GetAbsOrigin();
		TransmitStatus(m_iChannel, m_fTargetShown, pos);
	}

	BaseClass::Think();
}

void CInfoHudTarget::TransmitStatus(int iIndex, bool fEnabled, const Vector &vTarget) {
	CRecipientFilter filter;
	filter.AddRecipient(GetLocalPlayer());

	struct TargetStateMessage {
		int32 index;
		int32 enabled;
		float x, y, z;
	} m = { 0 };

	m.index = iIndex;
	m.enabled = fEnabled;
	m.x = vTarget.x;
	m.y = vTarget.y;
	m.z = vTarget.z;

	UserMessageBegin(filter, "HudTargetStateUpdated");
	WRITE_LONG(m.index);
	WRITE_LONG(m.enabled);
	WRITE_FLOAT(m.x);
	WRITE_FLOAT(m.y);
	WRITE_FLOAT(m.z);
	MessageEnd();

	//#ifdef _DEBUG
	//	Log("%s: message sent.\n", __FUNCTION__);
	//#endif
}

void CInfoHudTarget::TransmitHeader(int iIndex, const char *pszHeader) {
	CRecipientFilter filter;
	filter.AddRecipient(GetLocalPlayer());

	struct TargetHeaderMessage {
		int32 index;
		const char *header;
	} m = { 0 };

	m.index = iIndex;
	m.header = pszHeader;

	UserMessageBegin(filter, "HudTargetHeaderUpdated");
	WRITE_LONG(m.index);
	WRITE_STRING(m.header);
	MessageEnd();

	//#ifdef _DEBUG
	//	Log("%s: message sent.\n", __FUNCTION__);
	//#endif
}
