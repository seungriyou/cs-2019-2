#include <stdio.h>
#define ROWS 6
#define COLS 6
#define MAX_TERMS 36
/*
* 희소 행렬 표현 방법 #2를 이용한 두 배열의 합을 구하는 프로그램
* 컴퓨터과학부 2018920031 유승리
*
* main(void): 두 배열의 내용을 입력하고 함수 실행
* addSparseMatrix(SparseMatrix a, SparseMatrix b): 희소 행렬 표현 방법 #2를 이용하여 두 배열 a, b의 합을 구하는 함수
* showResult(SparseMatrix m): 결과 배열 m을 2차원 matrix 형식으로 출력하는 함수
*/

typedef struct {
	int row;
	int col;
	int value;
} element;

typedef struct {
	element data[MAX_TERMS];
	int rows;	// 행의 개수
	int cols;	// 열의 개수
	int terms;	// 항의 개수
} SparseMatrix;

// 희소 행렬 표현 방법 #2를 이용하여 두 배열 a, b의 합을 구하는 함수
// c = a + b
SparseMatrix addSparseMatrix(SparseMatrix a, SparseMatrix b)
{
	SparseMatrix c;
	int ca = 0, cb = 0, cc = 0;	// 각 배열의 항목을 가리키는 index

	// 배열 a와 배열 b의 크기가 같은지 확인
	if (a.rows != b.rows || a.cols != b.cols)
	{
		printf("두 희소행렬의 크기가 같지 않습니다!\n");
		exit(1);
	}

	c.rows = a.rows;
	c.cols = a.cols;
	c.terms = 0;

	// 배열 a와 배열 b의 항의 개수보다 index가 작을 때
	while (ca < a.terms && cb < b.terms)
	{
		// 각 항목의 순차적인 번호를 계산
		int inda = a.data[ca].row*a.cols + a.data[ca].col;
		int indb = b.data[cb].row*b.cols + b.data[cb].col;

		// 배열 a 항목이 배열 b 항목보다 앞에 있는 경우
		if (inda < indb)
		{
			c.data[cc++] = a.data[ca++];
		}
		// 배열 a 항목과 배열 b 항목이 같은 위치에 있는 경우
		else if (inda == indb)
		{
			// 배열 a 항목과 배열 b 항목의 값의 합이 0이 아닌 경우
			if ((a.data[ca].value + b.data[cb].value) != 0)
			{
				c.data[cc].row = a.data[ca].row;
				c.data[cc].col = a.data[ca].col;
				c.data[cc++].value = a.data[ca++].value + b.data[cb++].value;
			}
			else
			{
				ca++;
				cb++;
			}
		}
		// 배열 b 항목이 배열 a 항목보다 앞에 있는 경우
		else
			c.data[cc++] = b.data[cb++];
	}

	// 배열 a와 배열 b에 남아 있는 항들을 배열 c로 복사
	for (; ca < a.terms;)
		c.data[cc++] = a.data[ca++];
	for (; cb < b.terms; )
		c.data[cc++] = b.data[cb++];
	c.terms = cc;

	return c;
}

// 결과 배열 m을 2차원 matrix 형식으로 출력하는 함수
void showResult(SparseMatrix m)
{
	int indm = 0;
	int i, j, k;

	printf(" > 두 Matrix의 합은 다음과 같습니다.\n");
	printf("  ┌ ");
	for (k = 0; k < m.cols; k++)
		printf("   ");
	printf("┐\n");

	for (i = 0; i < m.rows; i++)
	{
		for (j = 0; j < m.cols; j++)
		{
			if (i != 0 || i != m.rows - 1)
			{
				if (j == 0)
					printf("  │ ");
			}
			
			if (i == m.data[indm].row && j == m.data[indm].col)
			{
				printf("%2d ", m.data[indm++].value);
			}
			else
				printf(" 0 ");
		}
		printf("│\n");
	}

	printf("  └ ");
	for (k = 0; k < m.cols; k++)
		printf("   ");
	printf("┘\n");
}

// main 함수
int main(void)
{
	// 두 배열의 값 입력
	SparseMatrix m1 = { {{0,0,1}, {1,1,1}, {2,1,1}, {3,2,1}, {4,3,1}, {5,1,2}, {5,3,-1}}, 6,6,7 };
	SparseMatrix m2 = { {{0,0,1}, {1,0,1}, {2,1,1}, {3,2,1}, {4,0,1}, {5,0,1}}, 6,6,6 };

	SparseMatrix m3;
	m3 = addSparseMatrix(m1, m2);
	showResult(m3);

	return 0;
}