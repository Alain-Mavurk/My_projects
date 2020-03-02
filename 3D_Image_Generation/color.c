#include "color.h"
#define prl { printf("\n"); }

color_t color(double r, double g, double b){
	color_t c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

json_element_t* color_to_json(color_t c){
	json_element_t* e = json_object();
	json_object_set(e, "r", json_double(c.r));
	json_object_set(e, "g", json_double(c.g));
	json_object_set(e, "b", json_double(c.b));
	return e;
}

color_t color_from_json(json_element_t* e){
	return color(json_as_double(json_object_get(e, "r")), json_as_double(json_object_get(e, "g")), json_as_double(json_object_get(e, "b")));
}


double clamp(double number, double min, double max){
	if(number < min) return min;
	if(number > max) return max;
	return number;
}

color_t color_add(color_t c1, color_t c2){
	return color(clamp(c1.r + c2.r, 0, 1), clamp(c1.g + c2.g, 0, 1), clamp(c1.b + c2.b, 0, 1));;
}

color_t color_sub(color_t c1, color_t c2){
	return color(clamp(c1.r - c2.r, 0, 1), clamp(c1.g - c2.g, 0, 1), clamp(c1.b - c2.b, 0, 1));
}

color_t color_mul(color_t c1, color_t c2){
	return color(clamp(c1.r * c2.r, 0, 1), clamp(c1.g * c2.g, 0, 1), clamp(c1.b * c2.b, 0, 1));
}

color_t color_scale(double a, color_t c){
	return color(clamp(c.r * a, 0, 1), clamp(c.g * a, 0, 1), clamp(c.b * a, 0, 1));
}

void color_to_BGR(color_t c, unsigned char* buffer){
	if(buffer == NULL) return;
	*buffer = (unsigned char)clamp((c.b*256), 0, 255);
	*(buffer+1) = (unsigned char)clamp((c.g*256), 0, 255);
	*(buffer+2) = (unsigned char)clamp((c.r*256), 0, 255);
}

double avrg_color(color_t c){
	return (c.r + c.g + c.b)/3;
}

unsigned char color_to_graylevel(color_t c){
 	return (unsigned char)clamp(avrg_color(c)*256, 0, 255);
 }

bool color_to_BW(color_t c){
 	return (avrg_color(c) >= 0.5); 
 }

void color_print(color_t c){
	printf("<r=%g, g=%g, b=%g>", c.r, c.g, c.b);
}