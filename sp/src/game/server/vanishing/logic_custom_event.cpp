#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Fires custom events.
// However, due to technical limitations, the event fired does not have any parameters.
class CLogicCustomEvent : public CLogicalEntity {

	DECLARE_CLASS(CLogicCustomEvent, CLogicalEntity);

	public:

	CLogicCustomEvent();

	// Inputs
	void InputFireEvent(inputdata_t &inputdata);

	DECLARE_DATADESC();

};

LINK_ENTITY_TO_CLASS(logic_custom_event, CLogicCustomEvent);

BEGIN_DATADESC(CLogicCustomEvent)

// Inputs
DEFINE_INPUTFUNC(FIELD_STRING, "FireEvent", InputFireEvent),

END_DATADESC()

CLogicCustomEvent::CLogicCustomEvent() = default;

void CLogicCustomEvent::InputFireEvent(inputdata_t &inputdata) {
	const char *name = inputdata.value.String();

	if (!name || !name[0]) {
		return;
	}

//#ifdef _DEBUG
//	Log("%s: creating event %s.\n", __FUNCTION__, name);
//#endif

	IGameEvent* event = gameeventmanager->CreateEvent(name);

	if (event) {
		gameeventmanager->FireEvent(event);
#ifdef _DEBUG
		Log("%s: event fired (%s).\n", __FUNCTION__, name);
#endif
	}
}
