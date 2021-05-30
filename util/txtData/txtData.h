#pragma once
#include <vector>

class TxtData
{
public:
	static TxtData* getInstance()
	{
		static TxtData instance;
		return &instance;
	}
private:
	TxtData();
	~TxtData();

public:
	HRESULT init();
	void release();

	void txtSave(const char* saveFileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	vector<string> txtLoad(const char* loadFileName);
	vector<string> charArraySeparation(char charArray[]);
};

