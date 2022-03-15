#include "HeaderPubSubTypes.h"
#include "Quad.h"
#include "QuadPositionCmdPubSubTypes.h"
#include "logger.h"
#include <thread>

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");

  Item stand("Stand", dp, "mocap_srl_stand");

  // check for data
  // quad.checkForData();
  stand.checkForData();

  // temporary
  // quad.setState(initialized);

  // start logger
  std::atomic<LogFlag> log_flag{run};
  std::thread th_log(startLog, std::ref(log_flag), "mocap_srl_stand");

  // wait
  for (int i = 0; i < 10; ++i) {
    std::cout << 10-i << std::endl;
    std::cout << stand.getPose().header.timestamp << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  //stop logger
  log_flag.store(stop);
  th_log.join();

  return 0;
}