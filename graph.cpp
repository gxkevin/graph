// graph.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "traveler.h"
#include <ctime>
int main()
{
	clock_t start, end;
	start = clock();
	cout << "请输入两个.csv文件的路径:" << endl;
	//char* csv1 = new char[100];
	//char* csv2 = new char[100];
	//cin >> csv1; cin >> csv2;
	graph GG("GeneratedTopo.csv", "GeneratedDemand.csv");
	GG.printFinalPath();
	//GG.print();
	end = clock();
	cout << endl << "用时： " << (double)(end - start) / CLK_TCK << endl;
	cin.get();
    return 0;
}

