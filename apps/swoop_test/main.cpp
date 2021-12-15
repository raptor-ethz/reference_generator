#include "Quad.h"

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create instance of quad
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");

  quad.go_to_pos(-1.5, 1, 2, 0, 2000, false);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  // swoop 1

  Vec3 pos_ref(0, 1, 0.5);
  Vec3 vel_ref(0, 0, 0);
  Vec3 acc_ref(0, 0, 0);

  quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  // swoop 2
  pos_ref = Vec3(1.5, 1, 2);

  quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  // go back to initial position
  quad.go_to_pos(0, 0, 2, 0, 2000, false);
}