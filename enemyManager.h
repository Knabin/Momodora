#pragma once
#include "gameNode.h"
#include <vector>


class enemyManager : public gameNode
{
private:


private:
	/*
	enemyManager에서 해야 할 것
	파일 읽어 와야 함(타입(common, boss), 타입 인덱스(0~2, 0~4), 센터 x값, 센터 y값, 히트박스 width, 히트박스 height)
	stageManager가 보낸 vector에 enemy 생성해서 넣기(이때 파일 parsing해야 함!)
	stageManager로 vector 요청하고 충돌 처리 진행
	
	*/

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();

};
