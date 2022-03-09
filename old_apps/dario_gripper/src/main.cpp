#include "Item.h"
#include "Quad.h"
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create instance of quad
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("Box", dp, "mocap_srl_box");
  
  // check for data
  quad.check_for_data();
  stand.check_for_data();
  box.check_for_data();

  float x_ref = box.get_pose().pose.position.x;
  float y_ref = box.get_pose().pose.position.y;
  float z_ref = box.get_pose().pose.position.z;

  quad.go_to_pos(x_ref, y_ref, 1.5, 180, 4000, false);

  quad.go_to_pos(x_ref+0.025, y_ref, 0.45, 180, 7000, false);

  quad.go_to_pos(x_ref, y_ref, 1.5, 180, 4000, false);
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  quad.land(stand);
  return 0;
}