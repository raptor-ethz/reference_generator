#include "Gripper.h"
#include "Item.h"
#include "Quad.h"
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create participants
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  Item box("Box", dp, "mocap_srl_box");
  Gripper gripper("Gripper", dp, "grip_cmd");

  // check data
  quad.check_for_data();
  stand.check_for_data();
  box.check_for_data();

  // open gripper
  gripper.set_angle(45);

  quad.go_to_pos(box.get_pose().pose.position.x, box.get_pose().pose.position.y,
                 1.5, 0, 3000, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  gripper.set_angle(30);
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  gripper.set_angle(5);
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  gripper.set_angle(30);
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  gripper.set_angle(5);
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  gripper.set_angle(45);

  quad.go_to_pos(box.get_pose().pose.position.x, box.get_pose().pose.position.y,
                 0.45, 0, 2000, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  gripper.set_angle(5);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  quad.go_to_pos(box.get_pose().pose.position.x, box.get_pose().pose.position.y,
                 1.5, 0, 3000, false);

  quad.go_to_pos(1.5, -0.5, 2.5, 45, 3000, false);

  // drop object
  std::this_thread::sleep_for(std::chrono::seconds(1));
  gripper.set_angle(45);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  gripper.set_angle(5);

  quad.land(stand);

  // Gripper gripper("Gripper", dp, "grip_cmd");
  // while (true) {
  //   gripper.set_angle(0);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  //   gripper.set_angle(30);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  //   gripper.set_angle(60);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  // }
  return 0;
}