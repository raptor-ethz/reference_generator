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
  //Item drop("drop", dp, "mocap_srl_drop");
  Gripper gripper("Gripper", dp, "grip_cmd");

  // check for data (MOCAP)
  quad.check_for_data();
  stand.check_for_data();
  box.check_for_data();
  //drop.check_for_data();

  // first swoop
  //quad.swoop(box,gripper,2,0,0,0,2,0,45);
  for(int i=0; i<18; i++){
     std::cout<<"FLIGHT_NR"<<i<<std::endl;
    //quad.quick_swoop(box, gripper, 2, 0.05, 0.03, -0.02, 2, 50, 3); //potato
    //quad.quick_swoop(box, gripper, 2, 0.05, 0.03, -0.035, 2, 50, 0); //box
    quad.quick_swoop(box, gripper, 2, 0.05, 0.05, 0, 2, 50, 0); //paper_roll

    gripper.set_angle_sym(60);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    gripper.set_angle_sym(5);
    
    
    //FOTOS
    // std::this_thread::sleep_for(std::chrono::milliseconds(6000));
    // gripper.set_angle_sym(60);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // gripper.set_angle_sym(5);
    
    quad.go_to_pos(-2, 0.5, 2, 0, 4000, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  // quad.quick_release(drop, gripper, 1.5, 2, 2000);

  // // second swoop
  // quad.quick_swoop(box, gripper, 2, 0.05, 0.33, -0.04, 2, 200, 0);
  // quad.quick_release(drop, gripper, 1.5, 2, 2000);

  // // third swoop
  // quad.quick_swoop(box, gripper, 2, 0.05, -0.26, -0.01, 2, 100, 0);
  // quad.quick_release(drop, gripper, 1.5, 2, 2000);

  quad.land(stand);

  return 0;
}