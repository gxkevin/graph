// graph.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "traveler.h"
int main()
{
	cout << "请输入两个.csv文件的路径:" << endl;
	char* csv1 = new char[100];
	char* csv2 = new char[100];
	//cin >> csv1; cin >> csv2;
	graph GG("test1.cvs", "test2.cvs");
	//GG.print();
	cin.get();
    return 0;
}

