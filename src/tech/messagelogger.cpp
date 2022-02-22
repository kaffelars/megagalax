#include "consts.h"
#include "messagelogger.h"

namespace messagelogger
{
    struct loggedmessage
    {
        std::string origin;
        std::string message;
        warningseverity severity;
    };

    std::vector<loggedmessage> loggedmessages;
    void writemessage(loggedmessage m);
}

void messagelogger::logmessage(std::string origin, std::string message, warningseverity severity)
{
    if (loggedmessages.empty())
    {
        std::ofstream clearfile ("./data/programlog.txt");
        if (clearfile.is_open())
        {
            clearfile.close();
        }
    }

    loggedmessage m {.origin = origin, .message = message, .severity = severity};

    loggedmessages.push_back(m);

    writemessage(m);
}

void messagelogger::writemessage(loggedmessage m)
{
    std::ofstream myfile ("./data/programlog.txt", std::ios_base::app);
    if (myfile.is_open())
    {
        switch (m.severity)
        {
        case warningseverity::information:
            myfile << "infomessage:";break;
        case warningseverity::mild:
            myfile << "mild warning:";break;
        case warningseverity::moderate:
            myfile << "moderate warning:";break;
        case warningseverity::severe:
            myfile << "severe warning:";break;
        }
        myfile << "\n";
        myfile << "origin: \"" << m.origin << "\"\n";
        myfile << m.message << "\n";
        myfile << " ----------------------- " << "\n";

        myfile.close();
    }
}
