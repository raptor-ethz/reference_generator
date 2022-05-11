#include "Gripper.h"
#include "Item.h"
#include "Participant.h"
#include "Astar.h"

#include "QuadAction_msg.h"
#include "QuadAction_msgPubSubTypes.h"
#include "QuadFeedback_msg.h"
#include "QuadFeedback_msgPubSubTypes.h"
#include "QuadPosCmd_msg.h"
#include "QuadPosCmd_msgPubSubTypes.h"
#include "UserCmd_msg.h"
#include "UserCmd_msgPubSubTypes.h"

#include "RapidTrajectoryGenerator.h"
#include "Vec3.h"

#include <chrono>
#include <vector>

#include "fshelper.h"
#include <csignal>
#include <climits>

std::string g_log;

void sigintHandler(int signum)
{
    std::cout << "\nCaught signal interrupt: " << signum << std::endl;
    exit(signum);
}

void exitHandler()
{
    std::cout << "Caught exit." << std::endl;
    // check length
    if (g_log.length() > 2)
    {
        std::cout << "Saving log." << std::endl;
        saveLog(g_log);
    }
}

struct Obstacle {
    std::vector<float> marker_x, marker_y, marker_z;
};

int gridSize = 20, gridSize = 20, gridSize = 20;

// TODO: floor might not be imported yet, test
std::vector<int> convertPositionToGrid(std::vector<float> grid_start, std::vector<float> grid_end, std::vector<float> point)
{
  float x_0 = grid_start[0], x_1 = grid_end[0], y_0 = grid_start[1], y_1 = grid_end[1], z_0 = grid_start[2], z_1 = grid_end[2];
  float step_x = (x_1 - x_0) / gridSize, step_y = (y_1 - y_0) / gridSize, step_z = (z_1 - z_0) / gridSize;
  return {(int)floor((point[0] + 0.01 - x_0) / step_x), (int)floor((point[1] + 0.01 - y_0) / step_y), (int)floor((point[2] + 0.01 - z_0) / step_z)};
}

std::vector<float> convertGridToPosition(std::vector<float> grid_start, std::vector<float> grid_end, std::vector<int> point)
{
  float x_0 = grid_start[0], x_1 = grid_end[0], y_0 = grid_start[1], y_1 = grid_end[1], z_0 = grid_start[2], z_1 = grid_end[2];
  float step_x = (x_1 - x_0) / gridSize, step_y = (y_1 - y_0) / gridSize, step_z = (z_1 - z_0) / gridSize;
  return {x_0 + point[0] * step_x, y_0 + point[1] * step_y, z_0 + point[2] * step_z};
}

int pointToVertex(const std::vector<int> &point)
{
  return point[0] * gridSize + point[1];
}

int pointToVertex3D(const std::vector<int> &point)
{
  return point[0] * gridSize * gridSize + point[1] * gridSize + point[2];
}

std::vector<int> vertexToPoint(int vertex)
{
  std::vector<int> result = {vertex / gridSize, vertex % gridSize};
  return result;
}

std::vector<int> vertexToPoint3D(int vertex)
{
  std::vector<int> result = {vertex / (gridSize * gridSize), (vertex / gridSize) % gridSize, vertex % gridSize};
  return result;
}

/// TODO assert if drone is in blocked position
void initializeGrid(const std::vector<std::vector<int>> &points,
                    std::vector<std::vector<int>> &grid)
{
  for (int i = 0; i < gridSize; ++i)
  {
    std::vector<int> row;
    for (int j = 0; j < gridSize; ++j)
    {
      row.push_back(0);
    }
    grid.push_back(row);
  }

  for (int i = 0, n = points.size(); i < n; ++i)
  {
    int x = points[i][0];
    int y = points[i][1];
    grid[x][y] = 1;
  }
}

void initializeGrid(const std::vector<std::vector<int>> &points,
                    std::vector<std::vector<std::vector<int>>> &grid)
{
  // initialize grid
  for (int i = 0; i < gridSize; ++i)
  {
    std::vector<std::vector<int>> row;
    for (int j = 0; j < gridSize; ++j)
    {
      std::vector<int> height;
      for (int k = 0; k < gridSize; ++k)
      {
        height.push_back(0);
      }
      row.push_back(height);
    }
    grid.push_back(row);
  }

  for (int i = 0, n = points.size(); i < n; ++i)
  {
    int x = points[i][0], y = points[i][1], z = points[i][2];
    grid[x][y][z] = 1;
  }
}

