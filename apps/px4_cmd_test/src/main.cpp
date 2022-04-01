#include "Gripper.h"
#include "Item.h"
#include "Quad.h"

using namespace std::this_thread;
using namespace std::chrono;
int main() {

  /* FASTDDS DEFAULT PARTICIPANT  */
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */

  Item stand("Stand", dp, "mocap_srl_stand");
  // Item box("box", dp, "mocap_srl_box");
  // Gripper gripper("Gripper", dp, "grip_cmd");
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  /* END CREATE PARTICIPAN TS */

  // std::cout << "x:" << box.getPose().pose.position.x
  //           << "\t y:" << box.getPose().pose.position.y
  //           << "\t z:" << box.getPose().pose.position.z << std::endl;
  // quad.checkForData();
  // stand.checkForData();
  // pick_crate.checkForData();
  // place_crate.checkForData();

  if (!quad.takeOff()) {
    std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
              << std::endl;
    return 1;
  }

  // go to starting position
  quad.goToPos(1.5, 2, 1.5, 0, 4000, false);
  quad.goToPos(0, 0, 1.5, 0, 4000, false);
  /* LAND */
  // quad.emergencyLand();
  quad.land(stand);

  return 0;
}