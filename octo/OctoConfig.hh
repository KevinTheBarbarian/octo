
#ifndef __OCTO_CONFIG_HH__
#define __OCTO_CONFIG_HH__

#include <string>

class OctoConfig {
public:
    OctoConfig();

    void setDebugCompile(bool dbg);
    void setDebugParse(bool dbg);
    void setStartModule(const std::string& module);

    bool getDebugCompile() const;
    bool getDebugParse() const;
    const std::string& getStartModule() const;

private:
    bool        debugCompile_;
    bool        debugParse_;
    std::string startModule_;
};

#endif

