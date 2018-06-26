// exam10.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

int ChangeBinary(int a) {
	int i = a;
	int j;
	int k = 1;
	int sum = 0;
	while (i) {
		j = i % 10;
		sum += j * k;
		k *= 2;
		i /= 10;
	}
	return sum;
}

int main()
{
	int _;
	printf("2진수 입력 : ");
	scanf_s("%d", &_);
	printf("10진수로 변환 : %d\n", ChangeBinary(_));

    return 0;
}

