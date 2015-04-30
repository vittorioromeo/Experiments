#include <SSVUtils/Core/Core.hpp>

using namespace ssvu;

template<SizeT TS, typename TChunk> struct ChunkHolder
{
	// SizeT size{T{}()};
	TChunk chunk{};
};

template<typename TBase, template<typename> class TLHelper> struct Chunk { };
template<typename TBase, template<typename> class TLHelper, typename TTypes> class PolyFixedStorageVariadic2
{
	public:
		using ChunkType = Chunk<TBase, TLHelper>;
		template<typename T> using ChunkHolderType = ChunkHolder<sizeof(T), ChunkType>;

		using List2 = typename TTypes::template Apply<ChunkHolderType>::Unique;
		using TplType = typename List2::AsTpl;

	private:
		TplType chunks;

	public:
		template<typename T> inline auto& getChunk() noexcept
		{
			using ChunkHolderTypeForT = ChunkHolderType<T>;
			SSVU_ASSERT_STATIC_NM(List2::template has<ChunkHolderTypeForT>());
			return std::get<ChunkHolderTypeForT>(chunks).chunk;
			//return chunks[FixedStorageImpl::getSizeIdx<sizeof(T)>()];
		}
};


template<typename T>

int main()
{
	ssvu::lo() << sizeof(int) << "\n";
	ssvu::lo() << sizeof(float) << "\n";
	ssvu::lo() << sizeof(double) << "\n\n";

	// ssvu::lo() << SizeHolderT<int>() << "\n";
	// ssvu::lo() << SizeHolderT<float>() << "\n";
	// ssvu::lo() << SizeHolderT<double>() << "\n";

	PolyFixedStorageVariadic2<int, SizeHolderT, MPL::List<int>> x;
	x.getChunk<float>();

	return 0;
}


