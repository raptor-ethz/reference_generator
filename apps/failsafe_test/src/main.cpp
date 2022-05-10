#include "Gripper.h"
#include "Item.h"
#include "Quad.h"
#include "fshelper.h"
#include <csignal>

// global log string
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

int main()
{
  // register signal
  signal(SIGINT, sigintHandler);
  int x = atexit(exitHandler);
  // FASTDDS DEFAULT PARTICIPANT
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  // Item stand("Stand", dp, "mocap_srl_stand");
  Item box("box", dp, "mocap_srl_box");
  // Gripper gripper("Gripper", dp, "grip_cmd");
  Quad quad("Quad", &g_log, dp, "mocap_srl_raptor", "pos_cmd");

  box.initializeMocapSub();

  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

  // mission
  // takeoff test
  quad.goToPos(1, 1, 1.5, 0, 4000, false);
  quad.goToPos(0, 0, 1.5, 0, 4000, false);
  // real
  // quad.goToPos(0.5, 0, 1.5, 0, 4000, false);
  // quad.goToPos(0.5, 1, 1.5, 0, 4000, false);
  // quad.goToPos(1.5, 1, 1.5, 0, 4000, false);
  // quad.goToPos(1.5, 0, 1.5, 0, 4000, false);

  // simulator
  // quad.goToPos(1, 0, 2.5, 0, 4000, false);
  // quad.goToPos(1, 1, 2.5, 0, 4000, false);
  // quad.goToPos(0, 1, 2.5, 0, 4000, false);
  // quad.goToPos(0, 0, 2.5, 0, 4000, false);

  // LAND
  quad.emergencyLand();
  // quad.land(stand);

  return 0;
}