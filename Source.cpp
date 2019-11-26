/* Bobitan Denisa - Ioana

   Task:

   Implement correctly and efficiently the Depth-First Search. For graph representation, you should use adjacency lists.
   Also, implement Tarjan's algorithm and Topological sort.

*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#pragma warning(disable : 4996)

Profiler profiler("demo");

int step = 0;
int op;

struct adjListNode
{
	int dest;
	adjListNode *next;
};

struct list
{
	int count;
	adjListNode *first;
};

list *createEmptyList()
{
	list *l = (list*)malloc(sizeof(list));
	l->count = 0;
	l->first = NULL;
	return l;
}

list *l;

adjListNode *createAdjListNode(int dest)
{
	adjListNode *node = (adjListNode*)malloc(sizeof(adjListNode));
	node->dest = dest;
	node->next = NULL;
	return node;
}
struct adjList
{
	adjListNode *head;
	int color; //0 means not visited (white), 1 means in process (gray) and 2 means already visited (black)
	int d; //discovery time
	int f; //finish time
	int low_link; //id of node with smallest id in the current connected component
	bool on_stack; //is 1 if node is on stack
	adjListNode *p; //parent of the node in the dfs traversal tree
};

struct graph
{
	int v;
	adjList *list;
};

struct stack
{
	int count;
	adjListNode *first;
	adjListNode *last;
};

stack *createS()
{
	stack *s = (stack*)malloc(sizeof(stack));
	s->count = 0;
	s->first = s->last = NULL;
	return s;
}

void push(stack *s, int v) //insert at first
{
	adjListNode *node = createAdjListNode(v);
	if (s->first == NULL) //if s empty, insert node
	{
		s->first = s->last = node;
	}
	else //if s not empty, insert first
	{
		node->next = s->first;
		s->first = node;
	}
	s->count++;
}

int pop(stack *s) //delete first
{
	if (s->first)  //if s not empty, delete first
	{
		int aux = s->first->dest;
		s->first = s->first->next;
		s->count--;
		return aux;
	}
	return 0;
}

graph *createGraph(int v)
{
	graph *g = (graph*)malloc(sizeof(graph));
	g->v = v;
	g->list = (adjList*)malloc(v * sizeof(adjList));
	for (int i = 0; i < v; i++)
	{
		g->list[i].head = (adjListNode*)malloc(sizeof(adjListNode));

		g->list[i].color = 0; //white
		g->list[i].d = INT_MAX;
		g->list[i].f = 0;
		g->list[i].p = NULL;
	}
	return g;
}

void insertEdge(graph *g, int src, int dest) //insert the dest node in the adjList of the src node
{
	//directed graph
	adjListNode *node = createAdjListNode(dest);
	adjListNode *aux = g->list[src - 1].head;
	while (aux->next)
	{
		aux = aux->next;
	}
	aux->next = node;
}

graph *generateGraph()
{
	int v = 6;
	graph *g = createGraph(v);

	for (int i = 0; i < v; i++)
	{
		g->list[i].head = createAdjListNode(i + 1);
	}

	insertEdge(g, 1, 2);
	insertEdge(g, 1, 4);
	insertEdge(g, 2, 3);
	insertEdge(g, 3, 4);
	insertEdge(g, 4, 2);
	insertEdge(g, 5, 3);
	insertEdge(g, 5, 6);
	insertEdge(g, 6, 6);

	return g;
}

void displayAdjList(graph *g)
{
	for (int i = 0; i < g->v; i++)
	{
		printf("%d: ", g->list[i].head->dest);
		if (g->list[i].head->next)
		{
			adjListNode *node = g->list[i].head->next;
			while (node)
			{
				printf("%d ", node->dest);
				node = node->next;
			}
		}
		printf("\n");
	}
	printf("\n");
}

void dfsVisit(graph *g, int src, int time, int prt) //if prt is 0, it prints the nodes (for dfs demo), if 1, it populates the list for the topological sort demo
{
	time = time + 1;
	adjListNode *node = createAdjListNode(src);
	g->list[node->dest - 1].color = 1; //gray
	g->list[node->dest - 1].d = time;
	op += 2;
	if (prt == 0)
	{
		printf("%d ", node->dest);
	}
	adjListNode *aux = g->list[src - 1].head->next;
	op++;

	op++;
	while (aux)
	{
		op++;
		if (g->list[aux->dest - 1].color == 0) //if white
		{
			g->list[aux->dest - 1].p = g->list[node->dest].head;
			op++;
			dfsVisit(g, aux->dest, time, prt);
		}
		aux = aux->next;
		op++;

		op++;
	}
	g->list[node->dest - 1].color = 2; //black
	op++;
	time = time + 1;
	g->list[node->dest - 1].f = time;
	op++;
	if (prt == 1)
	{
		adjListNode *x = createAdjListNode(node->dest);
		x->next = l->first;
		l->first = x;
		l->count++;
	}
}

void dfs(graph *g, int src, int prt)
{
	if (prt == 0)
	{
		printf("DFS traversal: ");
	}
	else
	{
		l = createEmptyList();
	}
	int time = 0;
	for (int i = 0; i < g->v; i++)
	{
		if (g->list[i].color == 0)
		{
			dfsVisit(g, g->list[i].head->dest, time, prt);
		}
	}
	if (prt == 0)
	{
		printf("\n");
	}
}

void demoDFS()
{
	graph *g = generateGraph();
	displayAdjList(g);
	dfs(g, g->list[0].head->dest, 0); //dfs prints the nodes
	printf("\n");
	free(g);
}

void sccVisit(graph *g, int i, stack *s)
{
	adjListNode *node = createAdjListNode(i + 1);
	g->list[i].d = g->list[i].low_link = ++step;
	g->list[i].on_stack = true;
	push(s, node->dest);
	adjListNode *aux = g->list[i].head->next;
	while (aux)
	{
		if (g->list[aux->dest - 1].d == -1)
		{
			sccVisit(g, aux->dest - 1, s);
			g->list[node->dest - 1].low_link = min(g->list[node->dest - 1].low_link, g->list[aux->dest - 1].low_link);
		}
		else
		{
			if (g->list[aux->dest - 1].on_stack)
			{
				g->list[node->dest - 1].low_link = min(g->list[node->dest - 1].low_link, g->list[aux->dest - 1].d);
			}
		}
		aux = aux->next;
	}
	if (g->list[node->dest - 1].low_link == g->list[node->dest - 1].d)
	{
		printf("CC: ");
		while (s->first->dest != node->dest) //g->list[s->first->dest - 1].low_link == g->list[node->dest - 1].low_link
		{
			printf("%d ", s->first->dest);
			g->list[s->first->dest - 1].on_stack = false;
			pop(s);
		}
		printf("%d ", s->first->dest);
		printf("\n");
		g->list[node->dest - 1].on_stack = false;
		pop(s);
	}
}

void scc(graph *g) //Tarjan's algorithm
{
	stack *s = createS();
	for (int i = 0; i < g->v; i++)
	{
		g->list[i].low_link = -1;
		g->list[i].d = -1;
		g->list[i].on_stack = false;
	}
	for (int i = 0; i < g->v; i++)
	{
		if (g->list[i].d == -1)
		{
			sccVisit(g, i, s);
		}
	}
}

void demoTarjan()
{
	graph *g = generateGraph();
	scc(g);
	printf("\n");
	free(g);
}

void printList()
{
	printf("List: ");
	adjListNode *node = l->first;
	while (node)
	{
		printf("%d ", node->dest);
		node = node->next;
	}
	printf("\n");
}

void demoTopologicalSort()
{
	graph *g = createGraph(6);
	for (int i = 0; i < 6; i++) 
	{
		g->list[i].head = createAdjListNode(i + 1);
	}
	insertEdge(g, 6, 3);
	insertEdge(g, 6, 1);
	insertEdge(g, 5, 1);
	insertEdge(g, 5, 2);
	insertEdge(g, 3, 4);
	insertEdge(g, 4, 2);
	dfs(g, g->list[0].head->dest, 1); //dfs generates the sorted list
	printList();
	printf("\n");
	free(g);
}

boolean isUnique(int *ed, graph *g)
{
	adjListNode *aux = g->list[ed[0] - 1].head->next;
	while (aux != NULL)
	{
		if ((ed[0] == g->list[ed[0] - 1].head->dest && ed[1] == aux->dest) || (ed[1] == g->list[ed[0] - 1].head->dest && ed[0] == aux->dest))
		{
			return false;
		}
		aux = aux->next;
	}
	return true;
}

graph *generateRandGraph(int v, int e)
{
	graph *g = createGraph(v);
	for (int i = 0; i < v; i++)
	{
		g->list[i].head = createAdjListNode(i + 1);
	}
	int *ed = (int*)malloc(2 * sizeof(int));
	int k = 0;
	while (k < e)
	{
		FillRandomArray(ed, 2, 1, v, true, 0); //generate pairs of nodes which represent an edge
		if (isUnique(ed, g)) //if edge is unique, insert it in the graph
		{
			insertEdge(g, ed[0], ed[1]);
			k++;
		}
	}
	return g;
}

void report()
{
	int v;
	int e;
	graph *g;

	v = 100; //v fixed
	for (e = 1000; e <= 4500; e += 100)
	{
		g = generateRandGraph(v, e);
		op = 0;
		dfs(g, g->list[0].head->dest, 2);
		profiler.countOperation("V_fixed_E_variable", e, op);
	}

	e = 4500; //e fixed
	for (v = 100; v <= 200; v += 10)
	{
		g = generateRandGraph(v, e);
		op = 0;
		dfs(g, g->list[0].head->dest, 2);
		profiler.countOperation("V_variable_E_fixed", v, op);
	}

	free(g);

	profiler.showReport();
}

int main()
{
	int x = 1;
	while (x != 0)
	{
		scanf("%d", &x);
		if (x == 1)
		{
			demoDFS();
		}
		if (x == 2)
		{
			demoTarjan();
		}
		if (x == 3)
		{
			demoTopologicalSort();
		}
		if (x == 4)
		{
			report();
		}
	}

	return 0;
}