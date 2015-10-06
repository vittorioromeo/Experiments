#include <iostream>
#include <set>
#include <utility>
#include <cstddef>
#include <SSVUtils/SSVUtils.hpp>

namespace Internal
{
    template <typename T>
    struct PtrComparator
    {
        inline bool operator()(const T* mA, const T* mB) const noexcept
        {
            return *mA < *mB;
        }
    };

    template <typename T>
    using PtrSet = std::set<const T*, Internal::PtrComparator<T>>;

    template <typename T1, typename T2, typename T>
    struct BimapHelper;
    template <typename T1, typename T2>
    struct BimapHelper<T1, T2, T1>
    {
        using Current = T1;
        using Other = T2;
        template <typename TBimap>
        inline static const PtrSet<T1>& getSetCurrent(TBimap& mBimap) noexcept
        {
            return mBimap.set1;
        }
    };
    template <typename T1, typename T2>
    struct BimapHelper<T1, T2, T2>
    {
        using Current = T2;
        using Other = T1;
        template <typename TBimap>
        inline static const PtrSet<T2>& getSetCurrent(TBimap& mBimap) noexcept
        {
            return mBimap.set2;
        }
    };
}

template <typename T1, typename T2>
class SneakyBimap
{
    template <typename, typename, typename>
    friend struct Internal::BimapHelper;

public:
    using BMPair = std::pair<T1, T2>;
    using Storage = std::vector<ssvu::Uptr<BMPair>>;

    using iterator = typename Storage::iterator;
    using const_iterator = typename Storage::const_iterator;
    using reverse_iterator = typename Storage::reverse_iterator;
    using const_reverse_iterator = typename Storage::const_reverse_iterator;

private:
    Storage storage;
    Internal::PtrSet<T1> set1;
    Internal::PtrSet<T2> set2;

    template <typename T>
    inline constexpr BMPair& getPairImpl(const T* mPtr) const noexcept
    {
        SSVU_ASSERT_STATIC(std::is_standard_layout<BMPair>::value,
            "BMPair must have standard layout");
        return *(const_cast<BMPair*>(reinterpret_cast<const BMPair*>(mPtr)));
    }

    inline constexpr const char* getPairBasePtr(const T2* mItem) const noexcept
    {
        return reinterpret_cast<const char*>(mItem) - offsetof(BMPair, second);
    }

    inline constexpr BMPair& getPair(const T1* mItem) const noexcept
    {
        return getPairImpl(mItem);
    }
    inline constexpr BMPair& getPair(const T2* mItem) const noexcept
    {
        return getPairImpl(getPairBasePtr(mItem));
    }

    inline T2& getItem(const T1* mItem) noexcept
    {
        return getPair(mItem).second;
    }
    inline T1& getItem(const T2* mItem) noexcept
    {
        return getPair(mItem).first;
    }

    inline const T2& getItem(const T1* mItem) const noexcept
    {
        return getPair(mItem).second;
    }
    inline const T1& getItem(const T2* mItem) const noexcept
    {
        return getPair(mItem).first;
    }

    template <typename T>
    auto atImpl(const T& mKey) const noexcept -> const
        typename Internal::BimapHelper<T1, T2, T>::Other &
    {
        const auto& set(Internal::BimapHelper<T1, T2, T>::getSetCurrent(*this));
        const auto& itr(this->find(mKey));
        if(itr == std::end(set))
            throw std::out_of_range{"mKey was not found in set"};
        return getItem(*itr);
    }

