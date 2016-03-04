#pragma once
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <algorithm>
typedef enum{UNDISCOVERED, DISCOVERED, VISITED} VStatus; // vertex 's status
typedef enum{UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD} EStatus; // edge 's status
using namespace std;
template <typename Tv, typename Te> // vertex and edge
class graph{
private:
	void reset() { // all auxiliary information of vertexs and edges reset   
		for (int i = 0; i < n; i++) { // all vertex
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; j++)// all edge
				if (exists(i, j)) status(i, j) = UNDETERMINED;
		}
	}
	void BFS(int, int&);//(connected domain) breadth first search
	void DFS(int, int&);//(connected domain) depth first search
	void BCC(int, int&, stack<int>&);//(connected domain)two connected component decomposition algorithm based on DFS
	bool TSort(int, int&, stack<Tv>*);//(connected domain)Topological sort algorithm based on DFS
	template <typename PU> void PFS(int, PU);//(connected domain) Priority search framework
public:
// about vertex
	int n; //vertex count
	virtual int insert(Tv const&) = 0;// insert vertex, return number
	virtual Tv remove(int) = 0; //delete vertex and its connected edges, return information about that vertex
	virtual Tv& vertex(int) = 0; //get vertex v 's data (vertex exist)
	virtual int inDegree(int) = 0;// vertex v's in degree
	virtual int outDegree(int) = 0;//vertex v's out degree
	virtual firstNbr(int) = 0;// vertex v's first adjacent vertex
	virtual nextNbr(int, int) = 0;// vertex v's next adjacent vertex(compared to j)
	virtual VStatus& status(int) = 0;// the status to vertex v
	virtual int& dTime(int) = 0; // the time mark of vertex v: dTime
	virtual int& fTime(int) = 0; // the time mark of vertex v: fTime
	virtual int& parent(int) = 0;// vertex's father in the traversal tree
	virtual int& priority(int) = 0;// vertex's priority series in the traversal tree
// about edge
	int e;// edge count
	virtual bool exists(int, int) = 0;// if edge(v,u) is exist
	virtual void insert(Te const&, int, int, int) = 0;// insert edge e whose weight is w between vertex v and u
	virtual Te remove(int, int) = 0;// delete edge e between v and u, return information about that edge
	virtual EStatus& status(int, int) = 0;// the status of edge (v,u)
	virtual int& weight(int, int) = 0;// the weight of edge(v,u)
