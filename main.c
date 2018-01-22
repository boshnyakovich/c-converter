//переместить текст Дюны в ту же папку, что и программка
//поотлаживать, комп. в папку debug

#include "word_keeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int isInvalidSymbol(unsigned char text) { 
	return text == ' ' || text == '.' || text == 10 || text == 0 || text == '?' || text == '!'
		|| text == ':' || text == ';' || text == ',' || text == -30 || text == -62 || text == -80
		|| text == '\"'
		|| (text >= 1 && text <= 64) || (text >= 91 && text <= 96) || (text >= 123 && text <= 255);
}

/*
//логичнее было бы сделать валид символ и проверять букву, число или дефис, а не нагромождать такой огромной конструкцией
int isValidSymbol(char text) {
	return text = 
}
*/

int main(int argc, char *argv[]) {
	int max = 1000;
	struct word_keeper *Arr = (struct word_keeper*)calloc(100, max * sizeof(struct word_keeper));//еще почитать об этой конструкции

	char text;

	int N = 0; //Надо учиться правильно называть переменные. Посмотреть код на предмет сложных таких переменных
	int countSentences = 0;

	int start = 0;

	int flag = -1;


	FILE *inputFile = fopen("dune.txt", "r"); //это и аутпут надо как то сразу в меин объявить чтобы с консоли вводить нужное значение
	if (inputFile == NULL) {
		printf("Input file not found\n");
		return 1;
	}

	FILE *outputFile = fopen("output.txt", "w");
	//у инпута есть проверка, а тут нет. не хорошо. вдруг у нас недостаточно прав что-то записывать

	for (int i1 = 0, j1 = 0; ;) {
		text = getc(inputFile);//printf("%c", text);
		//а еще getc нету в новом стандарте языка. надо поискать ему замену

		if (text == EOF) break;

		if (text >= 'a' && text <= 'z' || text >= 'A' && text <= 'Z') {
			if (start == 0) {
				Arr[i1].word = (char*)calloc(100, 50 * sizeof(char));
				start = 1;
				/*
				get_next_word
				будет как-то принимать стартовое значение и значение где заканчивается и разница - число и чило равно количеству букв
				зная длину слова можно выделять столько сколько надо
				а то вдруг слово на 51 символ
				и жалко на предлоги 50 чаров выделять
				*/
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
			}

			if (start) {

				Arr[i1].word[j1] = 0;
				if (N == 0) {
					Arr[i1].n = 1;
					i1++;
					j1 = 0;
					start = 0;
					N++;
					continue;
				}

				for (int i = 0; i < N; i++) {
					if (Arr[i].word[0] == Arr[i1].word[0] || Arr[i].word[0] == Arr[i1].word[0] - 32 || Arr[i].word[0] - 32 == Arr[i1].word[0]) { 
						//если вгрузить более большой текст прога будет тратить время с геометрической прогрессией
						//может в массив выгрузить и сортировать бинарным поиском
						//если у нас допустим слово вот такое "А" оно слишком долго будет гулять по этому условию
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
						struct word_keeper *p = (struct word_keeper*)malloc(max * 2 * sizeof(struct word_keeper)); //эту и следущую поменять местами тут и там действия повторяются не понятно зачем
						//и мемккопи должна быть тут
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
	qsort(char* word, N, int N, char compare);/*
		  void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*))
		  первый элемент сорт. масс, 
		  количество элементов указ. в мас, 
		  размер в байтах каждого элемента в массиве,
		  compar - функция, которая сравнивает два элемента
		  
		  
		  функия массив водкиперов
		  создает массивы чаров
		  указатель на первый ворд
		  char*
		  char**
		  
		  2м элтом передаю fread
		  */);

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
