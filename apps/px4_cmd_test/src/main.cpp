#include "HeaderPubSubTypes.h"
#include "Quad.h"
#include "QuadPositionCmdPubSubTypes.h"

// FastDDS default participant
std::unique_ptr<DefaultParticipant> dp =
    std::make_unique<DefaultParticipant>(0, "raptor");

Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
Item stand("Stand", dp, "mocap_srl_stand");

bool terminate_mission = false;
bool mission_flag = true;

void flyMission()
{
  quad.takeOff();

  if (terminate_mission) {
    return;
  }

  quad.goToPos(1, 1, 2, 0, 4000, false);
  if (terminate_mission) {
    return;
  }
  quad.goToPos(0, 0, 2, 0, 4000, false);
  if (terminate_mission) {
    return;
  }

  quad.land(stand);

  mission_flag = false;
}

int main()
{
  // check for data
  quad.checkForData();
  stand.checkForData();

  // temporary
  quad.setState(initialized);

  std::thread mission_thread(flyMission);

  while (mission_flag) {
    std::cout << "Enter 'l' to land next: " << std::endl;
    char input;
    std::cin >> input;
    switch (input) {
    case 'l':
      terminate_mission = true;
      mission_thread.join();
      quad.land(stand);
      mission_flag = false;
      break;

    default:
      std::cout << "Unrecognized command: " << input << std::endl;
      break;
    }
  }

  // wait for the mission to finish
  mission_thread.join();

  return 0;
}