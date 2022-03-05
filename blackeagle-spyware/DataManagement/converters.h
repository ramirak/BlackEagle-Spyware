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

enum boundaryType { USER, DATA };
using namespace std;

map<string, string> itemFromJson(const char* fileName, enum boundaryType structType);
string jsonFromItem(map<string, string> generalMap, enum boundaryType structType);



