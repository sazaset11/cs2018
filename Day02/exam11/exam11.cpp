// exam11.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{	
	printf("integer size : %d\n", sizeof(int));
	printf("character size : %d\n", sizeof(char));
	printf("double size : %d\n", sizeof(double));

	int _temp;

	printf("_temp size : %d\n", sizeof(_temp));

	char a = -1;
	unsigned char b = a;

	printf("%d %d\n", a, b);

    return 0;
}

