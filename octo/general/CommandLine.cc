
#include "CommandLine.hh"

#include "collection_utils.hh"
#include "string_utils.hh"
#include "verify.hh"

using namespace std;

namespace general {
    void CommandLine::parse(int argc, char** argv)
    {
        bool literalMode = false;

        for(int i=1; i < argc; i++) {
            string opt = argv[i];

            if (literalMode) {
                handleArg_(opt);
            }

            if (chompPrefix(opt, CharSlice("--")) ||
                chompPrefix(opt, CharSlice("-"))) {

                if (opt == "") {
                    literalMode = true;
                } else if (isFlag_(opt)) {
                    handleFlag_(opt);
                } else if (isOption_(opt)) {
                    verify((i + 1) < argc, "Argument required for option '" + opt + "'");
                    string arg(argv[++i]);
                    handleOption_(opt, arg);
                } else {
                    verify(false, "Unknown flag or option provided: '" + opt + "'");
                }
            } else {
                handleArg_(opt);
            }
        }
    }

    void CommandLine::addFlag(const std::string& flag, const std::string& desc)
    {
        flags_[flag] = desc;
    }

    bool CommandLine::hasFlag(const std::string& name) const
    {
        verify(isFlag_(name), "Undefined flag '" + name + "'");
        return containsKey(config_, name);
    }

    bool CommandLine::isFlag_(const std::string& flag) const
    {
        return containsKey(flags_, flag);
    }

    bool CommandLine::isOption_(const std::string& flag) const
    {
        return containsKey(options_, flag);
    }

    void CommandLine::handleFlag_(const std::string& flag)
    {
        config_[flag].push_back("true");
    }

    void CommandLine::handleOption_(const std::string& opt, const std::string& arg)
    {
        config_[opt].push_back(arg);
    }

    void CommandLine::handleArg_(const std::string& arg)
    {
        args_.push_back(arg);
    }

    void CommandLine::printHelpMessage(const std::string& cmdName,
                                       const std::string& argDesc) const
    {
        cout << "\nUsage: " << cmdName;

        int maxWidth = maxOptionSize_();
        printOptionsBrief_(argDesc);
        printOptionsTable_(maxWidth + 2);

        cout << "\n";
    }

    int CommandLine::maxOptionSize_() const
    {
        int rv = 1;

        for(auto& opt : flags_) {
            rv = std::max(rv, (int) opt.first.size());
        }

        return rv;
    }

    void CommandLine::printOptionsBrief_(const std::string& argDesc) const
    {
        for(auto& opt : flags_) {
            cout << " [--" << opt.first << "]";
        }
        for(auto& opt : options_) {
            cout << " [--" << opt.first << " <arg>]";
        }

        cout << " " << argDesc << endl;
    }

    void CommandLine::printOptionsTable_(int width) const
    {
        cout << "\n";

        for(auto& opt : flags_) {
            printTableLine_(opt.first, opt.second, width);
        }
        for(auto& opt : options_) {
            printTableLine_(opt.first, opt.second, width);
        }
    }

    void CommandLine::printTableLine_(const std::string& opt, const std::string& desc, int width) const
    {
        cout << "  --" << opt;
        for(int i=opt.size(); i < width; i++) {
            cout << " ";
        }
        cout << ": " << desc << "\n";
    }

    const vector<string>& CommandLine::getArgs() const
    {
        return args_;
    }
}

