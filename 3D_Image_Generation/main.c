#include "json.h"
#include "color.h"
#include "scene3D.h"
#include "bmp.h"
#include "menu.h"
#include "raytracer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define prl {printf("\n");}
void save(void* args[]);

void viderBuffer(void){
  int c;
  while((c=getchar()) != EOF && c != '\n');
}

void* mycalloc(size_t nmemb, size_t sz){
	void* alloc = NULL;
	alloc = calloc(nmemb, sz);
	if(alloc == NULL) exit(EXIT_FAILURE);
	return alloc;
}
//File
void open(void* args[]){
	scene3D_t* s = (scene3D_t*)args[1];
	if(s != NULL){
		printf("Do you want to back up the data before all?\n");
		printf("1: YES\n");
		printf("0: NO\n");
		printf("Your choise: ");
		int b;
		scanf("%1d", &b);
		viderBuffer();
		if(b) save(args);
		scene3D_free(s);
	}
	char* fname = mycalloc(30, 1);
	printf("Please enter the name of an existing file: ");
	scanf("%30s", fname);
	viderBuffer();
	json_element_t* e = NULL;
	s = scene3D_from_json(json_object_get(e = json_load(fname), "scene3D"));
	camera_t* c = myalloc(sizeof(camera_t));
	*c = camera_from_json(json_object_get(e, "camera"));
	json_free(e);
	args[0] = (void*)c;
	args[1] = (void*)s;
	args[2] = fname;
}

void save(void* args[]){
	scene3D_t* s = (scene3D_t*)args[1];
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		char* fname = mycalloc(30, 1);
		printf("Please enter a name for the file with extension (.json): ");
		scanf("%30s", fname);
		viderBuffer();
		int i = 0;
		while(fname[i++] != '\0') if(fname[i] == '.') break;
		if(strcmp((fname+i), ".json")){
			if(args[2] != NULL) printf("The format you just entered is incorrect. The file name will be %s.\n", strcpy(fname, args[2]));
			else printf("The format you just entered is incorrect. The file name will be %s.\n", strcpy(fname, "copy.json"));
		} 
		free(args[2]);
		args[2] = fname;
		camera_t c = *(camera_t*)args[0];
		json_element_t* e = json_object();
		json_object_set(e, "camera", camera_to_json(c));
		json_object_set(e, "scene3D", scene3D_to_json(s));
		json_save(fname, e);
		json_free(e);
	}
}

void information(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL)printf("No scene is loaded.\n");
	else{
		char fname[strlen((char*)args[2])+1];
		strcpy(fname, (char*)args[2]);
		scene3D_t* s = (scene3D_t*)args[1];
		if(fname != NULL)
			printf("File name: %s\n", fname);

		printf("Size of the scene: %ld\n", scene3D_size(s));
		printf("Number of primitives: %ld\n", scene3D_primitives_size(s));
		printf("Number of light: %ld\n", scene3D_lights_size(s));
		scene3D_primitives_print(s);
		scene3D_lights_print(s);
	}
}

void quit(void* args[]){
	scene3D_t* s = (scene3D_t*)args[1];
	if(s != NULL){
		printf("Do you want to back up the data before leaving?\n");
		printf("1: YES\n");
		printf("0: NO\n");
		printf("Your choise: ");
		int b;
		scanf("%1d", &b);
		viderBuffer();
		if(b) save(args);
	} 
	bool* cont = (bool*)args[4];
	*cont = false;
}
//3D scene
//Primitive

point3D_t point3D_User(){
	double x, y, z;
	printf("\tx: ");
	scanf("%lf", &x);
	viderBuffer();
	printf("\ty: ");
	scanf("%lf", &y);
	viderBuffer();
	printf("\tz: ");
	scanf("%lf", &z);
	viderBuffer();
	return point3D(x, y, z);
}

vector3D_t vecto3D_User(){
	double x, y, z;
	printf("\tx: ");
	scanf("%lf", &x);
	viderBuffer();
	printf("\ty: ");
	scanf("%lf", &y);
	viderBuffer();
	printf("\tz: ");
	scanf("%lf", &z);
	viderBuffer();
	return vector3D_normalize(vector3D(x, y, z));
}

color_t color_User(){
	double r, g, b;
	printf("\tr: ");
	scanf("%lf", &r);
	viderBuffer();
	printf("\tg: ");
	scanf("%lf", &g);
	viderBuffer();
	printf("\tb: ");
	scanf("%lf", &b);
	viderBuffer();
	return color(r, g, b);
}

void add_sphere(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		double radius;
		point3D_t p;
		color_t c;
		printf("Please enter the radius of the sphere: ");
		scanf("%lf", &radius);
		viderBuffer();
		printf("Please enter the coordinates of the center of the sphere:\n");
		p = point3D_User();
		printf("Please enter the color data of the sphere:\n");
		c = color_User();
		scene3D_sphere_add(s, c, p, radius);
	}
}

