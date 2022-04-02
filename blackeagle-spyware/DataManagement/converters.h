#pragma once
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <json/reader.h>
#include <map>
#include <json/value.h>
#include <json/json.h>
#include <Windows.h>
#include <codecvt>
#include <locale>
enum boundaryType { USER, DATA };
using namespace std;

map<string, map<string, string>> itemsListFromJson(LPSTR pszOutBuffer);
map<string, string> itemFromJson(BOOL isJson, const char* buffer, enum boundaryType structType);
string jsonFromItem(map<string, string> generalMap, enum boundaryType structType);
wstring stringToWString(string str);
string wStringToString(wstring wstr);

