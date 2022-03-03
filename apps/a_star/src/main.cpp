#include "Item.h"
#include "Quad.h"
#include <iostream>
#include "graph.h"



const float x_0 = -1; 
const float y_0 = -0.5;
const float z_0 = 1.5;
const float stepSize = 0.5; //size of a grid unit

int gridSize = 6;
using namespace std::chrono;

int pointToVertex(const std::vector<int>& point) {
    return point[0]*gridSize + point[1];
}

std::vector<int> vertexToPoint(int vertex) {
    std::vector<int> result = {vertex / gridSize, vertex % gridSize};
    return result;
}

///TODO assert if drone is in blocked position
void initializeGrid(const std::vector<std::vector<int>>& points, std::vector<std::vector<int>>& grid) {
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


int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create instance of quad
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  // check for data
  quad.check_for_data();
  stand.check_for_data();

  /* SETUP & TAKEOFF */
  cpp_msg::Header px4_cmd;

  // arm
  std::cout << "[INFO] Arming Quad" << std::endl;
  px4_cmd.id = "arm";
  quad.px4_cmd_pub->publish(px4_cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  // takeoff
  std::cout << "[INFO] Taking off" << std::endl;
  px4_cmd.id = "takeoff";
  quad.px4_cmd_pub->publish(px4_cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(7000));

  // offboard
  std::cout << "[INFO] Start Offboard" << std::endl;
  px4_cmd.id = "offboard";
  quad.px4_cmd_pub->publish(px4_cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  std::cout << "[INFO] Fly Mission" << std::endl;
  /* SETUP & TAKEOFF */

  //fly to starting position
  quad.go_to_pos(x_0,y_0,z_0,0,4000,false);

  //obstacles
  std::vector<std::vector<int>> coords = {{2,0}, {3,0}, {2,1}, {3,1}, {2,2}, {3,2}};
  //start / ending
  std::vector<int> start = {0,0};
  std::vector<int> end = {5,0};
  //grid
  std::vector<std::vector<int>> grid(gridSize, std::vector<int>(gridSize));
  initializeGrid(coords,grid);
  //graph
  Graph graph(grid);


  // std::chrono::steady_clock::time_point start_astar = std::chrono::steady_clock::now();
  std::vector<int> vertices_astar = graph.astar(pointToVertex(start), pointToVertex(end));
  // std::chrono::steady_clock::time_point end_astar = std::chrono::steady_clock::now();
  // std::cout << "A* time difference: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_astar - start_astar).count() << " milliseconds.\n";
  //assert(vertices_astar == vertices_dijkstra);
  for (int i = 0, n = vertices_astar.size(); i < n; ++i) {
    std::vector<int> point = vertexToPoint(vertices_astar[i]);
    // std::cout<<"x: "<<x_0 + point[0]*stepSize<<std::endl;
    // std::cout<<"y: "<<y_0 + point[1]*stepSize<<std::endl;
    quad.go_to_pos(x_0 +point[0]*stepSize, y_0+point[1]*stepSize,z_0,0,3000,true);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  quad.land(stand);

  std::cout << "[INFO] Terminate Offboard" << std::endl;

  cpp_msg::QuadPositionCmd pos_cmd;
  pos_cmd.header.id = "break";
  quad.position_pub->publish(pos_cmd);

  std::cout << "[INFO] Disarming" << std::endl;
  px4_cmd.id = "disarm";
  quad.px4_cmd_pub->publish(px4_cmd);

  return 0;
}