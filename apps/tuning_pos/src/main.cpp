#include "Item.h"
#include "Quad.h"
#include <iostream>

using namespace std::this_thread;
using namespace std::chrono;
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create participants
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");

  // check for data
  quad.check_for_data();
  stand.check_for_data();

  const float z0 = 2.0;
  
  // go to initial position
  quad.go_to_pos(-2.0, 1.0, z0, 0, 4000, false);

  // move along + e_x
  quad.go_to_pos(1.0, 1.0, z0, 0, 4000, false);
  std::cout << "Position: \t" << quad.get_pose().pose.position.x
            << '\t' << quad.get_pose().pose.position.y
            << '\t' << quad.get_pose().pose.position.z
            << std::endl;

  sleep_for(milliseconds(500));

  quad.land(stand);

  return 0;
}