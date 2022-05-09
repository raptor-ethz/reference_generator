
#include "Quad.h"
#include "fshelper.h"
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

  Item box("box", dp, "mocap_srl_box"); // TODO! Abort if no mocap for srl-box available

  // box.initializeMocapSub();
  // ->SEG FAULT

  box.checkMocapData();
  for (int i = 0; i < 10; i++)
  {
    std::cout << "ITEM: x: \t" << box.getPose().position.x << "\t y: \t" << box.getPose().position.y << "\t z: \t" << box.getPose().position.z << std::endl;
  }

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

  quad.quickSwoop(box, gripper, 2, -0.07, -0.02, 0.05, 2.0, 0, 10);

  // LAND
  quad.emergencyLand();

  return 0;
}