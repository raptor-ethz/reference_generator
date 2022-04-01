#include "Gripper.h"
#include "Item.h"
#include "Quad.h"

using namespace std::this_thread;
using namespace std::chrono;
int main() {
  // FastDDS default participant
  /* FASTDDS DEFAULT PARTICIPANT  */
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  /* CREATE PARTICIPANTS */
  Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");
  Item stand("Stand", dp, "mocap_srl_stand");
  Item pick_crate("Pick", dp, "mocap_srl_pick_crate");
  Item place_crate("Place", dp, "mocap_srl_place_crate");
  Gripper gripper("Gripper", dp, "grip_cmd");
  /* END CREATE PARTICIPANTS */

  // quad.checkForData();
  // stand.checkForData();
  // pick_crate.checkForData();
  // place_crate.checkForData();
  // check mocap data
  if (!quad.checkForData() || !stand.checkForData() ||
      !place_crate.checkForData() || !pick_crate.checkForData()) {
    std::cerr << "Fatal Error (main:23) Terminate Process." << std::endl;
    return 1;
  }
  quad.setState(initialized);

  if (!quad.takeOff()) {
    // TODO: try again after 10s
    std::cerr << "Fatal Error (main:29) Terminate Process." << std::endl;
    return 1;
  }

  // // go to crates
  // quad.goToPos(pick_crate, 0, 0, 1, 0, 4000, false);
  // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  // quad.goToPos(place_crate, 0, 0, 1, 0, 4000, false);

  // go to initial position

  /* FANTA BOTTLE*/
  quad.quickSwoop(pick_crate, gripper, 1, 0.10, -0.11, -0.14, 1.5, 100,
                  0); // fanta bottle

  quad.place(place_crate, gripper, 0, 0.1, 0.15, 1.5); // place fanta

  /* VALSER BOTTLE*/
  quad.quickSwoop(pick_crate, gripper, 1, 0.10, 0.085, -0.18, 1.5, 100,
                  0);                                   // grab valser
  quad.place(place_crate, gripper, 0, -0.1, 0.15, 1.5); // place valser

  /* LAND */
  quad.land(stand);

  return 0;
}