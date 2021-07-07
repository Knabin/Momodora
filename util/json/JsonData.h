#pragma once
#include "json.h"

#pragma comment(lib, "jsoncpp.lib")

class JsonData
{
public:
	static JsonData* getInstance()
	{
		static JsonData instance;
		return &instance;
	}

private:
	JsonData();
	~JsonData();

public:
	HRESULT init();
	void release();

	void dataLoad(const char* loadFileName, vector <Utils::st_data_enemy > & vec);
};

