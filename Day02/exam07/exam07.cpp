// exam07.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	char a;
	//a = 64; // a = @와 같은의미
	printf("문자를 입력 : ");
	scanf_s("%c", &a);
	printf("%d, %c\n", a, a);

    return 0;
}

