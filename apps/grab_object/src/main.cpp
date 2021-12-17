#include "Gripper.h"
#include "Item.h"
#include "Quad.h"

using namespace std::this_thread;
using namespace std::chrono;
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create participants
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("Box", dp, "mocap_srl_box");
  Gripper gripper("Gripper", dp, "grip_cmd");

  // check for data
  quad.check_for_data();
  stand.check_for_data();
  box.check_for_data();

  // open gripper
  gripper.set_angle(45);

  quad.swoop(box, gripper, 2, 2, 2000);

  sleep_for(milliseconds(750));

  gripper.set_angle(45);

  sleep_for(milliseconds(300));

  gripper.set_angle(0);

  quad.land(stand);

  return 0;
}