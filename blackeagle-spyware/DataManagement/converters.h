#pragma once

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

void* itemFromJson(char* fileName, enum boundaryType structType);
char* jsonFromItem(void* generalStruct, enum boundaryType structType);
