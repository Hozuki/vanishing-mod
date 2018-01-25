#include "cbase.h"
#include "hud_target.h"
#include "hud.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ScalableImagePanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CBasePlayer *GetLocalPlayer();

static float inch_to_meter(const float inch) {
	return inch * 0.0254f;
}

static CHudTarget *g_pHudTargetElement = NULL;

#ifdef _DEBUG
static void HudSetTargetPos(const CCommand &args) {
	if (args.ArgC() != (1 + 4)) {
		Log("Usage: hud_set_target_pos <index> <x> <y> <z>\n  Index starts from 1.\n");
		return;
	}

	if (!g_pHudTargetElement) {
		return;
	}

	const int32 i = Q_atoi(args.Arg(1)) - 1;
	const float x = static_cast<float>(Q_atof(args.Arg(2)));
	const float y = static_cast<float>(Q_atof(args.Arg(3)));
	const float z = static_cast<float>(Q_atof(args.Arg(4)));

	const Vector target(x, y, z);

	g_pHudTargetElement->SetTargetState(i, true, target);
}

static ConCommand hud_set_target_pos("hud_set_target_pos", HudSetTargetPos, "Set target position", FCVAR_DEVELOPMENTONLY);

static void HudListTargets() {
	if (!g_pHudTargetElement) {
		Warning("CHudTarget is not found.\n");
	} else {
		g_pHudTargetElement->ListTargets();
	}
}

static ConCommand hud_list_targets("hud_list_targets", HudListTargets, "List HUD targets", FCVAR_DEVELOPMENTONLY);
#endif

DECLARE_HUDELEMENT(CHudTarget);
DECLARE_HUD_MESSAGE(CHudTarget, HudTargetStateUpdated);
DECLARE_HUD_MESSAGE(CHudTarget, HudTargetHeaderUpdated);

CHudTarget::CHudTarget(const char *pszElementName)
	: CHudElement(pszElementName), BaseClass(NULL, "HudTarget") {
}

void CHudTarget::Init() {
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	HOOK_HUD_MESSAGE(CHudTarget, HudTargetStateUpdated);
	HOOK_HUD_MESSAGE(CHudTarget, HudTargetHeaderUpdated);

	SetVisible(false);
	SetAlpha(255);

	SetHiddenBits(/*HIDEHUD_PLAYERDEAD | */HIDEHUD_NEEDSUIT);

	if (g_pHudTargetElement) {
		Error("%s: more than one CHudTarget instance is found!", __FUNCTION__);
	} else {
		g_pHudTargetElement = this;
	}

	SetPaintBackgroundEnabled(false);
	SetPaintBorderEnabled(false);

#define _T_ALLOC_ITEMS(type, count) static_cast<type *>(malloc(sizeof(type) * count))
	m_pTargetEnabled = _T_ALLOC_ITEMS(bool, HUD_TARGET_MAX_TARGETS);
	m_pTargets = _T_ALLOC_ITEMS(Vector, HUD_TARGET_MAX_TARGETS);
	m_pItems = _T_ALLOC_ITEMS(ViewItem, HUD_TARGET_MAX_TARGETS);
#undef _T_ALLOC_ITEMS

	for (int32 i = 0; i < HUD_TARGET_MAX_TARGETS; ++i) {
		m_pTargets[i] = Vector(0);
		m_pTargetEnabled[i] = false;
		//m_pItems[i].image = new vgui::ScalableImagePanel(this, NULL);
		m_pItems[i].header = new vgui::Label(this, NULL, "#Vanishing.Hud.Target");
		m_pItems[i].info = new vgui::Label(this, NULL, "");
		//m_pItems[i].image->SetPaintBackgroundEnabled(false);
		m_pItems[i].header->SetPaintBackgroundEnabled(false);
		m_pItems[i].info->SetPaintBackgroundEnabled(false);
	}
}

void CHudTarget::Paint() {
	BaseClass::Paint();
}

void CHudTarget::MsgFunc_HudTargetStateUpdated(bf_read &msg) const {
	struct TargetStateMessage {
		int32 index;
		int32 enabled;
		float x, y, z;
	} m = { 0 };

	m.index = msg.ReadLong();
	m.enabled = msg.ReadLong();

	m.x = msg.ReadFloat();
	m.y = msg.ReadFloat();
	m.z = msg.ReadFloat();

	//#ifdef _DEBUG
	//	Log("%s: message received {%d, (%f, %f, %f), enabled={%d}}.\n", __FUNCTION__, m.index, m.x, m.y, m.z, m.enabled);
	//#endif

	SetTargetState(m.index, m.enabled, Vector(m.x, m.y, m.z));
}

void CHudTarget::MsgFunc_HudTargetHeaderUpdated(bf_read &msg) const {
	struct TargetHeaderMessage {
		int32 index;
		char *header;
	} m = { 0 };

	m.index = msg.ReadLong();
	bool overflow;
	m.header = msg.ReadAndAllocateString(&overflow);

	if (overflow) {
		Warning("%s: read string overflow!\n", __FUNCTION__);

		free(m.header);
		return;
	}

	//#ifdef _DEBUG
	//	Log("%s: message received {%d, %s}.\n", __FUNCTION__, m.index, m.header);
	//#endif

	SetTargetHeader(m.index, m.header);
	free(m.header);
}

