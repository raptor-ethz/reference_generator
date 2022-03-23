#include "HeaderPubSubTypes.h"
#include "Quad.h"
#include "QuadPositionCmdPubSubTypes.h"
#include "logger.h"
#include "std_msgs/msgs/Header.h"
#include <thread>

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  cpp_msg::Header px4_info;

  DDSSubscriber px4_info_sub_(idl_msg::HeaderPubSubType(), &px4_info,
                              "px4_status_msgs", dp->participant());

  DDSPublisher px4_action_pub_(idl_msg::HeaderPubSubType(), "px4_commands",
                               dp->participant());

  cpp_msg::Header px4_action_cmd_{};
  px4_action_cmd_.id = "info";
  px4_action_pub_.publish(px4_action_cmd_);

  for (int i = 0; i < 100; ++i) {
    px4_action_cmd_.id = "info";
    px4_action_pub_.publish(px4_action_cmd_);
    std::cout << "ID: " << px4_info.id << std::endl;
    std::cout << "STMP: " << (int)px4_info.timestamp << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  // Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  // Item stand("Stand", dp, "mocap_srl_stand");

  // // check for data
  // quad.checkForData();
  // stand.checkForData();

  // temporary
  // quad.setState(initialized);

  // quad.takeOff();

  // start logger
  // std::atomic<LogFlag> log_flag{run};
  // std::thread th_log(startLog, std::ref(log_flag), "mocap_srl_quad");
  // th_log.detach();

  // wait for logger to initialize
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // stop logger
  // log_flag.store(stop);

  // quad.land(stand);

  return 0;
}