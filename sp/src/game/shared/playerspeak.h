
#ifndef _PLAYER_SPEAK_H_
#define _PLAYER_SPEAK_H_

#include "cbase.h"
#include <ctype.h>
#include <KeyValues.h>
#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "SoundEmitterSystem.h"
#include "igamesystem.h"
#include "soundchars.h"
#include "filesystem.h"
#include "tier0/vprof.h"
#include "checksum_crc.h"
#include "tier0/icommandline.h"
#include "tier1/strtools.h"

#ifndef CLIENT_DLL
#include "in_buttons.h"
#include "util.h"
#include "envmicrophone.h"
#include "sceneentity.h"
#endif

//#ifdef CLIENT_DLL
#include <vgui_controls/Controls.h>
#include <vgui/IVgui.h>
#ifdef CLIENT_DLL
#include "hud_closecaption.h"
#endif
//#endif

#define MAX_SOUND_NAME 80

void UTIL_PlayerSpeak( const char *soundname, float soundtime = 0.0f );

#endif