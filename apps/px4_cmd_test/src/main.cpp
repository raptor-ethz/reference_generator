#include "Gripper.h"
#include "Item.h"
#include "Quad.h"
#include "fshelper.h"
#include <csignal>

std::string g_log;

void sigintHandler(int signum) {
  exit(signum);
}

void exitHandler(){
  // check length
  if (g_log.length() > 2) {
    std::cout << "Safe log" << std::endl;
    saveLog(g_log);
  }
}

int main() {
  // register signal
  signal(SIGINT, sigintHandler);
  int x = atexit(exitHandler);
  // FASTDDS DEFAULT PARTICIPANT 
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  Item stand("Stand", dp, "mocap_srl_stand");
  // Item box("box", dp, "mocap_srl_box");
  // Gripper gripper("Gripper", dp, "grip_cmd");
  Quad quad("Quad", &g_log, dp, "mocap_srl_quad", "pos_cmd");

  if (!quad.takeOff()) {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

  // mission
  quad.goToPos(1.5, 2, 1.5, 0, 4000, false);
  quad.goToPos(0, 0, 1.5, 0, 4000, false);

  // LAND
  // quad.emergencyLand();
  quad.land(stand);

  return 0;
}