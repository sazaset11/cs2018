// self.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	////문자열 입력과 재출력 테스트
	//char stringtest[10];

	//printf("문자열 입력 : ");
	//scanf_s("%s", stringtest, sizeof(stringtest));

	//printf("%s\n", stringtest);
	//for (int i = 0; i < 10; i++) {
	//	printf("%d\t", stringtest[i]);
	//}
	//printf("\n");
	
	printf("10진수 상수 12 : %d\n", 12);
	printf("8진수 상수 12 : %#o\n", 12);
	printf("16진수 상수 12 : %x\n", 12);

    return 0;
}

