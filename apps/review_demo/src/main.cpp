#include "Gripper.h"
#include "Item.h"
#include "Quad.h"
#include "graph.h"

using namespace std::this_thread;
using namespace std::chrono;

// global log string
std::string g_log;

// PATHPLANNING HELPER FUNCTIONS

// Starting position in grid
const float x_0 = -1;
const float y_0 = -1;
const float z_0 = 0;

// Ending position in grid
const float x_1 = 3;
const float y_1 = 3;
const float z_1 = 4;

const float stepSize = 0.5; // size of a grid unit
// TODO: determine stepSize from gridSize and starting and ending positions

int gridSize = 8;
using namespace std::chrono;

// TODO: floor might not be imported yet, test
std::vector<int> convertPositionToGrid(float x, float y, float z)
{
  float step_x = (x_1 - x_0) / gridSize;
  float step_y = (y_1 - y_0) / gridSize;
  float step_z = (z_1 - z_0) / gridSize;
  return {(int)floor((x + 0.01 - x_0) / step_x), (int)floor((y + 0.01 - y_0) / step_y), (int)floor((z + 0.01 - z_0) / step_z)};
}

std::vector<float> convertGridToPosition(std::vector<int> point)
{
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

int main()
{

  std::string log;

  /* FASTDDS DEFAULT PARTICIPANT  */
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("box", dp, "mocap_srl_box");
  Item drop("drop", dp, "mocap_srl_drop");
  Gripper gripper("Gripper", &g_log, dp, "grip_cmd");
  Quad quad("Quad", &log, dp, "mocap_srl_quad", "pos_cmd", &gripper, &stand);
  /* END CREATE PARTICIPAN TS */

  // std::cout << "x:" << box.getPose().pose.position.x
  //           << "\t y:" << box.getPose().pose.position.y
  //           << "\t z:" << box.getPose().pose.position.z << std::endl;
  // quad.checkForData();
  // stand.checkForData();
  // pick_crate.checkForData();
  // place_crate.checkForData();

  // check mocap data
  // if (!quad.checkForData() || !stand.checkForData()) {
  //   std::cerr << "Fatal Error (main:23) Terminate Process." << std::endl;
  //   return 1;
  // }

  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

  // // // POTATO swoop 2.5m
  const float dx = 0.05;
  const float dy = 0;
  const float dz = 0.065;
  int grip_angle = 2;

  // go to starting position
  // quad.goToPos(1.5, 2, 1.5, 90, 4000, false);
  // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  quad.quickSwoop(box, gripper, 2.5, dx, dy, dz, 2, 50, grip_angle);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // // go to pathplan start
  quad.goToPos(2.5, 2, 1.5, 0, 4000, false);

  /* PATHPLANNING */
  std::vector<std::vector<int>> coords = {
      {2, 5, 0}, {3, 5, 0}, {4, 5, 0},   {2, 6, 0}, {3, 6, 0}, {4, 6, 0},   {2, 7, 0}, {3, 7, 0},{4, 7, 0}, 
      {2, 5, 1}, {3, 5, 1}, {4, 5, 1},   {2, 6, 1}, {3, 6, 1}, {4, 6, 1},   {2, 7, 1}, {3, 7, 1},{4, 7, 1},
      {2, 5, 2}, {3, 5, 2}, {4, 5, 2},   {2, 6, 2}, {3, 6, 2}, {4, 6, 2},   {2, 7, 2}, {3, 7, 2},{4, 7, 2}, 
      {2, 5, 3}, {3, 5, 3}, {4, 5, 3},   {2, 6, 3}, {3, 6, 3}, {4, 6, 3},   {2, 7, 3}, {3, 7, 3},{4, 7, 3}, 
      {2, 5, 4}, {3, 5, 4}, {4, 5, 4},   {2, 6, 4}, {3, 6, 4}, {4, 6, 4},   {2, 7, 4}, {3, 7, 4},{4, 7, 4}, 
      {2, 5, 5}, {3, 5, 5}, {4, 5, 5},   {2, 6, 5}, {3, 6, 5}, {4, 6, 5},   {2, 7, 5}, {3, 7, 5},{4, 7, 5}, 
      {2, 5, 6}, {3, 5, 6}, {4, 5, 6},   {2, 6, 6}, {3, 6, 6}, {4, 6, 6},   {2, 7, 6}, {3, 7, 6},{4, 7, 6}, 
      {2, 5, 7}, {3, 5, 7}, {4, 5, 7},   {2, 6, 7}, {3, 6, 7}, {4, 6, 7},   {2, 7, 7}, {3, 7, 7},{4, 7, 7}, 
      {2, 5, 8}, {3, 5, 8}, {4, 5, 8},   {2, 6, 8}, {3, 6, 8}, {4, 6, 8},   {2, 7, 8}, {3, 7, 8},{4, 7, 8}};
  std::vector<std::vector<std::vector<int>>> grid;
  // start / ending
  std::vector<int> start = {8, 6, 3};
  std::vector<int> end = {0, 6, 3};

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
  Graph graph(grid);

  std::vector<int> vertices_astar =
      graph.astar(pointToVertex3D(start), pointToVertex3D(end));

  for (int i = 0, n = vertices_astar.size(); i < n; ++i)
  {
    std::vector<int> point = vertexToPoint3D(vertices_astar[i]);
    std::cout << "===================" << std::endl;
    std::cout << "x: " << x_0 + point[0] * stepSize << std::endl;
    std::cout << "y: " << y_0 + point[1] * stepSize << std::endl;
    std::cout << "z: " << z_0 + point[2] * stepSize << std::endl;
    quad.goToPos(x_0 + point[0] * stepSize, y_0 + point[1] * stepSize, z_0 + point[2] * stepSize, 0,
                 3000, true);
  }

  // // go to drop position
  quad.goToPos(drop.getPose().position.x, drop.getPose().position.y, 1.5, 0, 3000, false);

  // // release object
  gripper.setAngleSym(60);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  gripper.setAngleSym(grip_angle);

  // // LAND
  quad.land(stand);

  return 0;
}