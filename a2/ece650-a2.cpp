#include <iostream>

#include <vector>

#include <list>

#include <algorithm>

#include <string>

#include <sstream>

#include<climits>

using namespace std;

class graphClass {

  int vRange;
  vector < list < int >> adjucnt;

  public:
    graphClass(int V);
  void addEdge(int vrange, int v1, int v2);

  void clearVerticesList() {
    adjucnt.clear();
  };
  vector < list < int >> getVertexList() {
    return adjucnt;
  };
  int getVerticesRange() {
      return vRange;
   };
  void changevRange(int range) {
    vRange = range;
  }
  int getVerticesRangeFromInput(string str);
};

graphClass::graphClass(int V) {
  vRange = V;
}

int graphClass::getVerticesRangeFromInput(string st) {

  st.erase(std::remove(st.begin(), st.end(), 'V'), st.end());
  int v = std::stoi(st);
  return v+1;
}

void graphClass::addEdge(int range, int src, int dest) {

  adjucnt.resize(range);
  adjucnt[src].push_back(dest);
  adjucnt[dest].push_back(src);

}

bool BreadthFirstSearch(int vertices, vector < list < int >> adjucnt, int src, int dest, int dist[], int prev[]) {

  //queue
  list < int > queue;

  bool visited[vertices];

  for (int i = 0; i < vertices; i++) {
    visited[i] = false;
    dist[i] = INT_MAX;
    prev[i] = -1;
  }

  visited[src] = true;
  queue.push_back(src);
  dist[src] = 0;

  while (!queue.empty()) {
    int front = queue.front();
    queue.pop_front();
    list < int > ::iterator it;
    // for (int k=0; k< g[front].size(); k++){
    //     if( visited[ g[front][k] ] == false){
    //         queue.push_back(g [front] [k]);
    //         visited[ g[front][k] ] = true;
    //         prev[ g[front][k]] = front;
    //         dist[ g[front][k] ] = dist[front]+1;

    //         if (g[front][k] == dest)
    //             return true;

    //     }

    // }
    for (it = adjucnt[front].begin(); it != adjucnt[front].end(); ++it) {
      if (visited[ * it] == false) {
        visited[ * it] = true;
        dist[ * it] = dist[front] + 1;
        prev[ * it] = front;
        queue.push_back( * it);

        if ( * it == dest) { // We stop BreadthFirstSearch when we find destination.

          queue.clear();
          return true;
        }
      }

    }
  }
  return false;

}

void shortDistance(vector < list < int >> g, int vertices, int src, int dest) {

  int prev[vertices], dist[vertices];

  bool existingPath = BreadthFirstSearch(vertices, g, src, dest, dist, prev);
  if (existingPath == false) {
    cerr << "Error: Path between the given source and destination doesnot exist";
    return;
  }

  vector < int > path;
  int end = dest;
  path.push_back(end);
  while (prev[end] != -1) {
    path.push_back(prev[end]);
    end = prev[end];
  }

  for (int k = path.size() - 1; k >= 1; --k) {
    cout << path[k] << "-";
  }
  cout << path[0];
}

int main() {

  graphClass g(0);
  vector < list < int >> adjucnt = g.getVertexList();

  while (!cin.eof()) {

    string input_line;
    getline(std::cin, input_line);

    if (input_line[0] == 'V') {
      g.clearVerticesList();
      int v = g.getVerticesRangeFromInput(input_line);
      g.changevRange(v);
      continue;
    } else if (input_line[0] == 'E') {
      adjucnt.resize(g.getVerticesRange());
      //string str = "E {<1,3>,<3,2>,<3,4>,<4,5>,<5,2>}";
      string str = input_line;
      str.erase(std::remove(str.begin(), str.end(), 'E'), str.end());
      str.erase(std::remove(str.begin(), str.end(), '{'), str.end());
      str.erase(std::remove(str.begin(), str.end(), '}'), str.end());
      str.erase(std::remove(str.begin(), str.end(), '<'), str.end());
      str.erase(std::remove(str.begin(), str.end(), '>'), str.end());

      vector < string > v;
      stringstream ss(str);
      int flag = 0;
      while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        if (std::stoi(substr) < 1 || std::stoi(substr) > g.getVerticesRange()-1) {
          flag = 1;
          break;
        }
        v.push_back(substr);
      }

      if (flag == 0) {

        for (unsigned int i = 0; i < v.size(); i = i + 2) {
          g.addEdge(g.getVerticesRange(), std::stoi(v[i]), std::stoi(v[i + 1]));
        }
        adjucnt = g.getVertexList();
        continue;
      } else if (flag == 1) {
        cerr << "Error: one or more vertex out of range: !";
	g.changevRange(0);
        adjucnt.clear();

      }

    } else if (input_line[0] == 's') {

      if (g.getVerticesRange() != 0 && !adjucnt.empty()) {
        stringstream sss(input_line);
        int j = 0;
        string arr[3];
        while (sss.good() && j < 3) {
          string ele;
          getline(sss, ele, ' ');
          arr[j] = ele;
          ++j;
        }

        int src = std::stoi(arr[1]);
        int dest = std::stoi(arr[2]);
        if ((src >= 0 && dest >= 0) && (src < g.getVerticesRange() && dest < g.getVerticesRange())) {
          shortDistance(adjucnt, g.getVerticesRange(), src, dest);
          continue;
        } else {
        //  g.changevRange(0);
        //  adjucnt.clear();
          cerr << "Error:Vertex not valid";
          continue;
        }

      } else {

        if (g.getVerticesRange() == 0) {
          g.changevRange(0);
          adjucnt.clear();
          cerr << "Error:No vertex range available!";
        } else {
          g.changevRange(0);
          adjucnt.clear();
          cerr << "Error:No Edges are available!";
        }
      }

    }

  }
  return 0;
}
