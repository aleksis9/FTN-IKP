#include <iostream>

struct Queue {
	int maxsize;
	int pointer;
	int* elements;
};

Queue queue;

void InitializeQueue(int maxsize) {
	queue.maxsize = maxsize;
	queue.pointer = 0;
	queue.elements = (int*)malloc(maxsize * sizeof(int));
}

bool IsEmpty() {
	if (queue.pointer == 0)
		return true;
	return false;
}

bool IsFull() {
	if (queue.pointer == queue.maxsize - 1)
		return true;
	return false;
}

void Enqueue(int element) {
	if (IsFull())
		return;
	queue.elements[queue.pointer++] = element;
}

void MoveToLeft() {
	for (int i = 0; i < queue.pointer - 1; i++) {
		queue.elements[i] = queue.elements[i + 1];
	}
	queue.elements[queue.pointer] = NULL;
	queue.pointer--;
}

int Dequeue() {
	if (IsEmpty())
		return NULL;
	int element = queue.elements[0];
	MoveToLeft();
	return element;
}

int main()
{
	InitializeQueue(20);
	int value;
	Enqueue(5);
	Enqueue(10);
	Enqueue(15);
	value = Dequeue();
	printf("%d\n", value);
	Enqueue(20);
	value = Dequeue();
	printf("%d\n", value);
	value = Dequeue();
	printf("%d\n", value);
	Enqueue(20);
	value = Dequeue();
	printf("%d\n", value);
	value = Dequeue();
	printf("%d\n", value);
}
