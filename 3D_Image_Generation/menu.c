#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define NOCHOICE 10

typedef struct{
	char* actionDescription;
	void (*function)(void* args[]);
}action_t;

typedef enum{
	SubMenu,
	Action
}choice_t;

typedef struct{
	choice_t choice;
	union{
		menu_t* subMenu;
		action_t action;
	};
}item_t;

struct menu_s{
	menu_t* parent;
	char* menuDescription;
	item_t item [9];
};

void* myAlloc(size_t sz){
	void* alloc = malloc(sz);
	if(alloc == NULL) exit(EXIT_FAILURE);
	return alloc;
}

menu_t* createMenu(const char* text){
	menu_t* newMenu = NULL;
	newMenu = myAlloc(sizeof(menu_t));
	newMenu->parent = NULL;
	newMenu->menuDescription = myAlloc(strlen(text)+1);
	newMenu->menuDescription = strcpy(newMenu->menuDescription, text);
	
	for(int i = 0; i < 9; i++) newMenu->item[i].choice = NOCHOICE;
	
	return newMenu;
}


void addMenuAction(menu_t* m, const char* text, void(*f)(void* args[])){
	int i;
	
	for(i = 0; i < 9; i++){
		
		if(m->item[i].choice != SubMenu && m->item[i].choice != Action){
			m->item[i].choice = Action;
			m->item[i].action.function = f;
			m->item[i].action.actionDescription = myAlloc(strlen(text)+1);
			m->item[i].action.actionDescription = strcpy(m->item[i].action.actionDescription, text);
			break;
		}
	
	}
}

void addSubMenu(menu_t* m, menu_t* sm){
	if(sm->parent == NULL){
		int i;
		
		for(i = 0; i < 9; i++){
			
			if(m->item[i].choice != SubMenu && m->item[i].choice != Action){
				m->item[i].subMenu = sm;
				m->item[i].choice = SubMenu;
				break;
			}
		
		}
		sm->parent = m;
	
	}
}

void deleteMenu(menu_t* m){
	if(m->parent == NULL){
		int i;
		
		for(i = 0; i < 9; i++){
			
			switch(m->item[i].choice){
				case SubMenu:
					m->item[i].subMenu->parent = NULL;
					deleteMenu(m->item[i].subMenu); break;	
				case Action:
					free(m->item[i].action.actionDescription); break;
			}
		}
		free(m->menuDescription);
		free(m);
	
	}
}

void viderbuffer(void){
  int c;
  while((c=getchar()) != EOF && c != '\n');
}

void launchMenu(menu_t* m, void* args[]){
	printf("%s\n\n", m->menuDescription);
	int i;
	char number;
	for(i = 0; i < 9; i++){
		switch(m->item[i].choice){
			case SubMenu:
				printf(" %d - %s\n\n", i+1, m->item[i].subMenu->menuDescription); break;
			case Action:
				printf(" %d - %s\n\n", i+1, m->item[i].action.actionDescription); break;
		}
	}
	if(m->parent != NULL) printf("Choice number or choice p to precedent menu: ");
	else printf("Choice number: ");
	scanf("%1c", &number);
	viderbuffer();
	if('1' <= number && number <= '9'){
		switch(m->item[number-'1'].choice){
			case Action:
				system("clear");
				m->item[number-'1'].action.function(args);
				printf("\n"); 
				break;
			case SubMenu:
				system("clear");
				launchMenu(m->item[number-'1'].subMenu, args); break;
			default:
				system("clear");
				printf("Please enter valid number\n\n\n");
				launchMenu(m, args); break;
		}
	}else if(number == 'p'){
		system("clear");
		if(m->parent != NULL) launchMenu(m->parent, args);
	}else{
		system("clear");
		printf("Please enter valid number\n\n\n");
		launchMenu(m, args);
	}
	bool cont = *(bool*)args[4];
	if(cont) launchMenu(m, args);
}