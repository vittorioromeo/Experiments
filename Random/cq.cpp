#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/GrowableArray/GrowableArray.hpp>
#include <SSVUtils/Test/Test.hpp>
#include <SSVStart/SSVStart.hpp>

using ssvu::SizeT;

enum class CBPGrow
{
    Automatic,
    Manual
};

enum class CBPOverwrite
{
    Disallowed,
    Allowed
};

template <typename T, CBPGrow TPGrow = CBPGrow::Automatic,
    CBPOverwrite TPOverwrite = CBPOverwrite::Disallowed>
class CircularBuffer
{
private:
    ssvu::GrowableArray<T> data;
    SizeT size{0u}, capacity{0u}, iHead{0u}, iTail{0u};

    inline void growTo(SizeT mCapacity)
    {
        data.grow(capacity, mCapacity);
        capacity = mCapacity;
    }

    inline void growIfNeeded()
    {
        if(TPGrow == CBPGrow::Manual) return;
        if(size + 2 < capacity) return;

        growTo(1 + ssvu::toSizeT(capacity * 2.f));
    }

    inline auto getIdx(SizeT mX) const noexcept { return mX % capacity; }

public:
    inline CircularBuffer(SizeT mCapacity = 10) { growTo(mCapacity); }

    template <typename... TArgs>
    inline auto& emplace(TArgs&&... mArgs)
    {
        growIfNeeded();
        SSVU_ASSERT(capacity > 0);

        auto ptr(&data[getIdx(iTail)]);
        new(ptr) T(FWD(mArgs)...);
        ++iTail;
        ++size;

        if(TPOverwrite == CBPOverwrite::Allowed)
        {
            ssvu::clampMax(size, capacity);
        }

        SSVU_ASSERT(size <= capacity);
        return *ptr;
    }

    inline auto& dequeue() noexcept
    {
        SSVU_ASSERT(size > 0);
        auto& result(data[iHead]);

        ++iHead;
        --size;

        return result;
    }

    inline auto& get(SizeT mI) noexcept { return data[getIdx(iHead + mI)]; }
    inline const auto& get(SizeT mI) const noexcept
    {
        return data[getIdx(iHead + mI)];
    }

    inline const auto& getSize() const noexcept { return size; }
    inline const auto& getCapacity() const noexcept { return capacity; }

    inline auto& operator[](SizeT mI) noexcept { return data[mI]; }
    inline const auto& operator[](SizeT mI) const noexcept { return data[mI]; }
};

template <typename T>
class TSCircularBuffer
{
public:
    using CBType = CircularBuffer<T, CBPGrow::Manual, CBPOverwrite::Allowed>;

private:
    CBType cb;
    std::mutex mtx;

public:
    template <typename... TArgs>
    inline auto& enqueue(TArgs&&... mArgs)
    {
        std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{mtx};
        return cb.enqueue(FWD(mArgs)...);
    }

    inline auto& dequeue()
    {
        std::lock_guard<std::mutex> SSVU_UNIQUE_NAME{mtx};
        return cb.dequeue();
    }
};

class ManagedSocket
{
public:
    using TSCBType = TSCircularBuffer<sf::Packet>;

private:
    sf::UdpSocket socket;
    TSCBType tscbIn, tscbOut;
    std::future<void> futIn, futOut;
    sf::IpAddress ip;
    sf::Port port;
    bool busy{true};

    template <typename TP>
    inline auto& tsEnqueueIn(TP&& mP)
    {
        return tscbIn.enqueue(FWD(mP));
    }
    inline auto& tsDequeueIn() { return tscbIn.dequeue(); }

    template <typename TP>
    inline auto& tsEnqueueOut(TP&& mP)
    {
        return tscbOut.enqueue(FWD(mP));
    }
    inline auto& tsDequeueOut() { return tscbOut.dequeue(); }

    template <typename TF>
    inline void whileBusy(TF mFn)
    {
        while(busy)
        {
            mFn();
        }
    }

    inline void futInImpl()
    {
        whileBusy([this]
            {
                auto& nextBuffer(
				if(socket.receive(recvBuffer, senderIp, senderPort) != sf::Socket::Done)
				{

				}
            });
    }

    inline void futOutImpl()
    {
        whileBusy([this]
            {

            });
    }

public:
    inline ~ManagedSocket() { busy = false; }
};


SSVUT_TEST(cqtest1)
{
    CircularBuffer<int> cb;

    SSVUT_EXPECT_OP(cb.getSize(), ==, 0);

    auto& x = cb.emplace(255);
    auto& y = cb.emplace(155);

    SSVUT_EXPECT_OP(cb.getSize(), ==, 2);

    SSVUT_EXPECT_OP(x, ==, 255);
    SSVUT_EXPECT_OP(cb[0], ==, 255);
    SSVUT_EXPECT_OP(y, ==, 155);
    SSVUT_EXPECT_OP(cb[1], ==, 155);

    SSVUT_EXPECT_OP(cb.dequeue(), ==, 255);

    SSVUT_EXPECT_OP(x, ==, 255);
    SSVUT_EXPECT_OP(cb[0], ==, 255);
    SSVUT_EXPECT_OP(y, ==, 155);
    SSVUT_EXPECT_OP(cb[1], ==, 155);

    SSVUT_EXPECT_OP(cb.dequeue(), ==, 155);

    SSVUT_EXPECT_OP(x, ==, 255);
    SSVUT_EXPECT_OP(cb[0], ==, 255);
    SSVUT_EXPECT_OP(y, ==, 155);
    SSVUT_EXPECT_OP(cb[1], ==, 155);

    for(int i = 0; i < 100; ++i)
    {
        cb.emplace(i);
    }

    for(int i = 0; i < 100; ++i)
    {
        SSVUT_EXPECT_OP(cb.get(i), ==, i);
    }

    SSVUT_EXPECT_OP(cb.getSize(), ==, 100);

    for(int i = 0; i < 100; ++i)
    {
        SSVUT_EXPECT_OP(cb.dequeue(), ==, i);
    }

    SSVUT_EXPECT_OP(cb.getSize(), ==, 0);
}

SSVUT_TEST(cqtest2)
{
    CircularBuffer<int, CBPGrow::Manual, CBPOverwrite::Allowed> cb{5};

    SSVUT_EXPECT_OP(cb.getSize(), ==, 0);

    for(int i = 0; i < 100; ++i)
    {
        cb.emplace(i);
    }

    SSVUT_EXPECT_OP(cb.getSize(), ==, 5);
}


int main()
{
    SSVUT_RUN();


    return 0;
}