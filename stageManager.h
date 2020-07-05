#pragma once
#include <vector>
#include <list>
#include "gameNode.h"

class player;

class stageManager : public gameNode
{
private:
	player * _player;

	// 0 ~ 2: commonStage, 3 ~ 5: bossStage
	int _currentIdx;

	/* 
	stageManager에서 해야 할 것
	파일 읽어 와야 함 (맵 이름, 배경 이미지(key값), 픽셀 이미지(key값), 타입(common, boss), 타입 인덱스(몇 번째 맵인지))
	생성할 때 팩토리 패턴은 어떨지 고민해 보기
	enemyManager 쪽으로 각 stage 안에 있는 enemy vector 넘겨서 enemy 받아오기
	*/
	

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayerMemoryAddressLink(player *p) { _player = p; }

	int getCurrentIndex() { return _currentIdx; }
	bool getIsBossStage() 
	{
		if (_currentIdx > 2) return true; 
		return false; 
	}
};

