#include <stdio.h>

typedef enum {
	RED,
	BLUE,
	GREEN,
	PINK
}Colors;

int colors[4] = { RED, BLUE, GREEN, PINK }; // 0 1 2 3

#define MAX_ROWS 10
#define MAX_COLS 10

#define GET_ROW k/cols
#define GET_COL k%cols

int map[MAX_ROWS * MAX_COLS];
int rows = 0;
int cols = 0;

void initMap() {
	for (int i = 0;i < rows * cols;i++) {
		map[i] = -1;
	}
}

int checkUpperLeftCorner(int k) {
	if (GET_ROW - 1 < 0 || GET_COL - 1 < 0) return 1; // daca nu pot accesa diagonala trec mai departe in backtracking si merg cu return 1 ca sa pot incrementa k
	if (map[k] == map[k - cols - 1]) return 0;
	return 1;
}

int checkUpperRightCorner(int k) {
	if (GET_ROW - 1 < 0 || GET_COL + 1 >= MAX_COLS - 1) return 1; // daca nu pot accesa diagonala trec mai departe in backtracking si merg cu return 1 ca sa pot incrementa k
	if (map[k] == map[k - cols + 1]) return 0;
	return 1;
}

int checkLowerLeftCorner(int k) {
	if (GET_ROW + 1 >= MAX_ROWS - 1 || GET_COL - 1 < 0) return 1; // daca nu pot accesa diagonala trec mai departe in backtracking si merg cu return 1 ca sa pot incrementa k
	if (map[k] == map[k + cols - 1]) return 0;
	return 1;
}

int checkLowerRightCorner(int k) {
	if (GET_ROW + 1 >= MAX_ROWS - 1 || GET_COL + 1 >= MAX_COLS - 1) return 1; // daca nu pot accesa diagonala trec mai departe in backtracking si merg cu return 1 ca sa pot incrementa k
	if (map[k] == map[k + cols + 1]) return 0;
	return 1;
}

int valid(int k) {
	if (!checkUpperLeftCorner(k) || !checkUpperRightCorner(k) || !checkLowerRightCorner(k) || !checkLowerLeftCorner(k)) return 0;
	return 1;
}

int solutie(int k) {
	return k == rows * cols - 1;
}

void printMap(int map[], int rows, int cols) {
	// afisez matriceal array-ul map
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			printf("%d ", map[i * cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void back(int k) {
	for (int colorsIndex = 0;colorsIndex < 4;colorsIndex++) {
		map[k] = colors[colorsIndex];
		if (valid(k)) {
			if (solutie(k)) {
				printMap(map, rows, cols);
			}
			else {
				back(k + 1);
			}
		}
		map[k] = -1;
	}
}

int main(void) {

	printf("Numar de randuri: ");
	if (scanf("%d", &rows) != 1) {
		printf("Nu s-a citit bine rows\n");
	}

	printf("Numar de coloane: ");
	if (scanf("%d", &cols) != 1) {
		printf("Nu s-a citit bine cols\n");
	}

	//printMap(map, rows, cols);

	initMap();

	back(0);
	
	return 0;
}