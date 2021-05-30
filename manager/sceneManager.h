#pragma once
#include <map>

class GameNode;

class SceneManager
{
public:
	static SceneManager* getInstance()
	{
		static SceneManager instance;
		return &instance;
	}

private:
	SceneManager();
	~SceneManager();

public:
	typedef map<string, GameNode*>			 mapSceneList;
	typedef map<string, GameNode*>::iterator mapSceneIter;

private:
	static GameNode* _currentScene;
	mapSceneList _mSceneList;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	GameNode* addScene(string sceneName, GameNode* scene);

	HRESULT changeScene(string sceneName);

};

