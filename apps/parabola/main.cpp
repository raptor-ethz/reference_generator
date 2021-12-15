#include "Quad.h"


void discretize_parabola(std::vector<std::vector<double>> &discretization,
                        const int gridPoints,
                        const double a,
                        const double b,
                        const double c)
{
  // z = a * x * x + b * x + c

  double step = 4/double(gridPoints);

  for(double t = -2.0; t <= 2.0; t += step)
  {
      discretization.at(0).push_back(t);
      discretization.at(1).push_back(a*t*t + b*t + c);
  }
  
  assert(discretization.at(0).size() >= gridPoints);

}

int main() {
  // FastDDS default participant
  std::unique_ptr<DefaultParticipant> dp =
      std::make_unique<DefaultParticipant>(0, "raptor");

    // create instance of quad
    Quad quad("Quad", dp, "mocap_srl_quad", "pos_cmd");

    // creat instance of landing stand
    Item stand("Stand", dp, "mocap_srl_stand");

    quad.check_for_data();
    stand.check_for_data();


    const int gridPoints = 300; // -> 6second per circle

    const double a = 0.375;
    const double b = 0.0;
    const double c = 0.5;

    std::vector<std::vector<double>> parabola(2);

    discretize_parabola(parabola, gridPoints, a, b, c);

    quad.set_thresh(0.05, 0.05, 0.05);

    // go to inital position
    quad.go_to_pos(-2.0, 1, 2, 0, 5000, false);

    // control loop
    for (int i = 0; i < gridPoints; ++i)
    {
      quad.go_to_pos(parabola.at(0).at(i),
                      1, parabola.at(1).at(i), 0, 20, true);
      // std::cout << "Reference: " << parabola.at(0).at(i) << "\t" 
      //           << parabola.at(1).at(i) << std::endl;
    }
    

  

    // go back to initial position
    quad.land(stand);
}