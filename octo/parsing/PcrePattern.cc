
#include "PcrePattern.hh"

#include <sstream>
#include <vector>

#include "pcre.h"

using namespace general;
using namespace std;

namespace parsing {
    // NOTE: consider using pcre_study as well.

    PcrePattern::PcrePattern(const string& pattern, int options)
        : impl_(new Impl(pattern, options))
    {
    }

    PcrePattern::~PcrePattern()
    {
    }

    bool PcrePattern::matchAnchored(CharSlice target, int options, CharSlice& output) const
    {
        vector<CharSlice> matches;
        int errcode = 0;

        bool success = impl_->match(target, options, matches, errcode);

        if (success) {
            assert(matches.size() > 0);
            output = matches[0];
        }

        return success;
    }

    // Impl

    PcrePattern::Impl::Impl(const string& pattern, int options)
        : pattern_(pattern)
    {
        const char* errmsg = 0;
        int erroff = 0;

        pcre_ = pcre_compile(pattern_.c_str(),
                             options,
                             & errmsg,
                             & erroff,
                             0);

        if (! pcre_) {
            ostringstream msg;
            msg << "Regex /" << pattern_ << "/ has error at offset " << erroff << ": " << errmsg;
            throw runtime_error(msg.str());
        }

        int rc = pcre_fullinfo(pcre_, 0, PCRE_INFO_CAPTURECOUNT, & captures_);

        if (rc != 0) {
            pcre_free(pcre_);
            pcre_ = 0;

            throw runtime_error("cannot get capture information");
        }
    }

    PcrePattern::Impl::~Impl()
    {
        if (pcre_) {
            pcre_free(pcre_);
        }
    }

    bool PcrePattern::Impl::match(CharSlice target, int options, vector<CharSlice>& matches, int& errcode) const
    {
        assert(pcre_);

        vector<int> ovector;
        ovector.resize(captures_*3 + 3);

        int count = pcre_exec(pcre_,
                              0, // study
                              target.data(),
                              target.size(),
                              0, // start offset
                              options,
                              & ovector[0],
                              (int) ovector.size());

        bool success = count > 0;

        if (success) {
            matches.reserve(count);

            for(int i=0; i < count; i++) {
                int startpt = ovector[i*2];
                int endpt   = ovector[i*2+1];

                CharSlice captured;

                if (startpt != -1 && endpt > startpt) {
                    captured = target.slice(ovector[i*2], ovector[i*2+1]);
                }
                matches.push_back(captured);
            }

            errcode = 0;
        } else {
            errcode = count;
        }

        return success;
    }
}

