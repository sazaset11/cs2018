// exam01.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

int sum(int a, int b) {
	return a + b;
}

void _b() {
	printf("it is b\n");
}

void _a() {
	printf("it is a\n");
	_b();
}

int main()
{
	printf("%d\n", sum(1, 2));
	_a();

    return 0;
}

