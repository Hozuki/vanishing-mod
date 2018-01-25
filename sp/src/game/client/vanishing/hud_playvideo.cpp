#include "cbase.h"
#include "hud_target.h"
#include "hud.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_video.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CHudPlayVideo : public CHudElement, public vgui::Panel {

	DECLARE_CLASS_SIMPLE(CHudPlayVideo, vgui::Panel);

	public:

	CHudPlayVideo(const char *pszElementName);

	virtual void Init();

	void MsgFunc_VideoCommand(bf_read &msg);

	private:

	static void OnVideoComplete(const void *);

	VideoPanel * m_pVideoPanel;

};

DECLARE_HUDELEMENT(CHudPlayVideo);
DECLARE_HUD_MESSAGE(CHudPlayVideo, VideoCommand);

CHudPlayVideo::CHudPlayVideo(const char *pszElementName)
	: CHudElement(pszElementName), vgui::Panel(nullptr, "HudPlayVideo"),
	m_pVideoPanel(nullptr) {
}

void CHudPlayVideo::Init() {
	HOOK_HUD_MESSAGE(CHudPlayVideo, VideoCommand);
}

extern bool VideoPanel_Create(unsigned int nXPos, unsigned int nYPos, unsigned int nWidth, unsigned int nHeight,
	const char *pVideoFilename, VideoPanelCallback callback, void *ptr);

void CHudPlayVideo::MsgFunc_VideoCommand(bf_read &msg) {
	if (m_pVideoPanel) {
		m_pVideoPanel->OnClose();
	}

	bool overflow;
	auto *pVideoFileName = msg.ReadAndAllocateString(&overflow);

	if (overflow) {
		Warning("%s: string buffer overflow!\n", __FUNCTION__);
		free(pVideoFileName);

		return;
	}

	char strFullpath[MAX_PATH];
	Q_strncpy(strFullpath, "media/", MAX_PATH);	// Assume we must play out of the media directory
	char strFilename[MAX_PATH];
	Q_StripExtension(pVideoFileName, strFilename, MAX_PATH);
	Q_strncat(strFullpath, pVideoFileName, MAX_PATH);

	if (VideoPanel_Create(0, 0, ScreenWidth(), ScreenHeight(), strFullpath, OnVideoComplete, this)) {
		Log("%s: Playback of '%s' has begun.\n", __FUNCTION__, pVideoFileName);
	} else {
		Warning("%s: Playback of '%s' failed!\n", __FUNCTION__, pVideoFileName);
	}

	free(pVideoFileName);
}

void CHudPlayVideo::OnVideoComplete(const void *) {
	auto *event = gameeventmanager->CreateEvent("van_vidcmd_complete");

	if (event) {
#ifdef _DEBUG
		Log("%s: Sending event: video complete.\n", __FUNCTION__);
#endif

		gameeventmanager->FireEvent(event);
	}
}
