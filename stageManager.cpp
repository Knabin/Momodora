#include "stdafx.h"
#include "stageManager.h"
#include "player.h"
#include "bossStage.h"
#include "commonStage.h"

HRESULT stageManager::init()
{
	//TODO: stageManager에서 처리하기
	commonStage * common1 = new commonStage();
	common1->init(0);
	common1->setPlayerMemoryAddressLink(_player);

	commonStage * common2 = new commonStage();
	common2->init(1);
	common2->setPlayerMemoryAddressLink(_player);

	commonStage * common3 = new commonStage();
	common3->init(2);
	common3->setPlayerMemoryAddressLink(_player);

	bossStage * boss1 = new bossStage();
	boss1->init(0);
	boss1->setPlayerMemoryAddressLink(_player);

	//commonStage* stage2 = new commonStage();
	//stage2->init(1);
	//_vCommonStage.push_back(stage2);
	//stage2->setPlayerMemoryAddressLink(_player);

	//bossStage * b = new bossStage();
	//_vBossStage.push_back(b);
	//_currentIdx = 0;


	SCENEMANAGER->addScene("테스트", common1);
	SCENEMANAGER->addScene("테스트2", common2);
	SCENEMANAGER->addScene("테스트3", common3);
	SCENEMANAGER->addScene("테스트4", boss1);

	SCENEMANAGER->changeScene("테스트");

	_currentIdx = 0;


	return S_OK;
}

void stageManager::release()
{
}

void stageManager::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		SCENEMANAGER->changeScene("테스트");
		_currentIdx = 0;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		SCENEMANAGER->changeScene("테스트2");
		_currentIdx = 1;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		SCENEMANAGER->changeScene("테스트3");
		_currentIdx = 2;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F4))
	{
		SCENEMANAGER->changeScene("테스트4");
		_currentIdx = 3;
	}

	SCENEMANAGER->update();	
}

void stageManager::render()
{
	SCENEMANAGER->render();
	
}