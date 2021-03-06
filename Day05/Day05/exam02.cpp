#include "stdafx.h"
#include <string>
//배열을 이용하여 que구조 만들기
int main()
{
	char szCmd[32];
	char cBufStack[256] = {};
	int top = 0;
	char tmp;

	while (1) {
		printf("커멘드를 입력하세요 : ");
		scanf_s("%s", szCmd, sizeof(szCmd));
		getchar();

		if (!strcmp(szCmd, "quit")) {
			printf("프로그램을 종료합니다.\n");
			break;
		}
		else if (!strcmp(szCmd, "ins")) {
			printf("데이터를 입력하세요 : ");
			scanf_s("%c", &tmp, sizeof(tmp));
			printf("입력값 : %c\n", tmp);
			cBufStack[top++] = tmp;
		}
		else if (!strcmp(szCmd, "del")) {
			if (!cBufStack[0]) {
				printf("삭제할 문자가 없습니다.\n");
				break;
			}
			printf("나온 문자 : %c\n", cBufStack[0]);
			for (int i = 0; i <= top; i++) {
				cBufStack[i] = cBufStack[i + 1];
			}
			top--;
		}
		else if (!strcmp(szCmd, "dump")) {
			printf("현재의 que : %s\n", cBufStack);
		}
		else {
			printf("입력하신 커멘드는 : %s 입니다.\n", szCmd);
		}
		printf("==========================================\n");
	}

	return 0;
}