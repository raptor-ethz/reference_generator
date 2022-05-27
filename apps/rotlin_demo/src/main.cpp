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
  Gripper gripper("Gripper", &g_log, dp, "grip_cmd", GripperType::grip_rotlin);
  Quad quad("Quad", &g_log, dp, "mocap_srl_raptor", "pos_cmd", &gripper);

  // open gripper
  gripper.setAngleRotLin(90, 90, 0);

  Item parcel("parcel", &g_log, dp, "mocap_srl_parcel"); // TODO! Abort if no mocap for srl-box available
  Item drop("drop", &g_log, dp, "mocap_srl_drop");       // TODO! Abort if no mocap for srl-box available
  Item pick("pick", &g_log, dp, "mocap_srl_pick");
  // declare pos data vectors
  parcel.setInitialPosition();
  drop.setInitialPosition();
  pick.setInitialPosition();

  const std::vector<float> pick_vec = pick.getPoseAsVector();
  const std::vector<float> parcel_vec = parcel.getPoseAsVector();
  const std::vector<float> drop_vec = drop.getPoseAsVector();

  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
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

  quad.goToPos(parcel_vec, -0.05, 0, 1.2, 0, 4000, false);

  quad.goToPos(parcel_vec, -0.05, 0, 0.12, 0, 4000, false);

  gripper.setAngleRotLin(175, 175, 115);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  quad.goToPos(parcel_vec, 0, 0, 1.2, 0, 4000, false);

  quad.goToPos(pick_vec, -1, 1, 0.3, 0, 3000, false);
  quad.goToPos(pick_vec, 0, 1, 0.3, 0, 3000, false);
  quad.goToPos(pick_vec, 0, 1, 0, 0, 3000, false);

  gripper.setAngleRotLin(90, 90, 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  quad.goToPos(pick_vec, 0, 1, 0.6, 0, 3000, false);

  // LAND
  quad.goToPos(-0.5, -0.5, 1.5, 0, 3000, false);

  quad.goToPos(-0.5, -0.5, 0.1, 0, 2000, false);
  quad.goToPos(-0.5, -0.5, 0.05, 0, 2000, false);
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