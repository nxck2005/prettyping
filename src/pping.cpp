#include "../lib/cxxopts.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>


// For color
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define IS_TTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define IS_TTY isatty
#define FILENO fileno
#endif

// CONSTANTS
// copied from the original ping source code at http://www.ping127001.com/pingpage/ping.text
#define MAXWAIT 10 // max time to wait for response in sec
#define MAXPACKET 4096 // max packet size
#define VERBOSE 1 // verbose flag
#define QUIET 2 // quiet flag
#define FLOOD 4 // floodping flag (should i have this???)
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif


// To manage terminal colors
// slapped in from my itfl project
class TerminalColor {
    public:
    const std::string red;
    const std::string green;
    const std::string blue;
    const std::string reset;

    // Set the ansi codes if the term isnt a TTY. Logic is in the private func
    TerminalColor() :
        red(colorSupport() ? "\033[31m" : ""),
        green(colorSupport() ? "\033[32m" : ""),
        blue(colorSupport() ? "\033[34m" : ""),
        reset(colorSupport() ? "\033[0m" : "")
    {}
    
    private:
    bool colorSupport() const {
        #ifdef _WIN32
        // Use normal text if user is on windows.
        return false;
        #else
        return IS_TTY(FILENO(stdout));
        #endif
    }
};

class PingTool {
    public:
    private:
};

int main(int argc, char* argv[]) {
    try {
        cxxopts::Options options("prettyping", "A prettier ping tool");
        options.add_options()
            ("a, address", "Domain to ping", cxxopts::value<std::string>())
            ("i, ip", "IP address to ping", cxxopts::value<std::string>())
            ("c, count", "Count of packets", cxxopts::value<int>())
            ("v, version", "Print version information")
            ("h, help", "Print usage");

        options.parse_positional({"ip", "address"});
        options.allow_unrecognised_options();

        // Return an object that contains proxy objects for the arguments
        auto result = options.parse(argc, argv);
        auto unmatchedresult = result.unmatched();

        const TerminalColor color;

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("version")) {
            std::cout << "pping version " << "0.0.1" << std::endl;
            return 0;
        }

        if (result.count("address") == 0 && result.count("ip") == 0) {
            std::cerr << color.red << "Error: " << color.reset << "Missing required arguments. \n\n" << options.help() << std::endl;
            return 1;
        }


        // create socket for icmp protocol
        int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        if (sockfd == -1) {
            std::cerr << color.red << "Error: " << color.reset << "socket returned a negative file descriptor (are you elevated?)" << std::endl;
            return 1;
        }

        u_char packet[MAXPACKET];
        sockaddr_in from; // here, basically
        sockaddr_in to;
        int datalen;
        std::string hostname;
        std::string hnamebuf[MAXHOSTNAMELEN];
        int npackets;
        int preload = 0;
        int ntransmitted = 0;
        int ident;

        int nrecieved = 0;
        int timing = 0;
        int tmin = INT32_MAX;
        int tmax = 0;
        int tsum = 0;
        
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;        
    }
}