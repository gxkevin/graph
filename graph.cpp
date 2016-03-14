// graph.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "traveler.h"
#include <ctime>
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
#include <deque>
using namespace std;
typedef enum { CANUSE, INSTACK, INQUEUE } VertexStatus;
typedef enum { RECORDED, AVAILABLE } EdgeStatus;
struct edge {
	int eId;//编号
	int endId;//入射
	int weight;//权重
	EdgeStatus eStatus;
};
class vertex {
public:
	int vId;//编号
	vector<edge*> vec_edge;//所有从本顶点出发的边的vector
	vertex* toParent;//用于路径记录
	VertexStatus vStatus;//顶点的状态
	bool isMustPassing;
	vertex(int id, edge*& e, VertexStatus status = CANUSE, bool is = false, vertex* toP = nullptr)
		:vId(id), vStatus(status), isMustPassing(is), toParent(toP) {
		vec_edge.push_back(e);
	}//初始化
	vertex(int id, VertexStatus status = CANUSE, bool is = false, vertex* toP = nullptr)
		:vId(id), vStatus(status), isMustPassing(is), toParent(toP) {	}
	//void newEdge(edge* e) { vec_edge.push_back(e); }//创建新的边
	//vector<edge*>& getEdgeOfVector() { return vec_edge; }//获得边的vector
	//int getEdgeQuantity() { return vec_edge.size(); }//获取出弧的数量
	/*bool AVIEdgeStatus(int n) {
		if (n < vec_edge.size()) return vec_edge[n]->getStatus() == AVAILABLE;
		else return false;
	}
	bool AVIVertexStatus(deque<int> path, int n) {
		if (find(path.begin(), path.end(), n) != path.end()) return false;
		return true;
	}*/
	//int nextVertexId(int n) { return vec_edge[n]->endId; }
	//edge* getEdge(int n) { return vec_edge[n]; }
};
class graph {
private:
	map<int, vertex*> Map;
	int start_id;//起点
	int end_id;//终点
	int MinWeight;//最小权重值，用作比较
	int currentWeight;
	//vector<vertex*> MustPassing;//必须经过的点
	vector<int> MustPassing;//必须经过的点
	int num_v;//顶点数量
	int	num_e;//边数量
	vector<int> final_path;//边路径
public:
	graph(const char* gfile, const char* pfile);//构造函数
												//vector<int>& split(const string &str, char delim, vector<int> &elems_int);
	vector<int>& split(const string &str, const string &delimiters, vector<int> &elems_int);//用于多个delimiters
	void print();//用来打印图的信息
	void DFS_shortPath();//深度优先搜索最短路径
	//void nextVertex(vertex* topVertex, int& outdegree);
	void dfs_shortPath(vertex*& currentVertex, vertex*& nextVertex, int outdegree);//递归的部分， 是否需要传deque<int>* 存疑,还是不传快？
	void judgePath();
	void printFinalPath();
	void BFS_record();
	void bfs_record(vertex* current);

};
vector<int> vertexPath;//临时的点路径标记
vector<edge*> edgePath;//临时的边路径标记
int count_pass = 0;//用于BFS考察是否已经经过所有set中的点
void graph::BFS_record(){
	vertex* topVertex = Map.find(start_id)->second;
	if (topVertex->isMustPassing)
		count_pass++;
	bfs_record(topVertex);
}
void graph::bfs_record(vertex * current){

}
void graph::DFS_shortPath() {
	
	vertexPath.push_back(start_id);//将起点索引放入点路径
	vertex* topVertex = Map.find(start_id)->second;
	topVertex->vStatus = INSTACK;//设置起点状态
	int outdegree = 0;//用来作为访问弧终点的索引
	while (outdegree < topVertex->vec_edge.size()) {
		//if (topVertex->AVIEdgeStatus(outdegree)) {
		dfs_shortPath(topVertex, Map.find(topVertex->vec_edge[outdegree]->endId)->second, outdegree);
		//}

		outdegree++;
	}
}
void graph::dfs_shortPath(vertex*& currentVertex, vertex*& nextVertex, int outdegree) {
	//判断是否尾递归
	if (nextVertex->vId != end_id) {//不是终点
		int index = nextVertex->vec_edge.size();
		if (index > 0) {//出度大于0
			nextVertex->vStatus = INSTACK;//标记点入栈
			vertexPath.push_back(nextVertex->vId);//入栈操作
			edgePath.push_back(currentVertex->vec_edge[outdegree]);
			//cout << nextEdge->getEId() << ' ';
			//nextEdge->status = RECORDED;//标记边入栈
			for (int i = 0; i < index; i++) {//针对栈顶的节点
				vertex* endVertex = Map.find(nextVertex->vec_edge[i]->endId)->second;//get栈顶节点的下一目的地
				if (endVertex->vStatus == CANUSE) {//若下一目的地不在栈里
					dfs_shortPath(nextVertex, endVertex, i);//递归深入
					endVertex->vStatus = CANUSE;
				}
			}
			//nextEdge->status = AVAILABLE;
			nextVertex->vStatus = CANUSE;//调整状态
			vertexPath.pop_back();
			edgePath.pop_back();//当前节点出发的个方向已寻完
		}
	}
	else {
		vertexPath.push_back(nextVertex->vId);
		edgePath.push_back(currentVertex->vec_edge[outdegree]);
		judgePath();
		vertexPath.pop_back();
		edgePath.pop_back();
	}
	return;
}
void graph::judgePath() {//当路径尾是终点时会调用此函数
	int p_size = MustPassing.size();
	if (p_size < vertexPath.size()) {//路径节点数要大于path中节点数量
		for (vector<int>::iterator it = MustPassing.begin(); it != MustPassing.end(); it++)
			if (find(vertexPath.begin(), vertexPath.end(), (*it)) == vertexPath.end())
				return;//检查是否vertexPath包含MustPassing
		int p_weight = 0;//所要比较的路径的总权重
		for (vector<edge*>::iterator it = edgePath.begin(), ed = edgePath.end(); it != ed; it++)
			p_weight += (*it)->weight;//计算所要比较的路径的总权重
		if (final_path.empty() || p_weight < MinWeight) {//如果图类中的路径存在
			final_path.clear();//edgePath是更好地路径
			for (vector<edge*>::iterator it = edgePath.begin(), ed = edgePath.end(); it != ed; it++)
				final_path.push_back((*it)->eId);//更新final_path
			MinWeight = p_weight;
		}
		/*cout << final_path->at(0);
		for (int i = 1; i < final_path->size(); i++)
			cout << '|' << final_path->at(i);
		cout << endl;*/
	}
}
//void graph::nextVertex(vertex* topVertex, int& outdegree) {
//	while (outdegree < topVertex->getEdgeQuantity()) {
//		if (topVertex->AVIEdgeStatus(outdegree))
//			break;
//		outdegree++;
//	}
//}
void graph::printFinalPath() {
	cout << final_path[0];
	for (int i = 1; i < final_path.size(); i++)
		cout << '|' << final_path[i];
}
graph::graph(const char* gfile, const char* pfile) {
	//构造函数graph的作用，接受两个.csv文件的路径，初始化点集和边集，初始化MustPassing点集，
	num_v = num_e = 0;
	fstream outFile;
	outFile.open(gfile, ios::in);//打开点集边集文件

	string buffer = "";//缓存每行的数据
	while (std::getline(outFile, buffer)) {//一行一行地读
										   //读取文件中的一行到buffer中
		vector<int> int_from_file;
		split(buffer, ",", int_from_file);//返回处理后int的vector
										  //边信息录入
		edge* read_edge = new edge;
		read_edge->eId = int_from_file[0]; read_edge->endId = int_from_file[2];
		read_edge->weight = int_from_file[3];
		map<int, vertex*>::iterator iterator_start = Map.find(int_from_file[1]);//iterator_start指向出射点是否存在
		if (iterator_start != Map.end()) {//如果出顶点已经存在于map中
			iterator_start->second->vec_edge.push_back(read_edge);//单建立一个边
		}
		else {//出顶点不存在与map中,建立一个边和一个点
			vertex*  read_vertex = new vertex(int_from_file[1], read_edge);
			Map.insert(pair<int, vertex*>(int_from_file[1], read_vertex));
			num_v++;
		}//主要考虑哪些出度为0的vertex
		map<int, vertex*>::iterator iterator_end = Map.find(int_from_file[2]);
		if (iterator_end == Map.end()) {
			vertex* read_vertex = new vertex(int_from_file[2]);
			Map.insert(pair<int, vertex*>(int_from_file[2], read_vertex));
			num_v++;
		}
		num_e++;//vertex和edge数量记录增加
	}//循环结束时，图构造完毕

	outFile.close();
	//下面对第二个.csv文件做处理
	outFile.open(pfile, ios::in);//打开MustPassing文件

	std::getline(outFile, buffer);
	vector<int> int_from_file;
	split(buffer, ",|", int_from_file);//返回处理后int的vector
	start_id = int_from_file[0];
	end_id = int_from_file[1];//起点和终点一定在第一二位置上
	for (int i = 2; i < int_from_file.size(); i++) {
		vertex* tempV = Map.find(int_from_file[i])->second;
		tempV->isMustPassing = true;
		MustPassing.push_back(int_from_file[i]);
	}//查找并添加到MustPassing中去 
	outFile.close();
	MinWeight = 0;
	//print();//打印图的一些信息
	DFS_shortPath();
}

