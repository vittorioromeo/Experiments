#include <iostream>
#include <SSVUtils/SSVUtils.hpp>

enum class Token
{
    Num,
    POpen,
    PClose,
    OpAdd,
    OpSub
};
enum class Grammar
{
    Expr,
    Op
};
using Tkn = Token;
using Gmr = Grammar;

enum class PsltType
{
    Tkn,
    Gmr
};

template <typename TT, typename TG>
class Parselet
{
private:
    PsltType type;
    union
    {
        TT tkn;
        TG gmr;
    };

public:
    inline Parselet() noexcept {}
    inline Parselet(TT mTkn) noexcept : type{PsltType::Tkn}, tkn{mTkn} {}
    inline Parselet(TG mGmr) noexcept : type{PsltType::Gmr}, gmr{mGmr} {}

    inline PsltType getType() const noexcept { return type; }
    inline TT getTkn() const noexcept
    {
        assert(type == PsltType::Tkn);
        return tkn;
    }
    inline TG getGmr() const noexcept
    {
        assert(type == PsltType::Gmr);
        return gmr;
    }

    inline bool operator!=(const Parselet& mP) const noexcept
    {
        if(type != mP.type) return true;
        if(type == PsltType::Tkn && tkn != mP.tkn) return true;
        if(type == PsltType::Gmr && gmr != mP.gmr) return true;
        return false;
    }
};

SSVU_TEST(ParseletTests)
{
    Parselet<Tkn, Gmr> pt{Tkn::Num};
    Parselet<Tkn, Gmr> pg{Gmr::Expr};

    EXPECT(pt.getType() == PsltType::Tkn);
    EXPECT(pg.getType() == PsltType::Gmr);
}
SSVU_TEST_END();

template <typename TT, typename TG>
class GmrExpansion
{
private:
    TG gmr;
    std::vector<Parselet<TT, TG>> parselets;

    template <typename T1, typename T2, typename... TArgs>
    inline void createParselet(T1 mArg1, T2 mArg2, TArgs... mArgs)
    {
        createParselet<T1>(mArg1);
        createParselet<T2, TArgs...>(mArg2, mArgs...);
    }
    template <typename T>
    inline void createParselet(T mArg)
    {
        parselets.emplace_back(mArg);
    }

public:
    template <typename... TArgs>
    inline GmrExpansion(TG mGmr, TArgs... mArgs) noexcept : gmr{mGmr}
    {
        createParselet(mArgs...);
    }

    inline TG getGmr() const noexcept { return gmr; }
    inline const decltype(parselets)& getParselets() const noexcept
    {
        return parselets;
    }
};

SSVU_TEST(GmrExpansionTests)
{
    GmrExpansion<Tkn, Gmr> gmr1{Gmr::Expr, Gmr::Expr, Gmr::Op, Gmr::Op};
    GmrExpansion<Tkn, Gmr> gmr2{Gmr::Expr, Tkn::POpen, Gmr::Op, Tkn::PClose};
    GmrExpansion<Tkn, Gmr> gmr3{Gmr::Expr, Gmr::Op};

    EXPECT(gmr1.getParselets().size() == 3);
    EXPECT(gmr2.getParselets().size() == 3);
    EXPECT(gmr3.getParselets().size() == 1);
}
SSVU_TEST_END();

template <typename TT, typename TG>
class GmrRules
{
private:
    std::vector<GmrExpansion<TT, TG>> expansions;

public:
    template <typename... TArgs>
    inline void createExpansion(TArgs&&... mArgs)
    {
        expansions.emplace_back(std::forward<TArgs>(mArgs)...);
    }
    inline const decltype(expansions)& getExpansions() const noexcept
    {
        return expansions;
    }
};

SSVU_TEST(GmrRulesTests)
{
    GmrRules<Tkn, Gmr> gr1;
    gr1.createExpansion(Gmr::Expr, Gmr::Expr, Gmr::Op, Gmr::Op);
    gr1.createExpansion(Gmr::Expr, Gmr::Op);
    gr1.createExpansion(Gmr::Op, Gmr::Op);
    EXPECT(gr1.getExpansions().size() == 3);
}
SSVU_TEST_END();

