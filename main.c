#include <stdio.h>
#include <malloc.h>

struct Words {
	char *word;
	int n;
};

void quickSort(struct Words *numbers, int left, int right){
	int pivot; // разрешающий элемент
	int l_hold = left;
	int r_hold = right;
	pivot = numbers[left].n;
	char *pivot_w = numbers[left].word;

	while (left < right){ // пока границы не сомкнутся
		while ((numbers[right].n >= pivot) && (left < right))
			right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
		if (left != right){
			numbers[left].n = numbers[right].n; // перемещаем элемент [right] на место разрешающего
			numbers[left].word = numbers[right].word;
			left++; // сдвигаем левую границу вправо
		}
		while ((numbers[left].n <= pivot) && (left < right))
			left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
		if (left != right){ // если границы не сомкнулись
			numbers[right].n = numbers[left].n; // перемещаем элемент [left] на место [right]
			numbers[right].word = numbers[left].word;
			right--; // сдвигаем правую границу вправо
		}
	}
	numbers[left].n = pivot; // ставим разрешающий элемент на место
	numbers[left].word = pivot_w;
	pivot = left;
	left = l_hold;
	right = r_hold;
	if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
		quickSort(numbers, left, pivot - 1);
	if (right > pivot)
		quickSort(numbers, pivot + 1, right);
}

int isValidSymbol(char text){
	return text == ' ' || text == '.' || text == 10 || text == 0 || text == '?' || text == '!'
		|| text == ':' || text == ';' || text == ',' || text == -30 || text == -62 || text == -80
		|| text == '\"'
		|| (text >= 1 && text <= 64) || (text >= 91 && text <= 96) || (text >= 123 && text <= 255);
}

int main(int argc, char *argv[]){
	int max = 1000;
	struct Words *Arr = (struct Words*)malloc(max * sizeof(struct Words));

	char text;

	int N = 0;
	int S_N = 0;

	int start = 0;

	int flag = -1;


	FILE *inputFile = fopen("dune.txt", "r");
	if (inputFile == NULL){
		printf("Input file not found\n");
		return 1;
	}

	FILE *outputFile = fopen("answer.txt", "w");

	for (int i1 = 0, j1 = 0; ;){
		text = getc(inputFile);//printf("%c", text);

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
			if ((text == '.' || text == '!' || text == '?') && start){
				S_N++;
			}

			if (!isValidSymbol(text)){
				printf("ERROR)%d\n", text);
				return 0;
			}//else{printf("!%c", text);}

////////////ВООООТ//////
            if(start){
////////////ВООООТ//////

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

			for (int i = 0; i < N; i++){
				if (Arr[i].word[0] == Arr[i1].word[0] || Arr[i].word[0] == Arr[i1].word[0] - 32 || Arr[i].word[0] - 32 == Arr[i1].word[0]) {
					for (int j = 1;; j++){
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
				if (N == max){
					struct Words *p = (struct Words*)malloc(max * 2 * sizeof(struct Words));
					max = 2 * max;
					for (int i = 0; i < N; i++)
						p[i] = Arr[i];

					free(Arr);
					Arr = p;
				}
			}

////////////ВООООТ//////
            }
////////////ВООООТ//////

		}
	}

	quickSort(Arr, 0, N - 1);

	fprintf(outputFile, "Total sentences: %d\n", S_N);

	for (int i = 0; i < N; i++){
		fprintf(outputFile, "\n%s\t%d", Arr[i].word, Arr[i].n);
	}
	fflush(outputFile);

	fclose(inputFile);
	fclose(outputFile);

	for(int i=0;i<N;i++)
		free(Arr[i].word);
	free(Arr);

	return 0;
}