//algorithm
	void bfs(int);// breadth first search algorithm
	void dfs(int);// depth first search algorithm
	void bcc(int);// two connected component decomposition algorithm based on dfs
	stack<Tv>* tSort(int);//topologcial sort algorithm base dfs
	void prim(int);// minimum spanning tree
	void dijkstra(int);//minumum path algorithm
	template <typename PU> void pfs(int, PU);//poriority search framework
};
template <typename Tv, typename Te>// breadth first search algorithm
void graph<Tv, Te>::bfs(int s) {// assert:0 <= s <= n
	reset(); int clock = 0; int v = s;// initializaiton
	do { // check vertexs one by one
		if (UNDISCOVERED == status(v))// once vertex v is undiscovered
			BFS(v, clock);// start BFS from vertex v
	} while (s != (v = (++v % n)));//check according order
}
template <typename Tv, typename Te>
void graph<Tv, Te>::BFS(int v, int& clock) {
	queue<int> Q;
	status(v) = DISCOVERED; Q.push(v);// initialize starting point
	while (!Q.empty()) {// before Q become empty, go ahead
		int v = Q.front(); Q.pop(); // get front vertex
		dTime(v) = ++clock;
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {// enumerate all neighbors of vertex v
			if (UNDISCOVERED == status(u)) {// if u wasn't discovered
				status(u) = DISCOVERED; Q.push(u);
				status(v, u) = TREE; parent(u) = v; // expand the supported tree
			}
			else {
				status(v, u) = CROSS;// classify (v,u) cross boundary
			}
		}
		status(v) = VISITED; // over to this vertex
	}
}
template <typename Tv, typename Te>//depth-first search
void graph<Tv, Te>::dfs(int s) {//assert: 0 <= s < n
	reset(); int clock = 0; int v = s;//initialization
	do {// check all vertexs one by one
		if (UNDISCOVERED == status(v)) {//once vertex v is undiscovered
			DFS(v, clock);
		}
	} while (s != (v = (++v % n))); //check vertex according to order
}
template <typename Tv, typename Te>
void graph<Tv, Te>::DFS(int v, int& clock) {// assert: 0<= v < n
	dTime(v) = ++clock; status(v) = DISCOVERED;// discover current vertex
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
		switch (status(u)) {// treat depending on the state
		case UNDISCOVERED:
			status(v, u) = Tree; parent(u) = v; DFS(u, clock);
			break;
		case DISCOVERED:
			status(v, u) = BACKWARD;
			break;
		default:
			status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED; fTime(v) = ++clock;
}
template <typename Tv, typename Te>
stack<Tv>* graph<Tv,Te>::tSort(int) {//topologcial sort algorithm based on dfs
	reset(); int clock = 0; int v = s;
	stack<Tv>* S = new stack<Tv>;// record sorted vertex using stack
	do {
		if(UNDISCOVERED == status(v))
			if (!TSort(v, clock, S)) { //clock is not necessary
				while (!S.empty())// every connected area is not DAG
					S->pop();
				break;
			}
	} while (s != (v = (++v % n)));
	return S;
}
template <typename Tv, typename Te>
bool graph<Tv, Te>::TSort(int v, int& clock, stack<Tv>* S) {
	dTime(v) = ++clock; status(v) = DISCOVERED;// find vertex v
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {// enmerate all neighbors
		switch (status(u)) {//deal with vertex depending on v's status
		case UNDISCOVERED:
			parent(u) = v; status(v, u) = TREE;
			if (!TSort(u, clock, S))// depth again
				return false;//don't need depth, return false
			break;
		case DISCOVERED:
			status(v, u) = BACKWARD;//once find front edge
			return false;
		default:
			status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED; S->push(vertex(v));// the visited vertex into stack
	return true;
}
template <typename Tv, typename Te>
void graph<Tv, Te>::bcc(int s) {//two connected component decomposition algorithm based on dfs
	reset(); int clock = 0; int v = s; stack<int> S;// S take notes visited vertexs
	do {
		if (UNDISCOVERED == status(v)) {// once vertex not be found
			BCC(v, clock, S);//BCC from current vertex
			S.pop();//connected domain orign
		}
	} while (s != (v = (++v % n)));
}
#define hca(x) (fTime(x))
template <typename Tv, typename Te>
void graph<Tv, Te>::BCC(int v, int& clock, stack<int>& S) {
	hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v);
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
		switch (status(u)) {//deal with according to different situations
		case UNDISCOVERED:
			parent(u) = v; status(v, u) = TREE; BCC(u, clock, S);//continue to deepen
			if (hca(u) < dTime(v))//after traversal, if u point to v's ancestor
				hca(v) = min(hca(v, hca(u)));//u can do it
			else//otherwise, v as joint point
				while (v != S.top())//pop vertex in the stack
					S.pop();
		}
		break;
		case DISCOVERED:
			status(v, u) = BACKWARD;
			if (u != parent(v))
				hca(v) = min(hca(v), dTime(u));//more less more high
			break;
		default:
			status(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
	}
	status(v) = VISITED;
}
#undef hca
template <typename Tv, typename Te> template <typename PU>
void graph<Tv, Te>::pfs(int s, PU prioUpdater) { // priority first search
	reset(); int v = s;// initialization
	do // check vertex one by one
		if (UNDISCOVERED == status(v))
			PFS(v, prioUpdater);
	while (s != (v = (++v % n)));
}
template <typename Tv, typename Te> template <typename PU>
void graph<Tv, Te>::PFS(int s, PU prioUpdater) {//priority first search
	priority(s) = 0; status(s) = VISITED; parent(s) = -1;
	while (s) {
		for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w))//enumerate all the neighbors of s
			prioUpdater(this, s, w);//update priority of vertex w and its parents 
		for (int shortest = INT_MAX, w = 0; w < n; w++)
			if(status(w) == UNDISCOVERED)
				if (shortest > priority(w)) {
					shortest = priority(w); s = w;
				}
		if (VISITED == status(s))
			break;
		status(s) = VISITED; status(parent(s), s) = TREE;
	}
}
template <typename Tv, typename Te>
void graph<Tv, Te>::prim(int v) {// minimum spanning tree
	this->pfs(v, PrimPU);
}
template <typename Tv, typename Te>
void graph<Tv, Te>::dijkstra(int v) {//minumum path algorithm
	this->pfs(v, DijkstraPU);
}
/*-------------------------------------------------------------------------------------------------------------------
===================================================================================================================*/
template <typename Tv> struct Vertex {
	Tv data; int inDegree, outDegree; VStatus status;// data in/out degree and status
	int dTime, fTime; // time mark
	int parent; int priority;//father vertex in traversal tree, priority series
	Vertex(Tv const& d = (Tv)0) : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} // don't mind weight out temporary
};

