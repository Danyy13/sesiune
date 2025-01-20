#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 20
#define PRIORITIES_NUMBER 8
#define MAX_STACK_SIZE 10

typedef struct queueNode {
	char processName[MAX_STRING_SIZE];
	struct queueNode* next;
}QueueNode;

typedef struct {
	QueueNode* head;
	QueueNode* tail;
}Queue;

typedef struct {
	int priority;
	Queue queue;
}Priorities;

typedef struct {
	char processName[MAX_STRING_SIZE];
	int priority;
}Stack;

void initQueue(Queue* queue) {
	queue->head = queue->tail = NULL;
}

void initPriorities(Priorities priorities[], unsigned int size) {
	for (int i = 0;i < size;i++) {
		priorities[i].priority = i + 1;
		initQueue(&(priorities[i].queue));
	}
}

void printQueue(Queue* queue) {
	QueueNode* node = queue->head;

	while (node != NULL) {
		printf("%s ", node->processName);
		node = node->next;
	}

	printf("\n");
}

void printPriorities(Priorities priorities[], unsigned int size) {
	for (int i = 0;i < size;i++) {
		printf("P%d - ", priorities[i].priority);
		printQueue(&(priorities[i].queue));
	}
}

QueueNode* newQueueNode(char* processName) {
	QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
	if (!node) {
		fprintf(stderr, "Nu s-a putut aloca memorie pentru nodul cozii\n");
		exit(-1);
	}

	strcpy(node->processName, processName);
	node->next = NULL;

	return node;
}

void enqueue(Queue* queue, QueueNode* node) {
	if (queue->head == NULL) {
		queue->head = queue->tail = node;
	} else {
		queue->tail->next = node;
		queue->tail = node;
	}
}

void enqueueByPriority(Priorities priorities[], int priority, QueueNode* node) {
	enqueue(&(priorities[priority - 1].queue), node); // priority - 1 pentru ca array-ul incepe indexarea de la 0 si prioritatile de la 1
}

QueueNode* dequeue(Queue* queue) {
	if (queue->head == NULL) {
		printf("Lista este goala\n");
	} else if(queue->head == queue->tail) {
		QueueNode* dequeuedNode = queue->head;
		queue->head = queue->tail = NULL;
		return dequeuedNode;
	} else {
		QueueNode* dequeuedNode = queue->head;
		queue->head = queue->head->next;
		return dequeuedNode;
	}
}

void push(Stack stack[], int* stackSize, int priority, char *processName) {
	if (stackSize == MAX_STACK_SIZE - 1) {
		printf("Stiva este plina\n");
		return;
	}

	++(*stackSize);

	stack[*stackSize].priority = priority;
	strcpy(stack[*stackSize].processName, processName);
}

Stack* pop(Stack stack[], int* stackSize) {
	if (*stackSize == -1) {
		//printf("Stiva este goala\n");
		return NULL;
	}

	return &(stack[(*stackSize)--]);
}

void moveFromStackToQueue(Priorities priorities[], Stack stack[], int *stackSize) {
	Stack* popped = NULL;

	while ((popped = pop(stack, stackSize)) != NULL) { // golim stiva
		QueueNode* nodeToAdd = newQueueNode(popped->processName);

		enqueue(&(priorities[popped->priority - 1].queue), nodeToAdd); // punem nodul in coada prioritatii pe care o are elementul din stiva
	}
}

void printStack(Stack stack[], int stackSize) {
	printf("Stiva: ");
	for (int i = 0;i <= stackSize;i++) {
		printf("%s cu P%d ", stack[i].processName, stack[i].priority);
	}
	printf("\n");
}

int main(void) {
	Priorities priorities[PRIORITIES_NUMBER];
	unsigned int size = PRIORITIES_NUMBER;

	Stack stack[MAX_STACK_SIZE];
	int stackSize = -1;

	initPriorities(priorities, size);

	QueueNode* process1 = newQueueNode("process1");
	QueueNode* process2 = newQueueNode("process2");
	QueueNode* process3 = newQueueNode("process3");

	enqueueByPriority(priorities, 1, process1);
	enqueueByPriority(priorities, 1, process2);
	enqueueByPriority(priorities, 2, process3);
	
	//dequeue(&(priorities[1].queue));

	printPriorities(priorities, size);

	push(stack, &stackSize, 2, "process4");
	push(stack, &stackSize, 1, "process3");
	push(stack, &stackSize, 5, "process5");

	printStack(stack, stackSize);
	printf("\n");

	moveFromStackToQueue(priorities, stack, &stackSize);

	printPriorities(priorities, size);

	return 0;
}