#include "Header.h"
#include <cstring>

char* path;

void createPathBin() {
    path = new char[strlen(pathToDataBases) + strlen(binBase) + 1];
    strcpy_s(path, strlen(pathToDataBases) + 1, pathToDataBases);
    strcat_s(path, strlen(pathToDataBases) + strlen(binBase) + 1, binBase);
}

void writeToFileBin() {

    //FILE* in = fopen("input.txt", "r");
    //FILE* out = fopen("output.txt", "w");
    //float a, b, c;
    //while (fscanf(in, "abc /%f,%f,%f\n", &a, &b, &c) == 3)
    //    fprintf(out, "text %7.3f %7.3f %7.3f\n", a, b, c);
}

//char str[] = "Police Academy";
//char key[] = "aeiou";
//
//std::cout << "����� ������� ���� � ������ " << """ << str << ""n";
//char* pch = strpbrk(str, key);                            // ������ �����
//
//while (pch != NULL)                                         // ���� ���� ������� ����� � ������
//{
//    std::cout << *pch << " ";                               // ������ �������� �������
//    pch = strpbrk(pch + 1, key);                                // ����� ������� ����
//}
//
//std::cout << "n";
