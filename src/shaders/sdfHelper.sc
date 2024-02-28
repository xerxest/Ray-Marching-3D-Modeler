// #include "sdfDefinitions.h"
#include "../../build/RuntimeSDFData.sh"

uniform vec4 u_params_test[6]; 

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
	// import GLSL generated from SDF Nodes
	return RootDist(_pos);
}