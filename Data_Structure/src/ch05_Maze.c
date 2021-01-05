#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAZE_SIZE 10

// ==== 스택 코드의 시작 ====
typedef struct {
	short r;
	short c;
} element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

// 스택 초기화 함수
void init_stack(StackType *s)
{
	s->top = -1;
}

// 공백 상태 검출 함수
int is_empty(StackType *s)
{
	return (s->top == -1);
}

// 포화 상태 검출 함수
int is_full(StackType *s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}

// 삽입 함수
void push(StackType *s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

// 삭제 함수
element pop(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

// 피크 함수
element peek(StackType *s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}
// ==== 스택 코드의 끝 ====


element here = { 1,0 }, entry = { 1,0 };

char maze[MAZE_SIZE][MAZE_SIZE] = {
	{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
{ '0', '0', '0', '0', '1', '0', '0', '0', '0', '1' },
{ '1', '0', '0', '0', '1', '0', '0', '0', '0', '1' },
{ '1', '0', '1', '1', '1', '0', '0', '1', '0', '1' },
{ '1', '0', '0', '0', '1', '0', '0', '1', '0', '1' },
{ '1', '0', '1', '0', '1', '0', '0', '1', '0', '1' },
{ '1', '0', '1', '0', '1', '0', '0', '1', '0', '1' },
{ '1', '0', '1', '0', '1', '0', '0', '1', '0', '1' },
{ '1', '0', '1', '0', '0', '0', '0', '1', '0', 'x' },
{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1' },
};

// 위치를 스택에 삽입
void push_loc(StackType *s, int r, int c)
{
	if (r < 0 || c < 0) return;
	if (maze[r][c] != '1' && maze[r][c] != '.') {
		element tmp;
		tmp.r = r;
		tmp.c = c;
		push(s, tmp);
	}
}

// 미로를 화면에 출력한다.
void maze_print(char maze[MAZE_SIZE][MAZE_SIZE])
{
	printf("\n");
	for (int r = 0; r < MAZE_SIZE; r++) {
		for (int c = 0; c < MAZE_SIZE; c++) {
			//printf("%c", maze[r][c]);
			if (maze[r][c] == '1')
				printf("■");
			else if (maze[r][c] == '.')
				printf("v ");
			else if (maze[r][c] == 'x')
				printf(">>");
			else
				printf("  ");
		}
		printf("\n");
	}
}

int main(void)
{
	int r, c;
	StackType s;

	init_stack(&s);
	here = entry;
	while (maze[here.r][here.c] != 'x') {
		r = here.r;
		c = here.c;
		maze[r][c] = '.';
		maze_print(maze);
		push_loc(&s, r - 1, c);
		push_loc(&s, r + 1, c);
		push_loc(&s, r, c - 1);
		push_loc(&s, r, c + 1);

		if (is_empty(&s)) {
			printf("미로 탐색에 실패하였습니다!\n");
			return;
		}
		else
			here = pop(&s);
	}
	printf("\n미로 탐색에 성공하였습니다!\n");
	printf("(현재 위치 및 경로: v, 출구: >>)\n\n");
	return 0;
}