void add_plane(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		point3D_t p;
		vector3D_t v;
		color_t c;
		printf("Please enter the coordinates of the point of the plane:\n");
		p = point3D_User();
		printf("Please enter the coordinates of the normal of the plane:\n");
		v = vecto3D_User();
		printf("Please enter the color data of the plane:\n");
		c = color_User();
		scene3D_plane_add(s, c, p, v);
	}
}

void modify(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		color_t c;
		scene3D_primitives_print(s);
		unsigned int choix;
		printf("Please select a number: ");
		scanf("%u", &choix);
		viderBuffer();
		system("clear");
		printf("You have chosen to modify the following primitive:\n\n");
		scene3D_primitive_print(s, choix); prl;
		
		switch(scene3D_primitive_type(s, choix)){
			case SPHERE: ;
				double radius;
				printf("Please enter the new radius of the sphere: ");
				scanf("%lf", &radius);
				viderBuffer();
				printf("Please enter the new color data for the sphere:\n");
				c = color_User();
				scene3D_primitive_set_color(s, choix, c);
				scene3D_sphere_set_radius(s, choix, radius);
				break;
			case PLANE: ;
				vector3D_t v;
				printf("Please enter the new coordinates of the normal of the plane:\n");
				v = vecto3D_User();
				printf("Please enter the new data of the color of the plane:\n");
				c = color_User();
				scene3D_primitive_set_color(s, choix, c);
				scene3D_plane_set_normal(s, choix, v);
				break;
		}
		scene3D_primitive_print(s, choix);
	}
}

void translate(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		scene3D_primitives_print(s);
		unsigned int choix;
		printf("Please select a number: ");
		scanf("%u", &choix);
		viderBuffer();
		vector3D_t v = vecto3D_User();
		scene3D_primitive_translate(s,choix,v);
		scene3D_primitive_print(s, choix);
	}
}

void delete(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL)printf("No scene is loaded.\n");
	else{
		scene3D_primitives_print(s);
		unsigned int choix;
		printf("Please select a number: ");
		scanf("%u", &choix);
		viderBuffer();
		system("clear");
		printf("You have chosen to delete the following primitive:\n\n");
		scene3D_primitive_print(s, choix);
		scene3D_primitive_delete(s, choix);
	}
}

//Lumiere
void ambient_Light(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		color_t c;
		printf("Please enter the color data of the ambient light:\n");
		c = color_User();
		scene3D_light_set_color(s, 0, c);
	}
}


void add_Unidirectional_Light(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		printf("Please enter the coordinates of the direction of the unidirectional light:\n");
		vector3D_t v = vecto3D_User();
		printf("Please enter the unidirectional color data:\n");
		color_t c =  color_User();
		scene3D_uni_add(s, c, v);
	}
}

void add_Omnidirectional_Light(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		printf("Please enter the coordinates of the direction of the omnidirectional light:\n");
		point3D_t p = point3D_User();
		printf("Please enter new color data for omnidirectional light:\n");
		color_t c =  color_User();
		scene3D_omni_add(s, c, p); 
	}
}



void modifyl(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL)printf("No scene is loaded.\n");
	else{
		color_t c;
		scene3D_lights_print(s);
		unsigned int choix;
		printf("Please select a number: ");
		scanf("%u", &choix);
		viderBuffer();
		system("clear");
		printf("You have chosen to modify the following light:\n\n");
		scene3D_light_print(s, choix); prl;
		
		switch(scene3D_light_type(s, choix)){
			case AMBIENT: 
				printf("Please enter new color data for ambient light:\n");
				c = color_User();
				scene3D_light_set_color(s,choix,c);
				break;
			case OMNI: ;
				point3D_t p;
				printf("Please enter the new coordinates of the omni direction:\n");
				p = point3D_User();
				printf("Please enter the new color data for omni:\n");
				c = color_User();
				scene3D_light_set_color(s,choix,c);
				scene3D_omni_set_position(s,choix,p);
				break;
			case UNI: ;
				vector3D_t v;
				printf("Please enter the new coordinates of the uni direction:\n");
				v = vecto3D_User();
				printf("Please enter the new color data for uni:\n");
				c = color_User();
				scene3D_light_set_color(s, choix, c);
				scene3D_uni_set_direction(s, choix, v);
				break;
		}
		scene3D_light_print(s, choix);
	}
}
void deletel(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		scene3D_lights_print(s); 
		unsigned int choix;
		printf("Please select a number: ");
		scanf("%u", &choix);
		viderBuffer();
		system("clear");
		printf("You have chosen to remove the following light:\n\n");
		scene3D_light_print(s, choix);
		scene3D_light_delete(s, choix);
	}
} 
//generation
//Export

void export(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		size_t depth;
		int i = 0;
		if(args[3] != NULL) depth = *(size_t*)args[3];
		else depth = 24;
		camera_t c = *(camera_t*)args[0];
		char* fname = mycalloc(30, 1);
		printf("Please enter a file name in bmp format (exp: filename.bmp): ");
		scanf("%30s", fname);
		viderBuffer();
		while(fname[i++] != '\0') if(fname[i] == '.') break;
		if(strcmp((fname+i), ".bmp")) printf("The format you just entered is incorrect. The file name will be %s.\n", strcpy(fname, "image.bmp"));
		printf("Your file is being generated...\n");
		camera_raytracing(fname, depth, c, s);
		free(fname);
	}
}



