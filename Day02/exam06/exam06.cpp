// exam06.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	double fare = 660;
	double fare_kw = 88.5;
	double fare_sum, use_kw, tax;

	printf("전기 사용량을 입력하세요(kw) : ");
	scanf_s("%lf", &use_kw);

	fare_sum = fare + (use_kw * fare_kw);
	tax = fare_sum * 0.09;

	printf("전기 사용 요금은 %lf원 입니다.\n", fare_sum + tax);
	
    return 0;
}