template <typename Te> struct Edge { 
	Te data; int weight; EStatus status;// data weight status
	Edge(Te const& d, int w) : data(d), weight(w), status(UNDETERMINED) {}
};
template <typename Tv, typename Te> struct BfsPU {//vertex priority updater for bfs algorithm
	virtual void operator()(graph<Tv, Te>* g, int uk, int v) {
		if(g->status(v) == UNDISCOVERED)
			if (g->priority(v) > g->priority(uk) + 1) {
				g->priority(v) = g->priority(uk) + 1;
				g->parent(v) = uk;
			}
	}
};
template <typename Tv, typename Te> struct DfsPU {//vertex priority updater for bfs algorithm
	virtual void operator()(graph<Tv, Te>* g, int uk, int v) {
		if (g->status(v) == UNDISCOVERED)
			if (g->priority(v) > g->priority(uk) - 1) {
				g->priority(v) = g->priority(uk) - 1;
				g->parent(v) = uk;
				return;//As long as there is an adjacent vertex can be updated, return
			}
	}
};
template <typename Tv, typename Te> struct PrimPU {// Prim algorithm vertex priority updater
	virtual void operator()(graph<Tv, Te>* g, int uk, int v) {
		if(g->status(v) == UNDISCOVERED)// 
			if (g->priority(v) > g->weight(uk, v)) {
				g->priority(v) = g->weight(uk, v);
				g->parent(v) = uk;
			}
	}
};
template <typename Tv, typename Te> struct DijkstraPU {//vertex priority updater for Dijkstra algorithm
	virtual void operator()(graph<Tv, Te>* g, int uk, int v) {
		if (g->status(s) == UNDISCOVERED)// for those vertex(not be found)
			if (g->priority(v) > g->priority(uk) + g->weight(uk, v)) {
				g->priority(v) = g->priority(uk) + g->weight(uk, v);
				g->parent(v) = uk;
			}
	}
};
//-------------------------------------------------------------------------------------------------------------------
template <typename Tv, typename Te>
class GraphMatrix : public graph<Tv, Te> {// graph in the form of adjacency matrix
private:
	vector<Vertex<Tv>> V;//vertex set 
	vector<vector<Edge<Te>*>> E; // edge set
public:
	GraphMatrix() { n = e = 0; }
	~GraphMatrix() {
		for (int j = 0; j < n; j++)// all edge created dynamicly
			for (int k = 0; k < n; k++)
				delete E[j][k];// delete one by one
	}
// basic operation: inquiry ist vertex(0 <= i < n)
	virtual Tv& vertex(int i) { return V[i].data; } // return data
	virtual int inDegree(int i) { return V[i].inDegree; } // return inDegree
	virtual int outDegree(int i) { return V[i].outDegree; } //return outDegree
	virtual int firstNbr(int i) { return nextNbr(i, n); } // return the first adjacent vertex
	virtual int nextNbr(int i, int j) {// the next adjacent vertex  compared j
		while ((-1 < j) && (!exists(i, --j))); return j; // inverse linear test
	}
	virtual VStatus& status(int i) { return V[i].status; }// status
	virtual int& dTime(int i) { return V[i].dTime; }// time mark dTime
	virtual int& fTime(int i) { return V[i].fTime; } // time mark fTime
	virtual int& parent(int i) { return V[i].parent; }// father in traversal tree
	virtual int& priority(int i) { return V[i].priority; }//priority series in traversal tree
// dynamic operation about vertex
	virtual int insert(Tv const& vertex) { // insert vertex ,return number
		for (int j = 0; j < n;j++) E[j].insert(nullptr); n++;//every vertex is reserved for a potential associated edge
		E.insert(vector<Edge<Te>*>(n, nullptr));// create a new vertex corresponding edge vectors
		return V.insert(Vertex<Tv>(vertex)) - V.begin();//adds vertex to vertex vector
	}
	virtual Tv remove(int i) {// delete i st vertex and connected edge
		for (int j = 0; j < n; j++)
			if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }
		E.erase(E.begin()+i); n--;
		Tv vBak = vertex(i); V.erase(V.begin()+i);
		for (int j = 0; j < n; j++)
			if (exists(j, i)) { delete E[j][i]; E[j].erase(E[j].begin() + i); V[j].outDegree--; }
		return vBak;
	}
// Side confirmation operation
	virtual bool exists(int i, int j) {// edge(i,j) exist?
		return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != nullptr;
	}
// basic operation of edge, inquiry edge between i and j ( 0 <= i, j < n && exists(i, j)
	virtual EStatus& status(int i, int j) { return E[i][j]->status; }
	virtual Te& edge(int i, int j) { return E[i][j]->data; }
	virtual int& weight(int i, int j) { return E[i][j]->weight; }
// dynamic operation of edge
	virtual void insert(Te const& edge, int w, int i, int j) { // insert edge(i,j) whose weight is w
		if (exists(i, j)) return;// promise this edge do not exist
		E[i][j] = new Edge<Te>(edge, w);
		e++; V[i].outDegree++; V[j].inDegree++;
	}
	virtual Te remove(int i, int j) {// delete edge between vertex i and j 
		Te eBak = edge(i, j); delete E[i][j]; E[i][j] = nullptr;// after backups delete edge record
		e--; V[i].outDegree--; V[j].inDegree--;
		return eBak; // return information about deleted edge
	}
};