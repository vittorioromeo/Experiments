// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef OB_TESTING_OTHER_GRAPH
#define OB_TESTING_OTHER_GRAPH

namespace ssvut
{
	namespace Impl
	{
		template<typename TGraph> class GraphLink : public TGraph::StorageLinkBase
		{
			private:
				using NodePtr = typename TGraph::NodePtr;
				NodePtr node{TGraph::getNodeNull()};

			public:
				inline GraphLink(const NodePtr& mNode) noexcept : node{mNode} { SSVU_ASSERT(TGraph::isNodeValid(node)); }
				inline const NodePtr& getNode() const noexcept { SSVU_ASSERT(TGraph::isNodeValid(node)); return node; }
		};

		template<typename TGraph> class GraphNode : public TGraph::StorageNodeBase
		{
			private:
				using NodePtr = typename TGraph::NodePtr;
				using NodeDerived = typename TGraph::NodeDerived;

			public:
				template<typename... TArgs> inline void linkToSelf(TArgs&&... mArgs)
				{
					TGraph::StorageNodeBase::emplaceLink(TGraph::StorageNodeBase::getNodePtr(this), FWD(mArgs)...);
				}
				template<typename... TArgs> inline void linkTo(const NodePtr& mNode, TArgs&&... mArgs)
				{
					SSVU_ASSERT(TGraph::isNodeValid(mNode));
					TGraph::StorageNodeBase::emplaceLink(mNode, FWD(mArgs)...);
				}

				inline const decltype(TGraph::StorageNodeBase::links)& getLinks() const	{ return TGraph::StorageNodeBase::links; }
				inline bool isIsolated() const noexcept	{ return TGraph::StorageNodeBase::links.empty(); }
		};

		template<typename TGraph> struct GraphStorageFreeStore
		{
			using NodeDerived = typename TGraph::NodeDerived;
			using LinkDerived = typename TGraph::LinkDerived;
			using NodePtr = NodeDerived*;

			struct NodeBase
			{
				std::vector<LinkDerived> links;
				template<typename... TArgs> inline void emplaceLink(TArgs&&... mArgs) { links.emplace_back(FWD(mArgs)...); }
				inline static NodePtr getNodePtr(GraphNode<TGraph>* mNode) noexcept { return ssvu::castUp<NodeDerived>(mNode); }
			};
			struct LinkBase { };

			ssvu::VecUPtr<NodeDerived> nodes;

			inline static const NodePtr& getNodeNull() noexcept { static NodePtr result{nullptr}; return result; }
			inline static constexpr bool isNodeValid(const NodePtr& mNode) noexcept { return mNode != getNodeNull(); }

			template<typename... TArgs> inline NodePtr createNode(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(ssvu::isBaseOf<GraphNode<TGraph>, NodeDerived>(), "TNode must be derived from Graph::Node");
				return &ssvu::getEmplaceUPtr<NodeDerived>(nodes, FWD(mArgs)...);
			}
		};
	}

	template<typename TNode, typename TLink, template<typename> class TStorage = Impl::GraphStorageFreeStore> class Graph
	{
		public:
			using Node = Impl::GraphNode<Graph>;
			using Link = Impl::GraphLink<Graph>;
			using NodeDerived = TNode;
			using LinkDerived = TLink;
			using Storage = TStorage<Graph>;
			using StorageNodeBase = typename Storage::NodeBase;
			using StorageLinkBase = typename Storage::LinkBase;
			using NodePtr = typename Storage::NodePtr;
			friend Storage;
			friend Node;
			friend Link;

		private:
			Storage storage;
			std::vector<NodePtr> nodes;

		protected:
			template<typename... TArgs> inline NodePtr createNode(TArgs&&... mArgs)
			{
				auto result(storage.createNode(FWD(mArgs)...));
				nodes.emplace_back(result); return result;
			}

		public:
			inline const decltype(nodes)& getNodes() const noexcept				{ return nodes; }
			inline decltype(nodes)& getNodes() noexcept							{ return nodes; }
			inline static const NodePtr& getNodeNull() noexcept					{ return Storage::getNodeNull(); }
			inline static constexpr bool isNodeValid(NodePtr mNode) noexcept	{ return Storage::isNodeValid(mNode); }
			inline const NodePtr& getLastAddedNode() noexcept					{ SSVU_ASSERT(!nodes.empty()); return nodes.back(); }
	};
}

#endif