//camera
void position(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		point3D_t p;
		camera_t* c = (camera_t*)args[0];
		printf("Please enter the new position coordinates:\n");
		p = point3D_User();
		c->pos = p;
	}
}

void direction(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		vector3D_t direction;
		camera_t* c = (camera_t*)args[0];
		printf("Please enter the new direction coordinates:\n");
		direction = vecto3D_User();
		c->dir = direction;
	} 
}



void mire (void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{	
		point3D_t p;
		camera_t* c = (camera_t*)args[0];
		printf("Please enter the coordinates of a position: \n");
		p = point3D_User();
		c->dir = point3D_subtract(p, c->pos);
	}
}

void focal(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		camera_t* c = (camera_t*)args[0];
		double f;
		printf("Please enter the distance from the screen to the camera: ");
		scanf("%lf", &f);
		viderBuffer();
		c->focal = f;
	}
}

void dimension(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		camera_t* c = (camera_t*)args[0];
		size_t height, width;
		printf("Please enter the new dimension of the picture:\n");
		printf("\theight: ");
		scanf("%lu", &height);
		viderBuffer();
		printf("\twidth: ");
		scanf("%lu", &width);
		viderBuffer();
		c->height = height;
		c->width = width;
	}
}

void depth(void* args[]){
	scene3D_t* s = ((scene3D_t*)args[1]);
	if(s == NULL) printf("No scene is loaded.\n");
	else{
		int choix;
		size_t* depth = myalloc(sizeof(size_t));
		printf("1- 24\n");
		printf("2- 8\n");
		printf("3- 1\n");
		printf("Please choose the number of bits per pixel: ");
		scanf("%1d", &choix);
		viderBuffer();
		switch(choix){
			case 1: *depth = 24; break;
			case 2: *depth = 8; break;
			case 3: *depth = 1; break;
			default : *depth = 24; break;
		}
		args[3] = (void*)depth;
	}
}

void anti_aliasing(){

}

int main(){
	system("clear");
  	menu_t* m;
  	menu_t* sm;
  	menu_t* ssm;
  	m = createMenu("Main menu");

		sm = createMenu("File");
	  	addMenuAction(sm,"Open",open);
	  	addMenuAction(sm,"Save",save);
	  	addMenuAction(sm,"Information",information);
	  	addMenuAction(sm,"Quitter",quit);
      	addSubMenu(m,sm);
  	
  		sm = createMenu("3D Scene");

     		ssm = createMenu("Primitives");

		 	addMenuAction(ssm,"Add Sphere",add_sphere);
		 	addMenuAction(ssm,"Modify",modify);
		 	addMenuAction(ssm,"Translate",translate);
		 	addMenuAction(ssm,"Delete",delete);
		 	addSubMenu(sm,ssm);

    		ssm = createMenu("Lights");
  		   	addMenuAction(ssm, "Ambient Light", ambient_Light);
		   	addMenuAction(ssm, "Add Unidirectional Light", add_Unidirectional_Light);
		   	addMenuAction(ssm, "Add Omnidirectional Light", add_Omnidirectional_Light);
		   	addMenuAction(ssm, "Modify", modifyl);
		   	addMenuAction(ssm, "Delete", deletel);
		   	addSubMenu(sm,ssm);
		addSubMenu(m,sm);
  
  		sm = createMenu("Generation");
  		addMenuAction(sm, "Export", export);
    
  			ssm = createMenu("Camera");
  		 	addMenuAction(ssm, "Position", position);
  		 	addMenuAction(ssm, "Direction", direction);
  		 	addMenuAction(ssm, "Mire", mire);
		 	addSubMenu(sm,ssm);

  			ssm = createMenu("Image");
  		 	addMenuAction(ssm, "Focal", focal);
  		 	addMenuAction(ssm, "Dimension", dimension);
  		 	addMenuAction(ssm, "Depth", depth);
  		 	addMenuAction(ssm, "Anti-aliasing", anti_aliasing);
  		 	addSubMenu(sm,ssm);
  	  	addSubMenu(m,sm); 
		addMenuAction(m, "Quit", quit);

    bool* cont = myalloc(sizeof(bool));
    *cont = true;
      
    void* args[5];
    args[0] = (camera_t*)NULL;
    args[1] = (scene3D_t*)NULL;
    args[2] = (char*)NULL;
    args[3] = (size_t*)NULL;
    args[4] = (void*)cont;
  	launchMenu(m, args);
 
  	deleteMenu(m);
  	free((bool*)args[4]);
  	free((size_t*)args[3]);
  	free((char*)args[2]);
  	scene3D_free((scene3D_t*)args[1]);
  	free((camera_t*)args[0]);
  	return EXIT_SUCCESS;
}