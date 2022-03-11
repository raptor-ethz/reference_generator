#include "graph.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <limits.h>
#include <math.h>

Graph::Graph(const std::vector<std::vector<int>> &grid) {
  assert(grid.size() > 0);
  this->numRows = grid.size();
  this->numCols = grid[0].size();
  this->numVertices = this->numRows * this->numCols;

  std::vector<std::vector<int>> vec(this->numVertices,
                                    std::vector<int>(this->numVertices, 0));
  this->adjMatrix = vec;

  for (int i = 0, n = grid.size(); i < n; ++i)
    for (int j = 0, m = grid[i].size(); j < m; ++j)
      setEdges(grid, i, j);

  // std::cout << "Adjacency Matrix: " << std::endl;
  // printAdjMatrix();
}

Graph::Graph(const std::vector<std::vector<std::vector<int>>> &grid) {
  assert(grid.size() > 0);
  this->numx = grid.size();
  this->numy = grid[0].size();
  this->numz = grid[0][0].size();
  this->numVertices = this->numx * this->numy * this->numz;

  std::vector<std::vector<std::vector<int>>> vec;
  for (int i = 0; i < this->numx; i++) {
    std::vector<std::vector<int>> tmp2D;
    for (int j = 0; j < this->numy; j++) {
      std::vector<int> tmp1D;
      for (int k = 0; k < this->numz; k++) {
        tmp1D.push_back(0);
      }
      tmp2D.push_back(tmp1D);
    }
    vec.push_back(tmp2D);
  }
  this->adjMatrix3D = vec;

  for (int i = 0, n = grid.size(); i < n; ++i)
    for (int j = 0, m = grid[i].size(); j < m; ++j)
      for (int k = 0, o = grid[i][j].size(); k < o; ++k)
        setEdges(grid, i, j, k);
}

void Graph::printAdjMatrix() {
  for (int i = 0, n = this->numVertices; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      std::cout << this->adjMatrix[i][j] << " ";
    std::cout << std::endl;
  }
}

int Graph::convertTo1D(int i, int j) { return i * this->numCols + j; }

int Graph::convertTo1D(int i, int j, int k) {
  return i * this->numy * this->numz + j * this->numz + k;
}

std::vector<int> Graph::convertTo2D(int vertex) {
  std::vector<int> result;
  int x = vertex / this->numCols, y = vertex % this->numCols;
  result.push_back(x);
  result.push_back(y);
  return result;
}

std::vector<int> Graph::convertTo3D(int vertex) {
  int x = vertex / (this->numy * this->numz),
      y = (vertex / (this->numz)) % this->numy, z = vertex % this->numz;
  return {x, y, z};
}

