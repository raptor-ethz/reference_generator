#include "Quad.h"
#include "Item.h"


int main() {
    // FastDDS default participant
    std::unique_ptr<DefaultParticipant> dp =
        std::make_unique<DefaultParticipant>(0, "raptor");

    // create instance of quad
    Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");

    // creat instance of stand
    Item stand("Stand", dp, "mocap_srl_stand");

    quad.go_to_pos(-1.5, 1, 2, 2000, false);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    quad.land(stand);

}