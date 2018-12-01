#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

struct myPuzzle {
	int *puzzle;
	int puzzleSize;
	int puzzleColumnAndRowSize;
	int current, move;
};

struct myPuzzle mp;

int *originalPuzzle;
int count = 0;
int maxCount = 0;
int currentRow, currentColumn;
int moveRow, moveColumn;


int getCoordinate(int key);
int finishCheck();
void exchangePuzzle();

void puzzleDisplay();
unsigned char getKey();
void init(int);
void user(char *);
void printscore(char *);
int main(int argc, char *argv[]) {
	int k1 = 0, k2 = 0, a, i, mode;
	char set[100], *res, Date[50], Time[50], scoresave[128] = { 0 };
	char *name;
	FILE *fp, *ps;
	fp = fopen("PuzzleConfig.txt", "r");
	if (fp == NULL) {
		fp = fopen("PuzzleConfig.txt", "w");
		if (fp == NULL) {
			printf("파일을 만들지 못했습니다.");
			return 1;
		}
		fputs("#################################\n", fp);
		fputs("#\tpuzzle config\t\t#\n", fp);
		fputs("#################################\n", fp);
		fputs("SIZE: 3\n", fp);
		fputs("MODE: GAME\n", fp);
		fputs("SCORE: PuzzleScore.txt", fp);
	}
	fclose(fp);




	mp.puzzleColumnAndRowSize = 3;
	fp = fopen("PuzzleConfig.txt", "r");
	while (1) {
		res = fgets(set, sizeof(set), fp);
		if (res == NULL) break;
		if (strncmp("SIZE: ", set, 6) == 0) {
			mp.puzzleColumnAndRowSize = atoi(&set[6]);
		}

		else if (strncmp("MODE: ", set, 6) == 0) {
			if (strncmp("TEST", &set[6], 4) == 0) mode = 0;
			else mode = 1;
		}
		else if (strncmp("SCORE: ", set, 6) == 0) {
			strcpy(scoresave, &set[7]);
		}
	}
	ps = fopen(scoresave, "r");
	if (ps == NULL) {
		ps = fopen(scoresave, "w");
		if (ps == NULL) {
			printf("점수 파일을 만들지 못했습니다.");
			return 1;
		}
		fputs("date/time\t\tname\t\tscore\n", ps);
	}
	fclose(ps);
	mp.puzzleSize = mp.puzzleColumnAndRowSize*mp.puzzleColumnAndRowSize;


	printf("Puzzle Game: by 박준형<17019031>\n");
	mp.puzzle = (int(*))malloc(mp.puzzleSize * sizeof(int));
	originalPuzzle = (int *)malloc(mp.puzzleSize * sizeof(int));
	if (mp.puzzle == 0 || originalPuzzle == 0) {
		printf("메모리가 부족합니다.\n");
		return 0;
	}


	printf("횟수제한을 입력해주세요:");
	scanf_s("%d", &maxCount);                                                   //최대횟수 입력
	getchar();

	while (1) {
		count = 0;
		init(mode);
		system("cls");
		puzzleDisplay();

		while (1) {

			count++;
			k1 = getKey();
			if (k1 == 'q') {

				free(mp.puzzle);
				free(originalPuzzle);
				return 0;
			}
			if (k1 == 'r') break;
			if (k1 == 's') {
				while (1) {
					printscore(scoresave);
					if (_getch() == 's') break;
				}
			}
			system("cls");
			getCoordinate(k1);
			exchangePuzzle();
			puzzleDisplay();
			if (finishCheck() == 0) break;
			if (finishCheck() == 1) {
				printf("\nYou Win!!!\n");

				user(scoresave);

				break;
			}

		}

		printf("다시 시도하시겠습니까? R or r");                                 //다시 플레이
		k1 = getKey();
		if (k1 != 'r'&&k1 != 'R') {

			free(mp.puzzle);
			free(originalPuzzle);
			fclose(fp);
			return 0;
		}
	}
}

