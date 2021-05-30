#pragma once
#include <vector>
#include <map>

class Effect;

class EffectManager
{
public:
	static EffectManager* getInstance()
	{
		static EffectManager instance;
		return &instance;
	}

private:
	EffectManager();
	~EffectManager();

private:
	//생산
	typedef vector<Effect*>				arrEffects;
	typedef vector<Effect*>::iterator	iterEffects;
	//가공
	typedef map<string, arrEffects>			  arrEffect;
	typedef map<string, arrEffects>::iterator iterEffect;
	//포장
	typedef vector<map<string, arrEffects>>			  arrTotalEffect;
	typedef vector<map<string, arrEffects>>::iterator iterTotalEffect;

private:
	arrTotalEffect _vTotalEffect;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void addEffect(string effectName, const char* imageName,
		int imageWidth, int imageHeight, int effectWidth, int effectHeight, 
		int fps, float elapsedTime, int buffer);

	void play(string effectName, int x, int y);
};

