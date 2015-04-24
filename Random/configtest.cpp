template
<
	bool TPar1,
	bool TPar2,
	typename T1,
	typename T2
>
struct Config
{
	static constexpr bool par1{TPar1};
	static constexpr bool par2{TPar2};
	using Type1 = T1;
	using Type2 = T2;

	inline static constexpr bool getPar1Static() { return TPar1; }
	inline constexpr bool getPar2() { return TPar2; }
};


template<typename TConfig> struct NoInher
{
	using typename TConfig::Type1;

	Type1 x;
	bool f{TConfig::par1};
	bool k{getPar1Static()};
};

template<typename TConfig> struct Inher : public TConfig
{
	using typename TConfig::Type1;

	Type1 x;
	bool f{TConfig::par1};
};

int main()
{

}