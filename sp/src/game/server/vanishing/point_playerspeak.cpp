#include "cbase.h"
#include "entityinput.h"
#include "ai_speech.h"
#include "game.h"
#include "recipientfilter.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CBasePlayer *GetLocalPlayer();

// Make player talk.
class CPointPlayerSpeak : public CLogicalEntity {

	DECLARE_CLASS(CPointPlayerSpeak, CLogicalEntity);

	public:

	virtual void Spawn();

	protected:

	// Inputs
	void InputSpeak(inputdata_t &inputdata);
	void InputEnable(inputdata_t &inputdata);
	void InputDisable(inputdata_t &inputdata);
	void InputToggle(inputdata_t &inputdata);

	bool			m_bStartDisabled;
	bool			m_bDisabled;

	DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS(point_playerspeak, CPointPlayerSpeak);

BEGIN_DATADESC(CPointPlayerSpeak)

DEFINE_KEYFIELD(m_bStartDisabled, FIELD_BOOLEAN, "StartDisabled"),

// Inputs
DEFINE_INPUTFUNC(FIELD_STRING, "Speak", InputSpeak),
DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),

END_DATADESC()

void CPointPlayerSpeak::Spawn() {
	m_bDisabled = m_bStartDisabled;
}

//-----------------------------------------------------------------------------
// Purpose: Speak a certain sentence.
//-----------------------------------------------------------------------------
void CPointPlayerSpeak::InputSpeak(inputdata_t &inputdata) {
	if (m_bDisabled) {
		return;
	}
#ifdef _DEBUG
	Msg("%s(%s)\n", __FUNCTION__, inputdata.value.String());
#endif

	const char *soundname = inputdata.value.String();
	float volume = suitvolume.GetFloat();
	const int pitch = PITCH_NORM;

#define RANDOMIZE_PITCH 0

#if RANDOMIZE_PITCH
	if (random->RandomInt(0, 1)) {
		pitch = random->RandomInt(0, 6) + 98;
	}
#endif

	CBasePlayer *player = GetLocalPlayer();

	// If friendlies are talking, reduce the volume of the suit
	if (!g_AIFriendliesTalkSemaphore.IsAvailable(player)) {
		volume *= 0.3;
	}

	if (volume <= 0.05) {
		// Inaudible
		return;
	}

	CRecipientFilter filter;
	filter.AddRecipient(player);

	UserMessageBegin(filter, "HudSentenceRequested");
	WRITE_STRING(soundname);
	WRITE_FLOAT(volume);
	WRITE_LONG(pitch);
	MessageEnd();
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputEnable(inputdata_t &inputdata) {
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns off the relay, preventing it from firing outputs.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputDisable(inputdata_t &inputdata) {
	m_bDisabled = true;
}

//------------------------------------------------------------------------------
// Purpose: Toggles the enabled/disabled state of the relay.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputToggle(inputdata_t &inputdata) {
	m_bDisabled = !m_bDisabled;
}
