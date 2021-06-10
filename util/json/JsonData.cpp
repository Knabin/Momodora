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

void JsonData::dataLoad(const char* loadFileName)
{
	ifstream openFile;
	openFile.open(loadFileName);

	if (openFile.is_open()) {
		string test((istreambuf_iterator<char>(openFile)),
			(istreambuf_iterator<char>()));

		std::string test2(test);
		auto length = static_cast<int>(test2.length());

		JSONCPP_STRING err;
		Json::Value root;

		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
		if (!reader->parse(test2.c_str(), test2.c_str() + length, &root, &err)) {
			cout << "error" << endl;
		}

		auto test3 = root["stage"];
		test3.size();
		test3[0]["type"].asInt();
		test3[0]["x"].asInt();
		test3[0]["y"].asInt();

	}
}
