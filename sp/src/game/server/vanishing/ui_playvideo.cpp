#include "cbase.h"
#include "entityinput.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CBasePlayer *GetLocalPlayer();

class CUIPlayVideo : public CLogicalEntity, public IGameEventListener2 {

	DECLARE_CLASS(CUIPlayVideo, CLogicalEntity);

	public:

	CUIPlayVideo();

	~CUIPlayVideo() = default;

	void Spawn() override;

	protected:

	void InputPlay(inputdata_t &inputdata);

	void FireGameEvent(IGameEvent *event) override;

	COutputEvent m_OnPlaybackFinished;

	string_t m_szVideoFileName;

	DECLARE_DATADESC();

	private:

	bool m_bIsLegal;
	static bool _isCreated;

};

bool CUIPlayVideo::_isCreated = false;

LINK_ENTITY_TO_CLASS(ui_playvideo, CUIPlayVideo);

BEGIN_DATADESC(CUIPlayVideo)

DEFINE_KEYFIELD(m_szVideoFileName, FIELD_STRING, "Video"),

// Inputs
DEFINE_INPUTFUNC(FIELD_VOID, "Play", InputPlay),

// Outputs
DEFINE_OUTPUT(m_OnPlaybackFinished, "OnPlaybackFinished"),

END_DATADESC()

CUIPlayVideo::CUIPlayVideo()
	: m_szVideoFileName(NULL_STRING), m_bIsLegal(false) {
}

void CUIPlayVideo::Spawn() {
	if (_isCreated) {
		Warning("%s: An instance exists, so the new instance will not have any effect!\n");
		return;
	}

	gameeventmanager->AddListener(this, "van_vidcmd_complete", true);

	_isCreated = true;
}

void CUIPlayVideo::InputPlay(inputdata_t &) {
	const auto *pszVideoFileName = STRING(m_szVideoFileName);

	if (!pszVideoFileName || !pszVideoFileName[0]) {
		return;
	}

#ifdef _DEBUG
	Log("%s: trying to play file '%s'...\n", __FUNCTION__, pszVideoFileName);
#endif

	CSingleUserRecipientFilter filter(GetLocalPlayer());
	UserMessageBegin(filter, "VideoCommand");
	WRITE_STRING(pszVideoFileName);
	MessageEnd();
}

void CUIPlayVideo::FireGameEvent(IGameEvent *event) {
	const auto *pEventName = event->GetName();

	if (Q_strcmp(pEventName, "van_vidcmd_complete")) {
		return;
	}

#ifdef _DEBUG
	Log("%s: Received event: playback complete.\n", __FUNCTION__);
#endif

	auto *pCaller = UTIL_EntityByIndex(event->GetInt("entindex", 0));
	m_OnPlaybackFinished.FireOutput(this, pCaller);
}
