#include "../AlgoVee/Common.hpp"
#include "../AlgoVee/Boilerplate.hpp"
#include "../AlgoVee/AVZ/Assets.hpp"
#include "../AlgoVee/AVZ/Base.hpp"
#include "../AlgoVee/AVZ/Widgets.hpp"
#include "../AlgoVee/AVZ/Ctx.hpp"
#include "../AlgoVee/AVZ/Fns.inl"

class AlgoVizTestApp : public Boilerplate::App
{
private:
    avz::Ctx ctx;

    inline void initInput()
    {
        auto& gs(gameState);

        gs.addInput({{IK::Escape}}, [this](FT)
            {
                gameWindow->stop();
            });
        gs.addInput({{IK::Space}},
            [this](FT)
            {
                ctx.skipAnim();
            },
            IT::Once);
        gs.addInput({{IK::X}},
            [this](FT)
            {
                ctx.setSpeedFactor(12.5f);
            },
            [this](FT)
            {
                ctx.setSpeedFactor(1.f);
            });

        gs.addInput({{IK::A}}, [this](FT)
            {
                gameCamera.pan(-4, 0);
            });
        gs.addInput({{IK::D}}, [this](FT)
            {
                gameCamera.pan(4, 0);
            });
        gs.addInput({{IK::W}}, [this](FT)
            {
                gameCamera.pan(0, -4);
            });
        gs.addInput({{IK::S}}, [this](FT)
            {
                gameCamera.pan(0, 4);
            });
        gs.addInput({{IK::Q}}, [this](FT)
            {
                gameCamera.zoomOut(1.1f);
            });
        gs.addInput({{IK::E}}, [this](FT)
            {
                gameCamera.zoomIn(1.1f);
            });
    }

    inline auto mergeImpl(
        avz::w::Vector<int>& mV, SizeT mLB, SizeT mMid, SizeT mUB)
    {
        std::vector<int> result;
        result.resize(mUB - mLB);

        auto i(0u);
        auto iA(mLB);
        auto iB(mMid);

        while(iA < mMid && iB < mUB)
        {
            if(mV[iA] < mV[iB])
            {
                result[i++] = mV[iA++];
            }
            else
            {
                result[i++] = mV[iB++];
            }
        }

        while(iA < mMid) result[i++] = mV[iA++];
        while(iB < mUB) result[i++] = mV[iB++];

        ssvu::lo("R") << result << "\n";



        /*
                    auto i0(mLB); auto i1(mMid);
                    for(auto j(mLB); j < mUB; ++j)
                    {
                        if(i0 < mMid && (i1 >= mUB || mV[i0] <= mV[i1]))
                        {
                            result[j] = mV[i0];
                            ++i0;
                        }
                        else
                        {
                            result[j] = mV[i1];
                            ++i1;
                        }
                    }
        */
        return result;
    }

    inline void mergeSortImpl(avz::w::Vector<int>& mV, SizeT mLB, SizeT mUB)
    {
        if(mUB - mLB < 2) return;

        // ssvu::lo("msi") << mLB << " -> " << mUB << std::endl;

        auto mid((mUB + mLB) / 2);

        for(auto i(mLB); i < mid; ++i)
        {
            mV.taHightlight(i);
            mV.taTranslateBy(i, Vec2f{0.f, 85.f});
        }
        mergeSortImpl(mV, mLB, mid);
        for(auto i(mLB); i < mid; ++i)
        {
            mV.taUnhightlight(i);
            mV.taTranslateBy(i, Vec2f{0.f, -85.f});
        }

        for(auto i(mid); i < mUB; ++i)
        {
            mV.taHightlight(i);
            mV.taTranslateBy(i, Vec2f{0.f, 85.f});
        }
        mergeSortImpl(mV, mid, mUB);
        for(auto i(mid); i < mUB; ++i)
        {
            mV.taUnhightlight(i);
            mV.taTranslateBy(i, Vec2f{0.f, -85.f});
        }

        auto mergedData(mergeImpl(mV, mLB, mid, mUB));

        auto k(0u);
        for(auto i(mLB); i < mUB; ++i)
        {
            mV.taSet(i, mergedData[k]);
            mV[i] = mergedData[k++];
        }
    }

    inline void initTest()
    {
        auto& v(ctx.create<avz::w::Vector<int>>(Vec2f{100.f, 100.f}));
        v.taPushFront(9);
        v.taPushBack(0);
        v.taPushBack(4);
        v.taPushBack(10);
        v.taPushBack(2);
        v.taPushFront(1);
        v.taPushFront(8);
        v.taPushFront(3);
        v.taPushFront(5);
        v.taPushBack(7);
        v.taPushBack(6);

        ssvu::lo() << v.data << std::endl;
        mergeSortImpl(v, 0, v.getSize());
        ssvu::lo() << v.data << std::endl;
        /*int num = 11;
        for(int i = 1; i < num; i++)
        {
            for(int j = 0; j < num - 1; j++)
            {
                if(v[j] > v[j + 1])
                {
                    ssvu::lo("must swap") << j << " <-> " << j + 1 << "\n";
                    v.taSwap(j, j + 1);
                }
            }
        }

        */
    }

    inline void update(FT mFT)
    {
        gameCamera.update<float>(mFT);
        ctx.update(mFT);
    }
    inline void draw()
    {
        gameCamera.apply();
        ctx.draw();
        gameCamera.unapply();
    }

public:
    inline AlgoVizTestApp(ssvs::GameWindow& mGameWindow)
        : Boilerplate::App{mGameWindow}, ctx{mGameWindow}
    {
        gameState.onUpdate += [this](FT mFT)
        {
            update(mFT);
        };
        gameState.onDraw += [this]
        {
            draw();
        };

        initInput();
        initTest();
    }
};

int main()
{
    Boilerplate::AppRunner<AlgoVizTestApp>{"AlgoVee tests", 1440, 900};
    return 0;
}
