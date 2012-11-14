// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRTDBG_MAP_ALLOC

#include "targetver.h"

#include <crtdbg.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define LockScope(lock,scope)do{lock; scope; }while(0);
// TODO: reference additional headers your program requires here
