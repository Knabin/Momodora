#include "stdafx.h"
#include "stage.h"

Stage::Stage(int stageNum, const char* fileName)
	: _stageNum(stageNum)
	, _fileName(fileName)
	, _background(nullptr)
	, _pixel(nullptr)
	, _player(nullptr)
{
}

Stage::~Stage()
{
}
