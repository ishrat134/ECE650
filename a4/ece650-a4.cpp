#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <algorithm>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"



using namespace std;
using namespace Minisat;

#define MAX 100000

#define INFINITY 200000

struct link_
{
	int data;
	link_* next;
};

struct Node
{
	int v;
	link_* next;
};

typedef struct {
	Node node[MAX + 1];
	int nodeCnt;

}Graph;

Graph g;

int visited[MAX + 1];
int pa[MAX + 1];

std::vector<int> find_vertexcover(int, vector<int>, int, Graph);
void vertexcover(int, vector<int>, Graph);


Graph readGraph(vector<int> vec_V, vector<int>vec_E)
{
	//ifstream cin("D://UW//ece650//a2//x22yi//1.txt");
	int num_E = vec_E.size();
	Graph graph;
	int i;
	for (i = 0; i < MAX; i++)
	{
		graph.node[i].v = i;
		graph.node[i].next = NULL;
	}

	int n1 = 0, n2 = 0;
	link_* s;
	graph.nodeCnt = 1;
	for (int i = 1; i <= num_E; i++) {
		n1 = vec_E[i];
		n2 = vec_E[i - 1];
		if (n2 == 0) {
			int n0 = n1;
			n1 = n2;
			n2 = n0;
		}

		if (graph.nodeCnt < n1) graph.nodeCnt = n1;
		if (graph.nodeCnt < n2) graph.nodeCnt = n2;
		s = new link_;
		s->data = n2;
		s->next = graph.node[n1].next;
		graph.node[n1].next = s;
		//delete(s);   

		s = new link_;
		s->data = n1;
		s->next = graph.node[n2].next;
		graph.node[n2].next = s;
		//delete(s);  
		i++;
	}
	return graph;
}


void shortestPath(Graph graph, int s, int d) // Using BFS. REFER TO :https://blog.csdn.net/jphaoren/article/details/6285780
{

	queue<int> que;
	link_* p = NULL;
	//int cnt = 0;   
	int parents = s;
	memset(visited, 0, sizeof(visited));
	memset(pa, 0, sizeof(pa));
	visited[s] = 1;
	pa[s] = -1;
	que.push(s);
	while (!que.empty()) {
		p = graph.node[que.front()].next;
		parents = que.front();
		que.pop();
		//cnt ++;   
		while (p != NULL)
		{
			if (!visited[p->data])
			{
				visited[p->data] = 1;
				pa[p->data] = parents;

				if (p->data == d)
				{

					break;
				}
				que.push(p->data);
			}
			p = p->next;
		}
	}

	parents = d;
	//cout<<parents<<" <- ";   
	vector <int> printlist;

	while (pa[parents] != -1)
	{
		//cout << parents << " - ";
		printlist.push_back(parents);

		parents = pa[parents];
	}
	//cout << parents << endl;
	printlist.push_back(parents);

	if (!printlist.empty()) {
		int m = printlist.size();
		for (int i = 1; i < m; i++) {
			cout << printlist[m - i];
			cout << "-";
		}
		cout << printlist[0];
		cout << endl;
	}
	else {
		cout << "Error: no shortest path." << endl;
	}

}
bool is_element_in_vector(vector<int> v, int element) {
	vector<int>::iterator it;

	it = find(v.begin(), v.end(), element);
	if (it != v.end()) {
		return true;
	}

	else {
		return false;
	}
}



void vertexcover(int vertex_num, vector<int>edge, Graph g) {
	Solver solver;

	vector<int> result;
	vector<int> output;

	int num = vertex_num;

	vector<int> vec_edge = edge;

	int hi = vertex_num;
	int low = 1;
	//int mid;
	vector<int> tmp = { -1 };

	while (hi >= low) {
		int mid = (hi + low) / 2;

		result = find_vertexcover(num, vec_edge, mid,g);

		bool nosat = std::equal(result.begin(), result.end(), tmp.begin());
		if (not nosat) {
			hi = mid - 1;
			output.clear();
			output = result;
		}
		else {
			low = mid + 1;
		}

	}
	sort(output.begin(), output.end());

	for (int i = 0; i < output.size(); ++i) {
		cout << output[i] << " ";
	}
	cout << "\n";

}