template <typename TT, typename TG>
class Node
{
public:
    using ParseletType = Parselet<TT, TG>;
    int depth{0};

private:
    Parselet<TT, TG> parselet;
    Node* parent{nullptr};
    std::vector<std::unique_ptr<Node>> children;

public:
    template <typename... TArgs>
    inline Node(TArgs&&... mArgs)
        : parselet(std::forward<TArgs>(mArgs)...)
    {
    }
    template <typename... TArgs>
    inline void createChild(TArgs&&... mArgs)
    {
        auto result(new Node(std::forward<TArgs>(mArgs)...));
        result->depth = depth + 1;
        result->parent = this;
        children.emplace_back(result);
    }
    inline const decltype(parselet)& getParselet() const noexcept
    {
        return parselet;
    }
    inline const decltype(children)& getChildren() const noexcept
    {
        return children;
    }
};

template <typename TT, typename TG>
class Parser
{
public:
    using ParseletType = Parselet<TT, TG>;
    using NodeType = Node<TT, TG>;
    GmrRules<TT, TG> rules;

private:
    std::vector<ParseletType> tokenStack, parseStack;
    std::vector<NodeType> nodeStack;

    inline void shift()
    {
        // Pop a token from tokenStack and push it on parseStack and nodeStack
        parseStack.emplace_back(tokenStack.back());
        nodeStack.emplace_back(tokenStack.back());
        tokenStack.pop_back();
    }

    inline bool parseStackMatchesExpansion(
        std::size_t mStartAt, const GmrExpansion<TT, TG>& mGmrExpansion)
    {
        const auto& expSize(mGmrExpansion.getParselets().size());
        if(expSize + mStartAt > parseStack.size()) return false;

        for(auto i(0u); i < expSize; ++i)
            if(mGmrExpansion.getParselets().at(i) != parseStack[mStartAt + i])
                return false;

        return true;
    }

    inline void reduceRecursively()
    {
        debugPrint();

        for(const auto& r : rules.getExpansions())
        {
            ssvu::lo("matches?") << r.getParselets() << std::endl;

            for(auto i(0u); i < parseStack.size(); ++i)
                if(parseStackMatchesExpansion(i, r))
                {
                    ssvu::lo("matches!") << "yes!" << std::endl;

                    // Remove base nodes from parse stack and substitue them
                    // with exapansion
                    for(auto i(0u); i < r.getParselets().size(); ++i)
                        parseStack.pop_back();
                    parseStack.emplace_back(r.getGmr());

                    // Pop N nodes from node stack into removedNodes
                    const auto& n(r.getParselets().size());
                    std::vector<NodeType> removedNodes;
                    for(auto k(0u); k < n; ++k)
                    {
                        NodeType node{std::move(nodeStack.back())};
                        removedNodes.emplace_back(std::move(node));
                        nodeStack.pop_back();
                    }

                    // Create new node for reduction
                    nodeStack.emplace_back(r.getGmr());

                    // Add removed nodes as children
                    for(auto& n : removedNodes)
                        nodeStack.back().createChild(std::move(n));

                    reduceRecursively();
                    return;
                }
        }

        debugPrint();
    }

    inline void debugPrint()
    {
        ssvu::lo("tokenStack") << tokenStack << "\n";
        ssvu::lo("parseStack") << parseStack << "\n";
        ssvu::lo("nodeStack") << "\n" << nodeStack << "\n" << std::endl;
    }

public:
    template <typename... TArgs>
    inline void createRule(TArgs... mArgs)
    {
        rules.createExpansion(mArgs...);
    }

    inline void parse(const std::vector<TT>& mTkns)
    {
        // Reset parser state
        tokenStack.clear();
        parseStack.clear();
        nodeStack.clear();

        // Push all tokens on the token stack
        for(const auto& t : mTkns)
            tokenStack.emplace(std::begin(tokenStack), t);

        while(!tokenStack.empty())
        {
            shift();
            reduceRecursively();
        }
        return;
        // Stack is empty, shift
        assert(parseStack.empty());
        shift();

        // Reduce recursively
        reduceRecursively();
    }
};

// GRAMMAR:
//
//	Expr 	-> 	Expr Op Expr
//			->	POpen Expr PClose
//  		-> 	Num
//
// 	Op 		->	OpAdd
//			->	OpSub

