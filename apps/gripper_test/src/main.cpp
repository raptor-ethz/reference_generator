#include "Gripper.h"
#include "MocapPubSubTypes.h"
int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Gripper gripper("Gripper", dp, "grip_cmd");
  while (true) {
    std::cout << "front arm to 45" << std::endl;
    gripper.set_front_arm(79);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "back arm to 45" << std::endl;
    gripper.set_back_arm(45);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "front arm to 0" << std::endl;
    gripper.set_front_arm(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "back arm to 0" << std::endl;
    gripper.set_back_arm(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return 0;
}