#include <SSVUtils/Core/Core.hpp>

using namespace ssvu;

template<typename T> using SizeHolderT = CTSizeT<sizeof(T)>;

template<typename T, typename TChunk> struct ChunkHolder
{
	// SizeT size{T{}()};
	TChunk chunk{};
};

template<typename TBase, template<typename> class TLHelper> struct Chunk { };
template<typename TBase, template<typename> class TLHelper, typename TTypes> class PolyFixedStorageVariadic2
{
	public:
		using ChunkType = Chunk<TBase, TLHelper>;
		template<typename T> using ChunkHolderType = ChunkHolder<T, ChunkType>;
		using List1 = TTypes;
		using List2 = typename List1::template Apply<SizeHolderT>;
		using List3 = typename List2::Unique::template Apply<ChunkHolderType>;
		using TplType = typename List3::AsTpl;

	private:
		TplType chunks;

	public:
		template<typename T> inline auto& getChunk() noexcept
		{
			using ChunkHolderTypeForT = ChunkHolderType<CTSizeT<sizeof(T)>>;
			SSVU_ASSERT_STATIC_NM(List3::template has<ChunkHolderTypeForT>());
			return std::get<ChunkHolderTypeForT>(chunks).chunk;
			//return chunks[FixedStorageImpl::getSizeIdx<sizeof(T)>()];
		}
};




int main()
{
	ssvu::lo() << sizeof(int) << "\n";
	ssvu::lo() << sizeof(float) << "\n";
	ssvu::lo() << sizeof(double) << "\n\n";

	ssvu::lo() << SizeHolderT<int>() << "\n";
	ssvu::lo() << SizeHolderT<float>() << "\n";
	ssvu::lo() << SizeHolderT<double>() << "\n";

	PolyFixedStorageVariadic2<int, SizeHolderT, MPL::List<int>> x;
	x.getChunk<float>();

	return 0;
}