void Graph::setEdges(const std::vector<std::vector<std::vector<int>>> &grid,
                     int i, int j, int k) {
  if (grid[i][j][k] == 1)
    return;
  if (i > 0 && grid[i - 1][j][k] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i - 1, j, k);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (i < numx - 1 && grid[i + 1][j][k] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i + 1, j, k);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (j > 0 && grid[i][j - 1][k] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i, j - 1, k);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (j < numy - 1 && grid[i][j + 1][k] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i, j + 1, k);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (k > 0 && grid[i][j][k - 1] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i, j, k - 1);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (k < numz - 1 && grid[i][j][k + 1] != 1) {
    int u = convertTo1D(i, j, k), v = convertTo1D(i, j, k + 1);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
}

void Graph::setEdges(const std::vector<std::vector<int>> &grid, int i, int j) {
  if (grid[i][j] == 1)
    return;
  if (i > 0 && grid[i - 1][j] != 1) {
    int u = convertTo1D(i, j), v = convertTo1D(i - 1, j);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
    if (j > 0 && grid[i - 1][j - 1] != 1) {
      v = convertTo1D(i - 1, j - 1);
      this->adjMatrix[u][v] = 1;
      this->adjMatrix[v][u] = 1;
    } else if (j < numCols - 1 && grid[i - 1][j + 1] != 1) {
      v = convertTo1D(i - 1, j + 1);
      this->adjMatrix[u][v] = 1;
      this->adjMatrix[v][u] = 1;
    }
  }
  if (j > 0 && grid[i][j - 1] != 1) {
    int u = convertTo1D(i, j), v = convertTo1D(i, j - 1);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
  if (i < numRows - 1 && grid[i + 1][j] != 1) {
    int u = convertTo1D(i, j), v = convertTo1D(i + 1, j);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
    if (j > 0 && grid[i + 1][j - 1] != 1) {
      v = convertTo1D(i + 1, j - 1);
      this->adjMatrix[u][v] = 1;
      this->adjMatrix[v][u] = 1;
    } else if (j < numCols - 1 && grid[i + 1][j + 1] != 1) {
      v = convertTo1D(i + 1, j + 1);
      this->adjMatrix[u][v] = 1;
      this->adjMatrix[v][u] = 1;
    }
  }
  if (j < numCols - 1 && grid[i][j + 1] != 1) {
    int u = convertTo1D(i, j), v = convertTo1D(i, j + 1);
    this->adjMatrix[u][v] = 1;
    this->adjMatrix[v][u] = 1;
  }
}

std::vector<int> Graph::getAdjacent(int u) {
  std::vector<int> result;

  for (int v = 0; v < this->numVertices; ++v)
    if (this->adjMatrix[u][v] != 0)
      result.push_back(v);

  return result;
}

int Graph::minDistanceVertex(std::vector<int> distances,
                             std::vector<bool> visited) {
  int min = INT_MAX, index = 0;

  for (int v = 0; v < this->numVertices; ++v) {
    if (!visited[v] && distances[v] < min) {
      min = distances[v];
      index = v;
    }
  }

  return index;
}

std::vector<int> Graph::dijkstra(int u, int v) {
  std::vector<int> distances(this->numVertices, INT_MAX);
  std::vector<bool> visited(this->numVertices, false);
  std::vector<int> parents(this->numVertices, -1);
  std::vector<int> result;

  distances[u] = 0;

  for (int i = 0; i < this->numVertices; ++i) {
    int j = minDistanceVertex(distances, visited);

    visited[j] = true;

    for (int k = 0; k < this->numVertices; ++k) {
      if (!visited[k] && this->adjMatrix[j][k] != 0 &&
          distances[j] != INT_MAX) {
        if (distances[j] + this->adjMatrix[j][k] < distances[k]) {
          distances[k] = distances[j] + this->adjMatrix[j][k];
          parents[k] = j;
        }
      }
    }
  }

  if (parents[v] != -1)
    for (int i = v; i != u; i = parents[i])
      result.push_back(i);

  std::reverse(result.begin(), result.end());
  return result;
}

int Graph::minDistanceAstar(std::vector<int> f, std::vector<int> open) {
  int min = INT_MAX, index = 0;

  for (int v = 0, n = open.size(); v < n; ++v) {
    if (f[v] < min) {
      min = f[v];
      index = v;
    }
  }

  return index;
}

std::vector<int> Graph::generateSuccessors(int u) {
  std::vector<int> successors;
  for (int i = 0; i < this->numVertices; ++i)
    if (this->adjMatrix[u][i] != 0)
      successors.push_back(i);
  return successors;
}

std::vector<int> Graph::astar(int u, int v) {
  std::vector<int> parents(this->numVertices, -1);
  std::vector<int> result;

  bool worked = calculateAstarPath(u, v, &parents);
  std::cout << worked << std::endl;

  if (parents[v] != -1)
    for (int i = v; i != u; i = parents[i])
      result.push_back(i);

  std::reverse(result.begin(), result.end());
  return result;
}

int Graph::heuristic(int u, int v, std::string metric) {
  std::vector<int> u_coords = convertTo2D(u), v_coords = convertTo2D(v);
  int result = INT_MAX;

  if (metric == "euclidean")
    result = sqrt(pow(v_coords[0] - u_coords[0], 2) +
                  pow(v_coords[1] - u_coords[1], 2));
  else
    result = abs(v_coords[0] - u_coords[0]) + abs(v_coords[1] - u_coords[1]);

  return result;
}

bool Graph::calculateAstarPath(int u, int v, std::vector<int> *parents) {
  std::vector<int> f(this->numVertices, INT_MAX);
  std::vector<int> g(this->numVertices, INT_MAX);
  std::vector<int> h(this->numVertices, INT_MAX);
  std::vector<int> open;
  std::vector<int> closed;

  open.push_back(u);

  f[u] = 0;
  g[u] = 0;
  h[u] = 0;

  while (open.size() > 0) {
    int minOpenIndex = minDistanceAstar(f, open);
    int minVertex = open[minOpenIndex];

    open.erase(open.begin() + minOpenIndex);

    std::vector<int> successors = generateSuccessors(minVertex);

    for (int k = 0, n = successors.size(); k < n; ++k) {
      int successor = successors[k];

      if (successor == v) {
        (*parents)[successor] = minVertex;
        return true;
      }

      int g_new = g[minVertex] + this->adjMatrix[minVertex][successor];
      int h_new = heuristic(successor, v, "manhattan");
      int f_new = g[successor] + h[successor];

      if (f[successor] == INT_MAX || f_new < f[successor]) {
        open.push_back(successor);
        f[successor] = f_new;
        g[successor] = g_new;
        h[successor] = h_new;
        (*parents)[successor] = minVertex;
      }
    }
    closed.push_back(minVertex);
  }

  return false;
}