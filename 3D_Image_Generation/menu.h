#ifndef __MENU_H__
#define __MENU_H__

typedef struct menu_s menu_t;

menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m, const char* text, void(*f)(void* args[]));
void addSubMenu(menu_t* m, menu_t* sm);
void deleteMenu(menu_t* m);
void launchMenu(menu_t* m, void* args[]);


#endif