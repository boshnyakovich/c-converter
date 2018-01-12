#include "word_keeper.h"
#include <stdio.h>
#include <malloc.h>

void quickSort(struct word_keeper *numbers, int left, int right) { 
	int buffer;
	int l_hold = left;
	int r_hold = right;
	buffer = numbers[left].n;
	char *buffer_w = numbers[left].word;

	while (left < right) {
		while ((numbers[right].n >= buffer) && (left < right))
			right--;
		if (left != right) {
			numbers[left].n = numbers[right].n;
			numbers[left].word = numbers[right].word;
			left++;
		}
		while ((numbers[left].n <= buffer) && (left < right))
			left++;
		if (left != right) {
			numbers[right].n = numbers[left].n;
			numbers[right].word = numbers[left].word;
			right--;
		}
	}
	numbers[left].n = buffer;
	numbers[left].word = buffer_w;
	buffer = left;
	left = l_hold;
	right = r_hold;
	if (left < buffer)
		quickSort(numbers, left, buffer - 1);
	if (right > buffer)
		quickSort(numbers, buffer + 1, right);
}

int isInvalidSymbol(char text) {
	return text == ' ' || text == '.' || text == 10 || text == 0 || text == '?' || text == '!'
		|| text == ':' || text == ';' || text == ',' || text == -30 || text == -62 || text == -80
		|| text == '\"'
		|| (text >= 1 && text <= 64) || (text >= 91 && text <= 96) || (text >= 123 && text <= 255);
}

int main(int argc, char *argv[]) {
	int max = 1000;
	struct word_keeper *Arr = (struct word_keeper*)malloc(max * sizeof(struct word_keeper));

	char text;

	int N = 0;
	int countSentences = 0;

	int start = 0;

	int flag = -1;


	FILE *inputFile = fopen("dune.txt", "r"); 
	if (inputFile == NULL) {
		printf("Input file not found\n");
		return 1;
	}

	FILE *outputFile = fopen("output.txt", "w");

	for (int i1 = 0, j1 = 0; ;) {
		text = getc(inputFile);

		if (text == EOF) break;

		if (text >= 'a' && text <= 'z' || text >= 'A' && text <= 'Z') {
			if (start == 0) {
				Arr[i1].word = (char*)malloc(50 * sizeof(char));
				start = 1;
			}

			Arr[i1].word[j1] = text;
			j1++;
		}
		else {
			if ((text == '.' || text == '!' || text == '?') && start) {
				countSentences++;
			}

			if (!isInvalidSymbol(text)) {
				printf("ERROR)%d\n", text);
				return 0;
			}//else{printf("!%c", text);}

			if (start) {

				Arr[i1].word[j1] = 0;
				if (N == 0) {
					//Arr_N[i1]=1;
					Arr[i1].n = 1;
					i1++;
					j1 = 0;
					start = 0;
					N++;
					continue;
				}

				for (int i = 0; i < N; i++) {
					if (Arr[i].word[0] == Arr[i1].word[0] || Arr[i].word[0] == Arr[i1].word[0] - 32 || Arr[i].word[0] - 32 == Arr[i1].word[0]) {
						for (int j = 1;; j++) {
							if (Arr[i].word[j] != Arr[i1].word[j]) break;
							if (Arr[i].word[j] == 0 && Arr[i1].word[j1] == 0) {
								flag = i;
								break;
							}
						}
					}
					if (flag != -1) {
						Arr[flag].n++;
						flag = 999999;
						break;
					}
				}
				
				if (flag == 999999) {
					j1 = 0;
					flag = -1;
					start = 0;
				}
				else {
					Arr[i1].n = 1;
					i1++;
					j1 = 0;
					start = 0;
					N++;
					if (N == max) {
						struct word_keeper *p = (struct word_keeper*)malloc(max * 2 * sizeof(struct word_keeper));
						max = 2 * max;
						for (int i = 0; i < N; i++)
							p[i] = Arr[i];

						free(Arr);
						Arr = p;
					}
				}

			}

		}
	}
	quickSort(Arr, 0, N - 1);

	fprintf(outputFile, "Total sentences: %d\n", countSentences);

	for (int i = 0; i < N; i++) {
		fprintf(outputFile, "\n%s\t%d", Arr[i].word, Arr[i].n);
	}

	fclose(inputFile);
	fclose(outputFile);

	for (int i = 0; i<N; i++)
		free(Arr[i].word);
	free(Arr);

	return 0;
}
