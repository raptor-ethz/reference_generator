
#include "Quad.h"
#include "fshelper.h"
#include <vector>
#include <csignal>

#define SWOOP
//#define PICK_AND_PLACE
//#define BOX_LIFT

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

  Item box("box", &g_log, dp, "mocap_srl_box");          // TODO! Abort if no mocap for srl-box available
  Item parcel("parcel", &g_log, dp, "mocap_srl_parcel"); // TODO! Abort if no mocap for srl-box available
  Item pick("pick", &g_log, dp, "mocap_srl_pick");       // TODO! Abort if no mocap for srl-box available
  Item place("place", &g_log, dp, "mocap_srl_place");    // TODO! Abort if no mocap for srl-box available
  Item drop("drop", &g_log, dp, "mocap_srl_drop");       // TODO! Abort if no mocap for srl-box available

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
  parcel.setInitialPosition();

  const std::vector<float> pick_vec = pick.getPoseAsVector();
  const std::vector<float> place_vec = place.getPoseAsVector();
  const std::vector<float> box_vec = box.getPoseAsVector();
  const std::vector<float> drop_vec = drop.getPoseAsVector();
  const std::vector<float> parcel_vec = parcel.getPoseAsVector();

  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

#ifdef SWOOP
  // SWOOP FOR BOX
  quad.quickSwoop(box_vec, gripper, 2, -0.06, 0.02, 0.04, 2.0, 0, 5);
  // quad.goToPos(2.82842, -0.490158, 2, 0, 4000, false);
  quad.goToPos(drop_vec.at(0), -0.490158, 2, 0, 4000, true);
  quad.goToPos(drop_vec.at(0) - 0.15, drop_vec.at(1) - 0.2, 2, 0, 3000, true);
  quad.goToPos(drop_vec, -0.15, -0.1, 0.5, 0, 4000, true);
  gripper.setAngleSym(80);
  quad.goToPos(drop_vec.at(0) - 0.15, drop_vec.at(1) - 0.0, 2.75, 0, 3000, true);

  // intermediate land
  quad.goToPos(-0.5, -0.5, 1.5, 0, 4000, true);
  gripper.setAngleSym(110);
  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.emergencyLand();
#endif

#ifdef PICK_AND_PLACE
  quad.goToPos(drop_vec.at(0) - 0.15, drop_vec.at(1) - 0.0, 1.0, 0, 4000, false);
  quad.goToPos(drop_vec.at(0) - 0.15, drop_vec.at(1) - 0.0, 2.75, 0, 3000, true);

  // Pick Bottle
  quad.goToPos(pick_vec.at(0) - 0.11, pick_vec.at(1) + 0.18, 2.75, 0, 3000, false);
  quad.goToPos(pick_vec, -0.21, 0.0, 0.62, 0, 4000, false);

  // PICK UP BOTTLE
  // quad.goToPos(pick_vec, -1.0, 0.035, 0.65, 0, 3000, false);
  // quad.goToPos(pick_vec, -0.11, 0.035, 0.65, 0, 4000, false);
  quad.goToPos(pick_vec, -0.21, 0.0, 0.28, 0, 3000, false);
  gripper.setAngleSym(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(150));
  quad.goToPos(pick_vec, -0.21, 0.0, 0.65, 0, 4000, true);
  quad.goToPos(pick_vec, -1.5, 0.10, 0.65, 0, 4000, true);
  quad.goToPos(pick_vec, -1.5, 0.10, 0, 0, 4000, true);
  quad.goToPos(place_vec, -0.20, -0.08, 0.7, 0, 5000, false);
  quad.goToPos(place_vec, -0.20, -0.08, 0.12, 0, 3000, false);
  gripper.setAngleSym(60);
  quad.goToPos(place_vec, -0.15, 0.00, 0.7, 0, 3000, false);

  // intermediate land
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, false);
  gripper.setAngleSym(110);
  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.goToPos(-0.5, -0.5, 0.05, 0, 2000, false);
  quad.emergencyLand();
#endif

#ifdef BOX_LIFT
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, false);
  quad.goToPos(place_vec, -0.15, 0.00, 0.7, 0, 3000, false);
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, true);
  quad.goToPos(parcel_vec, -0.135, -0.105, 0.5, 0, 4000, false);
  quad.goToPos(parcel_vec, -0.135, -0.105, 0.2, 0, 4000, false);
  gripper.setAngleSym(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(150));
  quad.goToPos(parcel_vec, 0.0, 0.0, 1.2, 0, 4000, true);
  quad.goToPos(parcel_vec, 0.0, 0.0, 2.5, 0, 4000, false);
  quad.goToPos(pick_vec, 0.0, 0.65, 0.45, 0, 4000, false);
  quad.goToPos(pick_vec, 0.0, 0.65, 0.10, 0, 4000, false);
  gripper.setAngleSym(110);
  std::this_thread::sleep_for(std::chrono::milliseconds(150));
  quad.goToPos(parcel_vec, 0.0, 0.0, 2.5, 0, 4000, false);

  // intermediate land
  quad.goToPos(-0.5, -0.5, 1.5, 0, 4000, false);
  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.goToPos(-0.5, -0.5, 0.05, 0, 2000, false);
  quad.emergencyLand();
#endif

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