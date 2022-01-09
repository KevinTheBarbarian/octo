
#ifndef __COMMAND_LINE_HH__
#define __COMMAND_LINE_HH__

#include <iostream>
#include <map>
#include <vector>

namespace general {
    class CommandLine {
    public:
        void addFlag(const std::string& name, const std::string& desc);
        void addOption(const std::string& name, const std::string& desc);

        void parse(int argc, char** argv);

        bool hasFlag(const std::string& name) const;

        void printHelpMessage(const std::string& cmdName,
                              const std::string& argDesc) const;

        const std::vector<std::string>& getArgs() const;

    private:
        bool isFlag_(const std::string& arg) const;
        bool isOption_(const std::string& arg) const;

        void handleArg_(const std::string& arg);
        void handleFlag_(const std::string& flag);
        void handleOption_(const std::string& opt, const std::string& arg);

        int maxOptionSize_() const;
        void printOptionsBrief_(const std::string& argDesc) const;
        void printOptionsTable_(int width) const;
        void printTableLine_(const std::string& opt, const std::string& desc, int width) const;

        std::map<std::string, std::string>              flags_;
        std::map<std::string, std::string>              options_;
        std::map<std::string, std::vector<std::string>> config_;
        std::vector<std::string>                        args_;
    };
}

#endif

