#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "args.h"
#define NOOPTION ((option_t*)NULL)
#define ALLOCFAILED() {fprintf(stderr, "Allocation failed\n"); exit(EXIT_FAILURE);}


void* myalloc(size_t sz){
	void* alloc = malloc(sz);
	if(alloc == NULL) ALLOCFAILED();
	return alloc;
}

option_t* opt_void(option_t* l, const char* kw, void (*f)()){
	option_t* o = l;
	option_t* prev =NULL;
	while(o!=NULL){
		prev=o;
		o=o->next;
	}
	if (prev == NULL)
	{
		l = (option_t*)myalloc(sizeof(option_t));
		l->keyword = myalloc(strln(kw)+1);
		l->keyword = strcpy(l->keyword, kw);
		l->spec = OptVoid;
		l->fct.opt_void = f;
		l->next = NULL;
	}else {
		prev->next = (option_t*)myalloc(sizeof(option_t));
		prev->next->keyword = myalloc(strln(kw)+1);
		prev->next->keyword = strcpy(l->keyword, kw);
		prev->next->spec = OptVoid;
		prev->next->fct.opt_void = f;
		prev->next->next = NULL;
	}
	return l;
}

option_t* opt_int(option_t* l, const char* kw, void (*f)(int)){
	option_t* o = l;
	option_t* prev = NULL;
	while(o!=NULL){
		prev=o;
		o=o->next;
	}
	if (prev == NULL)
	{
		l = (option_t*)myalloc(sizeof(option_t));
		l->keyword = myalloc(strln(kw)+1);
		l->keyword = strcpy(l->keyword, kw);
		l->spec = OptInt;
		l->fct.opt_int = f;
		l->next = NULL;
	}else {
		prev->next = (option_t*)myalloc(sizeof(option_t));
		prev->next->keyword = myalloc(strln(kw)+1);
		prev->next->keyword = strcpy(l->keyword, kw);
		prev->next->spec = OptInt;
		prev->next->fct.opt_int = f;
		prev->next->next = NULL;
		
	}
	return l;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float)){
	option_t* o = l;
	option_t* prev = NULL;
	while(o!=NULL){
		prev=o;
		o=o->next;
	}
	if (prev == NULL)
	{
		l = (option_t*)myalloc(sizeof(option_t));
		l->keyword = myalloc(strln(kw)+1);
		l->keyword = strcpy(l->keyword, kw);
		l->spec = OptFloat;
		l->fct.opt_float = f;
		l->next = NULL;
	}else {
		prev->next = (option_t*)myalloc(sizeof(option_t));
		prev->next->keyword = myalloc(strln(kw)+1);
		prev->next->keyword = strcpy(l->keyword, kw);
		prev->next->spec = OptFloat;
		prev->next->fct.opt_float = f;
		prev->next->next = NULL;
		
	}
	return l;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)){
	option_t* o = l;
	option_t* prev = NULL;
	while(o!=NULL){
		prev=o;
		o=o->next;
	}
	if (prev == NULL)
	{
		l = (option_t*)myalloc(sizeof(option_t));
		l->keyword = myalloc(strln(kw)+1);
		l->keyword = strcpy(l->keyword, kw);
		l->spec = OptString;
		l->fct.opt_str = f;
		l->next = NULL;
	}else {
		prev->next = (option_t*)myalloc(sizeof(option_t));
		prev->next->keyword = myalloc(strln(kw)+1);
		prev->next->keyword = strcpy(l->keyword, kw);
		prev->next->spec = OptString;
		prev->next->fct.opt_str = f;
		prev->next->next = NULL;
	}
	return l;
}

void opt_delete(option_t* l){
	if (l->next!=NULL) opt_delete(l->next);
	free(l);
}

void process_options(option_t* l, int argc, char* *argv){
	option_t* o = NULL;
	for (int i = 0; i < argc; i++)
	{
		o = l;
		if (argv[i][0]=='-')
		{
			while(strcmp(o->keyword,argv[i])){
				o = o->next;
				if (o == NULL){fprintf(stderr, "NOOPTION");exit(EXIT_FAILURE);}
			}
			switch(o->spec){
				case OptVoid:
					i++;
					o->fct.opt_void(); break;
				case OptInt:
					i++;
					o->fct.opt_int(atoi(argv[i])); break;
				case OptFloat:
					i++;
					o->fct.opt_float(atof(argv[i])); break;
				case OptString:
					i++;
					o->fct.opt_str(argv[i]); break;
			}
		}
	}
}

