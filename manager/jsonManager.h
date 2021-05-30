#pragma once
class JsonManager
{
public:
	static JsonManager* getInstance()
	{
		static JsonManager instance;
		return &instance;
	}

private:
	JsonManager();
	~JsonManager();
};