void goToPosAstar(std::vector<float> start_coords, std::vector<float> end_coords, Obstacle obstacle)
{
  // put
  std::vector<std::vector<float>> obs_coords;
  for (int i = 0; i < obstacle.marker_x.size(); i++)
  {
    std::vector<float> coord{obstacle.marker_x[i], obstacle.marker_y[i], obstacle.marker_z[i]};
    obs_coords.push_back(coord);
    std::cout << "Obstacle: " << obstacle.marker_x[i] << " " << obstacle.marker_y[i] << " " << obstacle.marker_z[i] << std::endl;
  }

  std::vector<float> min_values, max_values;
  for (int j = 0; j < 3; ++j)
  {
    float min_value = INT_MAX, max_value = INT_MIN;
    for (int i = 0, n = obs_coords.size(); i < n; ++i)
    {
      min_value = std::min(min_value, obs_coords[i][j]);
      max_value = std::max(max_value, obs_coords[i][j]);
    }
    std::cout << "Min value: " << min_value << std::endl;
    std::cout << "Max value: " << max_value << std::endl;
    min_values.push_back(min_value);
    max_values.push_back(max_value);
  }

  std::vector<std::vector<float>> constraints;
  float i = min_values[0];
  while (i <= max_values[0])
  {
    float j = min_values[1];
    while (j <= max_values[1])
    {
      float k = min_values[2];
      while (k <= max_values[2])
      {
        constraints.push_back({i, j, k});
        std::cout << "constraint: " << i << " " <<  j << " " << k << std::endl;
        k += std::max((max_values[2] - min_values[2]) / (2 * gridSize),0.001f);
      }
      j += std::max((max_values[1] - min_values[1]) / (2 * gridSize),0.001f);
    }
    i += std::max((max_values[0] - min_values[0]) / (2 * gridSize),0.001f);
  }

  // grid
  std::vector<std::vector<std::vector<int>>> grid;

  // TODO: add minimum and maximum coordinates for the room
  std::vector<float>
      grid_start = {std::min(start_coords[0], std::min(end_coords[0], min_values[0])) - 1, std::min(start_coords[1], std::min(end_coords[1], min_values[1])) - 1, std::min(start_coords[2], std::min(end_coords[2], min_values[2])) - 1};
  std::vector<float> grid_end = {std::max(start_coords[0], std::max(end_coords[0], max_values[0])) + 1, std::max(start_coords[1], std::max(end_coords[1], max_values[1])) + 1, std::max(start_coords[2], std::max(end_coords[2], max_values[2])) + 1};

  std::cout << "Grid start: " << grid_start[0] << " " << grid_start[1] << " " << grid_start[2] << std::endl;
  std::cout << "Grid end: " << grid_end[0] << " " << grid_end[1] << " " << grid_end[2] << std::endl;

  std::vector<int> start = convertPositionToGrid(grid_start, grid_end, start_coords);
  std::vector<int> end = convertPositionToGrid(grid_start, grid_end, end_coords);

  std::vector<std::vector<int>> coords;
  for (int i = 0, n = constraints.size(); i < n; ++i) {
    std::cout << convertPositionToGrid(grid_start, grid_end, constraints[i]) << std::endl;
    coords.push_back(convertPositionToGrid(grid_start, grid_end, constraints[i]));
  }
  

  for (int i = 0; i < gridSize; i++)
  {
    std::vector<std::vector<int>> tmp2D;
    for (int j = 0; j < gridSize; j++)
    {
      std::vector<int> tmp1D;
      for (int k = 0; k < gridSize; k++)
      {
        tmp1D.push_back(0);
      }
      tmp2D.push_back(tmp1D);
    }
    grid.push_back(tmp2D);
  }

  initializeGrid(coords, grid);

  // graph
  Astar solver(grid, false);

  // std::chrono::steady_clock::time_point start_astar =
  // std::chrono::steady_clock::now();
  std::vector<int> vertices_astar =
      solver.astarPath(pointToVertex3D(start), pointToVertex3D(end));
  // std::chrono::steady_clock::time_point end_astar =
  // std::chrono::steady_clock::now(); std::cout << "A* time difference: " <<
  // std::chrono::duration_cast<std::chrono::milliseconds>(end_astar -
  // start_astar).count() << " milliseconds.\n";
  // assert(vertices_astar == vertices_dijkstra);
//   if (vertices_astar.size() == 0) {
//     return;
//   }
  std::vector<int> prev = vertexToPoint3D(vertices_astar[0]);
  for (int i = 0, n = vertices_astar.size(); i < n; ++i)
  {
    std::cout << "sending a pos cmd \n";
    std::vector<int> point_grid = vertexToPoint3D(vertices_astar[i]);
    // if (abs(point_grid[0] - prev[0]) + abs(point_grid[1] - prev[1]) + abs(point_grid[2] - prev[2]) == 1)
    // {
    //   prev = {point_grid[0], point_grid[1], point_grid[2]};
    //   continue;
    // }
    std::vector<float> point = convertGridToPosition(grid_start, grid_end, point_grid);
    std::cout<<"x: "<<point[0]<<std::endl;
    std::cout<<"y: "<<point[1]<<std::endl;
    std::cout<<"z: "<<point[2]<<std::endl;
    // goToPos(point[0], point[1], point[2], 0,
    //         3000, false);
  }
}

int main()
{
    
    Obstacle obstacle1;
    obstacle1.marker_x = {1,1,1,1};
    obstacle1.marker_y = {1,-1,1,-1};
    obstacle1.marker_z = {0,0,1,1};
    /* MISSION */
    std::vector<float> start{0, 0, 1};
    std::vector<float> end{2, 0, 1};
    std::vector<std::vector<float>> obs{{1, 1, 2}};
    goToPosAstar(start, end, obstacle1);
    
    // quad.emergencyLand();
    // quad.land(stand);

    return 0;
}