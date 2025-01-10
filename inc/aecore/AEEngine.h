// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEEngine.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	the main include file for the Alpha engine
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_ENGINE_H
#define AE_ENGINE_H

#ifdef AE_DBG_MEMORY
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif // _DEBUG

#define AE_CHECK_LEAKS() _CrtDumpMemoryLeaks()
#define AE_CHECK_MEMORY()          \
    {                              \
        if (_heapchk() != _HEAPOK) \
            DebugBreak();          \
    }
#else
#define AE_CHECK_LEAKS()
#define AE_CHECK_MEMORY()
#endif

// WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define AE_UNUSED(p) ((void)(p))

// ---------------------------------------------------------------------------
// Alpha engine includes
#include "AEStdLib.h"
#include "AEDebug.h"
#include "AEMath.h"
#include "AEUtil.h"
#include "AEFrameRateController.h"
#include "AESystem.h"
#ifndef AE_NO_GFX
#include "AEGraphics.h"
#endif
#include "AEInput.h"
#include "AEGameStateMgr.h"
#include "AEColors.h"
#include "AEAudio.h"

// ---------------------------------------------------------------------------

#endif // ENGINE_H
