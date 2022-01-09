
#include "ParseTreeWalker.hh"
#include "verify.hh"

using namespace general;
using namespace std;

namespace parsing {
    ParseTreeWalker::ParseTreeWalker(const ParseTree& tree)
        : tree_(tree), index_(0), found_ (false)
    {
    }

    void ParseTreeWalker::requireNode(const string& tag)
    {
        string actual;
        matchNode_(tag, actual);
        verify(found_, "required subtree '" + tag + "' but found '" + actual + "'");
    }

    bool ParseTreeWalker::matchNode(const string& tag)
    {
        string actual;
        return matchNode_(tag, actual);
    }

    bool ParseTreeWalker::matchNode_(const string& tag, string& actual)
    {
        found_ = false;

        auto& children = tree_.getChildren();

        if (children.size() > index_) {
            auto node = children[index_];

            if (node.hasTag()) {
                if (node.getTag() == tag) {
                    current_ = node;
                    found_   = true;
                    index_++;
                    return true;
                } else {
                    actual = node.getTag();
                }
            } else {
                actual = "<no tag>";
            }
        } else {
            actual = "<eos>";
        }

        current_ = ParseTree();
        return false;
    }

    bool ParseTreeWalker::next()
    {
        auto& children = tree_.getChildren();

        found_ = (children.size() > index_);

        if (found_) {
            current_ = children[index_++];
        } else {
            current_ = ParseTree();
        }

        return found_;
    }

    const ParseTree& ParseTreeWalker::getCurrent() const
    {
        verify(found_);
        return current_;
    }

    bool ParseTreeWalker::isComplete() const
    {
        return index_ == tree_.getChildren().size();
    }

    size_t ParseTreeWalker::size() const
    {
        return tree_.getChildren().size();
    }

    const ParseTree& ParseTreeWalker::get(size_t i) const
    {
        auto& children = tree_.getChildren();
        verify(i < children.size(), "ParseTreeWalker::get: index out of bounds");
        return children[i];
    }
}

