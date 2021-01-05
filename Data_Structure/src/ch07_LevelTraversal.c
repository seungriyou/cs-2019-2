#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode {
	int data;
	struct TreeNode *left, *right;
} TreeNode;

// ========= 원형 큐 코드 시작 =========
#define MAX_QUEUE_SIZE 100
typedef TreeNode * element;
typedef struct {
	element data[MAX_QUEUE_SIZE];
	int front, rear;
} QueueType;

// 오류 함수
void error(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

// 초기화
void init_queue(QueueType *q)
{
	q->front = q->rear = 0;
}

// 공백 상태 검출 함수
int is_empty(QueueType *q)
{
	return (q->front == q->rear);
}

// 포화 상태 검출 함수
int is_full(QueueType *q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

// 삽입 함수
void enqueue(QueueType *q, element item)
{
	if (is_full(q))
		error("큐가 포화상태입니다");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}

// 삭제 함수
element dequeue(QueueType *q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

// ========= 레벨 순회 코드 시작 =========
void level_order(TreeNode *ptr)
{
	QueueType q;
	init_queue(&q);
	if (ptr == NULL) return;
	enqueue(&q, ptr);
	while (!is_empty(&q)) {
		ptr = dequeue(&q);
		printf(" [%d] ", ptr->data);
		if (ptr->left)
			enqueue(&q, ptr->left);
		if (ptr->right)
			enqueue(&q, ptr->right);
	}
}

// ========= 최댓값 탐색 코드 시작 =========
int findMax(TreeNode *ptr)
{
	// base case
	if (ptr == NULL)
		return INT_MIN; // int형의 최솟값으로 반환

	// recursion
	int max = ptr->data;
	int maxL = findMax(ptr->left);
	int maxR = findMax(ptr->right);

	if (maxL > max)
		max = maxL;
	if (maxR > max)
		max = maxR;
	return max;
}

// ========= 최솟값 탐색 코드 시작 =========
int findMin(TreeNode *ptr)
{
	// base case
	if (ptr == NULL)
		return INT_MAX; // int형의 최댓값으로 반환

	// recursion
	int min = ptr->data;
	int minL = findMin(ptr->left);
	int minR = findMin(ptr->right);

	if (minL < min)
		min = minL;
	if (minR < min)
		min = minR;
	return min;
}

// ========= 트리 선언 =========
TreeNode n1 = { 2, NULL, NULL };
TreeNode n2 = { 12, &n1, NULL };
TreeNode n3 = { 1, NULL , NULL };
TreeNode n4 = { 7, &n3, &n2 };
TreeNode n5 = { 9, NULL, NULL };
TreeNode n6 = { 5, &n5, &n4 };
TreeNode n7 = { 3, NULL, NULL };
TreeNode n8 = { 11, &n7, NULL };
TreeNode n9 = { 4, NULL, &n8 };
TreeNode n10 = { 8, &n6, &n9 };
TreeNode *root = &n10;
/*
				8
			 /	   \
		   5		 4
		 /	   \	   \
		9		7		11
			 /	   \	/
		    1	   12  3
			      /
				 2
*/

int main(void)
{
	printf("1) 최소값 = %d\n", findMin(root));
	printf("2) 최대값 = %d\n", findMax(root));
	printf("3) 레벨 순회 =");
	level_order(root);
	printf("\n\n");
	return 0;
}