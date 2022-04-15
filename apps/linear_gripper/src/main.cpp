#include "Gripper.h"
#include "Item.h"
#include "Quad.h"

using namespace std::this_thread;
using namespace std::chrono;

// global log string
std::string g_log;


int main()
{
  std::string log;

  /* FASTDDS DEFAULT PARTICIPANT  */
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("box", dp, "mocap_srl_box");
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

  // go to starting position
  quad.goToPos(1, 1, 1.5, 0, 4000, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  /* SWOOP*/

  // constants
  const float h0 = 2.0;
  const float length = 1.5;
  const float dx = -0.02;
  const float dy = -0.015;
  const float dz = -0.02;
  int grip_angle = 30;

  // attack pose
  gripper.setAngleSym(0);

  // // go to start position
  // quad.goToPos(box.getPose().position.x + dx,
  //              box.getPose().position.y + dy + length, h0, 90, 4500, false);
  // // std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // swoop to object
  quad.goToPos(box.getPose().position.x + dx, box.getPose().position.y + dy,
               h0, 0, 4500, false);
  // go to medium height
  quad.goToPos(box.getPose().position.x + dx, box.getPose().position.y + dy,
               box.getPose().position.z + dz + 0.64, 0, 3000, false);
  // go down
  quad.goToPos(box.getPose().position.x + dx, box.getPose().position.y + dy,
               box.getPose().position.z + dz + 0.44, 0, 4500, false);
  // close gripper
  gripper.setAngleSym(grip_angle);
  std::this_thread::sleep_for(std::chrono::milliseconds(750));

  // swoop away from object
  quad.goToPos(box.getPose().position.x + dx,
               box.getPose().position.y + dy, h0, 0, 3000, false);

  // go to drop position
  quad.goToPos(2, 1.5, 1.5, 0, 4000, false);

  // release object
  gripper.setAngleSym(0);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // LAND
  quad.land(stand);

  return 0;
}