vector<int>& graph::split(const string &str, const string &delimiters, vector<int> &elems_int) {
	string::size_type pos, prev = 0;
	while ((pos = str.find_first_of(delimiters, prev)) != string::npos) {
		//pos表示在str(第prev个字符之后)中发现delimiters(分隔符)中的
		if (pos > prev) {
			if (str[pos] == '\n') break;//在prev之后的str中没有找到delimiteres
			int c = stoi(string(str, prev, pos - prev));
			elems_int.push_back(c);
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
		cout << "vertex's id: " << it->first << "    status:" << it->second->vStatus << endl;
		cout << "\tedge: " << endl;
		int i = 0;
		for (vector<edge*> et = it->second->vec_edge; i < et.size(); i++) {
			cout << "\tedge's id " << et[i]->eId << " endVId: " << et[i]->endId << "weight: " << et[i]->weight << endl;
		}
		cout << endl;
	}
}
int main()
{
	clock_t start, end;
	start = clock();
	cout << "请输入两个.csv文件的路径:" << endl;
	//char* csv1 = new char[100];
	//char* csv2 = new char[100];
	//cin >> csv1; cin >> csv2;
	//graph GG("case2_topo_50points.csv", "case2_demand_50points.csv");
	graph GG("case1_topo.csv", "case1_demand.csv");
	GG.printFinalPath();
	//GG.print();
	end = clock();
	cout << endl << "用时： " << (double)(end - start) / CLK_TCK << endl;
	cin.get();
    return 0;
}
