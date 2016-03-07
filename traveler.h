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
typedef enum { CANUSE, INSTACK, INQUEUE } VertexStatus;
typedef enum {RECORDED, AVAILABLE} EdgeStatus;
class edge {
private:
	int eId;//编号
	//int startId;//出射
	int endId;//入射
	int weight;//权重
	EdgeStatus eStatus;
public:
	edge(int id, int end, int w, EdgeStatus e = AVAILABLE)
		:eId(id), endId(end), weight(w),eStatus(e) {}//初始化边
	int getEId() { return eId; }//获取边的编号
	int getEndId() { return endId; }//获得入射顶点编号
	int getWeight() { return weight; }//获得
	EdgeStatus getStatus() { return eStatus; }
};
class vertex {
private:
	int vId;//编号
	vector<edge*> vec_edge;//所有从本顶点出发的边的vector
	//vector<edge*> arc;//弧
	VertexStatus vStatus;//顶点的状态
public:
	vertex(int id, edge* e, VertexStatus status = CANUSE)
		:vId(id), vStatus(status)  {	newEdge(e);   }//初始化
	vertex(int id, VertexStatus status = CANUSE)
		:vId(id), vStatus(status) {}
	int VId() { return vId; }//获得边id
	void setStatus(VertexStatus s) { vStatus = s; }
	VertexStatus status() { return vStatus; }//获得顶点的状态
	void newEdge(edge* e) { vec_edge.push_back(e); }//创建新的边
	vector<edge*>& getEdge() { return vec_edge; }//获得边的vector
	int getEdgeQuantity() { return vec_edge.size(); }//获取出弧的数量
	EdgeStatus getEdgeStatus(int n) { return vec_edge[n]->getStatus(); }

};
class graph {
private:
	map<int,vertex*> Map;
	//set<int,edge*> Edges;//边集
	//set<vertex*> Vertexs;//点集
	int start_id;//起点
	int end_id;//终点
	int MinWeight;//最小权重值，用作比较
	set<vertex*> MustPassing;//必须经过的点
	int num_v;//顶点数量
	int	num_e;//边数量
public:
	graph(const char* gfile, const char* pfile);//构造函数
	//vector<int>& split(const string &str, char delim, vector<int> &elems_int);
	vector<int>& split(const string &str, const string &delimiters, vector<int> &elems_int);//用于多个delimiters
	void print();//用来打印图的信息
	stack<edge>* DFS_shortPath(int start, int end);//深度优先搜索最短路径
	vertex* nextVertex(vertex* topVertex);
	void dfs_shortPath(vertex* currentVertex, int outdegree);
};
stack<edge>* graph::DFS_shortPath(int start, int end){
	stack<int> pathOfVertexStack;//保存路径
	pathOfVertexStack.push(start);//将起点索引放入路径栈中
	vertex* topVertex = Map.find(start)->second;
	topVertex->setStatus(INSTACK);//设置起点状态
	int outdegree = 0;//用来作为访问弧终点的索引
	while (outdegree < topVertex->getEdgeQuantity()) {
		if (AVAILABLE == topVertex->getEdgeStatus(outdegree)) {
			dfs_shortPath(topVertex, outdegree);
		}
	}
	return NULL;
}
vertex* graph::nextVertex(vertex* topVertex) {

}
graph::graph(const char* gfile, const char* pfile) {
//构造函数graph的作用，接受两个.csv文件的路径，初始化点集和边集，初始化MustPassing点集，
	num_v = num_e = 0;
	fstream outFile;
	outFile.open(gfile, ios::in);//打开点集边集文件
	while (!outFile.eof()) {//一行一行地读
		string buffer;//缓存每行的数据
		getline(outFile, buffer);//读取文件中的一行到buffer中
		vector<int> int_from_file;
		split(buffer, ",", int_from_file);//返回处理后int的vector
		//边信息录入
		edge* read_edge = new edge(int_from_file[0], int_from_file[2], int_from_file[3]);
		map<int, vertex*>::iterator iterator_start = Map.find(int_from_file[1]);//iterator_start指向出射点是否存在
		map<int, vertex*>::iterator iterator_end = Map.find(int_from_file[2]);
		if (iterator_start != Map.end()) {//如果出顶点已经存在于map中
			iterator_start->second->newEdge(read_edge);//单建立一个边
		}
		else {//出顶点不存在与map中,建立一个边和一个点
			//vector<edge*> vec_edge; vec_edge.push_back(read_edge);
			vertex* read_vertex = new vertex(int_from_file[1], read_edge);
			Map.insert(pair<int, vertex*>(int_from_file[1], read_vertex));
			num_v++;
		}//主要考虑哪些出度为0的vertex
		if (iterator_end == Map.end()) {
			vertex* read_vertex = new vertex(int_from_file[2]);
			num_v++;
		}
		num_e++;//vertex和edge数量记录增加
		//cout << int_from_file[0] << endl;
		//cout << "读取成功一个" << endl;
	}//循环结束时，图构造完毕
	outFile.close();
	//下面对第二个.csv文件做处理
	outFile.open(pfile, ios::in);//打开MustPassing文件
	string buffer;
	getline(outFile, buffer);
	vector<int> int_from_file;
	split(buffer, ",|", int_from_file);//返回处理后int的vector
	start_id = int_from_file[0];
	end_id = int_from_file[1];//起点和终点一定在第一二位置上
	for (int i = 2; i < int_from_file.size(); i++) {
		MustPassing.insert(Map.find(int_from_file[i])->second);
	}//查找并添加到MustPassing中去
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
		//pos表示在str(第prev个字符之后)中发现delimiters(分隔符)中的
		if (pos > prev) {
			if (str[pos]=='\n') break;//在prev之后的str中没有找到delimiteres
			//if (1 == pos - prev) break;//在prev之后的str中没有找到delimiteres
			//cout << str << endl;
			int c = stoi(string(str, prev, pos - prev));
			elems_int.push_back(c);
			//elems.emplace_back(str, prev, );//通过emplace_back可以直接在vector最后分配内存创建object
		}
		prev = pos + 1;
	}
	if (prev < str.size()) {
		int c = stoi(string(str, prev, str.size() - prev));
		elems_int.push_back(c);
	}
	return elems_int;
}
void graph::print()//打印图的一些信息
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
