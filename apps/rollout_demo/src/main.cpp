
#include "Quad.h"
#include "fshelper.h"
#include <vector>
#include <csignal>

std::string g_log;

void sigintHandler(int signum)
{
  exit(signum);
}

void exitHandler()
{
  // check length
  if (g_log.length() > 2)
  {
    std::cout << "Safe log" << std::endl;
    saveLog(g_log);
  }
}

int main()
{
  // register signal
  signal(SIGINT, sigintHandler);
  int x = atexit(exitHandler);
  // FASTDDS DEFAULT PARTICIPANT
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  Gripper gripper("Gripper", &g_log, dp, "grip_cmd", GripperType::grip_rot);
  Quad quad("Quad", &g_log, dp, "mocap_srl_raptor", "pos_cmd", &gripper);

  Item box("box", &g_log, dp, "mocap_srl_box");       // TODO! Abort if no mocap for srl-box available
  Item pick("pick", &g_log, dp, "mocap_srl_pick");    // TODO! Abort if no mocap for srl-box available
  Item place("place", &g_log, dp, "mocap_srl_place"); // TODO! Abort if no mocap for srl-box available
  Item drop("drop", &g_log, dp, "mocap_srl_drop");    // TODO! Abort if no mocap for srl-box available

  // Obstacle obstacle("obstacle", dp, "markers_srl_obstacle");
  gripper.setAngleSym(110);
  // box.initializeMocapSub();
  // drop.initializeMocapSub();
  // pick.initializeMocapSub();
  // place.initializeMocapSub();

  // declare pos data vectors
  pick.setInitialPosition();
  place.setInitialPosition();
  box.setInitialPosition();
  drop.setInitialPosition();

  const std::vector<float> pick_vec = pick.getPoseAsVector();
  const std::vector<float> place_vec = place.getPoseAsVector();
  const std::vector<float> box_vec = box.getPoseAsVector();
  const std::vector<float> drop_vec = drop.getPoseAsVector();

  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }
  // define obstacles

  std::vector<std::vector<int>> obs;
  std::vector<float> start_grid = {-2, -2, 0.5};
  std::vector<float> end_grid = {2.75, 2.75, 5.25};
  // block everything between 8,12,0 to 14,20,6
  // (x,y,z) = (0, 1, 0.5) -> (1.5, 3, 2)
  for (int x = 8; x < 16; ++x)
  {
    for (int y = 12; y < 20; ++y)
    {
      for (int z = 0; z < 8; ++z)
      {
        obs.push_back({x, y, z});
      }
    }
  }

  for (int x = 16; x < 20; ++x)
  {
    for (int y = 12; y < 20; ++y)
    {
      for (int z = 0; z < 6; ++z)
      {
        obs.push_back({x, y, z});
      }
    }
  }

  // simple potato grasp
  //  quad.goToPos(box, -0.07, 0.05, 1, 0, 3000, false);
  //  quad.goToPos(box, -0.07, 0.05, 0.3, 0, 3000, false);
  //  gripper.setAngleSym(10);
  //  quad.goToPos(box, 0, 0, 1.5, 0, 3000, false);
  //  quad.goToPos(0, 0, 1.5, 0, 4000, false);
  //  gripper.setAngleSym(60);
  //  quad.goToPos(0, 1, 1.5, 0, 4000, false);

  // quad.goToPos(box, 0, 0, 0.5, 0, 4000, false);
  // quad.goToPos(pick, -1.5, 0, 0.5, 0, 4000, false);
  // quad.goToPos(pick, 0, 0, 0.5, 0, 4000, false);
  // quad.goToPos(place, 0, 0, 0.5, 0, 4000, false);
  // quad.goToPos(drop, 0, 0, 0.5, 0, 4000, false);
  // SWOOP FOR BOX
  // quad.quickSwoop(box_vec, gripper, 2, +0.02, -0.08, 0.02, 2.0, 0, 10);
  quad.goToPos(2.82842, -0.490158, 2, 0, 4000, false);

  /* A-STAR PATHPLANNING */
  //  quad.goToPos(box, 0, 0, 1.5, 0, 4000, false);
  std::vector<float> start_vec;
  start_vec.push_back(quad.getPose().position.x);
  start_vec.push_back(quad.getPose().position.y);
  start_vec.push_back(quad.getPose().position.z);
  std::vector<int> stard_co = convertPositionToGrid(start_grid, end_grid, start_vec);

  // quad.goToPosAstar(start_vec, drop_vec, obstacle);
  std::vector<int> start{18, 8, 6};
  std::vector<int> end{4, 16, 4};

  quad.goToPosAstarStatic(start, end, obs);
  // start = end;
  // end.at(0) = 18;
  // end.at(1) = 16;
  // end.at(2) = 6;
  // quad.goToPosAstarStatic(start, end, obs);

  quad.goToPos(drop_vec, -0.15, -0.2, 0.5, 0, 4000, false);
  gripper.setAngleSym(80);

  // PICK UP BOTTLE
  // quad.goToPos(pick_vec, -1.0, 0.035, 0.65, 0, 3000, false);
  // quad.goToPos(pick_vec, -0.11, 0.035, 0.65, 0, 4000, false);
  // quad.goToPos(pick_vec, -0.11, 0.035, 0.28, 0, 6000, false);
  // gripper.setAngleSym(0);
  // std::this_thread::sleep_for(std::chrono::milliseconds(150));
  // quad.goToPos(pick_vec, -0.11, 0.035, 0.65, 0, 4000, false);
  // quad.goToPos(pick_vec, -1.0, 0.035, 0.65, 0, 4000, false);

  // quad.goToPos(place_vec, -0.15, -0.02, 0.7, 0, 5000, false);
  // quad.goToPos(place_vec, -0.15, -0.02, 0.175, 0, 3000, false);
  // gripper.setAngleSym(60);
  // quad.goToPos(place_vec, -0.15, -0.02, 0.7, 0, 3000, false);

  // LAND
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, false);
  gripper.setAngleSym(110);
  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.goToPos(-0.5, -0.5, -0.1, 0, 2000, false);
  quad.emergencyLand();

  return 0;
}

/*
  // SWOOP FOR BOX
  quad.quickSwoop(box_vec, gripper, 2, +0.02, -0.08, 0.02, 2.0, 0, 10);
  // quad.goToPos(box, 0, 0, 1.5, 0, 4000, false);
  quad.goToPos(drop_vec, -0.15, -0.2, 0.5, 0, 4000, false);
  gripper.setAngleSym(80);

  // PICK UP BOTTLE
  quad.goToPos(pick_vec, -1.0, 0.035, 0.65, 0, 3000, false);
  quad.goToPos(pick_vec, -0.11, 0.035, 0.65, 0, 4000, false);
  quad.goToPos(pick_vec, -0.11, 0.035, 0.28, 0, 6000, false);
  gripper.setAngleSym(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(150));
  quad.goToPos(pick_vec, -0.11, 0.035, 0.65, 0, 4000, false);
  quad.goToPos(pick_vec, -1.0, 0.035, 0.65, 0, 4000, false);

  quad.goToPos(place_vec, -0.15, -0.02, 0.7, 0, 5000, false);
  quad.goToPos(place_vec, -0.15, -0.02, 0.175, 0, 3000, false);
  gripper.setAngleSym(60);
  quad.goToPos(place_vec, -0.15, -0.02, 0.7, 0, 3000, false);

  // LAND
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, false);
  gripper.setAngleSym(110);
  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.goToPos(-0.5, -0.5, -0.1, 0, 2000, false);
  quad.emergencyLand();

  */