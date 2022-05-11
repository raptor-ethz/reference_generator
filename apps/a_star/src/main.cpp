#include "Quad.h"
#include "Obstacle.h"
#include "fshelper.h"
#include <csignal>

// global log string
std::string g_log;

void sigintHandler(int signum)
{
    std::cout << "\nCaught signal interrupt: " << signum << std::endl;
    exit(signum);
}

void exitHandler()
{
    std::cout << "Caught exit." << std::endl;
    // check length
    if (g_log.length() > 2)
    {
        std::cout << "Saving log." << std::endl;
        saveLog(g_log);
    }
}

int main()
{
    // register signal
    signal(SIGINT, sigintHandler);
    int x = atexit(exitHandler);
    // FASTDDS DEFAULT PARTICIPANT
    std::unique_ptr<DefaultParticipant> dp =
        std::make_unique<DefaultParticipant>(0, "raptor");

    /* CREATE PARTICIPANTS */
    Obstacle obstacle1("obstacle1", dp, "mocap_markers_srl_obstacle");
    Quad quad("Quad", &g_log, dp, "mocap_srl_quad", "pos_cmd");

    // if (!quad.takeOff())
    // {
    //     std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
    //               << std::endl;
    //     return 1;
    // }

    std::cout << "Start ASTAR\n";

    /* MISSION */
    std::vector<int> start{18, 8, 4};
    std::vector<int> end{4, 16, 4};
    std::vector<std::vector<int>> obs;
    // block everything between 8,12,0 to 14,20,6
    // (x,y,z) = (0, 1, 0.5) -> (1.5, 3, 2)
    for (int x = 8; x < 16; ++x) {
      for (int y = 12; y < 20; ++y) {
        for (int z = 0; z < 8; ++z) {
          obs.push_back({x,y,z});
        }
      }
    }
    
    quad.goToPosAstarStatic(start, end, obs);
    std::cout << "***********************************\n";
    start = end;
    end.at(0) = 18;
    end.at(1) = 16;
    end.at(2) = 6;
    quad.goToPosAstarStatic(start, end, obs);
    
    // quad.emergencyLand();
    // quad.land(stand);

    return 0;
}