#ifndef __VANISHING_CLIENT_HUD_PLAYERSPEAK_H__
#define __VANISHING_CLIENT_HUD_PLAYERSPEAK_H__

#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include "vgui_controls/Panel.h"

#define HUD_TARGET_MAX_TARGETS (10)

class CHudPlayerSpeak : public CHudElement, public vgui::Panel {

	DECLARE_CLASS_SIMPLE(CHudPlayerSpeak, vgui::Panel);

	public:

	CHudPlayerSpeak(const char *pszElementName);

	virtual void Init();

	void MsgFunc_HudSentenceRequested(bf_read &msg) const;

};

#endif
