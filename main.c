#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>


struct word_keeper {
	char *word;
	int n;
};


int insert(struct word_keeper *Arr, int Arr_size, char *word, int word_size, int left, int right) {
	if (right - left == 1) {
		if (_stricmp(word, Arr[left].word) == 0) {
			Arr[left].n++;
			return 0;
		}
		else {
			if (_stricmp(word, Arr[right].word) == 0) {
				Arr[right].n++;
				return 0;
			}
			else {
				for (int i = Arr_size; i > right; i--) {
					Arr[i] = Arr[i - 1];
				}
				Arr[right].word = (char*)malloc(word_size);
				for (int i = 0; i < word_size; i++) {
					Arr[right].word[i] = word[i];
				}

				Arr[right].n = 1;
				return 1;
			}
		}
	}

	int ret = 0;

	int center = (int)((right - left) / 2 + left);
	if (_stricmp(word, Arr[center].word) < 0) {
		ret = insert(Arr, Arr_size, word, word_size, left, center);
	}
	else {
		if (_stricmp(word, Arr[center].word) > 0) {
			ret = insert(Arr, Arr_size, word, word_size, center, right);
		}
		else {
			Arr[center].n++;
			return 0;
		}
	}
	return ret;
}


int compare(const struct word_keeper * x1, const struct word_keeper * x2) {

	//printf("1) %s\t%d\n2) %s\t%d\n3) %d\n\n", x1->word, x1->n, x2->word, x2->n, x1->n-x2->n); getch();

	return x1->n - x2->n;              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}


int isInvalidSymbol(unsigned char text) {
	return text == ' ' || text == '.' || text == 10 || text == 0 || text == '?' || text == '!'
		|| text == ':' || text == ';' || text == ',' || text == -30 || text == -62 || text == -80
		|| text == '\"'
		|| (text >= 1 && text <= 64) || (text >= 91 && text <= 96) || (text >= 123 && text <= 255);
}

int main(int argc, char *argv[]) {
	int max = 1000;
	struct word_keeper *Arr = (struct word_keeper*)calloc(max, sizeof(struct word_keeper));

	char text;

	char buff[100];


	int N = 0;
	int countSentences = 0;

	int start = 0;


	FILE *inputFile;
	fopen_s(&inputFile, "dune.txt", "r");

	if (inputFile == NULL) {
		printf("Input file not found\n");
		return 1;
	}

	FILE *outputFile;
	fopen_s(&outputFile, "output.txt", "w");
	if (outputFile == NULL) {
		printf("Output file was not created\n");
		return 1;
	}

	for (int i1 = 0, j1 = 0;;) {
		text = getc(inputFile);

		if (text >= 'a' && text <= 'z' || text >= 'A' && text <= 'Z') {
			if (start == 0) {
				start = 1;
			}

			buff[j1] = text;
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

				buff[j1] = 0;
				if (N == 0) {
					Arr[i1].word = (char*)malloc(j1 + 1);
					for (int i = 0; i <= j1; i++) {
						Arr[i1].word[i] = buff[i];
					}

					Arr[i1].n = 1;
					i1++;
					j1 = 0;
					start = 0;
					N++;
					continue;
				}

				int flag_new_word = 0;//0- новое слово 1- найдено в словаре
				if (_stricmp(buff, Arr[0].word) == 0) {//проверка на совпадение с первым словом из словаря
					Arr[0].n++;
					flag_new_word = 0;
				}
				else {
					if (_stricmp(buff, Arr[N - 1].word) == 0) {//проверка на совпадение с последним словом из словаря
						Arr[N - 1].n++;
						flag_new_word = 0;
					}
					else {

						if (_stricmp(buff, Arr[0].word) < 0) {//проверка - расположение слова перед словарем
							for (int i = N; i > 0; i--) {//сдвиг элементов словаря вправо на 1
								Arr[i] = Arr[i - 1];
							}
							Arr[0].word = (char*)malloc(j1 + 1);//на месте первого создание нового слова
							for (int i = 0; i <= j1; i++) {
								Arr[0].word[i] = buff[i];
							}
							Arr[0].n = 1;
							flag_new_word = 1;
						}
						else {
							if (_stricmp(buff, Arr[N - 1].word) > 0) {//проверка - расположение слова после словаря
								Arr[i1].word = (char*)malloc(j1 + 1);//в конце добавляем новое слово
								for (int i = 0; i <= j1; i++) {
									Arr[i1].word[i] = buff[i];
								}
								Arr[i1].n = 1;
								flag_new_word = 1;
							}
							else {//если все условия неверны, то ищем внутри словаря (принцип деления на 2)
								flag_new_word = insert(Arr, N, buff, j1 + 1, 0, N - 1);
							}
						}
					}
				}


				j1 = 0;
				start = 0;
				if (flag_new_word) {//если слова повторное, то наращивать размеры не нужно
					i1++;
					N++;
				}

				if (N == max) {//если память полностью заполнена - выделяем в 2 раза больше
					max = 2 * max;
					Arr = (struct word_keeper*)realloc(Arr, max * sizeof(struct word_keeper));
					//printf("Memory++\n");
				}


			}

		}

		if (text == EOF) break;//проверка на конец файла должна быть в конце цикла, иначе можно пропустить последнее слово в тексте
	}

	qsort(Arr, N, sizeof(struct word_keeper), compare);//сортировка из библиотеки

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
