$input v_color0, v_texcoord0

/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

// References:
// Sphere tracing: a geometric method for the antialiased ray tracing of implicit surfaces - John C. Hart
// http://web.archive.org/web/20110331200546/http://graphics.cs.uiuc.edu/~jch/papers/zeno.pdf
//
// Modeling with distance functions
// http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

#include "../../bgfx.cmake/bgfx/examples/common/common.sh"
#include "iq_sdf.sh"

// TODO move to shared file
#define SDF_BOX 0.0
#define SDF_SPHERE 1.0

#define SDF_OPP_FLAG 0.0
#define SDF_NODE_FLAG 1.0

#define SDF_MIN 0.0
#define SDF_MAX 1.0

struct sdfNode
{
	float type;
	float v1, v2, v3, v4, v5;
};

uniform mat4 u_mtx;
uniform vec4 u_lightDirTime;
uniform vec4 u_size;
uniform vec4 u_sdfData[2];
uniform vec2 u_operationData[1];
uniform vec4 u_postfixData[3];


#define u_lightDir u_lightDirTime.xyz
#define u_time     u_lightDirTime.w

void getSdfData(inout struct sdfNode data[2])
{
	// TODO 
	data[0].type = SDF_BOX;
	data[0].v1 = 1.0;
	data[0].v2 = 1.0;
	data[0].v3 = 1.0;
	data[0].v4 = 1.0;
	data[0].v5 = 1.0;

	data[1].type = SDF_SPHERE;
	data[1].v1 = 2.0;
	data[1].v2 = 0.0;
	data[1].v3 = 0.0;
	data[1].v4 = 1.0;
	data[1].v5 = 1.0;

}

void getPostFixData(inout float data[3])
{
	// TODO 
	data[0] = SDF_NODE_FLAG;
	data[1] = SDF_NODE_FLAG;
	data[2] = SDF_OPP_FLAG;
}

#define MAX_STACK_SIZE 100

struct Stack {
    float data[MAX_STACK_SIZE];
    int top;
};

void initStack(inout Stack stack) {
    stack.top = -1;
}

bool isStackEmpty(inout Stack stack) {
    return stack.top == -1;
}

bool isStackFull(inout Stack stack) {
    return stack.top == MAX_STACK_SIZE - 1;
}

void push(inout Stack stack, in float value) {
    if (!isStackFull(stack)) {
        stack.data[++stack.top] = value;
    }
}

float pop(inout Stack stack) {
    if (!isStackEmpty(stack)) {
        return stack.data[stack.top--];
    }
    return 0.0;  // Default value for an empty stack
}

float sceneDist(vec3 _pos)
{
	int sdfNodeIndex = 0;
	int oppNodeIndex = 0;
	struct Stack sdfStack;
	initStack(sdfStack);

	struct sdfNode sdfData[2];
	getSdfData(sdfData);

	float postfixData[3] = {SDF_NODE_FLAG,SDF_NODE_FLAG,SDF_OPP_FLAG};
	// getPostFixData(postfixData);

	// for each element in bool postfix array 
	for (int i = 0; i < 3; i++)
	{
		// if Sdf node 
		if(postfixData[i] == SDF_NODE_FLAG)
		{
			// eval push ot stack
			int newDist = -1;

			if(sdfData[sdfNodeIndex].type == SDF_SPHERE)
			{
				newDist = sdSphere(_pos + vec3( 3.0, 0.0, 0.0), 
				1.0);
			}

			if(sdfData[sdfNodeIndex].type == SDF_BOX)
			{
				newDist = udRoundBox(
				_pos,
				vec3 (sdfData[sdfNodeIndex].v1,sdfData[sdfNodeIndex].v2, sdfData[sdfNodeIndex].v3)
				,0.5);
			}
			
			push(sdfStack,newDist);
			sdfNodeIndex++;
		}
		else
		{
			// TODO use opp 
			int newDist, d1 , d2;

			d1 = pop(sdfStack);
			d2 = pop(sdfStack);

			newDist = min(d1,d2);

			push(sdfStack,newDist);

		}

	}
	// TODO fix normal
	return pop(sdfStack);
}

vec3 calcNormal(vec3 _pos)
{
	const vec2 delta = vec2(0.002, 0.0);
	float nx = sceneDist(_pos + delta.xyy) - sceneDist(_pos - delta.xyy);
	float ny = sceneDist(_pos + delta.yxy) - sceneDist(_pos - delta.yxy);
	float nz = sceneDist(_pos + delta.yyx) - sceneDist(_pos - delta.yyx);
	return normalize(vec3(nx, ny, nz) );
}

float calcAmbOcc(vec3 _pos, vec3 _normal)
{
	float occ = 0.0;
	float aostep = 0.2;
	for (int ii = 1; ii < 4; ii++)
	{
		float fi = float(ii);
		float dist = sceneDist(_pos + _normal * fi * aostep);
		occ += (fi * aostep - dist) / pow(2.0, fi);
	}

	return 1.0 - occ;
}

float trace(vec3 _ray, vec3 _dir, float _maxd)
{
	float tt = 0.0;
	float epsilon = 0.001;

	for (int ii = 0; ii < 64; ii++)
	{
		float dist = sceneDist(_ray + _dir*tt);
		if (dist > epsilon)
		{
			tt += dist;
		}
	}

	return tt < _maxd ? tt : 0.0;
}

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

float fresnel(float _ndotl, float _bias, float _pow)
{
	float facing = (1.0 - _ndotl);
	return max(_bias + (1.0 - _bias) * pow(facing, _pow), 0.0);
}

vec4 lit(float _ndotl, float _rdotv, float _m)
{
	float diff = max(0.0, _ndotl);
	float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
	return vec4(1.0, diff, spec, 1.0);
}

void main()
{
	vec4 tmp;
	tmp = mul(u_mtx, vec4(v_texcoord0.xy, 0.0, 1.0) );
	vec3 eye = tmp.xyz/tmp.w;

	tmp = mul(u_mtx, vec4(v_texcoord0.xy, 1.0, 1.0) );
	vec3 at = tmp.xyz/tmp.w;

	float maxd = length(at - eye);
	vec3 dir = normalize(at - eye);

	float dist = trace(eye, dir, maxd);

	if (dist > 0.5)
	{
		vec3 pos = eye + dir*dist;
		vec3 normal = calcNormal(pos);

		vec2 bln = blinn(u_lightDir, normal, dir);
		vec4 lc = lit(bln.x, bln.y, 1.0);
		float fres = fresnel(bln.x, 0.2, 5.0);

		float val = 0.9*lc.y + pow(lc.z, 128.0)*fres;
		val *= calcAmbOcc(pos, normal);
		val = pow(val, 1.0/2.2);

		gl_FragColor = vec4(val, val, val, 1.0);
		gl_FragDepth = dist/maxd;
	}
	else
	{
		gl_FragColor = v_color0;
		gl_FragDepth = 1.0;
	}
}
