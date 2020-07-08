#include "stdafx.h"
#include "enemyManager.h"
#include "stageManager.h"

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init()
{

	
	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	cout << "현재 위치~~~~ ? " << _sm->getCurrentIndex() << endl;
}

void enemyManager::render()
{

}
