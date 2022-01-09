
#ifndef __PARSE_TREE_WALKER_HH__
#define __PARSE_TREE_WALKER_HH__

#include "ParseTree.hh"

namespace parsing {
    // This class provides a useful approach for iterating parse trees.

    class ParseTreeWalker {
    public:
        ParseTreeWalker(const ParseTree& tree);

        // Operations for sequential access.

        // Select the next subnode (regardless of contents); returns true if there is a next node.
        bool next();

        // Select the next subnode if it has the specified tag.
        bool matchNode(const std::string& tag);

        // Require the next subnode to have the specified tag.
        void requireNode(const std::string& tag);

        const ParseTree& getCurrent() const;

        bool isComplete() const;

        // Operations for non-sequential access.

        size_t size() const;

        const ParseTree& get(size_t i) const;

    private:
        // Select the next subnode if it has the specified tag, and return true.  Otherwise, return
        // false and put the actual tag in 'actual'.
        bool matchNode_(const std::string& tag, std::string& actual);

        ParseTree tree_;
        ParseTree current_;
        size_t    index_;
        bool      found_;
    };
}

#endif

