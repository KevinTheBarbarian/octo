
#ifndef __PARSE_TREE_HH__
#define __PARSE_TREE_HH__

#include "Slice.hh"
#include "Printable.hh"

#include <memory>

namespace parsing {
    class ParseTree : public general::Printable {
    public:
        ParseTree();
        ParseTree(const std::string& tag, general::CharSlice token);
        ParseTree(const std::string& tag, const std::vector<ParseTree>& subNodes);

        ~ParseTree();

        virtual void print(std::ostream& out) const;
        void printOutline(std::ostream& out, int indent) const;

        bool hasTag() const;
        bool hasToken() const;
        bool hasChildren() const;

        const std::string& getTag() const;
        const std::string& getToken() const;
        const std::vector<ParseTree>& getChildren() const;

        // Shorthand for hasTag() && getTag() == t
        bool hasTag(const std::string& t) const;

    private:
        struct Impl {
            general::NotCopyable nocopy;

            std::string            tag;
            std::string            token;
            std::vector<ParseTree> subNodes;
        };

        void processSubNode_(const ParseTree& subNode);
        void appendSubNode_(const ParseTree& subNode);

        std::shared_ptr<Impl> impl_;
    };
}

#endif