    template <typename T>
    inline void eraseImpl(const T& mKey)
    {
        SSVU_ASSERT(this->has(mKey));

        const auto& pair(getPair(*this->find(mKey)));

        set1.erase(&pair.first);
        set2.erase(&pair.second);

        ssvu::eraseRemoveIf(storage,
            [&pair](const ssvu::Uptr<std::pair<T1, T2>>& mI)
            {
                return mI.get() == &pair;
            });

        SSVU_ASSERT(!this->has(mKey));
    }

public:
    template <typename TA1, typename TA2>
    inline std::pair<T1, T2>& emplace(TA1&& mArg1, TA2&& mArg2)
    {
        SSVU_ASSERT(!this->has(mArg1) && !this->has(mArg2));

        auto& pair(ssvu::getEmplaceUptr<std::pair<T1, T2>>(
            storage, std::forward<TA1>(mArg1), std::forward<TA2>(mArg2)));
        set1.emplace(&pair.first);
        set2.emplace(&pair.second);

        return pair;
    }
    inline std::pair<T1, T2>& insert(const BMPair& mPair)
    {
        return this->emplace(mPair.first, mPair.second);
    }

    inline void erase(const T1& mKey) { this->eraseImpl(mKey); }
    inline void erase(const T2& mKey) { this->eraseImpl(mKey); }

    inline const T2& at(const T1& mKey) const noexcept
    {
        return this->atImpl(mKey);
    }
    inline const T1& at(const T2& mKey) const noexcept
    {
        return this->atImpl(mKey);
    }

    inline T2& operator[](const T1& mKey) noexcept
    {
        return this->has(mKey) ? this->get(mKey)
                               : this->emplace(mKey, T2{}).second;
    }
    inline T1& operator[](const T2& mKey) noexcept
    {
        return this->has(mKey) ? this->get(mKey)
                               : this->emplace(T1{}, mKey).first;
    }

    inline T2& get(const T1& mKey) noexcept
    {
        SSVU_ASSERT(this->has(mKey));
        return getItem(*this->find(mKey));
    }
    inline T1& get(const T2& mKey) noexcept
    {
        SSVU_ASSERT(this->has(mKey));
        return getItem(*this->find(mKey));
    }

    inline const T2& operator[](const T1& mKey) const noexcept
    {
        return this->get(mKey);
    }
    inline const T1& operator[](const T2& mKey) const noexcept
    {
        return this->get(mKey);
    }

    inline void clear() noexcept
    {
        storage.clear();
        set1.clear();
        set2.clear();
    }

    inline bool empty() const noexcept { return storage.empty(); }
    inline auto size() const noexcept -> decltype(storage.size())
    {
        return storage.size();
    }

    inline auto count(const T1& mKey) const noexcept
        -> decltype(set1.count(&mKey))
    {
        return set1.count(&mKey);
    }
    inline auto count(const T2& mKey) const noexcept
        -> decltype(set2.count(&mKey))
    {
        return set2.count(&mKey);
    }

    inline auto find(const T1& mKey) const noexcept
        -> decltype(set1.find(&mKey))
    {
        return set1.find(&mKey);
    }
    inline auto find(const T2& mKey) const noexcept
        -> decltype(set2.find(&mKey))
    {
        return set2.find(&mKey);
    }

    inline bool has(const T1& mKey) const noexcept
    {
        return this->find(mKey) != std::end(set1);
    }
    inline bool has(const T2& mKey) const noexcept
    {
        return this->find(mKey) != std::end(set2);
    }

    inline auto begin() noexcept -> decltype(storage.begin())
    {
        return storage.begin();
    }
    inline auto end() noexcept -> decltype(storage.end())
    {
        return storage.end();
    }
    inline auto begin() const noexcept -> decltype(storage.begin())
    {
        return storage.begin();
    }
    inline auto end() const noexcept -> decltype(storage.end())
    {
        return storage.end();
    }
    inline auto cbegin() const noexcept -> decltype(storage.cbegin())
    {
        return storage.cbegin();
    }
    inline auto cend() const noexcept -> decltype(storage.cend())
    {
        return storage.cend();
    }
    inline auto rbegin() noexcept -> decltype(storage.rbegin())
    {
        return storage.rbegin();
    }
    inline auto rend() noexcept -> decltype(storage.rend())
    {
        return storage.rend();
    }
    inline auto crbegin() const noexcept -> decltype(storage.crbegin())
    {
        return storage.crbegin();
    }
    inline auto crend() const noexcept -> decltype(storage.crend())
    {
        return storage.crend();
    }
};

