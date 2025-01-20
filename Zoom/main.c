#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_LENGTH 30
#define NAME_LENGTH 30

typedef struct student {
	char name[NAME_LENGTH];
	struct student* next;
	struct student* prev;
}Student;

typedef struct {
	Student* head;
	Student* tail;
}WaitingRoom; // lista cu studentii din fiecare conferinta

typedef struct meeting{
	int id;
	char password[PASSWORD_LENGTH];
	WaitingRoom waitingRoom;
	struct meeting* next;
	struct meeting* prev;
}Meeting;

typedef struct meetingList {
	Meeting* head;
	Meeting* tail;
}MeetingList;

void initMeetingList(MeetingList* list) {
	list->head = list->tail = NULL;
}

void initWaitingRoom(WaitingRoom* list) {
	list->head = list->tail = NULL;
}

Student* newStudent(char* name) {
	Student* node = (Student*)malloc(sizeof(Student));
	if (!node) {
		fprintf(stderr, "Nu s-a putut aloca memorie pentru student\n");
		exit(-1);
	}

	strcpy(node->name, name);
	node->next = NULL;
	node->prev = NULL;

	return node;
}

void appendStudent(WaitingRoom* list, Student* node) {
	if (list->head == NULL) {
		list->head = list->tail = node;
	} else if(strcmp(node->name, list->head->name) < 0) { // daca trebuie sa vina primul
		list->head->prev = node;
		node->next = list->head;
		list->head = node;
	} else if (strcmp(node->name, list->tail->name) > 0) { // daca trebuie sa vina ultimul
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	} else {
		Student* traverser = list->head;

		while (traverser != NULL) {
			if (strcmp(node->name, traverser->name) < 0) {
				node->next = traverser;
				node->prev = traverser->prev->prev;
				traverser->prev->next = node;
				traverser->prev = node;
				return;
			}
			traverser = traverser->next;
		}
	}
}

void printStudents(WaitingRoom* list) {
	Student* node = list->head;

	while (node != NULL) {
		printf("%s ", node->name);
		node = node->next;
	}
	
	printf("\n");
}

Meeting* newMeeting(int id, char* password, WaitingRoom waitingRoom) {
	Meeting* node = (Meeting*)malloc(sizeof(Meeting));
	if (!node) {
		fprintf(stderr, "Nu s-a putut aloca memorie pentru conferinta\n");
		exit(-2);
	}

	strcpy(node->password, password);
	node->id = id;
	node->waitingRoom = waitingRoom;
	node->next = NULL;
	node->prev = NULL;

	return node;
}

void appendMeeting(MeetingList* list, Meeting* node) {
	if (list->head == NULL) {
		list->head = list->tail = node;
	}
	else {
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
}

void printMeetings(MeetingList* list) {
	Meeting* meetingNode = list->head;
	Student* studentNode = NULL;

	while (meetingNode != NULL) {
		printf("Conferinta %d: ", meetingNode->id);

		studentNode = meetingNode->waitingRoom.head;
		printStudents(&studentNode);
		meetingNode = meetingNode->next;
	}

	printf("\n");
}

void removeStudent(WaitingRoom* list, Student* student) {
	if (list->head == NULL) {
		printf("Lista este goala\n");
	} else if (list->head == list->tail && student == list->head) { // un singur element
		free(student);
		list->head = list->tail = NULL;
	} else if (student->prev == NULL) { // primul element
		student->next->prev = NULL;
		list->head = student->next;
		student->next = NULL;
		free(student);
	} else if (student->next == NULL) { // ultimul element
		student->prev->next = NULL;
		list->tail = student->prev;
		student->prev = NULL;
		free(student);
	} else {
		student->prev->next = student->next;
		student->next->prev = student->prev;
		student->next = student->prev = NULL;
		free(student);
	}
}

void acceptStudents(MeetingList* list, char firstLetter) {
	Meeting* meetingNode = list->head;
	Student* studentNode = NULL;

	while (meetingNode != NULL) {
		studentNode = meetingNode->waitingRoom.head;
		
		while (studentNode != NULL) {
			if (studentNode->name[0] == firstLetter) {
				Student* studentToRemove = studentNode;
				studentNode = studentNode->next;
				removeStudent(&(meetingNode->waitingRoom), studentToRemove);
			} else {
				studentNode = studentNode->next;
			}
		}

		meetingNode = meetingNode->next;
	}
}

int main(void) {
	MeetingList meetings;
	WaitingRoom room1;
	WaitingRoom room2;
	WaitingRoom room3;

	initMeetingList(&meetings);
	initWaitingRoom(&room1);
	initWaitingRoom(&room2);
	initWaitingRoom(&room3);

	appendStudent(&room1, newStudent("levi"));
	appendStudent(&room1, newStudent("larisa"));
	appendStudent(&room1, newStudent("andra"));
	appendStudent(&room1, newStudent("alexandra"));
	appendStudent(&room1, newStudent("andrei"));

	appendStudent(&room2, newStudent("tania"));
	appendStudent(&room2, newStudent("brianna"));
	appendStudent(&room2, newStudent("noemi"));
	appendStudent(&room2, newStudent("alina"));
	appendStudent(&room2, newStudent("teo"));

	appendStudent(&room3, newStudent("matei"));
	appendStudent(&room3, newStudent("paul"));
	appendStudent(&room3, newStudent("andi"));
	appendStudent(&room3, newStudent("nusa"));
	appendStudent(&room3, newStudent("razvan"));


	appendMeeting(&meetings, newMeeting(123, "ok", room1));
	appendMeeting(&meetings, newMeeting(101, "bine", room2));
	appendMeeting(&meetings, newMeeting(001, "rau", room3));


	printMeetings(&meetings);

	acceptStudents(&meetings, 'a');

	printMeetings(&meetings);

	return 0;
}