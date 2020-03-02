#include "json.h"
#include<string.h>
#define ALLOCFAILED() {fprintf(stderr, "Allocation failed\n"); return NULL;}
#define WRONGTYPE() {fprintf(stderr, "It is the wrong type\n"); exit(EXIT_FAILURE);}
#define prl { printf("\n"); }

struct json_object_s{
	char* key;
	json_element_t* value;
	json_object_t* next;
};

struct json_element_s{
	json_type_t type;
	 union{	
	 	bool boolean;
		struct{
			enum{
				JSON_LONG,
				JSON_DOUBLE,
			}type;
			union{
				long int integer;
				double doubles;
			};
		}number;
		char* string;
		struct{
			json_element_t* *array;
			size_t length_array;
		}array_s;
		json_object_t* object;
	};		
};

json_type_t json_type(const json_element_t* e){
	if(e != json_null()) return e->type;
	return JSON_NULL;
}

void* myalloc(size_t sz){
	void* alloc = malloc(sz);
	if(alloc == NULL) exit(EXIT_FAILURE);
	return alloc;
}

void* myrealloc(void* alloc, size_t sz){
	alloc = realloc(alloc, sz);
	if(alloc == NULL) exit(EXIT_FAILURE);
	return alloc;
}

json_element_t* json_boolean(bool b){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_BOOLEAN;
	e->boolean = b;
	return e;
}

bool json_as_boolean(const json_element_t* e) {
	if(json_type(e) != JSON_BOOLEAN) WRONGTYPE();
	return e->boolean;
}

json_element_t* json_integer(long i){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_NUMBER;
	e->number.type = JSON_LONG;
	e->number.integer = i;
	return e;
}


json_element_t* json_double(double d){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_NUMBER;
	e->number.type = JSON_DOUBLE;
	e->number.doubles = d;
	return e;
}

bool json_is_integer(const json_element_t* e){
	if(json_type(e) != JSON_NUMBER) WRONGTYPE();
	return (e->number.type == JSON_LONG);
}


bool json_is_double(const json_element_t* e){
	if(json_type(e) != JSON_NUMBER) WRONGTYPE();
	return (e->number.type == JSON_DOUBLE);
}


long json_as_integer(const json_element_t* e){
	if(json_is_double(e)) return (long)e->number.doubles;
	return e->number.integer;
}

double json_as_double(const json_element_t* e){
	if(json_is_integer(e)) return (double)e->number.integer;
	return e->number.doubles;
}

json_element_t* json_string(const char* s){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_STRING;
	e->string = myalloc(strlen(s)+1);
	e->string = strcpy(e->string, s);
	return e;
}

const char* json_as_string(const json_element_t* e){
	if(json_type(e) != JSON_STRING) WRONGTYPE();
	return e->string;
}

json_element_t* json_array(){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_ARRAY;
	e->array_s.array = NULL;
	e->array_s.length_array = 0;
	return e;
}

size_t size_of_array(json_element_t* e){
	if(json_type(e) != JSON_ARRAY) WRONGTYPE();
	return e->array_s.length_array;
}


json_element_t* json_array_get(const json_element_t* e, size_t i){
	if(json_type(e) != JSON_ARRAY) WRONGTYPE();
	if(i >= e->array_s.length_array) return json_null();
	return e->array_s.array[i];
}


void json_array_set(json_element_t* e, size_t i, json_element_t* se){
	if(json_type(e) != JSON_ARRAY) WRONGTYPE();
	size_t length = e->array_s.length_array;
	if(i >= length){
		e->array_s.length_array = i+1;
		e->array_s.array = myrealloc(e->array_s.array, sizeof(json_element_t)*(i+1));
		e->array_s.array[i] = se;
		while(length < i){
			e->array_s.array[length] = json_null();
			length++;
		}
	}else{
		json_free(e->array_s.array[i]);
		e->array_s.array[i] = se;
	}
}

json_element_t* json_object(){
	json_element_t* e = myalloc(sizeof(json_element_t));
	e->type = JSON_OBJECT;
	e->object = NULL;
	return e;
}

