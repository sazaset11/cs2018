// Day08.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	int val = 3;
	int *ip = &val;

	printf("%d[%d]\t%d[%d]\n", ip, *ip, &ip, *&ip);

    return 0;
}

