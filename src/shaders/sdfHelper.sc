#include "sdfDefinitions.h"
// #include "../../build/RuntimeSDFData.sh"

const vec4 u_SD_type[] = {vec4(1,-1,-1,-1),vec4(0,-1,-1,-1),vec4(1,-1,-1,-1),}; 
const vec4 u_SD_position[] = {vec4(5,0,0,-1),vec4(10,0,0,-1),vec4(5,5,0,-1),}; 
const vec4 u_SD_size[] = {vec4(1,1,1,-1),vec4(1,1,1,-1),vec4(1,1,1,-1),}; 
const vec4 u_opps[] = {vec4(9,-1,-1,-1),vec4(9,-1,-1,-1),}; 
const vec4 u_postFix[] = {vec4(13,-1,-1,-1),vec4(13,-1,-1,-1),vec4(12,-1,-1,-1),vec4(13,-1,-1,-1),vec4(12,-1,-1,-1),}; 
const int postFixLength = 5;

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

// Only works if the length is odd
	for (int i = 0; i < 2; i++)
	{

		if(u_postFix[i].x == SDNODE_TYPE) 
		{

			float newDist = -1;

			if(u_SD_type[sdfNodeIndex].x == SDSPHERE)
			{
				newDist = sdSphere(_pos + u_SD_position[sdfNodeIndex].xyz, 
				1.0);
			}

			if(u_SD_type[sdfNodeIndex].x == UDROUNDBOX)
			{
				newDist = udRoundBox(
				_pos + u_SD_position[sdfNodeIndex].xyz,
				vec3 (1.0,1.0,1.0)
				,0.5);
			}
			
			push(sdfStack,newDist);
			sdfNodeIndex++;
		}
		else if (u_postFix[i].x == OPP_TYPE)
		{
			// TODO use opp 
			float d1 , d2;

			d1 = pop(sdfStack);
			d2 = pop(sdfStack);

			push(sdfStack,min(d1,d2));

		}

	}
	
	return pop(sdfStack);

}