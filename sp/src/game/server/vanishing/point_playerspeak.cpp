//========= Copyright ?1996-2005, Valve Corporation, All rights reserved. ====
//	
//	Defines a logical entity which passes achievement related events to the gamerules system.

#include "cbase.h"
#include "entityinput.h"
#include "playerspeak.h"

#include "game.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

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
	const float volume = suitvolume.GetFloat();
	const int pitch = PITCH_NORM;

	UTIL_PlayerSpeak(soundname, volume, pitch);
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
