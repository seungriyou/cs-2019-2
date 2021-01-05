#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 50
#define SWAP(x, y ,t) ((t) = (x), (x) = (y), (y) = (t))

int list[MAX_SIZE];
int n;

int partition(int list[], int left, int right)
{
	int pivot, temp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = list[left];
	do {
		do
			low++;
		while (list[low] < pivot);
		do
			high--;
		while (list[high] > pivot);
		if (low < high) SWAP(list[low], list[high], temp);
	} while (low < high);

	SWAP(list[left], list[high], temp);
	return high;
}

void quick_sort(int list[], int left, int right)
{
	if (left < right) {
		int q = partition(list, left, right);
		quick_sort(list, left, q - 1);
		quick_sort(list, q + 1, right);
	}
}

int main(void)
{
	int i;
	n = MAX_SIZE;
	srand(time(NULL));
	for (i = 0; i < n; i++) // 1~10000 사이의 난수 생성 및 list에 넣기
		list[i] = (rand() % 10000) + 1;

	printf("============================================================================\n");
	printf(" 퀵 정렬 알고리즘으로 1~10000 사이의 난수 50개를 오름차순 정렬하는 프로그램\n");
	printf("============================================================================\n\n");
	printf(" > 오름차순 정렬 전:\n  ");
	for (i = 0; i < n; i++)
		printf("%d  ", list[i]);
	printf("\n\n");
	quick_sort(list, 0, n - 1); // 퀵 정렬
	printf(" > 오름차순 정렬 후:\n  ");
	for (i = 0; i < n; i++)
		printf("%d  ", list[i]);
	printf("\n\n");

	return 0;
}