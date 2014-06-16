// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_ASTNODE
#define TESTSCRIPT_ENGINE_ASTNODE

namespace Eng
{
	template<typename TL> class ASTNode
	{
		template<typename> friend class NodeCtx;
		template<typename> friend class Parser;

		private:
			ASTTypeId typeId;
			ASTNodePtr<TL> parent{nullptr};
			std::vector<ASTNodePtr<TL>> children;

		public:
			inline virtual ~ASTNode() { }
			inline ASTTypeId getTypeId() const noexcept { return typeId; }

			inline void emplaceChild(ASTNodePtr<TL> mNode)
			{
				mNode->parent = this;
				children.emplace_back(mNode);
			}

			inline ASTNodePtr<TL> getParent() noexcept			{ return parent; }
			inline decltype(children)& getChildren() noexcept	{ return children; }

			template<typename T> T& getAs() noexcept			{ return *reinterpret_cast<T*>(this); }
	};
}

#endif
