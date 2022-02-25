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

std::map<std::string, std::string> itemFromJson(const char* fileName);
std::string jsonFromItem(std::map<std::string, std::string> generalMap, enum boundaryType structType);