void init(int mode) {


	int r, i, j;

	if (mode == 1) {

		srand(time(NULL));
		for (i = 0; i < mp.puzzleSize; i++) {
			r = rand() % (mp.puzzleSize);
			mp.puzzle[i] = r;
			for (j = 0; j < i; j++) {
				if (mp.puzzle[j] == r) {
					i--;
				}
			}


		}
	}
	else if (mode == 0) {
		for (i = 0; i < mp.puzzleSize - 2; i++) mp.puzzle[i] = i + 1;
		mp.puzzle[i] = 0;
		mp.puzzle[i + 1] = mp.puzzleSize - 1;
	}
	for (i = 0; i < mp.puzzleSize; i++) {
		if (mp.puzzle[i] == 0) {
			mp.move = (i);
		}
		if (i + 1 > mp.puzzleSize - 1) originalPuzzle[i] = 0;
		else originalPuzzle[i] = i + 1;
	}
}

unsigned char getKey() {
	int m;
	m = _getch();
	if (m == 224) m = _getch();

	return m;
}

void puzzleDisplay() {
	int i, j, k;

	printf("Puzzle Game: by 박준형<17019031>\n");

	printf("┌");
	for (k = 0; k < mp.puzzleColumnAndRowSize - 1; k++) printf("─┬");
	printf("─┐\n");
	for (i = 0; i<mp.puzzleColumnAndRowSize; i++) {
		printf("│");

		for (j = 0; j<mp.puzzleColumnAndRowSize; j++) {

			if (mp.puzzle[i * mp.puzzleColumnAndRowSize + j] == 0) printf("* │");
			else printf("%2d│", mp.puzzle[i * mp.puzzleColumnAndRowSize + j]);
			if (j == mp.puzzleColumnAndRowSize - 1) printf("\n");

		}
		if (i < mp.puzzleColumnAndRowSize - 1) {
			printf("├");
			for (k = 0; k<mp.puzzleColumnAndRowSize - 1; k++) printf("─┼");
			printf("─┤\n");
		}


	}
	printf("└");
	for (i = 0; i < mp.puzzleColumnAndRowSize - 1; i++) printf("─┴");
	printf("─┘\n");

	printf("\n\ncount:%d\tMax: %d", count, maxCount);

	printf("\n\nCoordinate: <%d,%d> -> <%d,%d>", (mp.current) % mp.puzzleColumnAndRowSize, (mp.current) / mp.puzzleColumnAndRowSize, (mp.move) % mp.puzzleColumnAndRowSize, (mp.move) / mp.puzzleColumnAndRowSize);
}

int getCoordinate(int key) {

	mp.current = mp.move;
	switch (key) {

	case 72:				//up


		if (mp.current>mp.puzzleColumnAndRowSize - 1) mp.move = mp.current - mp.puzzleColumnAndRowSize;
		break;
	case 75:					//left


		if ((mp.current % mp.puzzleColumnAndRowSize)>0) mp.move = mp.current - 1;
		break;
	case 77:						//right


		if ((mp.current % mp.puzzleColumnAndRowSize)<mp.puzzleColumnAndRowSize - 1) mp.move = mp.current + 1;
		break;
	case 80:						//down


		if (mp.current<mp.puzzleSize - mp.puzzleColumnAndRowSize) mp.move = mp.current + mp.puzzleColumnAndRowSize;
		else moveColumn = currentColumn;
		break;

	default:

		break;
	}
}

void exchangePuzzle() {
	int temp = 0;

	temp = mp.puzzle[mp.move];
	mp.puzzle[mp.move] = mp.puzzle[mp.current];
	mp.puzzle[mp.current] = temp;



}
int finishCheck() {
	int i = 0, j = 0;

	for (i = 0; i<mp.puzzleSize; i++) {
		if (mp.puzzle[i] != originalPuzzle[i]) j = 1;
	}
	if (maxCount <= count) {
		printf("\n You Lose..\n");

		return 0;
	}
	else if (j == 0) {
		return 1;
	}


}
void user(char *scoresave) {
	char Date[10], Time[10], name[10];
	FILE *fp;
	printf("이름을 입력하세요");
	scanf("%s", &name);
	_strdate(Date);
	_strtime(Time);
	fp = fopen(scoresave, "a");
	fprintf(fp, "%s:%s\t%s\t\t%d\n", Date, Time, name, count);
	fclose(fp);
}
void printscore(char *scoresave) {
	FILE *fp;
	char str[128];
	fp = fopen(scoresave, "r");
	system("cls");
	if (fp == NULL) printf("파일 못열었어요 ㅠㅠ");
	while (1) {
		if (fgets(str, sizeof(str), fp) == EOF) break;
		printf("%s\n", str);
	}
}
