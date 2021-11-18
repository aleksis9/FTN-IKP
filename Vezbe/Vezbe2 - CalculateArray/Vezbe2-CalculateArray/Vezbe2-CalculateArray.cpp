#include <iostream>

float* CalculateArray(char* buffer);

int main()
{
    std::cout << "Hello World!\n";

	int koliko;
	printf("Unesite koliko brojeva zelite:");
	scanf_s("%d", &koliko);
	char* buffer = (char*)malloc(sizeof(int) + koliko * sizeof(char));

	*(int*)buffer = koliko;

	for (int i = 0; i < koliko; i++) {
		int num;
		printf("Unesite broj[%d]: ", i + 1);
		scanf_s("%d", &num);
		*(buffer + 4 + i) = (char)num;
	}

	float * avg_and_sum = CalculateArray(buffer);

	printf("Prosecna vrednost: %.2f\nSuma brojeva: %.2f\n", avg_and_sum[0], avg_and_sum[1]);

	free(buffer);
	free(avg_and_sum);

	return 0;
}

float* CalculateArray(char* buffer) {
	int numberOfValues = *(int*)buffer;
	float* ret_val = (float*)malloc(2*sizeof(float));

	float sum = 0;
	float avg = 0;

	for (int i = 0; i < numberOfValues; i++) {
		sum += buffer[4 + i];
	}
	avg = sum / (float)numberOfValues;

	ret_val[0] = avg;
	ret_val[1] = sum;

	return ret_val;
}