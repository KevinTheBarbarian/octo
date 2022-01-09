
#include "ResultTransformer.hh"
#include "Context.hh"

#include "string_utils.hh"

using namespace general;
using namespace std;

namespace parsing {
    ResultTransformer::ResultTransformer(const std::string& tag,
                                         const RuleRef&     rule,
                                         ResultFunction*    rfn)
        : Matcher(tag), rule_(rule), rfn_(rfn)
    {
    }

    ParseResult ResultTransformer::parse(const Context& context)
    {
        ParseResult pr = rule_.parse(context);
        return (*rfn_)(pr);
    }

    void ResultTransformer::print(ostream& out) const
    {
        out << "xform(rule(" << rule_ << "),xform(" << *rfn_ << "))" << flush;
    }

    void ResultTransformer::scanGC(vector<HeapObject*>& queue)
    {
        rule_.scanGC(queue);
        queue.push_back(rfn_);
    }
}

