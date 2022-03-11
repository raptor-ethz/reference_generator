#include "HeaderPubSubTypes.h"
#include "Quad.h"
#include "QuadPositionCmdPubSubTypes.h"
#include "logger.h"

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");

  // check for data
  quad.checkForData();
  stand.checkForData();

  std::atomic<LogFlag> log_flag{run};

  startLog(log_flag);

  // temporary
  quad.setState(initialized);

  quad.takeOff();
  quad.goToPos(1, 1, 2, 0, 4000, false);
  quad.goToPos(0, 0, 2, 0, 4000, false);
  quad.land(stand);

  return 0;
}