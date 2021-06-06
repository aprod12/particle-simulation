#pragma once

struct vec4 {

	float x, y, z, w;
	vec4(float x_, float y_ , float z_, float w_) : x(x_), y(y_), z(z_), w(w_){}
	vec4() : x(0), y(0), z(0), w(1) {}

	
};

vec4 operator+(vec4 v1, vec4 v2){

	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

int dot(vec4 v1, vec4 v2){

	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}