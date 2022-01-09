
#include "ParseTree.hh"

#include "collection_utils.hh"
#include "verify.hh"

using namespace general;
using namespace std;

namespace parsing {
    ParseTree::ParseTree()
    {
    }

    ParseTree::ParseTree(const std::string& tag, CharSlice token)
        : impl_(new Impl)
    {
        impl_->tag = tag;
        token.toObject(impl_->token);
    }

    ParseTree::ParseTree(const std::string& tag, const vector<ParseTree>& subNodes)
        : impl_(new Impl)
    {
        impl_->tag = tag;

        for(const ParseTree& node : subNodes) {
            processSubNode_(node);
        }
    }

    void ParseTree::processSubNode_(const ParseTree& subNode)
    {
        if (subNode.hasToken()) {
            // Token nodes (nodes with a string match) are only preserved if they also have a tag.
            // Untagged token nodes are assumed to be 'syntax elements' like semicolons and braces.
            if (subNode.hasTag()) {
                appendSubNode_(subNode);
            }
        } else if (subNode.hasChildren()) {
            // If a subnode has semantic meaning, it should be tagged.  Tagged subnodes are included
            // here as 'children'; untagged ones are assumed to exist for parsing convenience, and
            // their children are adopted into this node's list of children.
            if (subNode.hasTag()) {
                appendSubNode_(subNode);
            } else {
                for(const ParseTree& subSubNode : subNode.getChildren()) {
                    appendSubNode_(subSubNode);
                }
            }
        }
    }

    void ParseTree::appendSubNode_(const ParseTree& node)
    {
        impl_->subNodes.push_back(node);
    }

    ParseTree::~ParseTree()
    {
    }

    void ParseTree::print(std::ostream& out) const
    {
        printOutline(out, 0);
    }

    void ParseTree::printOutline(std::ostream& out, int indent) const
    {
        string pad(indent, ' ');
        out << pad;

        if (! impl_.get()) {
            out << "ParseTree(<null>)";
        }

        if (hasTag()) {
            out << "<" << impl_->tag << ">";
        }

        if (hasToken()) {
            out << "'" << impl_->token << "'";
        } else {
            string nlpad = "\n" + pad;
            printCollectionOutline(out,
                                   impl_->subNodes,
                                   nlpad + "[\n", ",\n", nlpad + "]",
                                   indent + 4);
        }
    }

    bool ParseTree::hasTag() const
    {
        return impl_.get() && ! impl_->tag.empty();
    }

    bool ParseTree::hasToken() const
    {
        return impl_.get() && ! impl_->token.empty();
    }

    bool ParseTree::hasChildren() const
    {
        return impl_.get() && ! impl_->subNodes.empty();
    }

    const std::string& ParseTree::getTag() const
    {
        verify(hasTag(), "No tag present");
        return impl_->tag;
    }

    const std::string& ParseTree::getToken() const
    {
        verify(hasToken(), "Not a token");
        return impl_->token;
    }

    const vector<ParseTree>& ParseTree::getChildren() const
    {
        static vector<ParseTree> empty;
        return impl_.get() ? impl_->subNodes : empty;
    }

    bool ParseTree::hasTag(const std::string& t) const
    {
        return hasTag() && getTag() == t;
    }
}