std::vector<int> find_vertexcover(int vertex_num, vector<int>edge, int k,Graph gr) {

	Solver solver;

	Var propositions[vertex_num][k];


	//vector<vector<Minisat::Lit> > vec;

	for (int i = 0; i < vertex_num; i++) {
		for (int j = 0; j < k; j++) {
			propositions[i][j] = solver.newVar();
		}
	}

	// clause1: at least (exactly only one) one vertex is the ith vertex in the vertex cover, i in [1,k]

	for (int i = 0; i < k; i++) {
		vec<Lit> clause1;

		for (int j = 0; j < vertex_num; j++) {
			clause1.push(mkLit(propositions[j][i]));
		}

		solver.addClause(clause1);
		clause1.clear();

	}

	// clause 2. No one vertex can appear twice in a vertex cover

	for (int m = 0; m < vertex_num; m++) {
		for (int p = 0; p < k - 1; p++) {
			for (int q = p + 1; q < k; q++) {
				solver.addClause(mkLit(propositions[m][p], true), mkLit(propositions[m][q], true));
			}
		}
	}
	// clause 3. No more than one vertex appears in the mth position of the vertex cover.

	for (int m = 0; m < k; m++){
		for (int p = 0; p < vertex_num - 1; p++) {
			for (int q = p + 1; q < vertex_num; q++) {
				solver.addClause(mkLit(propositions[p][m], true), mkLit(propositions[q][m], true));
			}
		}
	}

	// 4. Every edge is incident to at least one vertex in the vertex cover
	for (int i = 0; i < edge.size(); i++){	
		vec<Lit> clause4;
		for (int m = 0; m < k; m++){
			clause4.push(mkLit(propositions[edge[i]][m]));

			clause4.push(mkLit(propositions[edge[i+1]][m]));
		}
		solver.addClause(clause4);
		clause4.clear();
		i++;
	}

	std::vector<int> vertex_cover;


	if (solver.solve()) {

		for (int i = 0; i < vertex_num; i++){
			for (int j = 0; j < k; j++){
				if (toInt(solver.modelValue(propositions[i][j])) == 0){
					vertex_cover.push_back(i);
				}
			}
		}

		return vertex_cover;
		//sort	
	}
	else {
		return { -1 };
	}
}

int main(int argc, char** argv) {
	vector<int> vec_V;
	vector<int> vec_E;


	// read from stdin until EOF
	while (!::cin.eof()) {

		string line;
		//getline(cin, line);

		//istringstream input(line);

		while (getline(cin, line)) {
			char arr[MAX];

			strcpy(arr, line.c_str());

			if (arr[0] == 'V') {
				//cout << "vertex\n";
				//cout << line << endl; //a3: edit it to print the graph that it has read before accepting an s command.

				int num = 0;
				for (int i = 0; arr[i] != '\0'; ++i) {

					if (arr[i] >= '0' && arr[i] <= '9')

						num = num * 10 + arr[i] - '0';
				}


				for (int j = 0; j <= num; ++j) {
					vec_V.push_back(j);
				}
				//for (vector<int>::iterator it = vec_V.begin(); it != vec_V.end(); it++) {
				//	//cout << *it << " ";
				//}//print vector
				while (vec_V.empty()) {
					//cout << "Error: NO vertex is enterd." << endl;
					break;
				}
				break;
			}


			else if (arr[0] == 'E') {
				int num = 0;
				int d_edge = 0;
				//cout << line << endl;//a3: edit it to print the graph that it has read before accepting an s command.
				for (int i = 0; arr[i] != '\0'; i++) {

					if (arr[i] > '0' && arr[i] <= '9') {
						/*cout << arr[i] << "\n";*/
						num = num * 10 + arr[i] - '0';
					}
					else {
						if (arr[i] == '0' && num == 0) {
							vec_E.push_back(0);
						}
						if (arr[i] == '0' && (num != 0)) {
							num = num * 10 + arr[i] - '0';
							vec_E.push_back(num);
							num = 0;
						}
						if (arr[i] != '0' && (num != 0)) {
							vec_E.push_back(num);
							num = 0;
						}

					}
				}
				for (int i = 0; i < vec_E.size(); i++) {

					if (is_element_in_vector(vec_V, vec_E[i]) == false) {
						//cout << vec_E[i];
						cout << "Error:some vertex in  edge not exists.";// check the error of edge: not a vertex
						vec_E.erase(vec_E.begin() + i);
						return 0;
					}
				}
				while (vec_E.empty()) {
					cout << " " << endl;
					return 0;
				}
				Graph graph = readGraph(vec_V, vec_E);

				// get vertex cover using minisat
				int vertex_num = vec_V.size();

				vertexcover(vertex_num, vec_E,graph);


			}
		}
	}
	return 0;
}