namespace ssvu
{
    template <>
    struct Stringifier<Tkn>
    {
        template <bool TFmt>
        inline static void impl(std::ostream& mStream, const Tkn& mValue)
        {
            std::string s;
            switch(mValue)
            {
                case Tkn::Num: s = "NUM"; break;
                case Tkn::OpAdd: s = "+"; break;
                case Tkn::OpSub: s = "-"; break;
                case Tkn::POpen: s = "("; break;
                case Tkn::PClose: s = ")"; break;
            }

            Internal::callStringifyImpl<TFmt>(mStream, s);
        }
    };
    template <>
    struct Stringifier<Gmr>
    {
        template <bool TFmt>
        inline static void impl(std::ostream& mStream, const Gmr& mValue)
        {
            std::string s;
            switch(mValue)
            {
                case Gmr::Expr: s = "EXPR"; break;
                case Gmr::Op: s = "OP"; break;
            }

            Internal::callStringifyImpl<TFmt>(mStream, s);
        }
    };
    template <typename TT, typename TG>
    struct Stringifier<Parselet<TT, TG>>
    {
        template <bool TFmt>
        inline static void impl(
            std::ostream& mStream, const Parselet<TT, TG>& mValue)
        {
            if(mValue.getType() == PsltType::Tkn)
            {
                Internal::callStringifyImpl<TFmt>(mStream, "T_");
                Internal::callStringifyImpl<TFmt>(mStream, TT(mValue.getTkn()));
            }
            else if(mValue.getType() == PsltType::Gmr)
            {
                Internal::callStringifyImpl<TFmt>(mStream, "G_");
                Internal::callStringifyImpl<TFmt>(mStream, TG(mValue.getGmr()));
            }
        }
    };
    template <typename TT, typename TG>
    struct Stringifier<Node<TT, TG>>
    {
        template <bool TFmt>
        inline static void printNode(
            std::ostream& mStream, const Node<TT, TG>& mValue, int mDepth)
        {
            for(auto i(0u); i < mDepth; ++i)
                Internal::callStringifyImpl<TFmt>(mStream, "|\t");

            Internal::callStringifyImpl<TFmt>(mStream, "|------(+) ");
            Internal::callStringifyImpl<TFmt>(mStream, mValue.getParselet());
            Internal::callStringifyImpl<TFmt>(mStream, "\n");

            for(auto i(0u); i < mValue.getChildren().size(); ++i)
            {
                Stringifier<Node<TT, TG>>::printNode<TFmt>(
                    mStream, *(mValue.getChildren().at(i).get()), mDepth + 1);
                if(i == mValue.getChildren().size() - 1)
                {
                    for(auto i(0u); i < mDepth + 1; ++i)
                        Internal::callStringifyImpl<TFmt>(mStream, "|\t");
                    Internal::callStringifyImpl<TFmt>(mStream, "\n");
                }
            }
        }

        template <bool TFmt>
        inline static void impl(
            std::ostream& mStream, const Node<TT, TG>& mValue)
        {
            Internal::callStringifyImpl<TFmt>(mStream, "\n\n");

            std::string derp{"\t"};
            Stringifier<Node<TT, TG>>::printNode<TFmt>(mStream, mValue, 0);
        }
    };
}


int main()
{
    SSVU_TEST_RUN_ALL();
    ssvu::lo() << "Start" << std::endl;

    /*std::vector<Tkn> tkns
    {
        Tkn::Num,
        Tkn::OpAdd,
        Tkn::POpen,
        Tkn::POpen,
        Tkn::Num,
        Tkn::OpAdd,
        Tkn::Num,
        Tkn::PClose,
        Tkn::OpAdd,
        Tkn::Num,
        Tkn::PClose
    };*/

    std::vector<Tkn> tkns{
        Tkn::Num, Tkn::OpAdd, Tkn::POpen, Tkn::Num, Tkn::PClose};

    // Desired CST:
    /*
                Expr
          T------|-------T
        Expr 	Op 		Expr
          |		 |       |
          |	   OpAdd     |
         Num            Num
    */

    // Desired AST:
    /*
            (+)
        T----|
        |    |
       Num  Num
    */

    Parser<Tkn, Gmr> parser;

    parser.createRule(Gmr::Expr, Gmr::Expr, Gmr::Op, Gmr::Expr);
    parser.createRule(Gmr::Expr, Tkn::POpen, Gmr::Expr, Tkn::PClose);
    parser.createRule(Gmr::Expr, Tkn::Num);

    parser.createRule(Gmr::Op, Tkn::OpAdd);
    parser.createRule(Gmr::Op, Tkn::OpSub);

    parser.parse(tkns);

    return 0;
}
