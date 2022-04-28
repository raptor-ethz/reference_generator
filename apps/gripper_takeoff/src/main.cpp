#include "Quad.h"
#include "fshelper.h"
#include "Gripper.h"
#include "Item.h"
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
  Quad quad("Quad", &g_log, dp, "mocap_srl_raptor", "pos_cmd");
  Gripper gripper("Gripper", &g_log, dp, "grip_cmd");
  Item box("box", dp, "mocap_srl_box");
  // box.initializeMocapSub();
  gripper.setAngleSym(80);
  if (!quad.takeOff())
  {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

  // test mission

  quad.goToPos(box.getPose().position.x, box.getPose().position.y,
               box.getPose().position.z + 2.00, 0, 4500, false);

  quad.goToPos(box.getPose().position.x, box.getPose().position.y,
               box.getPose().position.z + 1.00, 0, 4500, false);
  gripper.triggerGripper();
  quad.goToPos(box.getPose().position.x, box.getPose().position.y,
               box.getPose().position.z + 2.0, 0, 4500, false);

  quad.goToPos(box.getPose().position.x - 1.0, box.getPose().position.y,
               box.getPose().position.z + 1.5, 0, 4500, false);
  gripper.stopTriggerGripper();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  gripper.setAngleSym(80);
  quad.emergencyLand();
  // quad.goToPos(0, 0, 1.5, 0, 4000, false);

  // // static data collection
  // const int T_rest = 3000;
  // for (float z = 1.0; z > 0.5; z -= 0.1)
  // {
  //   quad.goToPos(0, 0, z, 0, T_rest, false);
  // }
  // for (float z = 0.5; z > 0; z -= 0.05)
  // {
  //   quad.goToPos(0, 0, z, 0, T_rest, false);
  // }
  // quad.goToPos(0, 0, 1, 0, 3000, false);

  // LAND
  quad.emergencyLand();

  return 0;
}