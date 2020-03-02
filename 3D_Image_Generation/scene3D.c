#include "scene3D.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define NOTYPE 10
#define prl {printf("\n");}

void coord3D_print(struct coord3D_s c){
	printf("<%g, %g, %g>", c.x, c.y, c.z);
}

vector3D_t vector3D(double x, double y, double z){
	vector3D_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vector3D_t vector3D_add(vector3D_t u, vector3D_t v){
	return vector3D(u.x + v.x, u.y + v.y, u.z + v.z);
}

vector3D_t vector3D_sub(vector3D_t u, vector3D_t v){
	return vector3D(u.x - v.x, u.y - v.y, u.z - v.z);
}

vector3D_t vector3D_scale(double a, vector3D_t u){
	return vector3D(u.x * a, u.y * a, u.z * a);
}

double vector3D_dot_product(vector3D_t u, vector3D_t v){
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

vector3D_t vector3D_cross_product(vector3D_t u, vector3D_t v){
 	return vector3D(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

double vector3D_norm2(vector3D_t u){
	return vector3D_dot_product(u, u);
}

double vector3D_norm(vector3D_t u){
	return sqrt(vector3D_norm2(u));	
}

vector3D_t vector3D_normalize(vector3D_t u){
	return vector3D(u.x/vector3D_norm(u), u.y/vector3D_norm(u), u.z/vector3D_norm(u));
}

point3D_t point3D(double x, double y, double z){
	point3D_t p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

point3D_t point3D_translate(point3D_t p, vector3D_t u){
	return point3D(p.x + u.x, p.y + u.y, p.z + u.z);
}

vector3D_t point3D_subtract(point3D_t p, point3D_t q){
	return vector3D(p.x - q.x, p.y - q.y, p.z - q.z);
}

ray_t ray(point3D_t p, vector3D_t d){
	ray_t r;
	r.p = p;
	r.d = vector3D_normalize(d);
	return r;
}

typedef struct{
	double radius;
	point3D_t center;
}sphere_t;

sphere_t sphere(point3D_t c, double r){
	sphere_t s;
	s.center = c;
	s.radius = fabs(r);
	return s;
}

sphere_t sphere_translate(sphere_t s, vector3D_t v){
	return sphere(point3D_translate(s.center, v), s.radius);
}

bool sphere_intersect(sphere_t s, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn){
	double b = vector3D_dot_product(r.d, point3D_subtract(r.p, s.center));
	double c = vector3D_norm2(point3D_subtract(r.p, s.center)) - pow(s.radius, 2);
	double l = -b - sqrt(pow(b, 2) - c);
	point3D_t q = point3D_translate(r.p, vector3D_scale(l, r.d));
	vector3D_t n = vector3D_normalize(point3D_subtract(q, s.center));
	if((pow(b, 2) - c) >= 0 && l >= 0){
		if(pl != NULL) *pl = l;
		if(pq != NULL) *pq = q;
		if(pn != NULL) *pn = n;
		return true;
	}
	return false;
}

typedef struct{
	point3D_t point;
	vector3D_t normalised;
}plane_t;

plane_t plane(point3D_t o, vector3D_t n){
	plane_t p;
	p.point = o;
	p.normalised = vector3D_normalize(n);
	return p;
}

plane_t plane_translate(plane_t p, vector3D_t v){
	return plane(point3D_translate(p.point, v), p.normalised);
}

bool plane_intersect(plane_t p, ray_t r, double* pl, point3D_t* pq, vector3D_t* pn){
	double l = vector3D_dot_product(point3D_subtract(p.point, r.p), p.normalised)/vector3D_dot_product(r.d, p.normalised);
	point3D_t q = point3D_translate(r.p, vector3D_scale(l, r.d));
	vector3D_t n = p.normalised;
	if(vector3D_dot_product(r.d, p.normalised) != 0 && l >= 0){
		if(pl != NULL) *pl = l;
		if(pq != NULL) *pq = q;
		if(pn != NULL) *pn = n;
		return true;
	}
	return false;
}

typedef struct{
	primitive_type_t type;
	union{
		sphere_t sphere;
		plane_t plane;
	};
	color_t color;
}primitive_t;

primitive_t primitive_sphere(sphere_t s, color_t c){
	primitive_t p;
	p.type = SPHERE;
	p.color = c;
	p.sphere = s;
	return p;
}

primitive_t primitive_plane(plane_t pl, color_t c){
	primitive_t p;
	p.type = PLANE;
	p.color = c;
	p.plane = pl;
	return p;
}

color_t primitive_get_color(primitive_t* p){
	return p->color;
}

void primitive_set_color(primitive_t* p, color_t c){
	p->color = c;
}

bool primitive_intersect(primitive_t* p, ray_t* r, double* pl, point3D_t* pq, vector3D_t* pn){
	if(p == NULL) return false;
	if(p->type == SPHERE) return sphere_intersect(p->sphere, *r, pl, pq, pn);
	if(p->type == PLANE) return plane_intersect(p->plane, *r, pl, pq, pn);
	return false;
}

typedef struct{
	light_type_t type;
	union{
		point3D_t omni;
		vector3D_t uni;
	};
	color_t color;
}light_t;

light_t ambient(color_t c){
	light_t l;
	l.type = AMBIENT;
	l.color = c;
	return l;
}

light_t omni(color_t c, point3D_t p){
	light_t l;
	l.type = OMNI;
	l.color = c;
	l.omni = p;
	return l;
}

light_t uni(color_t c, vector3D_t d){
	light_t l;
	l.type = UNI;
	l.color = c;
	l.uni = vector3D_normalize(d);
	return l;
}

ray_t light_ray(light_t l, point3D_t q){
	if(l.type == OMNI) return ray(q, point3D_subtract(l.omni, q));
	if(l.type == UNI) return ray(q, vector3D_scale(-1, l.uni));
	ray_t r;
	r.p = q;
	return r;
}

struct scene3D_s{
	primitive_t primitive;
	light_t light;
	scene3D_t* next;
};

void* mymalloc(size_t sz){
	void* alloc = NULL;
	alloc = malloc(sz);
	if(alloc == NULL){
		fprintf(stderr, "Allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return alloc;
}

scene3D_t* scene3D_new(){
	scene3D_t* s = (scene3D_t*)mymalloc(sizeof(scene3D_t));
	s->primitive.type = NOTYPE;
	s->light.type = NOTYPE;
	s->next = NULL;
	return s;
}

void scene3D_free(scene3D_t* s){
	if(s == NULL) return;
	if(s->next != NULL) scene3D_free(s->next);
	free(s);
}

bool primitive_empty(primitive_t p){
	return (p.type == NOTYPE);
}

bool light_empty(light_t l){
	return (l.type == NOTYPE);
}

bool scene3D_empty(scene3D_t* s){
	if(s == NULL) return true;
	return (primitive_empty(s->primitive) && light_empty(s->light));
}

void primitives_print(primitive_t p){
	switch(p.type){
		case SPHERE:
			printf("Sphere of radius %g centered on ", p.sphere.radius);
			coord3D_print(p.sphere.center);
			printf(" with color ");
			color_print(p.color);break;
		case PLANE:
			printf("Plane of normal ");
			coord3D_print(p.plane.normalised);
			printf(" passing through ");
			coord3D_print(p.plane.point);
			printf(" with color ");
			color_print(p.color);break;
	}
} 

void scene3D_primitives_print(scene3D_t* s){
	if(scene3D_empty(s)) return;
	unsigned int primitiveId = 0;
	scene3D_t* curr = s;
	if(curr->primitive.type == NOTYPE) printf("no primitive\n");
	while(curr != NULL && curr->primitive.type != NOTYPE){
		printf("(%u) : ", primitiveId);
		primitives_print(curr->primitive);prl;
		primitiveId++;
		curr = curr->next;
	}
}

void scene3D_primitive_print(scene3D_t* s, unsigned int primitiveId){
	if(scene3D_empty(s)) return;
	unsigned int cpt = 0;
	scene3D_t* curr = s;
	while(cpt != primitiveId){
		curr = curr->next;
		cpt++;
	}
	printf("(%u) : ", primitiveId);
	primitives_print(curr->primitive);prl;
}

void lights_print(light_t l){
	switch(l.type){
		case AMBIENT: 
			printf("Ambient light with color ");
			color_print(l.color);break;
		case OMNI:
			printf("Omnidirectional light centered on ");
			coord3D_print(l.omni);
			printf(" with color ");
			color_print(l.color);break;
		case UNI:
			printf("Unidirectional light of direction ");
			coord3D_print(l.uni);
			printf(" with color ");
			color_print(l.color);break;		
	}
}

void scene3D_lights_print(scene3D_t* s){
	if(scene3D_empty(s)) return;
	unsigned int lightId = 0;
	scene3D_t* curr = s;
	if(curr->light.type == NOTYPE) printf("no light\n");
	while(curr != NULL && curr->light.type != NOTYPE){
		printf("(%u) : ", lightId);
		lights_print(curr->light);prl;
		lightId++;
		curr = curr->next;
	}
}

void scene3D_light_print(scene3D_t* s, unsigned int lightId){
	if(scene3D_empty(s)) return;
	unsigned int cpt = 0;
	scene3D_t* curr = s;
	while(cpt != lightId){
		curr = curr->next;
		cpt++;
	}
	printf("(%u) : ", lightId);
	lights_print(curr->light);prl;
}

void scene3D_sphere_add(scene3D_t* s, color_t color, point3D_t c, double r){
	if(scene3D_empty(s)){
		s->primitive = primitive_sphere(sphere(c, r), color);
		return;
	}
	scene3D_t* curr = s;
	scene3D_t* prev;
	while(curr != NULL){
		if(primitive_empty(curr->primitive)){
			curr->primitive = primitive_sphere(sphere(c, r), color);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	scene3D_t* new = scene3D_new();
	new->primitive = primitive_sphere(sphere(c, r), color);
	prev->next = new;
}

void scene3D_plane_add(scene3D_t* s, color_t color, point3D_t o, vector3D_t n){
	if(scene3D_empty(s)){
		s->primitive = primitive_plane(plane(o, n), color);
		return;
	}
	scene3D_t* curr = s;
	scene3D_t* prev;
	while(curr != NULL){
		if(primitive_empty(curr->primitive)){
			curr->primitive = primitive_plane(plane(o, n), color);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	scene3D_t* new = scene3D_new();
	new->primitive = primitive_plane(plane(o, n), color);
	prev->next = new;
}

scene3D_t* scene3D_browse(scene3D_t* s, unsigned int Id){
	if(s == NULL) exit(EXIT_FAILURE);
	unsigned int cpt = 0;
	scene3D_t* curr = s;
	while(cpt != Id && curr != NULL){
		curr = curr->next;
		cpt++;
	}
	return curr;
}

primitive_type_t scene3D_primitive_type(scene3D_t* s, unsigned int primitiveId){
	scene3D_t* curr = scene3D_browse(s, primitiveId);
	return curr->primitive.type;
}

void scene3D_sphere_set_radius(scene3D_t* s, unsigned int primitiveId, double radius){
	if(scene3D_primitive_type(s, primitiveId) == SPHERE){
		scene3D_t* curr = scene3D_browse(s, primitiveId);
		curr->primitive.sphere.radius = radius;
	}
}

void scene3D_plane_set_normal(scene3D_t* s, unsigned int primitiveId, vector3D_t n){
	if(scene3D_primitive_type(s, primitiveId) == PLANE){
		scene3D_t* curr = scene3D_browse(s, primitiveId);
		curr->primitive.plane.normalised = n;
	}
}

void scene3D_primitive_set_color(scene3D_t* s, unsigned int primitiveId, color_t color){
	scene3D_t* curr = scene3D_browse(s, primitiveId);
	curr->primitive.color = color;
}

void scene3D_primitive_translate(scene3D_t* s, unsigned int primitiveId, vector3D_t v){
	scene3D_t* curr = scene3D_browse(s, primitiveId);
	switch(scene3D_primitive_type(s, primitiveId)){
		case SPHERE: 
			curr->primitive.sphere = sphere_translate(curr->primitive.sphere, v);break;
		case PLANE:
			curr->primitive.plane = plane_translate(curr->primitive.plane, v);break;	
	}
}

void scene3D_primitive_delete(scene3D_t* s, unsigned int primitiveId){
	scene3D_t* curr = scene3D_browse(s, primitiveId);
	while(curr != NULL){
		scene3D_t* suiv = scene3D_browse(s, ++primitiveId);
		if(suiv == NULL){
			curr->primitive.type = NOTYPE;
			return;
		}
		curr->primitive = suiv->primitive;
		curr = curr->next;
	}
}

void scene3D_ambient_add(scene3D_t* s, color_t color){
	if(s == NULL){
		scene3D_t* new = scene3D_new();
		new->light = ambient(color);
		return;
	}
	if(scene3D_empty(s)){
		s->light = ambient(color);
		return;
	}
	scene3D_t* curr = s;
	scene3D_t* prev;
	while(curr != NULL){
		if(light_empty(curr->light)){
			curr->light = ambient(color);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	scene3D_t* new = scene3D_new();
	new->light = ambient(color);
	prev->next = new;
}

void scene3D_omni_add(scene3D_t* s, color_t color, point3D_t p){
	if(s == NULL){
		scene3D_t* new = scene3D_new();
		new->light = omni(color, p);
		return;
	}
	if(scene3D_empty(s)){
		s->light = omni(color, p);
		return;
	}
	scene3D_t* curr = s;
	scene3D_t* prev;
	while(curr != NULL){
		if(light_empty(curr->light)){
			curr->light = omni(color, p);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	scene3D_t* new = scene3D_new();
	new->light = omni(color, p);
	prev->next = new;
}

void scene3D_uni_add(scene3D_t* s, color_t color, vector3D_t d){
	if(s == NULL){
		scene3D_t* new = scene3D_new();
		new->light = uni(color, d);
		return;
	}
	if(scene3D_empty(s)){
		s->light = uni(color, d);
		return;
	}
	scene3D_t* curr = s;
	scene3D_t* prev;
	while(curr != NULL){
		if(light_empty(curr->light)){
			curr->light = uni(color, d);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
	scene3D_t* new = scene3D_new();
	new->light = uni(color, d);
	prev->next = new;
}

light_type_t scene3D_light_type(scene3D_t* s, unsigned int lightId){
	scene3D_t* curr = scene3D_browse(s, lightId);
	return curr->light.type;
}

void scene3D_omni_set_position(scene3D_t* s, unsigned int lightId, point3D_t p){
	if(scene3D_light_type(s, lightId) == OMNI){
		scene3D_t* curr = scene3D_browse(s, lightId);
		curr->light.omni = p;
	}
}

void scene3D_uni_set_direction(scene3D_t* s, unsigned int lightId, vector3D_t d){
	if(scene3D_light_type(s, lightId) == UNI){
		scene3D_t* curr = scene3D_browse(s, lightId);
		curr->light.uni = vector3D_normalize(d);
	}
}

void scene3D_light_set_color(scene3D_t* s, unsigned int lightId, color_t color){
	scene3D_t* curr = scene3D_browse(s, lightId);
	curr->light.color = color;
}

void scene3D_light_delete(scene3D_t* s, unsigned int lightId){
	scene3D_t* curr = scene3D_browse(s, lightId);
	while(curr != NULL){
		scene3D_t* suiv = scene3D_browse(s, ++lightId);
		if(suiv == NULL){
			curr->light.type = NOTYPE;
			return;
		}
		curr->light = suiv->light;
		curr = curr->next;
	}
}

size_t scene3D_primitives_size(scene3D_t* s){
	scene3D_t* curr = s;
	int i = 0;
	while(curr != NULL){
		if(primitive_empty(curr->primitive)) break;
		i++;
		curr = curr->next;
	}
	return i;
}

size_t scene3D_lights_size(scene3D_t* s){
	scene3D_t* curr = s;
	size_t i = 0;
	while(curr != NULL){
		if(light_empty(curr->light)) break;
		i++;
		curr = curr->next;
	}
	return i;
}

size_t scene3D_size(scene3D_t* s){
	size_t i = 0;
	scene3D_t* curr = s;
	while(curr != NULL){
		if(scene3D_empty(curr)) break;
		i++;
		curr = curr->next;
	}
	return i;
}

json_element_t* coord3D_to_json(struct coord3D_s c){
	json_element_t* e = json_object();
	json_object_set(e, "x", json_double(c.x));
	json_object_set(e, "y", json_double(c.y));
	json_object_set(e, "z", json_double(c.z));
	return e;
}

json_element_t* primitive_to_json(primitive_t p){
	if(p.type == SPHERE){
		json_element_t* e = json_object();
		json_object_set(e, "type", json_string("SPHERE"));
		json_object_set(e, "color", color_to_json(p.color));
		json_object_set(e, "radius", json_double(p.sphere.radius));
		json_object_set(e, "center", coord3D_to_json(p.sphere.center));
		return e;
	}else if(p.type == PLANE){
		json_element_t* e = json_object();
		json_object_set(e, "type", json_string("PLANE"));
		json_object_set(e, "color", color_to_json(p.color));
		json_object_set(e, "normal", coord3D_to_json(p.plane.normalised));
		json_object_set(e, "point", coord3D_to_json(p.plane.point));
		return e;
	}else{
		return json_null();
	}
}

json_element_t* light_to_json(light_t l){
	if(l.type == AMBIENT){
		json_element_t* e = json_object();
		json_object_set(e, "type", json_string("AMBIENT"));
		json_object_set(e, "color", color_to_json(l.color));
		return e;
	}else if(l.type == OMNI){
		json_element_t* e = json_object();
		json_object_set(e, "type", json_string("OMNI"));
		json_object_set(e, "color", color_to_json(l.color));
		json_object_set(e, "position", coord3D_to_json(l.omni));
		return e;
	}else if(l.type == UNI){
		json_element_t* e = json_object();
		json_object_set(e, "type", json_string("UNI"));
		json_object_set(e, "color", color_to_json(l.color));
		json_object_set(e, "direction", coord3D_to_json(l.uni));
		return e;
	}else{
		return json_null();
	}
}

json_element_t* scene3D_to_json(scene3D_t* s){
	scene3D_t* curr = s;
	json_element_t* p = NULL;
	json_element_t* l = NULL;
	p = json_array();
	l = json_array();
	int size = scene3D_primitives_size(s);
	for(int i = 0; i < size; i++){
		json_array_set(p, i, primitive_to_json(curr->primitive));
		curr = curr->next;
	}
	size = scene3D_lights_size(s);
	curr = s;
	for(int i = 0; i < size; i++){
		json_array_set(l, i, light_to_json(curr->light));
		curr = curr->next;
	}
	json_element_t* e = json_object();
	json_object_set(e, "primitives", p);
	json_object_set(e, "lights", l);
	return e;
}

struct coord3D_s coord3D(double x, double y, double z){
	struct coord3D_s c;
	c.x = x;
	c.y = y;
	c.z = z;
	return c;
}

struct coord3D_s coord3D_from_json(const json_element_t* e){
	return coord3D(json_as_double(json_object_get(e, "x")), json_as_double(json_object_get(e, "y")), json_as_double(json_object_get(e, "z")));
}

scene3D_t* scene3D_from_json(const json_element_t* e){
	scene3D_t* s = scene3D_new();
	json_element_t* p = json_object_get(e, "primitives");
	size_t size = size_of_array(p);
	size_t i = 0;
	while(i < size){
		if(!strcmp(json_as_string(json_object_get(json_array_get(p, i), "type")), "SPHERE"))
			scene3D_sphere_add(s, color_from_json(json_object_get(json_array_get(p, i), "color")), coord3D_from_json(json_object_get(json_array_get(p, i), "center")), json_as_double(json_object_get(json_array_get(p, i), "radius")));
		else if(!strcmp(json_as_string(json_object_get(json_array_get(p, i), "type")), "PLANE"))
			scene3D_plane_add(s, color_from_json(json_object_get(json_array_get(p, i), "color")), coord3D_from_json(json_object_get(json_array_get(p, i), "point")), coord3D_from_json(json_object_get(json_array_get(p, i), "normal")));
		i++;	
	}
	json_element_t* l = json_object_get(e, "lights");
	size = size_of_array(l);
	i = 0;
	while(i < size){
		if(!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "AMBIENT"))
			scene3D_ambient_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")));
		else if(!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "OMNI"))
			scene3D_omni_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")), coord3D_from_json(json_object_get(json_array_get(l, i), "position")));
		else if(!strcmp(json_as_string(json_object_get(json_array_get(l, i), "type")), "UNI"))
			scene3D_uni_add(s, color_from_json(json_object_get(json_array_get(l, i), "color")), coord3D_from_json(json_object_get(json_array_get(l, i), "direction")));
		i++;
	}
	return s;
}

bool scene3D_intersect(scene3D_t* s, ray_t* r, double* ret_l, point3D_t* ret_q, vector3D_t* ret_n, color_t* ret_c){
	if(scene3D_empty(s)) return false;
	scene3D_t* curr = s;
	scene3D_t* min_inter = NULL;
	double min_l = -1;
	while(curr != NULL){
		if(primitive_intersect(&curr->primitive, r, ret_l, ret_q, ret_n)){
			if(ret_l != NULL && (*ret_l < min_l || min_l == -1)){
				min_l = *ret_l;
				min_inter = curr;
			} 
		}
		curr = curr->next;
	}
	if(min_inter != NULL){
		if(ret_c != NULL) *ret_c = min_inter->primitive.color;
		return primitive_intersect(&min_inter->primitive, r, ret_l, ret_q, ret_n);
	}else
		return false;
}

ray_t scene3D_light_ray(scene3D_t* s, unsigned int lightId, point3D_t q){
	scene3D_t* curr = scene3D_browse(s, lightId);
	return light_ray(curr->light, q);
}

color_t scene3D_light_color(scene3D_t* s, unsigned int lightId){
	scene3D_t* curr = scene3D_browse(s, lightId);
	return curr->light.color;
}

point3D_t scene3D_omni_position(scene3D_t* s, unsigned int lightId){
	scene3D_t* curr = scene3D_browse(s, lightId);
	return curr->light.omni;
}

vector3D_t scene3D_uni_direction(scene3D_t* s, unsigned int lightId){
	scene3D_t* curr = scene3D_browse(s, lightId);
	return curr->light.uni;
}

color_t scene3D_enlightement(scene3D_t* s, point3D_t q, vector3D_t n, color_t intra) {
  q = point3D_translate(q, vector3D_scale(1e-13, n));
  color_t c = color(0,0,0);
  ray_t r;
  double l, ll;
  for(size_t i = 0; i < scene3D_lights_size(s); i++) { 
    switch(scene3D_light_type(s,i)) {
    case AMBIENT:
      c = color_add(c, color_mul(scene3D_light_color(s,i), intra));
      break;
    case UNI:
      r = scene3D_light_ray(s, i, q);
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL))
        c = color_add(c, color_mul(color_scale(-vector3D_dot_product(scene3D_uni_direction(s,i), n), scene3D_light_color(s,i)), intra));
      break;
    case OMNI:
      r = scene3D_light_ray(s, i, q);
      ll = vector3D_norm(point3D_subtract(q, scene3D_omni_position(s,i)));
      if (!scene3D_intersect(s,&r,&l,NULL,NULL,NULL) || l > ll)
        c = color_add(c, color_mul(color_scale(vector3D_dot_product(vector3D_normalize(point3D_subtract(scene3D_omni_position(s,i), q)), n), scene3D_light_color(s,i)), intra));
      break;
    default:
      break;
    }
  }
  return c;
}