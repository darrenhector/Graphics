#include "matrix.h"
#include "utility.h"
#include <stdio.h>
#include <math.h>

mat4 MatrixMultiply(const mat4 left,const mat4 right)
{
	mat4 res;
	int i;
	for(i = 0; i < 4; ++i)
	{
		int j;
		for(j = 0; j < 4; ++j)
		{
			float sum = 0.0f;
			int k;
			for(k = 0;k < 4; ++k)
			{
				sum += right.m[i * 4 + k] * left.m[j + 4 * k];
			}
			res.m[i * 4 + j ] = sum;
		}
	}
	return res;
}
mat4 translate(const mat4 mat,float x,float y,float z)
{
	mat4 res = IDENTITY_MATRIX;
	res.m[12] = x;
	res.m[13] = y;
	res.m[14] = z;
	
	return MatrixMultiply(mat,res);
}

mat4 scale(const mat4 mat,float x,float y, float z)
{
	mat4 res = IDENTITY_MATRIX;
	res.m[ 0] = x;
	res.m[ 5] = y;
	res.m[10] = z;
	
	return MatrixMultiply(mat,res);
}

void print_matrix(const mat4 m)
{
	int i;
	for(i = 0;i != 4; ++i)
	{
		int j;
		for(j = 0;j != 4;++j)
		{
			printf("%f ",m.m[i * 4 + j]);
		}
		putchar('\n');
	}
}

mat4 RotateAboutX(const mat4 m, float angle)
{
  mat4 rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[5] = cosine;
  rotation.m[6] = -sine;
  rotation.m[9] = sine;
  rotation.m[10] = cosine;

  return MatrixMultiply(m, rotation);
}

mat4 RotateAboutY(const mat4 m, float angle)
{
  mat4 rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[0] = cosine;
  rotation.m[2] = -sine;
  rotation.m[8] = sine;
  rotation.m[10] = cosine;

  return MatrixMultiply(m, rotation);
}

mat4 RotateAboutZ(const mat4 m, float angle)
{
  mat4 rotation = IDENTITY_MATRIX;
  float sine = (float)sin(angle);
  float cosine = (float)cos(angle);
  
  rotation.m[0] = cosine;
  rotation.m[1] = -sine;
  rotation.m[4] = sine;
  rotation.m[5] = cosine;

  return MatrixMultiply(m, rotation);
}
/*97*/
