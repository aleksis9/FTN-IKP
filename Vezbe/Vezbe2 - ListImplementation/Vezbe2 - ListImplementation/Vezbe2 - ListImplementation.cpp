#include <iostream>
#include <conio.h>

struct Node {
	int id;
	int value;
	Node* next;
};

struct List {
	Node* head;
	Node* tail;
};

void Add(int element);
void Insert(int index, int element);
int ElementAt(int index);
void RemoveAt(int index);
void Clear();
int* ToArray();
void PrintList();

List list;

int main()
{
	list.head = NULL;
	list.tail = NULL;

	PrintList();

	Add(5);
	Add(12);
	Add(20);
	Insert(1, 10);
	Add(30);

	PrintList();
	int* array = ToArray();

	while (*array != NULL) {
		printf("%d ", *array);
		array++;
	}
	printf("\n");

	RemoveAt(1);
	PrintList();

	printf("Element at 2: %d\n", ElementAt(2));

	Clear();
	PrintList();


	//_getch();

}

//list->head se dodeli vrednost novi a kada se izadje iz ove funkcije novi se brise i to postaje neka random vrednost onda
//ako se ne alocira memorija za node sa malloc onda se taj podatak gubi kada se izadje iz funkcije
//mora Node* node jer malloc moze samo na pokazivac da se castuje
void Add(int element) {
	Node* novi = (Node*)malloc(sizeof(Node));
	if (list.head != NULL)
		novi->id = list.tail->id + 1;
	else
		novi->id = 0;
	novi->value = element;
	novi->next = NULL;

	if (list.head == NULL) {
		list.head = novi;
	}
	else {
		list.tail->next = novi;
	}
	list.tail = novi;
}

void Insert(int index, int element) {
	Node* node = list.head;
	while (node != NULL) {
		if (node->id != index) {
			node = node->next;
			continue;
		}
		node->value = element;
		return;
	}
}

int ElementAt(int index) {
	Node* node = list.head;
	while (node != NULL) {
		if (node->id != index) {
			node = node->next;
			continue;
		}
		return node->value;
	}
	return NULL;
}

void RemoveAt(int index) {
	Node* node = list.head;
	while (node != NULL) {
		if (node->id != index - 1) {
			node = node->next;
			continue;
		}
		node->next = node->next->next;
		break;
	}
	node = node->next;
	while (node != NULL) {
		node->id--;
		node = node->next;
	}
}

void Clear() {
	list.head = NULL;
	list.tail = NULL;
}

int* ToArray() {
	int size = 0;
	Node* node = list.head;
	while (node != NULL) {
		node = node->next;
		size++;
	}

	int* array = (int*)malloc(sizeof(size));
	size = 0;
	node = list.head;
	while (node != NULL) {
		array[size] = node->value;
		node = node->next;
		size++;
	}

	array[size] = NULL;
	return array;
}

void PrintList() {
	if (list.head == NULL) {
		printf("Lista je prazna\n");
		return;
	}

	Node* node = list.head;

	while (node != NULL)
	{
		printf("[%d]: %d\n", node->id, node->value);
		node = node->next;
	}
	printf("\n");

}