#ifndef __JSON_H__
#define __JSON_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define json_null() ((json_element_t*)NULL)


typedef struct json_element_s json_element_t;
typedef struct json_object_s json_object_t;

typedef enum{
	JSON_NULL,
	JSON_BOOLEAN,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
}json_type_t;

json_type_t json_type(const json_element_t* e);
void* myalloc(size_t sz);
void* myrealloc(void* alloc, size_t sz);
json_element_t* json_boolean(bool b);
bool json_as_boolean(const json_element_t* e);
json_element_t* json_integer(long i);
json_element_t* json_double(double d);
bool json_is_integer(const json_element_t* e);
bool json_is_double(const json_element_t* e);
long json_as_integer(const json_element_t* e);
double json_as_double(const json_element_t* e);
json_element_t* json_string(const char* s) ;
const char* json_as_string(const json_element_t* e);
json_element_t* json_array();
size_t size_of_array(json_element_t* e);
json_element_t* json_array_get(const json_element_t* e, size_t i);
void json_array_set(json_element_t* e, size_t i, json_element_t* se);
json_element_t* json_object();
json_element_t* json_object_get(const json_element_t* e, const char* key);
void json_object_set(json_element_t* e, const char* key, json_element_t* value);
void json_free(json_element_t* e);

void json_put(FILE* fd, const json_element_t* e);
void json_save(const char* fname, json_element_t* e);
json_element_t* json_get(FILE* fd);
json_element_t* json_load(const char* fname);

#endif
