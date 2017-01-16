// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include "..\targetver.h"

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#pragma warning(push)
#pragma warning(disable: 4510 4512 4610)
#include "git2.h"
#pragma warning(pop)
#include "SmartLibgit2Ref.h"

#include <set>
#include <tuple>
#include <algorithm>
#include <memory>

#ifdef _WIN64
#   define APP_X64_STRING   "x64"
#else
#   define APP_X64_STRING ""
#endif

