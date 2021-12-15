#include "Quad.h"

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // create instance of quad
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");

    // creat instance of landing stand
    Item stand("Stand", dp, "mocap_srl_stand");

    // go to inital position
    quad.go_to_pos(-2, 1, 2, 0, 2000, false);

  std::this_thread::sleep_for(std::chrono::seconds(2));

    //////////////////////////////////////////////////// swoop with mid_hold
    // swoop 1
    Vec3 pos_ref(0, 1, 0.5);
    Vec3 vel_ref(0, 0, 0);
    Vec3 acc_ref(0, 0, 0);

  quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);

    std::cout   << "Midpoint: " << quad.get_pose().pose.position.z
                << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // swoop 2
    pos_ref = Vec3(2, 1, 2);

    quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);
    //////////////////////////////////////////////////// swoop with mid_hold end

    //////////////////////////////////////////////////// swoop 0 hold
    // // swoop 1
    // Vec3 pos_ref(0, 1, 0.5);
    // Vec3 vel_ref(0, 0, 0);
    // Vec3 acc_ref(0, 0, 0);

    // quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);

    // std::cout   << "Midpoint: " << quad.get_pose().pose.position.z
    //             << std::endl;

    // // swoop 2
    // pos_ref = Vec3(2, 1, 2);

    // quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);
    //////////////////////////////////////////////////// swoop 0 hold end

    //////////////////////////////////////////////////// swoop dynamic
    // // swoop 1
    // Vec3 pos_ref(0, 1, 0.5);
    // Vec3 vel_ref(0.5, 0, 0);
    // Vec3 acc_ref(0, 0, 0);

    // quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);

    // std::cout   << "Midpoint: " << quad.get_pose().pose.position.z
    //             << std::endl;

    // // swoop 2
    // quad.set_velocity(Vec3(0.5, 0.0, 0.0));
    // pos_ref = Vec3(2, 1, 2);

    // quad.go_to_pos_min_jerk(pos_ref, vel_ref, acc_ref, 2);
    //////////////////////////////////////////////////// swoop dynamic end

    //////////////////////////////////////////////////// reached flag test
    // // go to first position
    // quad.go_to_pos(2, 1, 2, 0, 10000, true);

    // // go to second position
    // quad.go_to_pos(0, 0, 2, 0, 5000, true);
    //////////////////////////////////////////////////// reached flag test end

    // wait before landing
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // go back to initial position
    quad.land(stand);

  // go back to initial position
  quad.go_to_pos(0, 0, 2, 0, 2000, false);
}