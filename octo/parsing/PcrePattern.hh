
#ifndef __PCRE_PATTERN_HH__
#define __PCRE_PATTERN_HH__

#include "Slice.hh"

#include <memory>

#include "pcre.h"

using namespace std;

namespace parsing {
    class PcrePattern {
    public:
        PcrePattern(const std::string& pattern, int options = 0);
        ~PcrePattern();

        bool matchAnchored(general::CharSlice target, int options, general::CharSlice& output) const;

    private:
        class Impl {
        public:
            Impl(const std::string& pattern, int options);
            ~Impl();

            bool match(general::CharSlice target, int options, vector<general::CharSlice>& matches, int& errcode) const;

        private:
            general::NotCopyable nocopy_;
            std::string pattern_;
            pcre* pcre_;
            int captures_;
        };

        std::shared_ptr<Impl> impl_;
    };
}

#endif

