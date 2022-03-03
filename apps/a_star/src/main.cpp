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


  quad.land(stand);
  return 0;
}