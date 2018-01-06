//========= Copyright ?1996-2005, Valve Corporation, All rights reserved. ====
//	
//	Defines a logical entity which passes achievement related events to the gamerules system.

#include "cbase.h"
#include "gamerules.h"
#include "entityinput.h"
#include "entityoutput.h"
#include "playerspeak.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// Make player talk.
class CPointPlayerSpeak : public CLogicalEntity
{
public:
	DECLARE_CLASS( CPointPlayerSpeak, CLogicalEntity );

	CPointPlayerSpeak();

protected:

	// Inputs
	void InputSpeak( inputdata_t &inputdata );
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );
	
	bool			m_bDisabled;

	DECLARE_DATADESC();
};


LINK_ENTITY_TO_CLASS( point_playerspeak, CPointPlayerSpeak );


BEGIN_DATADESC( CPointPlayerSpeak )

	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_STRING, "Speak", InputSpeak ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),

END_DATADESC()



//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
CPointPlayerSpeak::CPointPlayerSpeak(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: Sends the achievement event to the achievement marking system.
//-----------------------------------------------------------------------------
void CPointPlayerSpeak::InputSpeak( inputdata_t &inputdata )
{
	if ( !m_bDisabled )
	{
		//Msg( "CPointPlayerSpeak::InputSpeak(%s)\n", inputdata.value.String() );
		UTIL_PlayerSpeak( inputdata.value.String() );
	}
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns off the relay, preventing it from firing outputs.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputDisable( inputdata_t &inputdata )
{ 
	m_bDisabled = true;
}


//------------------------------------------------------------------------------
// Purpose: Toggles the enabled/disabled state of the relay.
//------------------------------------------------------------------------------
void CPointPlayerSpeak::InputToggle( inputdata_t &inputdata )
{ 
	m_bDisabled = !m_bDisabled;
}