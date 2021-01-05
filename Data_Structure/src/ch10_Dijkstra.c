#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100
#define INF 1000000	/* 무한대 (연결이 없는 경우) */

typedef struct GraphType {
	int n;	// 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

int distance[MAX_VERTICES];
int found[MAX_VERTICES];
int order[MAX_VERTICES];	/* 최단경로의 정점 순서 기록 */


int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void print_status(GraphType* g)
{
	static int step = 1;
	printf("> STEP %d\n", step++);
	printf(" distance: ");
	for (int i = 0; i < g->n; i++) {
		if (distance[i] == INF)
			printf(" * ");
		else
			printf("%2d ", distance[i]);
	}
	printf("\n");
	printf(" found: ");
	for (int i = 0; i < g->n; i++)
		printf("%2d ", found[i]);
	printf("\n\n");
}

void shortest_path(GraphType* g, int start)
{
	int i, u, w;
	for (i = 0; i < g->n; i++)	/* 초기화 */
	{
		distance[i] = g->weight[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE;
	distance[start] = 0;
	order[0] = start;	/* 최단경로 순서의 첫번쩨로 시작 정점을 저장 */
	for (i = 0; i < g->n - 1; i++) {
		print_status(g);
		u = choose(distance, g->n, found);
		found[u] = TRUE;
		order[i + 1] = u; /* 최단경로 순서에 다음 정점을 저장 */
		for (w = 0; w < g->n; w++)
			if (!found[w])
				if (distance[u] + g->weight[u][w] < distance[w])
					distance[w] = distance[u] + g->weight[u][w];
	}
	for (int j = 0; j < g->n; j++) {
		if(found[j] == FALSE)
			order[i] = j; /* 최단경로 순서에 found[] 값이 FALSE인 정점, 즉 남은 정점을 저장 */
	}
}

int main(void)
{
	int i;
	printf("================================\n");
	printf("   Dijkstra 최단경로 알고리즘\n");
	printf("================================\n\n");

	GraphType g = {8,
	{{ 0, INF, INF, INF, INF, INF, INF, INF },
	{ 300, 0, INF, INF, INF, INF, INF, INF },
	{ 1000, 800, 0, INF, INF, INF, INF, INF },
	{ INF, INF, 1200, 0, INF, INF, INF, INF },
	{ INF, INF, INF, 1500, 0, 250, INF, INF },
	{ INF, INF, INF, 1000, INF, 0, 900, 1400 },
	{ INF, INF, INF, INF, INF, INF, 0, 1000 },
	{ 1700, INF, INF, INF, INF, INF, INF, 0 }} 
	};
	char* name[MAX_VERTICES] = {
		"로스엔젤레스", "샌프란시스코", "덴버", "시카고", "보스턴", "뉴욕", "마이애미", "뉴올리언즈"
	};	/* 정점의 이름 */

	shortest_path(&g, 4); /* 보스턴에서 출발 */

	printf(">> ORDER\n ");	/* 최단경로 순서대로 정점 출력 */
	for (i = 0; i < g.n - 1; i++) {
		printf("%s -> ", name[order[i]]);
	}
	printf("%s\n\n", name[order[i]]);
	return 0;
}