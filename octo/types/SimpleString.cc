
#include "SimpleString.hh"

#include "Slice.hh"

using namespace general;

namespace types {
    void SimpleString::assign(const char* data, size_t size)
    {
        data_.assign(data, size);
    }

    bool SimpleString::equalTo(const char* data) const
    {
        return CharSlice(data_) == CharSlice(data);
    }

    void SimpleString::toCppString(std::string& str) const
    {
        str = data_;
    }

    void SimpleString::scanGC(std::vector<HeapObject*>& queue)
    {
    }

    void SimpleString::printObject(std::ostream& output) const
    {
        output << "\"";

        for(size_t i=0, e=data_.size(); i != e; i++) {
            char ch = data_[i];

            switch(ch) {
            case '\n':
                output << "\\n";
                break;

            case '\r':
                output << "\\r";
                break;

            case '\t':
                output << "\\t";
                break;

            case '\\':
            case '\'':
            case '"':
                output << "\\" << ch;
                break;

            default:
                if (isprint(ch)) {
                    output << ch;
                } else {
                    char buf[16];
                    sprintf(buf, "\\x%2.2X", ch);
                    output << buf;
                }
            }
        }
        output << "\"";
    }
}

