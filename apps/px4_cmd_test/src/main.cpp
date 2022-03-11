#include "HeaderPubSubTypes.h"
#include "Quad.h"
#include "QuadPositionCmdPubSubTypes.h"
#include "logger.h"
#include <thread>

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");

  // check for data
  quad.checkForData();
  stand.checkForData();

  // temporary
  quad.setState(initialized);

  quad.takeOff();
  quad.goToPos(0, -1, 2, 0, 5000, false);

  // start logger
  std::atomic<LogFlag> log_flag{run};
  std::thread th_log(startLog, std::ref(log_flag), "mocap_srl_quad");
  th_log.detach();

  
  quad.goToPos(0, 1, 2, 0, 5000, false);
  quad.goToPos(2, 1, 2, 0, 5000, false);
  quad.goToPos(2, -1, 2, 0, 5000, false);
  quad.goToPos(0, -1, 2, 0, 5000, false);

  //stop logger
  log_flag.store(stop);
  
  quad.land(stand);

  return 0;
}