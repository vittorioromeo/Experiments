#include <iostream>
#include <utility>

#define SSVU_CTR_GET_CRUMB(mTag, mRank, mAcc) \
	getCtrCrumb(mTag(), CtrConstantIndex<mRank>(), CtrConstantIndex<mAcc>())

#define SSVU_CTR_GET(mTag) \
	SSVU_CTR_GET_CRUMB(mTag, 1, \
	SSVU_CTR_GET_CRUMB(mTag, 2, \
	SSVU_CTR_GET_CRUMB(mTag, 4, \
	SSVU_CTR_GET_CRUMB(mTag, 8, \
    SSVU_CTR_GET_CRUMB(mTag, 16, \
    SSVU_CTR_GET_CRUMB(mTag, 32, \
    SSVU_CTR_GET_CRUMB(mTag, 64, \
    SSVU_CTR_GET_CRUMB(mTag, 128, 0))))))))

#define SSVU_CTR_INC(mTag) \
	CtrConstantIndex<SSVU_CTR_GET(mTag) + 1> \
	constexpr getCtrCrumb(mTag, CtrConstantIndex<(SSVU_CTR_GET(mTag) + 1) & ~SSVU_CTR_GET(mTag)>, \
          					CtrConstantIndex<(SSVU_CTR_GET(mTag) + 1) & SSVU_CTR_GET(mTag)>) { return {}; }

#define SSVU_CTR_LINK_mNameSPACE(NS) using NS::getCtrCrumb;

#define REGISTERED_CLASS(mTag, mKey, mName) mKey mName &register_type(mTag, decltype(SSVU_CTR_GET(mTag))); \
	SSVU_CTR_INC(mTag) mKey mName 

template<std::size_t TS> struct CtrConstantIndex : std::integral_constant<std::size_t, TS> { };

template<typename TID, std::size_t TRank, std::size_t TAcc>
constexpr CtrConstantIndex<TAcc> getCtrCrumb(TID, CtrConstantIndex<TRank>, CtrConstantIndex<TAcc>) { return {}; } 

struct my_cnt {};

int const a = SSVU_CTR_GET(my_cnt);
SSVU_CTR_INC(my_cnt);
SSVU_CTR_INC(my_cnt);
SSVU_CTR_INC(my_cnt);
SSVU_CTR_INC(my_cnt);
SSVU_CTR_INC(my_cnt);
 
int const b = SSVU_CTR_GET(my_cnt);
 
SSVU_CTR_INC(my_cnt);
 
template<typename T> struct TestS
{
    enum { value = SSVU_CTR_GET(my_cnt) };

    
};
 
int main() 
{
    std::cout << TestS<int>::value << std::endl;
    std::cout << TestS<int>::value << std::endl;
    std::cout << TestS<int>::value << std::endl;

    std::cout << TestS<float>::value << std::endl;
}