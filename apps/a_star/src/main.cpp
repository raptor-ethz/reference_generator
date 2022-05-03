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

    if (!quad.takeOff())
    {
        std::cerr << "Terminate Process (" << __FILE__ << ":" << __LINE__ << ")"
                  << std::endl;
        return 1;
    }

    // mission
    std::vector<float> start{0, 0, 2};
    std::vector<float> end{2, 2, 2};
    std::vector<std::vector<float>> obs{{1, 1, 2}};
    quad.goToPosAstar(start, end, obs); // LAND
                                        // quad.emergencyLand();
    quad.emergencyLand();
    // quad.land(stand);

    return 0;
}