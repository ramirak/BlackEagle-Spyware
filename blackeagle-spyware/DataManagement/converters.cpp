#include "Converters.h"

map<string, string> itemFromJson(const char* fileName, enum boundaryType structType)
{
	Json::Reader reader;  //for reading the data
	Json::Value ourJson; //for modifying and storing new values

	ifstream file(fileName);
	reader.parse(file, ourJson);

	map<string, string> jsonMap;
	Json::Value::Members names = ourJson.getMemberNames();

	if (structType == USER)
	{
		for (int index = 0; index < names.size(); ++index)
		{
			string key = names[index];
			string value = ourJson[key].asString();
			jsonMap.insert(make_pair(key, value));
		}
	}
	else if (structType == DATA)
	{
		for (int index = 0; index < names.size(); ++index)
		{
			string key = names[index];
			if (key == "dataAttributes")
			{
				Json::Value::Members dataAttributes = ourJson[key].getMemberNames();
				for (int j = 0; j < dataAttributes.size(); j++)
				{
					string key2 = dataAttributes[j];
					string value = ourJson[key][key2].asString();
					jsonMap.insert(make_pair(key2, value));
				}
			}
			else
			{
				string value = ourJson[key].asString();
				jsonMap.insert(make_pair(key, value));
			}
		}
	}
	return jsonMap;
}

string jsonFromItem(map<string, string> generalMap, enum boundaryType structType)
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
