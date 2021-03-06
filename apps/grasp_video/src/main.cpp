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
  Gripper gripper("Gripper", &g_log, dp, "grip_cmd", GripperType::grip_rot);
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
  quad.goToPos(1.5, 2, 1.5, 90, 4000, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  /* SWOOP*/

  // CARDBOARD BOX swoop 2.5m
  // const float h0 = 2.0;
  // const float length = 2.5;
  // const float dx = -0.08;
  // const float dy = -0.1;
  // const float dz = 0.007;
  // int grip_angle = 0;

  // // PET BOTTLE swoop 2.5m
  // const float h0 = 2.0;
  // const float length = 2.5;
  // const float dx = -0.08;
  // const float dy = -0.1;
  // const float dz = 0.00;
  // int grip_angle = 2;

  // // PAPER ROLL swoop 2.5m
  // const float h0 = 2.0;
  // const float length = 2.5;
  // const float dx = -0.08;
  // const float dy = -0.1;
  // const float dz = 0.05;
  // int grip_angle = 2;

  // // POTATO swoop 2.5m
  const float h0 = 2.0;
  const float length = 2.5;
  const float dx = -0.08;
  const float dy = -0.1;
  const float dz = 0.01;
  int grip_angle = 2;

  // // POTATO swoop 2.0m
  //  const float h0 = 2.0;
  //  const float length = 2.0;
  //  const float dx = -0.07;
  //  const float dy = -0.07;
  //  const float dz = -0.02;
  // int grip_angle = 2;

  // // POTATO swoop 1.5m
  // const float h0 = 2.0;
  // const float length = 1.5;
  // const float dx = -0.07;
  // const float dy = -0.05;
  // const float dz = -0.02;
  // int grip_angle = 2;

  // attack pose
  gripper.setAngleAsym(60, grip_angle);

  // go to start position
  quad.goToPos(box.getPose().position.x + dx,
               box.getPose().position.y + dy + length, h0, 90, 4500, false);
  // std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // swoop to object
  quad.goToPos(box.getPose().position.x + dx, box.getPose().position.y + dy,
               box.getPose().position.z + dz + 0.21, 90, 4500, true);

  // close gripper
  gripper.setAngleAsym(grip_angle, grip_angle);
  // std::this_thread::sleep_for(std::chrono::milliseconds(0));

  // swoop away from object
  quad.goToPos(box.getPose().position.x + dx,
               box.getPose().position.y + dy - length, h0, 90, 3000, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  // go to drop position
  quad.goToPos(1.5, 0, 1.5, 90, 4000, false);

  // release object
  gripper.setAngleSym(60);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  gripper.setAngleSym(grip_angle);

  // LAND
  quad.land(stand);

  return 0;
}