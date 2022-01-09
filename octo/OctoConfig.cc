
#include "OctoConfig.hh"

using namespace std;

OctoConfig::OctoConfig()
    : debugCompile_(false),
      debugParse_  (false)
{
}

void OctoConfig::setDebugCompile(bool v)
{
    debugCompile_ = v;
}

void OctoConfig::setDebugParse(bool v)
{
    debugParse_ = v;
}

void OctoConfig::setStartModule(const string& module)
{
    startModule_ = module;
}

bool OctoConfig::getDebugCompile() const
{
    return debugCompile_;
}

bool OctoConfig::getDebugParse() const
{
    return debugParse_;
}

const string& OctoConfig::getStartModule() const
{
    return startModule_;
}

