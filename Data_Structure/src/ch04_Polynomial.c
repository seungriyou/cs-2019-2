#include <stdio.h>
#include <stdlib.h>

typedef int element;

typedef struct ListNode {
	element data;
	struct ListNode *link;
} ListNode;

ListNode* insert_last(ListNode *head,  element value)
{
	ListNode *node = (ListNode *)malloc(sizeof(ListNode));
	node->data = value;
	node->link = NULL;
	
	if (head == NULL)
	{
		head = node;
	}
	else
	{
		ListNode *p = head;
		while (p->link != NULL)
			p = p->link;
		p->link = node;
	}
	
	return head;
}

ListNode* alternate(ListNode *listA, ListNode *listB)
{
	ListNode *listC = NULL;
	ListNode *p1, *p2;
	
	if (listA == NULL)
		return listB;
	else if (listB == NULL)
		return listA;
	else
	{
		p1 = listA;
		p2 = listB;

		while ((p1 != NULL) || (p2 != NULL))
		{
			if (p1 != NULL)
			{
				listC = insert_last(listC, p1->data);
				p1 = p1->link;
			}
			if (p2 != NULL)
			{
				listC = insert_last(listC, p2->data);
				p2 = p2->link;
			}
		}
		return listC;
	}
}

void print_list(ListNode *head)
{
	for (ListNode *p = head; p != NULL; p = p->link)
	{
		printf("%d->", p->data);
	}
	printf("NULL \n");
}

int main(void)
{
	ListNode *listA = NULL;
	ListNode *listB = NULL;
	ListNode *listC = NULL;

	for (int i = 1; i <= 9; i += 2)
	{
		listA = insert_last(listA, i);
	}

	for (int i = 2; i <= 14; i += 2)
	{
		listB = insert_last(listB, i);
	}

	listC = alternate(listA, listB);

	print_list(listA);
	print_list(listB);
	print_list(listC);

	free(listA);
	free(listB);
	free(listC);

	return 0;
}