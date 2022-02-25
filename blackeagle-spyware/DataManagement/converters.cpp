#include "converters.h"
using namespace std;


map<string, std::string> itemFromJson(const char* fileName)
{
	Json::Reader reader;  //for reading the data
	Json::Value ourJson; //for modifying and storing new values

	ifstream file(fileName);
	reader.parse(file, ourJson);

	std::map<string, string> dataJsonMap;
	Json::Value::Members names = ourJson.getMemberNames();

	for (int index = 0; index < names.size(); ++index)
	{
		std::string key = names[index];
		if (key == "dataAttributes")
		{
			Json::Value::Members dataAttributes = ourJson[key].getMemberNames();
			for (int j = 0; j < dataAttributes.size(); j++)
			{
				std::string key2 = dataAttributes[j];
				std::string value = ourJson[key][key2].asString();
				dataJsonMap.insert(make_pair(key2, value));
			}
		}
		else
		{
			std::string value = ourJson[key].asString();
			dataJsonMap.insert(make_pair(key, value));
		}
	}
	return dataJsonMap;
}

std::string jsonFromItem(std::map<std::string, std::string> generalMap, enum boundaryType structType)
{
	Json::Value root;
	Json::StyledWriter writer;

	if (structType == USER)
	{
		auto it1 = generalMap.find("uid");
		if (it1 != generalMap.end())
			root["uid"] = it1->second;

		auto it2 = generalMap.find("password");
		if (it2 != generalMap.end())
			root["password"] = it2->second;

		return writer.write(root);
	}
	else if (structType == DATA)
	{
		auto it1 = generalMap.find("dataId");
		if (it1 != generalMap.end())
			root["dataId"] = it1->second;

		auto it2 = generalMap.find("dataType");
		if (it2 != generalMap.end())
			root["dataType"] = it2->second;

		auto it3 = generalMap.find("createdTimestamp");
		if (it3 != generalMap.end())
			root["createdTimestamp"] = it3->second;
		/*
			auto it4 = generalMap.find("dataAttributes");
			if (it4 != generalMap.end())
			{
				auto it5 = generalMap.find("Block");
				if (it5 != generalMap.end())
					root["dataAttributes"] = it5->second;

				auto it6 = generalMap.find("Log");
				if (it6 != generalMap.end())
					root["dataAttributes"] = it6->second;
			}
		*/
		Json::StyledWriter writer;
		return writer.write(root);
	}
	return "";
}
