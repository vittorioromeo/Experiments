// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_CTX
#define TESTSCRIPT_ENGINE_CTX

namespace Eng
{
    template <typename TL>
    class Ctx
    {
        template <typename>
        friend class Parser;

    public:
        Parser<TL>& parser;
        std::size_t offset;

        class Proxy
        {
        public:
            Ctx<TL>& ctx;
            NodePtr<TL> nodePtr;

        public:
            inline Proxy(Ctx<TL>& mCtx, NodePtr<TL> mNodePtr)
                : ctx(mCtx), nodePtr{mNodePtr}
            {
            }
            inline bool isToken(TokenType<TL> mTknType) const
            {
                if(dynamic_cast<ASTTokenNode<TL>*>(nodePtr))
                {
                    auto tknNode(ssvu::castUp<ASTTokenNode<TL>>(nodePtr));
                    if(tknNode->getToken().getType() == mTknType) return true;
                }

                return false;
            }
            template <typename T>
            inline bool isDerivedFrom() const noexcept
            {
                return dynamic_cast<T*>(nodePtr);
            }
        };

    public:
        inline Ctx(Parser<TL>& mParser, std::size_t mOffset)
            : parser(mParser), offset{mOffset}
        {
        }

        inline std::size_t getSize() const noexcept
        {
            return parser.parseStack.size() - offset;
        }
        inline Proxy operator[](std::size_t mIdx)
        {
            auto idxWO(mIdx + offset);
            SSVU_ASSERT(idxWO < parser.parseStack.size());
            return Proxy{*this, parser.parseStack[idxWO]};
        }
        inline void pop()
        {
            auto idxWO(offset);
            SSVU_ASSERT(idxWO < parser.parseStack.size());
            parser.parseStack.erase(std::begin(parser.parseStack) + offset);
        }
        template <typename T>
        inline T& popAs()
        {
            auto itr(std::begin(parser.parseStack) + offset);
            auto result(ssvu::castUp<T>(*itr));
            pop();
            return *result;
        }
        template <typename T, typename... TArgs>
        inline T& pushCreate(TArgs&&... mArgs)
        {
            auto& node(parser.template createNode<T>(FWD(mArgs)...));
            parser.parseStack.insert(
                std::begin(parser.parseStack) + offset, &node);
            return ssvu::castUp<T>(node);
        }


        inline Proxy getAhead(std::size_t mIdx)
        {
            // Real index, with offset
            auto idxWO(mIdx + offset + 0);

            // If parseStack is big enough, get proxy from parseStack
            if(idxWO < parser.parseStack.size())
                return Proxy{*this, parser.parseStack[idxWO]};

            // Otherwise calculate sourceStack index

            // idxWO -= parser.parseStack.size();
            // WTF?

            // And get proxy from sourceStack
            SSVU_ASSERT(idxWO < parser.sourceStack.size());
            return Proxy{*this, parser.sourceStack[idxWO]};
        }

        inline bool canLookAhead(std::size_t mIdx) const noexcept
        {
            auto idxWO(mIdx + offset);
            if(idxWO < parser.parseStack.size()) return true;

            // idxWO -= parser.parseStack.size();
            // WTF?

            return idxWO < parser.sourceStack.size();
        }
    };

    struct LookAheadResults
    {
        bool forceShift{false};
    };
}

#endif