json_element_t* json_object_get(const json_element_t* e, const char* key){
	if(json_type(e) != JSON_OBJECT) WRONGTYPE();
	json_object_t* o = e->object;
	while(o != NULL){
	  if(!strcmp(o->key, key)) return o->value;
		o = o->next;
	}
	return json_null();
}

json_object_t* create_object(const char* key, json_element_t* value){
	json_object_t* o = myalloc(sizeof(json_object_t));
	o->key = myalloc(strlen(key)+1);
	o->key = strcpy(o->key, key);
	o->value = value;
	o->next = NULL;
	return o;
}

void json_object_set(json_element_t* e, const char* key, json_element_t* value){
	if(json_type(e) != JSON_OBJECT) WRONGTYPE();
	if(e->object == NULL){
		e->object = create_object(key, value);
		return;
	}
	json_object_t* curr = e->object;
	json_object_t* prev;
	while(curr != NULL){
		if(!strcmp(curr->key, key)){
			json_free(curr->value);
			curr->value = value;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	prev->next = create_object(key, value);
}

void json_free_object(json_object_t* o){
 	 if(o->next!=NULL) json_free_object(o->next);
 	 free(o->key);
 	 json_free(o->value);
 	 free(o);
}

void json_free(json_element_t* e){
	switch(json_type(e)){
		case JSON_NULL: free(e); break;
		case JSON_BOOLEAN: free(e); break;
		case JSON_NUMBER: free(e); break;
		case JSON_STRING: free(e->string); free(e); break;
		case JSON_ARRAY: ;
			size_t i = 0;
			while(i < e->array_s.length_array){ json_free(e->array_s.array[i]); i++; } free(e->array_s.array); free(e); break;
		case JSON_OBJECT: 
			if(e->object != NULL) json_free_object(e->object); 
			free(e); break;
	} 
}

void json_put(FILE* fd, const json_element_t* e){
	switch(json_type(e)){
		case JSON_NULL: fprintf(fd,"null"); break;
		case JSON_BOOLEAN: fprintf(fd, json_as_boolean(e)?"true":"false"); break;
		case JSON_NUMBER:
				if (e->number.type == JSON_DOUBLE) fprintf(fd,"%.8lf",json_as_double(e));
				if (e->number.type == JSON_LONG) fprintf(fd, "%ld",json_as_integer(e)); 
				break;
		case JSON_STRING: fprintf(fd, "\"%s\"", json_as_string(e)); break;
		case JSON_ARRAY: ;
			size_t i;
			fprintf(fd,"[");
			for (i = 0;i < e->array_s.length_array;i++){
			 	json_put(fd,e->array_s.array[i]);
			 	if(i < e->array_s.length_array-1) fprintf(fd,",");
			}
			fprintf(fd,"]");
			break;
		case JSON_OBJECT: ;
			json_object_t* o = e->object;
			fprintf(fd,"{");
			while(o != NULL){
				fprintf(fd, "\"%s\":", o->key);
				json_put(fd, o->value);
				if(o->next != NULL) fprintf(fd, ",");
				o = o->next;
			}
			fprintf(fd,"}");
			break;
	}
}

void json_save(const char* fname, json_element_t* e){
	FILE* dest = NULL;
	dest = fopen(fname,"w");
	if (dest != NULL){
		json_put(dest, e);
		fclose(dest);
	}
}

char next_char(FILE* fd){
	char c;
 	while((c = fgetc(fd)) != EOF)
 		if(c != ' ' && c != '\n' && c != '\r' && c != '\t') return c;
 	exit(EXIT_FAILURE);
}

void check_next_char(FILE* fd, char c){
	if((next_char(fd)) != c) exit(EXIT_FAILURE);
}

bool is_next_char(FILE* fd, char c, bool cons){
	char a;
	if((a = next_char(fd)) != c || cons == false){
	    ungetc(a,fd);
		return false;
	}
   	return true;
}

void check_next_word(FILE* fd, const char* w){
  char mot[strlen(w)+1];
  fread(mot, strlen(w), 1, fd);
  mot[strlen(w)] = '\0';
  if (strcmp(mot, w)) exit(EXIT_FAILURE);
}

char* next_string(FILE* fd){
	char* string = NULL;
	int i = 0;
	char c = next_char(fd);
	while(c != '"' && c != EOF) c = next_char(fd);

	if(c == '"'){
		while((c = fgetc(fd)) != '"' && c != EOF){
			string = myrealloc(string, (i+2)*sizeof(char));
			string[i] = c;
			i++;
		}	
		string[i] = '\0';
		return string;
	}else{
		exit(EXIT_FAILURE);
	}
}

char* next_digits(FILE* fd){
	char* string = NULL;
	int i=0;
	char c = next_char(fd);
	while(('0' > c || c > '9') && c !=EOF) c = next_char(fd);
	
	if('0' <= c && c <= '9'){
		do{
			string = myrealloc(string, (i+2)*sizeof(char));
			string[i] = c;
			i++;
			c = fgetc(fd);
		}while('0' <= c && c <= '9' && c != EOF);
		ungetc(c, fd);
		string[i] = '\0';
		return string;
	}else{
		exit(EXIT_FAILURE);
	}	
}

json_element_t* json_get(FILE* fd) {
  
  int c = next_char(fd);
  if (c == 'n') {
    ungetc(c,fd);
    check_next_word(fd,"null");
    return json_null();
  }
  if (c == 't') {
    ungetc(c,fd);
    check_next_word(fd,"true");
    return json_boolean(true);
  }
  if (c == 'f') {
    ungetc(c,fd);
    check_next_word(fd,"false");
    return json_boolean(false);
  }
  if (c == '-') {
    json_element_t* se = json_get(fd);
    if (json_type(se) != JSON_NUMBER) {
      fprintf(stderr, "A number is expected after character '-'\n");
      exit(EXIT_FAILURE);
    }
    if (json_is_double(se)) {
      double d = json_as_double(se);
      json_free(se);
      return json_double(-d);
    }
    if (json_is_integer(se)) {
      long i = json_as_integer(se);
      json_free(se);
      return json_integer(-i);
    }
  }
  if ('0' <= c && c <= '9') {
    ungetc(c,fd);
    char* number = next_digits(fd);
    c = fgetc(fd);
    if (c == '.') {   	
      char* decimal = next_digits(fd);
      number = realloc(number, strlen(number) + strlen(decimal) + 2);
      strcat(number,".");
      strcat(number,decimal);
      json_element_t* e = json_double(strtod(number,NULL));
      free(number);
      free(decimal);
      return e;
    } else {
      ungetc(c,fd);
      json_element_t* e = json_integer(strtol(number,NULL,10));
      free(number);
      return e;
    }
  }
  if (c == '"') {
    ungetc(c,fd);
    char* str = next_string(fd);
    json_element_t* e = json_string(str);
    free(str);
    return e;
  }
  if (c == '[') {
    json_element_t* e = json_array();
    size_t i = 0;
    if (is_next_char(fd,']',true)) return e;
    while(1) {
      json_element_t* se = json_get(fd);
      json_array_set(e,i++,se);
      c = next_char(fd);
      if (c == ']') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
        exit(EXIT_FAILURE);
      }
    }
  }
  if (c == '{') {
    json_element_t* e = json_object();
    if (is_next_char(fd,'}',true)) return e;
    while(1) {
      check_next_char(fd,'"');
      ungetc('"',fd);
      char* key = next_string(fd);
      check_next_char(fd,':');
      json_element_t* value = json_get(fd);
      json_object_set(e,key,value);
      free(key);
      c = next_char(fd);
      if (c == '}') return e;
      if (c != ',') {
        fprintf(stderr, "Unexpected character '%c'; ']' or ',' excepted\n", c);
        exit(EXIT_FAILURE);
      }
    }
  }
  fprintf(stderr, "Unexpected character '%c'; 'n', 't', 'f', [0-9], '\"', '[' or '{' excepted\n", c);
  
  exit(EXIT_FAILURE);
}

json_element_t* json_load(const char* fname){
	json_element_t* e = NULL;
	FILE* fd = NULL;
	fd = fopen(fname, "r");
	if(fd == NULL) exit(EXIT_FAILURE);
	e = json_get(fd);
	fclose(fd);
	return e;
}


