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

  cpp_msg::Header px4_cmd;

  std::cout << "[INFO] Arming Quad..." << std::endl;
  px4_cmd.id = "arm";
  quad.px4_cmd_pub->publish(px4_cmd);

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  std::cout << "[INFO] Taking off..." << std::endl;
  px4_cmd.id = "takeoff";
  quad.px4_cmd_pub->publish(px4_cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(8000));

  /* MISSION */
  std::cout << "[INFO] Fly Mission" << std::endl;
  px4_cmd.id = "offboard";
  quad.px4_cmd_pub->publish(px4_cmd);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  quad.go_to_pos(1, 1, 2, 0, 4000, false);
  quad.go_to_pos(0, 0, 2, 0, 4000, false);
  /* END MISSION */

  cpp_msg::QuadPositionCmd pos_cmd;
  pos_cmd.header.id = "break";
  quad.position_pub->publish(pos_cmd);
  
  std::cout << "[INFO] Landing..." << std::endl;
  px4_cmd.id = "land";
  quad.px4_cmd_pub->publish(px4_cmd);

  std::this_thread::sleep_for(std::chrono::milliseconds(6000));

  std::cout << "[INFO] Disarming..." << std::endl;
  px4_cmd.id = "disarm";
  quad.px4_cmd_pub->publish(px4_cmd);

  return 0;
}