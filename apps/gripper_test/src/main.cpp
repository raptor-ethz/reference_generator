#include "Gripper.h"

// global log string
std::string g_log;

int main()
{
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Gripper gripper("Gripper", &g_log, dp, "grip_cmd", GripperType::grip_rot);

  /* rotational gripper */
  const int MAX = 110;
  const int MIN = 0;
  while (true)
  {
    gripper.setAngleAsym(MAX, MIN);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    gripper.setAngleAsym(MIN, MIN);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    gripper.setAngleAsym(MIN, MAX);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    gripper.setAngleAsym(MAX, MAX);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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