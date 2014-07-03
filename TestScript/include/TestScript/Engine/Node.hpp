// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef TESTSCRIPT_ENGINE_NODE
#define TESTSCRIPT_ENGINE_NODE

namespace Eng
{
	template<typename TL> class Node
	{
		template<typename> friend class Parser;

		private:
			NodeTID typeId;
			NodePtr<TL> parent{nullptr};
			std::vector<NodePtr<TL>> children;

		public:
			inline virtual ~Node() { }
			inline NodeTID getTypeId() const noexcept { return typeId; }

			inline void emplaceChild(NodePtr<TL> mNode)
			{
				mNode->parent = this;
				children.emplace_back(mNode);
			}

			inline NodePtr<TL> getParent() noexcept			{ return parent; }
			inline decltype(children)& getChildren() noexcept	{ return children; }

			template<typename T> T& getAs() noexcept			{ return *reinterpret_cast<T*>(this); }
	};
}

#endif
