#include "Gripper.h"
#include "MocapPubSubTypes.h"
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Gripper gripper("Gripper", dp, "grip_cmd");
  while (true) {
    std::cout << "setting angle to 0" << std::endl;
    gripper.set_angle(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "setting angle to 45" << std::endl;
    gripper.set_angle(45);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
  return 0;
}