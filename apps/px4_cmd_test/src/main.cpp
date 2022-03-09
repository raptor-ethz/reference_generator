#include "Quad.h"
// #include "MocapPubSubTypes.h"
#include "HeaderPubSubTypes.h"
#include "QuadPositionCmdPubSubTypes.h"
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");
      
    // gripper.set_back_arm(5);
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  // check for data
  quad.check_for_data();
  stand.check_for_data();

  /* SETUP & TAKEOFF */
  // temporary
  quad.set_state(initialized);

  quad.takeOff();
  /* SETUP & TAKEOFF */

  /* MISSION */
  quad.go_to_pos(1, 1, 2, 0, 4000, false);
  quad.go_to_pos(0, 0, 2, 0, 4000, false);
  /* END MISSION */

  quad.land(stand);

  return 0;
}