#include "converters.h"

//sudo apt install libjson-c-dev
//gcc Prepare.c -ljson-c -o test (for run)

//Login, Data

void* itemFromJson(const char* fileName, enum boundaryType structType)
{
	//FILE* fp;
	//char buffer[1024];

	//fopen_s(&fp, fileName, "r");
	//if (!fp)
	//{
	//	printf("Error opening the file");
	//	return NULL;
	//}
	//fread(buffer, 1024, 1, fp);
	//fclose(fp);

	//struct json_object* parsed_json;
	//parsed_json = json_tokener_parse(buffer); // Parse the Json File contents and convert them into a Json object

	//if (structType == USER)
	//{
	//	struct json_object* email;	  //String
	//	struct json_object* password; //String

	//	LoginDetails* login = (LoginDetails*)malloc(sizeof(LoginDetails));
	//	if (!login)
	//		return NULL;

	//	json_object_object_get_ex(parsed_json, "email", &email);
	//	size_t size_id = strlen(json_object_get_string(email));
	//	login->email = (char*)malloc(sizeof(size_id) + 1);
	//	if (!login->email)
	//		return NULL;
	//	strcpy_s(login->email, size_id, json_object_get_string(email));
	//	//printf("email: %s\n", login->email);

	//	json_object_object_get_ex(parsed_json, "password", &password);
	//	size_t size_password = strlen(json_object_get_string(password));
	//	login->password = (char*)malloc(sizeof(size_password) + 1);
	//	if (!login->password)
	//		return NULL;
	//	strcpy_s(login->password, size_password, json_object_get_string(password));
	//	//printf("password: %s\n", login->password);

	//	return login;
	//}
	//else if (structType == DATA)
	//{
	//	struct json_object* dataId;			  //String
	//	struct json_object* dataType;		  //UserData
	//	struct json_object* createdTimestamp; //Date
	//	struct json_object* dataAttributes;	  //Map<String, Object>
	//	struct json_object* dataAttribute;	  //String

	//	size_t n_attributes;
	//	size_t i;

	//	Data* data = (Data*)malloc(sizeof(Data));
	//	if (!data)
	//		return NULL;

	//	// Get the value of a key in Json object:
	//	// json_object_object_get_ex(Json object, the name of the key we want, where to store key's value)
	//	json_object_object_get_ex(parsed_json, "dataId", &dataId);
	//	size_t size_id = strlen(json_object_get_string(dataId));
	//	data->dataId = (char*)malloc(sizeof(size_id) + 1);
	//	if (!data->dataId)
	//		return NULL;
	//	strcpy_s(data->dataId, size_id, json_object_get_string(dataId));
	//	//printf("dataId: %s\n", data->dataId);

	//	json_object_object_get_ex(parsed_json, "dataType", &dataType);
	//	size_t size_type = strlen(json_object_get_string(dataType));
	//	data->dataType = (char*)malloc(sizeof(size_type) + 1);
	//	if (!data->dataType)
	//		return NULL;
	//	strcpy_s(data->dataType, size_type, json_object_get_string(dataType));
	//	//printf("dataType: %s\n", data->dataType);

	//	json_object_object_get_ex(parsed_json, "createdTimestamp", &createdTimestamp);
	//	size_t size_date = strlen(json_object_get_string(createdTimestamp));
	//	data->createdTimestamp = (char*)malloc(sizeof(size_date) + 1);
	//	if (!data->createdTimestamp)
	//		return NULL;
	//	strcpy_s(data->createdTimestamp, size_date, json_object_get_string(createdTimestamp));
	//	//printf("createdTimestamp: %s\n", data->createdTimestamp);

	//	json_object_object_get_ex(parsed_json, "dataAttributes", &dataAttributes);
	//	size_t size_attArr = strlen(json_object_get_string(dataAttributes));
	//	data->dataAttributes = (char*)malloc(sizeof(size_attArr) + 1);
	//	if (!data->dataAttributes)
	//		return NULL;
	//	//printf("dataAttributes: %s\n", json_object_get_string(dataAttributes));
	//	n_attributes = json_object_array_length(dataAttributes);
	//	//printf("%lu\n", n_attributes);
	//	for (i = 0; i < n_attributes; i++)
	//	{
	//		dataAttribute = json_object_array_get_idx(dataAttributes, i);
	//		size_t size_attribute = strlen(json_object_get_string(dataAttributes));
	//		data->dataAttribute = (char*)malloc(sizeof(size_attribute) + 1);
	//		if (!data->dataAttribute)
	//			return NULL;
	//		strcpy_s(data->dataAttribute, size_attribute, json_object_get_string(dataAttribute));
	//		//printf("%lu. %s\n", i + 1, data->dataAttribute);
	//	}
	//	return data;
	//}
	return NULL;
}

const char* jsonFromItem(void* generalStruct, enum boundaryType structType)
{/*
	struct json_object* jobj;

	if (structType == USER)
	{
		LoginDetails* login = (LoginDetails*)generalStruct;
		char* email = login->email;
		char* password = login->password;
		char str[256];

		snprintf(str, sizeof(str), "{\"email\": \"%s\", \"password\": \"%s\"}", email, password);

		jobj = json_tokener_parse(str);
		//printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
		return json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	}
	else if (structType == DATA)
	{
		Data* data = (Data*)generalStruct;
		char* dataId = data->dataId;
		char* dataType = data->dataType;
		char* createdTimestamp = data->createdTimestamp;
		char* dataAttributes = data->dataAttributes;
		//char *dataAttribute = data->dataAttribute;
		char str[1000];

		snprintf(str, sizeof(str), "{\"dataId\": \"%s\",\"dataType\": \"%s\",\"createdTimestamp\": \"%s\",\"dataAttributes\": [\ \"%s\" ]\ }",
			dataId, dataType, createdTimestamp, dataAttributes);

		jobj = json_tokener_parse(str);
		//printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
		return json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
	}	*/
	return NULL;
}
