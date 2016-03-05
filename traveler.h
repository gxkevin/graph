#pragma once
#include <queue>
#include <stack>
#include <set>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <map>
using namespace std;
typedef enum { UNDISCOVERED, INSTACK, AVAILABLE, INQUEUE } VertexStatus;
class edge {
private:
	int eId;//���
	//int startId;//����
	int endId;//����
	int weight;//Ȩ��
public:
	edge(int id, int end, int w)
		:eId(id), endId(end), weight(w) {}//��ʼ����
	int getEId() { return eId; }//��ȡ�ߵı��
	int getEndId() { return endId; }//������䶥����
	int getWeight() { return weight; }//���
};
class vertex {
private:
	int vId;//���
	vector<edge*> vec_edge;//���дӱ���������ıߵ�vector
	//vector<edge*> arc;//��
	VertexStatus vStatus;//�����״̬
public:
	vertex(int id, edge* e, VertexStatus status = UNDISCOVERED)
		:vId(id), vStatus(status)  {	newEdge(e);   }//��ʼ��
	vertex(int id, VertexStatus status = UNDISCOVERED)
		:vId(id), vStatus(status) {}
	int VId() { return vId; }//��ñ�id
	VertexStatus status() { return vStatus; }//��ö����״̬
	void newEdge(edge* e) { vec_edge.push_back(e); }//�����µı�
	vector<edge*>& getEdge() { return vec_edge; }//��ñߵ�vector
};
class graph {
private:
	map<int,vertex*> Map;
	//set<int,edge*> Edges;//�߼�
	//set<vertex*> Vertexs;//�㼯
	set<vertex*> MustPassing;//���뾭���ĵ�
	int MinWeight;//��СȨ��ֵ�������Ƚ�
	int num_v;//��������
	int	num_e;//������
public:
	graph(const char* gfile, const char* pfile);
	//vector<int>& split(const string &str, char delim, vector<int> &elems_int);
	vector<int>& split(const string &str, const string &delimiters, vector<int> &elems_int);//���ڶ��delimiters
	void print();//������ӡͼ����Ϣ
	//stack<edge>* DFS_shortPath(int start, int end);
};
graph::graph(const char* gfile, const char* pfile) {
//���캯��graph�����ã���������.csv�ļ���·������ʼ���㼯�ͱ߼�����ʼ��MustPassing�㼯��
	num_v = num_e = 0;
	fstream outFile;
	outFile.open(gfile, ios::in);//�򿪵㼯�߼��ļ�
	while (!outFile.eof()) {//һ��һ�еض�
		string buffer;//����ÿ�е�����
		getline(outFile, buffer);//��ȡ�ļ��е�һ�е�buffer��
		vector<int> int_from_file;
		split(buffer, ",", int_from_file);//���ش����int��vector
		//����Ϣ¼��
		edge* read_edge = new edge(int_from_file[0], int_from_file[2], int_from_file[3]);
		auto iterator_start = Map.find(int_from_file[1]);
		auto iterator_end = Map.find(int_from_file[2]);
		if (iterator_start != Map.end()) {//����������Ѿ�������map��
			iterator_start->second->newEdge(read_edge);//������һ����
		}
		else {//�����㲻������map��,����һ���ߺ�һ����
			//vector<edge*> vec_edge; vec_edge.push_back(read_edge);
			vertex* read_vertex = new vertex(int_from_file[1], read_edge);
			Map.insert(pair<int, vertex*>(int_from_file[1], read_vertex));
			num_v++;
		}//��Ҫ������Щ����Ϊ0��vertex
		if (iterator_end == Map.end()) {
			vertex* read_vertex = new vertex(int_from_file[2]);
			num_v++;
		}
		num_e++;//vertex��edge������¼����
		//cout << int_from_file[0] << endl;
		//cout << "��ȡ�ɹ�һ��" << endl;
	}//ѭ������ʱ��ͼ�������
	outFile.close();

	outFile.open(pfile, ios::in);//��MustPassing�ļ�
	if(!outFile.eof()) {//��ȡMustPassing����Ϣ
		string buffer;
		getline(outFile, buffer);
		vector<int> int_from_file;
		split(buffer, ",|", int_from_file);//���ش����int��vector
	}
	outFile.close();

}
//vector<int>& graph::split(const string &str, char delim, vector<int> &elems_int) {
//	istringstream iss(str);
//	for (string item; getline(iss, item, delim);) {
//		if (item.empty()) continue;
//		else { 
//			int c; c = stoi(item);
//			elems_int.push_back(c); 
//		}
//	}
//	return elems_int;
//}
vector<int>& graph::split(const string &str, const string &delimiters, vector<int> &elems_int) {
	string::size_type pos, prev = 0;
	while ((pos = str.find_first_of(delimiters, prev)) != string::npos) {
		//pos��ʾ��str(��prev���ַ�֮��)�з���delimiters(�ָ���)�е�
		if (pos > prev) {
			if (str[pos]=='\n') break;//��prev֮���str��û���ҵ�delimiteres
			//if (1 == pos - prev) break;//��prev֮���str��û���ҵ�delimiteres
			//cout << str << endl;
			int c = stoi(string(str, prev, pos - prev));
			elems_int.push_back(c);
			//elems.emplace_back(str, prev, );//ͨ��emplace_back����ֱ����vector�������ڴ洴��object
		}
		prev = pos + 1;
	}
	/*if (prev < str.size()) {
		int c = stoi(string(str, prev, str.size() - prev));
		elems_int.push_back(c);
	}*/
	return elems_int;
}
void graph::print()//��ӡͼ��һЩ��Ϣ
{
	for (auto it = Map.begin(); it != Map.end(); it++) {
		cout << "vertex's id: " << it->first << "    status:" << it->second->status() << endl;
		cout << "\tedge: " << endl;
		int i = 0;
		for (vector<edge*> et = it->second->getEdge(); i < et.size(); i++) {
			cout << "\tedge's id " << et[i]->getEId() << " endVId: " << et[i]->getEndId() << "weight: " << et[i]->getWeight() <<endl;
		}
		cout << endl;
	}
}