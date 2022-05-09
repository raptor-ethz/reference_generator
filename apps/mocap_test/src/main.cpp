#include "Quad.h"
#include "Item.h"

int main()
{
  std::string log;

  /* FASTDDS DEFAULT PARTICIPANT  */
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

  Quad quad("Quad", &log, dp, "mocap_srl_raptor", "pos_cmd");

  Item box("box", dp, "mocap_srl_box");
  /* PRINT MOCAP DATA */
  while (true)
  {
    std::cout << "ITEM: x: \t" << box.getPose().position.x + 0.5 << "\t y: \t" << box.getPose().position.y + 0.5 << "\t z: \t" << box.getPose().position.z << std::endl;
    std::cout << "QUAD: x: \t" << quad.getPose().position.x + 0.5 << "\t y: \t" << quad.getPose().position.y + 0.5 << "\t z: \t" << quad.getPose().position.z << std::endl;
  }
  return 0;
}