// exam05.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	int pay, bonus, sum;
	float tax, income;

	printf("월급을 입력하세요(만원단위) : ");
	scanf_s("%d", &pay);
	printf("보너스를 입력하세요(만원단위) : ");
	scanf_s("%d", &bonus);

	sum = pay + bonus;
	tax = sum * 0.09;
	income = sum - tax;
	printf("당신의 이번달 입금액은 %.1f만원 입니다.(세금을 %.1f만원 지불했습니다.)\n", income, tax);
	
    return 0;
}


