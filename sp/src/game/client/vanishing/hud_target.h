#ifndef __VANISHING_CLIENT_HUD_TARGET_H__
#define __VANISHING_CLIENT_HUD_TARGET_H__

#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include <vgui_controls/Panel.h>

#define HUD_TARGET_MAX_TARGETS (10)

class CHudTarget : public CHudElement, public vgui::Panel {

	DECLARE_CLASS_SIMPLE(CHudTarget, vgui::Panel);

	public:

	CHudTarget(const char *pszElementName);

	virtual void Init();
	virtual void LevelInit();
	virtual void LevelShutdown();
	virtual void Reset();

	void SetTargetState(int32 iIndex, bool fEnabled, const Vector &vPosition) const;
	void SetTargetHeader(int32 iIndex, const char *pszHeader) const;

#ifdef _DEBUG
	void ListTargets() const;
#endif

	virtual void Paint();

	void MsgFunc_HudTargetStateUpdated(bf_read &msg) const;
	void MsgFunc_HudTargetHeaderUpdated(bf_read &msg) const;

	protected:

	virtual void OnThink();

	private:

	void RemoveAllTargets() const;

	// hud_numericdisplay
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "Default");

	struct ViewItem {
		void SetVisible(bool fVisible) const;

		vgui::ScalableImagePanel *image;
		// Header (like "Target")
		vgui::Label *header;
		// Extra info (like "location: (0,0,0)")
		vgui::Label *info;
	};

	Vector *m_pTargets;
	bool *m_pTargetEnabled;
	ViewItem *m_pItems;

};

#endif
