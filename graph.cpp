// graph.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "traveler.h"
int main()
{
	cout << "����������.csv�ļ���·��:" << endl;
	char* csv1 = new char[100];
	char* csv2 = new char[100];
	//cin >> csv1; cin >> csv2;
	graph GG("test1.cvs", "test2.cvs");
	//GG.print();
	cin.get();
    return 0;
}

