#include "Gripper.h"
#include "MocapPubSubTypes.h"
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Gripper gripper("Gripper", dp, "grip_cmd");
  while (true) {
    gripper.set_angle(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    gripper.set_angle(30);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    gripper.set_angle(60);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  }
  return 0;
}