#include "Item.h"
#include "Quad.h"
#include "graph.h"
#include <iostream>

const float x_0 = -1;
const float y_0 = -0.5;
const float z_0 = 1.5;
const float stepSize = 0.5; // size of a grid unit

int gridSize = 3;
using namespace std::chrono;

int pointToVertex(const std::vector<int> &point) {
  return point[0] * gridSize + point[1];
}

int pointToVertex3D(const std::vector<int> &point) {
  return point[0] * gridSize * gridSize + point[1] * gridSize + point[2];
}

std::vector<int> vertexToPoint(int vertex) {
  std::vector<int> result = {vertex / gridSize, vertex % gridSize};
  return result;
}

std::vector<int> vertexToPoint3D(int vertex) {
  std::vector<int> result = {vertex / (gridSize*gridSize), (vertex / gridSize) % gridSize, vertex % gridSize};
  return result;
}

/// TODO assert if drone is in blocked position
void initializeGrid(const std::vector<std::vector<int>> &points,
                    std::vector<std::vector<int>> &grid) {
  for (int i = 0; i < gridSize; ++i) {
    std::vector<int> row;
    for (int j = 0; j < gridSize; ++j) {
      row.push_back(0);
    }
    grid.push_back(row);
  }

  for (int i = 0, n = points.size(); i < n; ++i) {
    int x = points[i][0];
    int y = points[i][1];
    grid[x][y] = 1;
  }
}

void initializeGrid(const std::vector<std::vector<int>> &points,
                    std::vector<std::vector<std::vector<int>>> &grid) {
  // initialize grid
  for (int i = 0; i < gridSize; ++i) {
    std::vector<std::vector<int>> row;
    for (int j = 0; j < gridSize; ++j) {
      std::vector<int> height;
      for (int k = 0; k < gridSize; ++k) {
        height.push_back(0);
      }
      row.push_back(height);
    }
    grid.push_back(row);
  }

  for (int i = 0, n = points.size(); i < n; ++i) {
    int x = points[i][0], y = points[i][1], z = points[i][2];
    grid[x][y][z] = 1;
  }
}

int main() {
  // FastDDS default participant
  // std::unique_ptr<DefaultParticipant> dp =
  //     std::make_unique<DefaultParticipant>(0, "raptor");

  // create instance of quad
  // Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  // Item stand("Stand", dp, "mocap_srl_stand");
  // check for data
  // quad.checkForData();
  // stand.checkForData();

  // obstacles
  // std::vector<std::vector<int>> coords = {{2, 0}, {3, 0}, {2, 1},
  //                                         {3, 1}, {2, 2}, {3, 2}};

  std::vector<std::vector<int>> coords = { {0,1,0}, {0,1,1}, {1,1,0}, {1,1,1} };

  // start / ending
  std::vector<int> start = {0, 0, 0};
  std::vector<int> end = {0, 2, 1};
  // grid
  std::vector<std::vector<std::vector<int>>> grid;

  for (int i = 0; i < gridSize; i++) {
    std::vector<std::vector<int>> tmp2D;
    for (int j = 0; j < gridSize; j++) {
      std::vector<int> tmp1D;
      for (int k = 0; k < gridSize; k++) {
        tmp1D.push_back(0);
      }
      tmp2D.push_back(tmp1D);
    }
    grid.push_back(tmp2D);
  }

  std::cout<<"sdasd 1"<<std::endl;
  initializeGrid(coords, grid);
  // graph
  std::cout<<"sdasd 2"<<std::endl;
  Graph graph(grid);

  // std::chrono::steady_clock::time_point start_astar =
  // std::chrono::steady_clock::now();
  std::cout<<"sdasd 3"<<std::endl;
  std::vector<int> vertices_astar =
      graph.astar(pointToVertex3D(start), pointToVertex3D(end));
  // std::chrono::steady_clock::time_point end_astar =
  // std::chrono::steady_clock::now(); std::cout << "A* time difference: " <<
  // std::chrono::duration_cast<std::chrono::milliseconds>(end_astar -
  // start_astar).count() << " milliseconds.\n";
  // assert(vertices_astar == vertices_dijkstra);
  std::cout<<"sdasd 4"<<std::endl;
  for (int i = 0, n = vertices_astar.size(); i < n; ++i) {
    std::vector<int> point = vertexToPoint3D(vertices_astar[i]);
    std::cout<<"x: "<<x_0 + point[0]*stepSize<<std::endl;
    std::cout<<"y: "<<y_0 + point[1]*stepSize<<std::endl;
    std::cout<<"z: "<<z_0 + point[2]*stepSize<<std::endl;
    // quad.goToPos(x_0 + point[0] * stepSize, y_0 + point[1] * stepSize, z_0, 0,
     //            3000, true);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return 0;
}