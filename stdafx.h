#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

using namespace std;

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "manager/keyManager.h"
#include "utils.h"
#include "collision/collision.h"
#include "manager/imageManager.h"
#include "manager/timeManager.h"
#include "manager/effectManager.h"
#include "manager/soundManager.h"
#include "manager/sceneManager.h"
#include "txtData/txtData.h"
#include "camera/camera.h"
#include "manager/stageEnemyManager.h"

using namespace TTYONE_UTIL;

//MS VS버전이 올라가면서 기존의 문자열 함수의 안정성을 올리기위해
//_s를 붙인 함수를 사용하게끔 경고하는 메시지를 무시해준다


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#define WINNAME (LPCTSTR)(TEXT("Momodora 3"))
#define WINSTARTX	200
#define WINSTARTY	50
#define WINSIZEX	960		
#define WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU
#define BGMVOLUME	0.3f

#define RND randomFunction::getInstance()
#define KEYMANAGER keyManager::getInstance()
#define IMAGEMANAGER imageManager::getInstance()
#define TIMEMANAGER timeManager::getInstance()
#define EFFECTMANAGER effectManager::getInstance()
#define SOUNDMANAGER soundManager::getInstance()
#define SCENEMANAGER sceneManager::getInstance()
#define TXTDATA txtData::getInstance()
#define CAMERA camera::getInstance()
#define STAGEENEMYMANAGER stageEnemyManager::getInstance()

#define SAFE_DELETE(p) {if(p) {delete(p); (p) = NULL;}}

//====================================
// ## 20.05.29 ## Extern ##
//====================================

extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;