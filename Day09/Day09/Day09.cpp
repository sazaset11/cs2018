// Day09.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"

struct student
{
	int num = 5;
	double grade = 3.1;
	char level;
};

int main()
{
	int num = 5;
	double grade = 3.1;

	printf("%8d,%8.3lf\n", num, grade);

	student _student1;
	_student1.num = 6;
	_student1.grade = 5.1;
	_student1.level = 'c';

	printf("%8d,%8.3lf\n", _student1.num, _student1.grade);
	printf("%8d,%8d\n", &num, &grade);
	printf("%d, %d, %d\n", &(_student1.num), &(_student1.grade), &(_student1.level));

    return 0;
}

