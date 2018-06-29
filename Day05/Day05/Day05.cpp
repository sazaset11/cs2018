// Day05.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	char word[100];
	int i = 0;
	bool check = false;

	scanf_s("%s", word, sizeof(word));

	while (word[i]) {
		if (word[i] == '/'&&word[i + 1] == '*') {
			check = true;
			i += 2;
			printf("/*");
		}
		if (word[i] == '*'&&word[i + 1] == '/') {
			check = false;
			i += 2;
			printf("*/");
			if (!word[i])	break;
		}
		if (check) {
			printf(" ");
		}
		else {
			printf("%c", word[i]);
		}
		i++;
	}
	printf("\n");

    return 0;
}

