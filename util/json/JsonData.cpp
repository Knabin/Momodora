#include "stdafx.h"
#include "JsonData.h"
#include <fstream>

using namespace std;

JsonData::JsonData()
{
}

JsonData::~JsonData()
{
}

HRESULT JsonData::init()
{
	return E_NOTIMPL;
}

void JsonData::release()
{
}

void JsonData::dataLoad(const char* loadFileName, vector<Utils::st_data_enemy>& vec)
{
	ifstream openFile;
	openFile.open(loadFileName);

	if (openFile.is_open()) {
		string file((istreambuf_iterator<char>(openFile)),
			(istreambuf_iterator<char>()));

		auto length = static_cast<int>(file.length());

		JSONCPP_STRING err;
		Json::Value root;

		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		if (!reader->parse(file.c_str(), file.c_str() + length, &root, &err)) {
			cout << "error!" << endl;
		}

		auto stage = root["stage"];

		for (auto data : stage)
		{
			vec.push_back(Utils::st_data_enemy(data["type"].asInt(), data["x"].asInt(), data["y"].asInt()));
		}
	}
}
