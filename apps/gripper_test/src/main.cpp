#include "Gripper.h"

int main()
{
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Gripper gripper("Gripper", dp, "grip_cmd");

  /* rotational gripper */
  while (true)
  {
    std::cout << "front arm to 45" << std::endl;
    gripper.setAngleAsym(60, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "back arm to 5" << std::endl;
    gripper.setAngleAsym(5, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "front arm to 5" << std::endl;
    gripper.setAngleAsym(5, 60);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "back arm to 45" << std::endl;
    gripper.setAngleAsym(60, 60);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "triggering gripper" << std::endl;
    gripper.triggerGripper();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    gripper.updateSensor();
    std::cout << "sensor reading: \t" << gripper.getSensorBackLeft() << "\t" << gripper.getSensorBackRight() << "\t" << gripper.getSensorFrontLeft() << "\t" << gripper.getSensorFrontRight() << "\t" << std::endl;
  }

  /* HASEL gripper */
  // std::cout << "closing HASEL gripper" << std::endl;
  // gripper.set_front_arm(0);
  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  // std::cout << "opening HASEL gripper" << std::endl;
  // gripper.set_front_arm(1);
  // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  return 0;
}