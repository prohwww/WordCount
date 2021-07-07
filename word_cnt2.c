#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct T_TWord
{
	char m_szName[16];
	int m_iCnt;
} T_TWord;

T_TWord *ReadFile(T_TWord *p_pWordList, char *p_pFileName); //파일 읽고 단어 저장
void SortWord(T_TWord *p_pWordList, int p_iNum); //단어 정렬
void PrintWord(T_TWord *p_pWordList); //출력
int GetIndex(const T_TWord *p_pWordList); //배열 크기 반환

int 
main(int argc, char *argv[])
{
	char *szFinder = NULL;
	T_TWord *pWordList;

	if (argc < 3 || argc > 3) {
		printf("Usage: ./word_cnt {data_file} {sort_method}\n");
		return -1;
	} else {
		szFinder = strstr(argv[1], ".txt");

		if (szFinder == NULL) {
			printf("{data_file} : text file\n");
			return -1;
		}
		pWordList = ReadFile(pWordList, argv[1]);

		if (pWordList == NULL) return -1;

		if (strcmp(argv[2], "A") == 0 || strcmp(argv[2], "a") == 0) {
			SortWord(pWordList, 1);
		} else if (strcmp(argv[2], "D") == 0 || strcmp(argv[2], "d") == 0) {
			SortWord(pWordList, 2);
		} else if (strcmp(argv[2], "M") == 0 || strcmp(argv[2], "m") == 0) {
			SortWord(pWordList, 3);
		} else if (strcmp(argv[2], "I") == 0 || strcmp(argv[2], "i") == 0) {
			SortWord(pWordList, 4);
		} else {
			printf("- {sort_method} :\n\t- A : alphabet ascending\n\t- D : alphabet descending\n\t- M : max occurrence first\n\t- I : min occurrence first\n");
			return -1;
		}
		
		if (pWordList != NULL) free(pWordList);

		return 0;
	}
}

int
GetIndex(const T_TWord *p_pWordList)
{	
	int iCnt = 0;
	int i = 0;

	while (p_pWordList[i].m_iCnt) {
		iCnt++;
		i++;
	}
	return iCnt;
}

void
SortWord(T_TWord *p_pWordList, int p_iNum)
{
	int i = 0;
	int j = 0;
	int iCnt1 = 0;
	int iCnt2 = 0;
	char szName1[20];
	char szName2[20];
	char szTemp[20];
	int iCntTemp = 0;
	int iIndex = GetIndex(p_pWordList);
	
	for (i = 0; i < iIndex; i++) {
		for (j = 0; j < iIndex-1; j++) {
			memset(szTemp, 0, sizeof(szTemp));
			memset(szName1, 0, sizeof(szName1));
			memset(szName2, 0, sizeof(szName2));

			strcpy(szName1, p_pWordList[j].m_szName);
			strcpy(szName2, p_pWordList[j+1].m_szName);
			iCnt1 = p_pWordList[j].m_iCnt;
			iCnt2 = p_pWordList[j+1].m_iCnt;

			if (p_iNum == 1 && strcmp(szName1, szName2) > 0) { //ASC
				strcpy(szTemp, szName1);
				strcpy(p_pWordList[j].m_szName, szName2);
				strcpy(p_pWordList[j+1].m_szName, szTemp);

				iCntTemp = iCnt1;
				p_pWordList[j].m_iCnt = iCnt2;
				p_pWordList[j+1].m_iCnt = iCntTemp;
			} else if (p_iNum == 2 && strcmp(szName1, szName2) < 0) { //DESC
				strcpy(szTemp, szName2);
				strcpy(p_pWordList[j+1].m_szName, szName1);
				strcpy(p_pWordList[j].m_szName, szTemp);

				iCntTemp = iCnt2;
				p_pWordList[j+1].m_iCnt = iCnt1;
				p_pWordList[j].m_iCnt = iCntTemp;
			} else if (p_iNum == 3 && iCnt1 < iCnt2) { //MAX
				strcpy(szTemp, szName1);
				strcpy(p_pWordList[j].m_szName, szName2);
				strcpy(p_pWordList[j+1].m_szName, szTemp);

				iCntTemp = iCnt1;
				p_pWordList[j].m_iCnt = iCnt2;
				p_pWordList[j+1].m_iCnt = iCntTemp;
			} else if (p_iNum == 4 && iCnt1 > iCnt2) { //MIN
				strcpy(szTemp, szName2);
				strcpy(p_pWordList[j+1].m_szName, szName1);
				strcpy(p_pWordList[j].m_szName, szTemp);

				iCntTemp = iCnt2;
				p_pWordList[j+1].m_iCnt = iCnt1;
				p_pWordList[j].m_iCnt = iCntTemp;
			} else {
				//Sort Skip
			}
		}
	}
	PrintWord(p_pWordList);
}

void
PrintWord(T_TWord *p_pWordList)
{
	int i = 0;
	int iWordCnt = 0;
	int iTotalCnt = 0;
	int iIndex = GetIndex(p_pWordList);

	for (i = 0; i < iIndex; i++) {
		printf("%-16s : %d\n"
				, p_pWordList[i].m_szName, p_pWordList[i].m_iCnt);

		iWordCnt++;
		iTotalCnt += p_pWordList[i].m_iCnt;
	}
	
	printf("------------------------------------\n");
	printf("distinct word cnt : %d\n", iWordCnt);
	printf("total word cnt : %d\n", iTotalCnt);
}

T_TWord *
ReadFile(T_TWord *p_pWordList, char *p_pFileName)
{
	int i = 0;
	int iSize = 20;
	int iIndex = 0;
	char szBuffer[1024];
	char *pStr = NULL;
	char szTemp[16];
	bool bFlag = false;

	p_pWordList = malloc(sizeof(T_TWord) * iSize);

	FILE *pFile;
	pFile = fopen(p_pFileName, "r");

	if (pFile == NULL) {
		printf("파일 오픈 실패! 파일 이름을 확인하세요.\n");
		return NULL;
	}

	while (!feof(pFile)) {
		fgets(szBuffer, sizeof(szBuffer), pFile);

		pStr = strtok(szBuffer, " --");

		while (pStr != NULL) {
			bFlag = false;
			memset(szTemp, 0, sizeof(char) * 16);
			
			for (i = 0; i < strlen(pStr); i++) { //소문자 변환 및 특문 제거
				if (pStr[i] >= 'A' && pStr[i] <= 'Z') {
					pStr[i] = pStr[i] + 32;
				}
				
				if ((pStr[i] >= 'a' && pStr[i] <= 'z')) {
					szTemp[i] = pStr[i];
				} 
				pStr[i] = szTemp[i];
			}

			if (strcmp(pStr, "") == 0) { 
				bFlag = true;
			}

			for (i = 0; i <= iIndex; i++) { //단어 중복 여부 검사
				if (strcmp(pStr, p_pWordList[i].m_szName) == 0 && !bFlag) {
					p_pWordList[i].m_iCnt++;
					bFlag = true;
					break;
				}
			}

			if (iIndex == iSize) { //배열 찼을 때 크기 증가
				iSize *= 2;
				p_pWordList = realloc(p_pWordList, sizeof(T_TWord) * iSize);
			}

			if (bFlag) {
				pStr = strtok(NULL, " ");
				continue;
			} else {
				strcpy(p_pWordList[iIndex].m_szName, pStr);
				p_pWordList[iIndex].m_iCnt = 1;

				iIndex++;
				pStr = strtok(NULL, " ");
			}
		}
	}

	fclose(pFile);

	return p_pWordList;
}
