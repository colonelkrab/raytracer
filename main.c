#include <raylib.h>
#include <stdio.h>
#include <math.h>
#define TMAX (float) 12000
#define BG BLACK
#define AMB (float) 0.08
const int sw = 800;
const int sh = 800;

typedef struct {
	Vector3 c;
	float r;
	Color color;
} __Sphere;

typedef struct {
	Vector3 pos;
} PointLight;

Vector3 screenToWorld(Vector2 pscreen);
Color traceRay(Vector3 org, Vector3 dir, int t_min, int t_max);
void intersectSphere(Vector3 org, Vector3 dir, __Sphere s, float *t1, float *t2);
float dot(Vector3 a, Vector3 b);
float abs_vec3(Vector3 v);
float colorIntensity(Vector3 light, Vector3 normal);
Vector3 vecAdd(Vector3 v1, Vector3 v2);
Vector3 vecScale(Vector3 v, float k);
Vector3 vecSub(Vector3 v1, Vector3 v2);
Color colorScale(Color c, float k);

// world
const __Sphere s = {{0, 0, 100}, 20, RED};
const PointLight l = {{-300,0,300}};

int main(void){
	Vector3 org = {0,0,0};
    	InitWindow(sw, sh, "raytracer");
    	while (!WindowShouldClose()){
        	BeginDrawing();

        	for(int i=0; i <= sw; i++) {
        		for(int j=0; j <= sh; j++){
            			Vector2 pscreen = {i , j};
            			Vector3 dir = screenToWorld(pscreen);
            			Color color = traceRay(org, dir, 1, TMAX);
				DrawPixel(i, j, color);
        		};
        	};
        	EndDrawing();
    	}

    	CloseWindow();
	return 0;
}


Vector3 screenToWorld(Vector2 pscreen){
	Vector3 pworld = {(pscreen.x * 1.0/sw) - 0.5 , 0.5 - (pscreen.y * 1.0/sh), 1};
	return pworld;
}


Color traceRay(Vector3 org, Vector3 dir, int t_min, int t_max){
    	float closest_t = t_max;
	float t1, t2;
	intersectSphere(org, dir, s, &t1, &t2);
	if (t1 < t2){
		closest_t = t1;
	}else {
		closest_t = t2;
	}

	if (closest_t < t_max){
    		Vector3 pSphere = vecAdd(vecScale(dir,closest_t), org);
    		Vector3 normal = vecSub(pSphere, s.c);
    		float i = colorIntensity(vecSub(l.pos, pSphere), normal);
		return colorScale(s.color, i);
	}else {
		return BG;
	}
}


void intersectSphere(Vector3 org, Vector3 dir, __Sphere s, float *t1, float *t2){
    float a = dot(dir, dir);
    float b = -2*dot(s.c, dir);
    float c = dot(s.c, s.c) - (s.r*s.r);

    float D = b*b - 4*a*c;
    if (D < 0){
        *t1 = TMAX;
        *t2 = TMAX;
        return;
    }

    *t1 = (-b + sqrt(D) / (2*a));
    *t2 = (-b - sqrt(D) / (2*a));
    return;
}

float dot(Vector3 a, Vector3 b) {
    	float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return result;
}

float abs_vec3(Vector3 v) {
	return sqrt(dot(v,v));
}


Vector3 vecAdd(Vector3 v1, Vector3 v2){
	Vector3 r = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return r;
}

Vector3 vecSub(Vector3 v1, Vector3 v2){
	Vector3 r = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return r;
}

Vector3 vecScale(Vector3 v, float k){
	Vector3 r = {v.x * k, v.y * k, v.z * k};
	return r;
}

Color colorScale(Color c, float k){
	Color r = {c.r * k, c.g * k, c.b * k, c.a};
	return r;
}

float colorIntensity(Vector3 light, Vector3 normal){
	float i = AMB;
	float l_dot_n = dot(light, normal);
	if (l_dot_n > 0){
    		i = i + (l_dot_n / (abs_vec3(light) * abs_vec3(normal)));
	}
	return i;
}