SSVU_TEST(SneakyBimapTests)
{
    SneakyBimap<int, std::string> sb;

    SSVUT_EXPECT(sb.empty());
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(!sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 0);
    SSVUT_EXPECT(sb.count("banana") == 0);

    sb.emplace(10, "banana");

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 1);
    SSVUT_EXPECT(sb.has(10));
    SSVUT_EXPECT(sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 1);
    SSVUT_EXPECT(sb.count("banana") == 1);
    SSVUT_EXPECT(sb.at(10) == "banana");
    SSVUT_EXPECT(sb[10] == "banana");
    SSVUT_EXPECT(sb.at("banana") == 10);
    SSVUT_EXPECT(sb["banana"] == 10);

    sb["banana"] = 25;

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 1);
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(sb.has(25));
    SSVUT_EXPECT(sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 0);
    SSVUT_EXPECT(sb.count(25) == 1);
    SSVUT_EXPECT(sb.count("banana") == 1);
    SSVUT_EXPECT(sb.at(25) == "banana");
    SSVUT_EXPECT(sb[25] == "banana");
    SSVUT_EXPECT(sb.at("banana") == 25);
    SSVUT_EXPECT(sb["banana"] == 25);

    sb["banana"] = 15;
    sb[15] = "melon";
    sb.emplace(10, "cucumber");

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 2);
    SSVUT_EXPECT(sb.has(10));
    SSVUT_EXPECT(sb.has(15));
    SSVUT_EXPECT(!sb.has(25));
    SSVUT_EXPECT(sb.has("melon"));
    SSVUT_EXPECT(sb.has("cucumber"));
    SSVUT_EXPECT(!sb.has("banana"));
    SSVUT_EXPECT(sb.count(10) == 1);
    SSVUT_EXPECT(sb.count(15) == 1);
    SSVUT_EXPECT(sb.count(25) == 0);
    SSVUT_EXPECT(sb.count("melon") == 1);
    SSVUT_EXPECT(sb.count("cucumber") == 1);
    SSVUT_EXPECT(sb.count("banana") == 0);
    SSVUT_EXPECT(sb.at(10) == "cucumber");
    SSVUT_EXPECT(sb[10] == "cucumber");
    SSVUT_EXPECT(sb.at("cucumber") == 10);
    SSVUT_EXPECT(sb["cucumber"] == 10);
    SSVUT_EXPECT(sb.at(15) == "melon");
    SSVUT_EXPECT(sb[15] == "melon");
    SSVUT_EXPECT(sb.at("melon") == 15);
    SSVUT_EXPECT(sb["melon"] == 15);

    sb.clear();

    SSVUT_EXPECT(sb.empty());
    SSVUT_EXPECT(sb.size() == 0);
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(!sb.has(15));
    SSVUT_EXPECT(!sb.has(25));
    SSVUT_EXPECT(!sb.has("melon"));
    SSVUT_EXPECT(!sb.has("cucumber"));
    SSVUT_EXPECT(!sb.has("banana"));

    sb["yolo"] = 15;

    SSVUT_EXPECT(!sb.empty());
    SSVUT_EXPECT(sb.size() == 1);
    SSVUT_EXPECT(!sb.has(10));
    SSVUT_EXPECT(sb.has(15));
    SSVUT_EXPECT(!sb.has("melon"));
    SSVUT_EXPECT(sb.has("yolo"));
    SSVUT_EXPECT(sb["yolo"] == 15);

    sb["yolo"] = 25;

    SSVUT_EXPECT(sb.has("yolo"));
    SSVUT_EXPECT(!sb.has(15));
    SSVUT_EXPECT(sb.has(25));
    SSVUT_EXPECT(sb["yolo"] == 25);

    sb.erase("yolo");

    SSVUT_EXPECT(!sb.has("yolo"));
    SSVUT_EXPECT(!sb.has(15));
    SSVUT_EXPECT(!sb.has(25));
    SSVUT_EXPECT(sb.empty());
}
SSVU_TEST_END();

int main()
{
    SSVU_TEST_RUN_ALL();
    return 0;
}
