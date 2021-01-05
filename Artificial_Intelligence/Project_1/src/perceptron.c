#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * input 차원이 n인 1-layer perceptron을 구현하는 프로그램
 * 컴퓨터과학부 2018920031 유승리
 *
 * x[n]: int
 * w[n]: float (초기 값: 0.0 <= w[n] < 100.0)
 * theta: float (초기 값: 10.0 <= theta < 50.0)
 */

void calculation(int xRow, int xCol, float * w, int ** x, int * out, float theta); 
void showResult(int xRow, int xCol, float * w, int ** x, int * out, float theta, int * pw); 
void modifyValue(int xCol, float * w, float * pt);

int main()
{
	int input; // input의 개수 (n)
	printf("*******************************************************\n");
	printf("*******  n-input AND gate (1-layer perceptron)  *******\n");
	printf("*******************************************************\n");
	printf("\n\n  > the number of inputs : ");
	scanf("%d", &input); // 사용자로부터 input의 개수 입력 받아 저장
	printf("\n");
	
	int i, j;
	int xRow = 1 << input; // == 2^n
	int xCol = input; // == n

	int ** x; // input인 x[2^n][n], int형 2차원 배열
	float * w; // weight인 w[n], float형 1차원 배열
	int * out; // output인 out[2^n], int형 1차원 배열

	int num;
	int wrong = 0; // 틀린 output의 개수
	int * pw = &wrong;
	int cnt = 0; // 맞는 output을 얻을 때까지 값을 수정한 횟수 

	srand(time(NULL)); // 중복 없는 난수 생성을 위해 현재 시간을 seed로 설정
	float theta = (float)(rand() % 400) / 10 + 10; // 랜덤으로 생성되는 초기 theta 값: 10.0 <= theta < 50.0
	float * pt = &theta;

	w = (float *)calloc(input, sizeof(float)); // w[n]을 생성하기 위한 동적 할당 (calloc)
	for (i = 0; i < input; i++)
	{
		w[i] = (float)(rand() % 1000) / 10; // 랜덤으로 생성되는 초기 w[n] 값: 0.0 <= w[n] < 100.0
	}
		
	out = (int *)calloc(xRow, sizeof(int)); // out[2^n]을 생성하기 위한 동적 할당 (calloc)

	x = (int **)calloc(xRow, sizeof(int*)); // x[2^n][n]을 생성하기 위한 동적 할당 (calloc)
	for (i = 0; i < xRow; ++i)
	{
		x[i] = (int *)calloc(xCol, sizeof(int));
	}

	// 10진수 -> 2진수 변환을 이용하여 x[2^n][n]에 input 값을 설정
	for (i = 0; i < xRow; i++)
	{
		num = i;
		for (j = 0; j < xCol; j++)
		{
			x[i][j] = num % 2;
			num = num / 2;

			if (num == 0)
				break;
		}
	}

	// 맞는 output을 얻을 때까지 프로그램 기능 무한 반복
	while (1)
	{
		calculation(xRow, xCol, w, x, out, theta);
		showResult(xRow, xCol, w, x, out, theta, pw);

		if (wrong == 0) // 각 input 별 output이 모두 맞으면 프로그램 종료, 총 시도 횟수 출력
		{
			printf("\n\n\n***********************************************\n");
			printf("*******  We've got a correct AND gate!  *******\n");
			printf("***********************************************\n");
			printf("\n  > the number of attempts : %d\n\n\n", cnt);
			break;
		}
		else
			modifyValue(xCol, w, pt); // w[n], theta 값 수정

		cnt++; // 맞는 output을 얻을 때까지 값을 수정한 횟수 1 증가
		wrong = 0; // 틀린 output의 개수 초기화
	}
	
	// 동적 할당한 메모리 반환
	for (i = 0; i < xRow; i++)
		free(x[i]);
	free(x);
	free(w);
	free(out);

	return 0;
}

// output = f(net)을 구하는 함수
void calculation(int xRow, int xCol, float * w, int ** x, int * out, float theta)
{
	int i, j;
	float mul, sum = 0; // sum == net
	
	for (i = 0; i < xRow; i++)
	{
		// net 값 연산, index가 같은 x와 w를 곱한 값들의 총합
		for (j = 0; j < xCol; j++)
		{
			mul = w[j] * x[i][j];
			sum += mul;
		}

		// net 값과 theta 값과의 비교에 따른 f(net) 연산 
		if (sum > theta)
			out[i] = 1;
		else
			out[i] = 0;
		
		sum = 0;
	}
}

// 연산 결과 표, 틀린 output 개수, 현재 w[n], theta의 값을 보여주는 함수
void showResult(int xRow, int xCol, float * w, int ** x, int * out, float theta, int * pw)
{
	int i, j;
	
	// 결과를 표 형식으로 출력
	printf("\n-------------------  RESULT  -------------------\n\n");
	for (i = xCol; i > 0; i--)
		printf("   X%d  |", i);
	printf("|  out\n");

	for (i = 0; i < xCol + 1; i++)
		printf("--------");
	printf("\n");

	for (i = 0; i < xRow; ++i)
	{
		for (j = xCol - 1; j >= 0; j--)
			printf("   %d   |", x[i][j]);
		printf("|   %d\n", out[i]);
	}

	// out[xRow-1]을 제외한 output이 0이 아닐 때마다 틀린 output의 개수 1 증가
	for (i = 0; i < xRow-1; i++)
	{
		if (out[i] != 0)
			(*pw)++;
	}
	// out[xRow-1]이 1이 아닐 때 틀린 output의 개수 1 증가
	if (out[xRow - 1] != 1)
		(*pw)++;

	printf("\n  > the number of wrong outputs : %d\n", *pw); // 이번 attempt에서 틀린 output의 개수 출력

	// 현재 w[n], theta의 값을 소수점 아래 첫째 자리까지 출력
	printf("\n  > the values of variables : \n\n"); 
	for (i = 0; i < xCol; i++)
		printf("    (%d) W%d = %.1f\n", i + 1, i + 1, w[i]);
	printf("    (%d) theta = %.1f\n", i + 1, theta);
}

// 실수 값을 사용자로부터 입력 받아 w[n], theta 값을 수정하는 함수
void modifyValue(int xCol, float * w, float * pt)
{
	int i;
	int num;
	float mod; // 사용자로부터 수정할 값을 입력 받는 float형 변수

	printf("\n\n\n\n================  MODIFICATION  ================\n\n");
	for (i = 0; i < xCol; i++)
		printf("    (%d) W%d\n", i + 1, i + 1);
	printf("    (%d) theta\n", i + 1);

	// w[n], theta 중 어떤 변수를 무슨 값으로 수정할지 사용자로부터 입력 받는 동작
	printf("\n  > Choose one variable to modify : ");
	scanf("%d", &num);
	printf("\n  > Enter the modified value (ex. 000.0) : "); // float형 변수이므로 정수가 아닌 실수도 입력 가능
	scanf("%f", &mod);

	// 사용자가 theta 값을 수정하는 경우
	if (num == i + 1)
		*pt = mod;

	// 사용자가 w[n] 값을 수정하는 경우
	for (i = 0; i < xCol; i++)
	{
		if (num == i + 1)
			w[i] = mod;
	}

	// 사용자가 실수로 num 값으로 범위에서 벗어난 값을 입력한 경우 다시 반복
	if (num < 1 || num > xCol + 1)
	{
		printf("\n\n  ** Out of range! Please enter a number among the indexes above (");
		for (i = 1; i <= xCol; i++)
			printf("%d, ", i);
		printf("%d). **\n", i);
		modifyValue(xCol, w, pt);
	}
}