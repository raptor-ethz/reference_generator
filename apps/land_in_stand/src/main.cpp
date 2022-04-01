#include "Item.h"
#include "Quad.h"

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  // creat instance of stand
  Item stand("Stand", dp, "mocap_srl_stand");
  // create instance of quad
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd", &stand);

  quad.land(stand);
}