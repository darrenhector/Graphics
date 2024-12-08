#ifndef MATRIX_H
#define MATRIX_H

typedef struct mat4
{
	float m[16];
}mat4;

static const mat4 IDENTITY_MATRIX = {{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
}};

static const mat4 ZERO_MATRIX = {{
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
}};

mat4 MatrixMultiply(const mat4 left,const mat4 right);

mat4 translate(const mat4 mat,float x,float y,float z);

mat4 scale(const mat4 mat,float x,float y, float z);

void print_matrix(const mat4 m);

mat4 RotateAboutX(const mat4 m, float angle);

mat4 RotateAboutY(const mat4 m, float angle);

mat4 RotateAboutZ(const mat4 m, float angle);

struct vec2{
	float x,y;
};

struct vec3{
	float x,y,z;
};

struct uvec3{
	unsigned x,y,z;
};
#endif