void CHudTarget::OnThink() {
	CBasePlayer *player = GetLocalPlayer();

	bool anyVisible = false;
	int32 screenW, screenH;
	vgui::surface()->GetScreenSize(screenW, screenH);
	SetSize(screenW, screenH);
	SetPos(0, 0);

	const Vector eye = player->EyePosition();

	if (player) {
		for (int32 i = 0; i < HUD_TARGET_MAX_TARGETS; ++i) {
			if (!m_pTargetEnabled[i]) {
				continue;
			}

			const Vector target = m_pTargets[i];

			int32 targetX, targetY;
			const bool isOnScreen = GetVectorInScreenSpace(target, targetX, targetY);

			if (isOnScreen) {
				vgui::Label *header = m_pItems[i].header;
				vgui::Label *info = m_pItems[i].info;
				//vgui::ScalableImagePanel *image = m_Items[i].image;
				const Color fgColor = GetFgColor();

				header->SetFont(m_hTextFont);
				// Handled in SetTargetHeader
				//header->SetText("#Vanishing.Hud.Target");
				header->SetFgColor(fgColor);

				const float distf = inch_to_meter(eye.DistTo(target));
				// The map size would be [-16384,16384] so this will not overflow...
				const int dist = static_cast<int>(roundf(distf));

				static const int _bufsize = 120;
				wchar_t buf[_bufsize] = { 0 };
				const wchar_t *szMeter = g_pVGuiLocalize->Find("#Vanishing.Hud.Meter");
				swprintf(buf, _bufsize, L"%d %s", dist, szMeter);
				info->SetFont(m_hTextFont);
				info->SetText(buf);
				info->SetFgColor(fgColor);

				int32 headerW, headerH;
				header->GetContentSize(headerW, headerH);
				header->SetWrap(true);
				header->SetSize(headerW, headerH);

				int32 infoW, infoH;
				info->GetContentSize(infoW, infoH);
				info->SetWrap(true);
				info->SetSize(infoW, infoH);

				const int32 maxW = max(headerW, infoW);
				const int32 maxH = headerH + infoH + 2;

				const int32 x = clamp(targetX, 0, screenW - maxW);
				const int32 y = clamp(targetY, 0, screenH - maxH);

				header->SetPos(x, y);
				info->SetPos(x, y + headerH + 2); // 2 pixels margin

				m_pItems[i].SetVisible(true);
				anyVisible = true;
			} else {
				m_pItems[i].SetVisible(false);
			}
		}
	}

	SetVisible(anyVisible);

	BaseClass::OnThink();
}

void CHudTarget::LevelInit() {
	RemoveAllTargets();
}

void CHudTarget::LevelShutdown() {
	RemoveAllTargets();
}

void CHudTarget::Reset() {
	RemoveAllTargets();
}

void CHudTarget::SetTargetState(int32 iIndex, bool fEnabled, const Vector &vPosition) const {
	if (iIndex < 0 || HUD_TARGET_MAX_TARGETS <= iIndex) {
		Warning("%s: trying to set a target state with index %d (max targets %d)\n", __FUNCTION__, iIndex, HUD_TARGET_MAX_TARGETS);
		return;
	}

	m_pTargets[iIndex] = vPosition;
	m_pTargetEnabled[iIndex] = fEnabled;
	m_pItems[iIndex].SetVisible(fEnabled);
}

void CHudTarget::SetTargetHeader(int32 iIndex, const char *pszHeader) const {
	if (iIndex < 0 || HUD_TARGET_MAX_TARGETS <= iIndex) {
		Warning("%s: trying to set a target header with index %d (max targets %d)\n", __FUNCTION__, iIndex, HUD_TARGET_MAX_TARGETS);
		return;
	}

	m_pItems[iIndex].header->SetText(pszHeader);
}

void CHudTarget::RemoveAllTargets() const {
	const Vector zero(0);
	for (int32 i = 0; i < HUD_TARGET_MAX_TARGETS; ++i) {
		SetTargetState(i, false, zero);
	}
}

void CHudTarget::ViewItem::SetVisible(bool fVisible) const {
	if (image) {
		image->SetVisible(fVisible);
	}
	if (header) {
		header->SetVisible(fVisible);
	}
	if (info) {
		info->SetVisible(fVisible);
	}
}

#ifdef _DEBUG
void CHudTarget::ListTargets() const {
	char buf[2048] = { 0 };
	for (int32 i = 0; i < HUD_TARGET_MAX_TARGETS; ++i) {
		const bool enabled = m_pTargetEnabled[i];
		const Vector target = m_pTargets[i];
		m_pItems[i].header->GetText(buf, ARRAYSIZE(buf));
		const char *enabledStatus = enabled ? "enabled" : "disabled";
		Log("Target %d [\"%s\"]: (%f %f %f), %s\n", i + 1, buf, target.x, target.y, target.z, enabledStatus);
	}
}

#endif
