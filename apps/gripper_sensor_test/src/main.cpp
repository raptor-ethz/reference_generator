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
  while (true)
  {
    gripper.sensorRequest();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "sensor reading: \t" << gripper.getSensorBackLeft() << "\t" << gripper.getSensorBackRight() << "\t" << gripper.getSensorFrontLeft() << "\t" << gripper.getSensorFrontRight() << "\t" << std::endl;
  }
  return 0;
}