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

  // check for data
  quad.checkForData();
  

  //PARTIAL

 // go to initial position
  quad.goToPos(-1.3, 0.5, 0.85, 0, 4000, false);

  //go up
  quad.goToPos(1.3, 0.5, 0.85, 0, 4000, false);


  // // ////LOW SWOOP
  // // go to initial position
  // quad.goToPos(-2.0, 0.5, 1.6, 0, 4000, false);

  // // go down
  // quad.goToPos(0.0, 0.5, 0.1, 0, 6000, false);

  // //go up
  // quad.goToPos(2.0, 0.5, 1.6, 0, 4000, false);


  ////HIGH SWOOP
  // // go to initial position
  // quad.goToPos(-2.0, 0.5, 3.1, 0, 4000, false);

  // // go down
  // quad.goToPos(0.0, 0.5, 1.6, 0, 6000, false);

  // //go up
  // quad.goToPos(2.0, 0.5, 3.1, 0, 4000, false);


  // //std::cout << "Position: \t" << quad.get_pose().pose.position.x
  //           << '\t' << quad.get_pose().pose.position.y
  //           << '\t' << quad.get_pose().pose.position.z
  //           <e< std::endl;

  quad.goToPos(1.0, 1.0, 0.15, 0, 4000, false);
  sleep_for(milliseconds(500));

  //quad.land(stand);

  return 0;
}