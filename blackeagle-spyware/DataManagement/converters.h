#pragma once
#include <stdio.h>
#include <cstdlib>
#include <string.h>
 

typedef struct {
	char* email;
	char* password;
}LoginDetails;

typedef struct {
	char* dataId;
	char* dataType;
	char* createdTimestamp;
	char* dataAttributes;
	char* dataAttribute;
}Data;

enum boundaryType { USER, DATA };

void* itemFromJson(const char* fileName, enum boundaryType structType);
const char* jsonFromItem(void* generalStruct, enum boundaryType structType);

