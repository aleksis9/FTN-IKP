#include <iostream>
#include <stdlib.h>
#include <Winsock2.h>

float* Calculate(char* buffer);

int main()
{
	char op;
	char buffer[9];
	char operation;
	int val1, val2;

	printf("Enter an operator (+, -, *, /): ");
	scanf_s("%c", &op);
	printf("Enter two operands: ");
	scanf_s("%d %d", &val1, &val2);

	buffer[0] = op;
	*(int*)(buffer + 1) = htonf(val1);
	*(int*)(buffer + 5) = htonf(val2);
	float* rez = Calculate(buffer);
	printf("Rezultat = %f", *rez);

	return 0;
}

float* Calculate(char* buffer) {
	float* ret_val = (float*)malloc(sizeof(float));

	char kod_operacije = buffer[0];
	int operand1 = ntohf(*(int*)(buffer + 1));
	int operand2 = ntohf(*(int*)(buffer + 5));

	switch (kod_operacije) {
	case '+': { *ret_val = operand1 + operand2; break; }
	case '-': { *ret_val = operand1 - operand2; break; }
	default: 
		break;
	}

	return ret_val;
}