#include <iostream>

struct Stack {
	int maxsize;
	int pointer;
	int* elements;
};

Stack stack;

void InitializeStack(int maxsize) {
	stack.maxsize = maxsize;
	stack.pointer = 0;
	stack.elements = (int*)malloc(sizeof(int) * maxsize);
}

bool IsEmpty() {
	if (stack.pointer == 0)
		return true;
	return false;
}

bool IsFull() {
	if (stack.pointer == stack.maxsize - 1)
		return true;
	return false;
}

void Push(int element) {
	if (IsFull())
		return;
	stack.elements[stack.pointer++] = element;
}

int Pop() {
	if (IsEmpty())
		return NULL;
	int element = stack.elements[--stack.pointer];
	stack.elements[stack.pointer] = NULL;
	return element;
}

int Top() {
	if (IsEmpty())
		return NULL;
	return stack.elements[stack.pointer - 1];
}

int main()
{
	InitializeStack(50);
	int value;
	Push(5);
	Push(10);
	Push(15);
	value = Pop();
	printf("%d\n", value);
	Push(20);
	value = Top();
	printf("%d\n", value);
	value = Pop();
	printf("%d\n", value);
	Push(20);
}

