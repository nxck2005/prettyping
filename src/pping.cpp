#include "../lib/cxxopts.hpp"
#include <bits/stdc++.h>

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
        cxxopts::Options options("prettyping", "A prettier ping tool \n\nUsage:\n ping <domain or ip>");
        options.add_options()
            ("a, address", "Domain to ping", cxxopts::value<std::string>())
            ("i, ip", "IP address to ping", cxxopts::value<std::string>())
            ("c, count", "Count of packets", cxxopts::value<int>())
            ("v, version", "Print version information")
            ("h, help", "Print usage");

        options.parse_positional({"address"});
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

        if (result.count("filename") == 0 && result.count("hash") == 0) {
            std::cerr << color.red << "Error: " << color.reset << "Missing required arguments. \n\n" << options.help() << std::endl;
            return 1;
        }

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;        
    }
}