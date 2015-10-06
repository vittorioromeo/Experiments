#ifndef ALGOVEE_AVZ_BASE
#define ALGOVEE_AVZ_BASE

namespace avz
{
    class Ctx;

    namespace Internal
    {
        class TACtx;
        class Widget;

        class TimedAction
        {
            friend class avz::Ctx;
            friend class avz::Internal::TACtx;

        private:
            FT current{0.f}, target{0.f};
            ssvu::Delegate<void(TimedAction&, FT)> action;
            bool simultaneous{false};

        public:
            inline TimedAction() = default;
            inline TimedAction(FT mTarget) : target{mTarget} {}

            inline TimedAction(const TimedAction&) = default;
            inline TimedAction(TimedAction&&) = default;

            inline void update(FT mFT)
            {
                current = ssvu::getClampedMax(current + mFT, target);
                action(*this, mFT);
            }
            inline bool isDone() const noexcept { return current >= target; }

            template <typename T>
            inline auto& operator+=(T&& mX)
            {
                action += ssvu::fwd<T>(mX);
                return *this;
            }

            inline float ease(float mA, float mB)
            {
                return ssvu::getMapEased<ssvu::Easing::Quad,
                    ssvu::Easing::InOut>(current, 0.f, target, mA, mB);
            }
        };
    }

    using TARecycler = ssvu::MonoRecycler<Internal::TimedAction>;
    using TAPtr = TARecycler::PtrType;
    using WRecycler = ssvu::PolyRecycler<Internal::Widget>;
    using WPtr = WRecycler::PtrType;

    namespace Internal
    {
        class TACtx
        {
            friend class avz::Ctx;

        private:
            std::vector<TAPtr> taQueue, taInExec;

            void simultaneously() { taQueue.back()->simultaneous = true; }

            void execFirstTA()
            {
                auto ptr(taQueue.front().get());
                taInExec.emplace_back(std::move(taQueue.front()));
                taQueue.erase(std::begin(taQueue));

                if(ptr->simultaneous) execFirstTA();
            }

        public:
            inline void update(FT mFT, float mSpeedFactor)
            {
                if(!taQueue.empty() && taInExec.empty())
                {
                    execFirstTA();
                }

                for(auto& ta : taInExec)
                {
                    ta->update(mFT * mSpeedFactor);
                }

                ssvu::eraseRemoveIf(taInExec, [](const auto& mTA)
                    {
                        return mTA->isDone();
                    });
            }

            inline void skipAnim()
            {
                for(auto& ta : taInExec)
                {
                    ta->current = ta->target;
                }
            }
        };
    }


    namespace Internal
    {
        struct Transform
        {
            Vec2f pos{0.f, 0.f};
            Vec2f scale{1.f, 1.f};
            float rot{0.f};
        };

        class Widget
        {
            friend class avz::Ctx;

        private:
            Ctx* ctx{nullptr};
            std::vector<WPtr> children;

            inline void transformHierarchyImpl(Widget* mParent)
            {
                tFinal.rot = tLocal.rot + mParent->tFinal.rot;
                tFinal.scale.x = tLocal.scale.x * mParent->tFinal.scale.x;
                tFinal.scale.y = tLocal.scale.y * mParent->tFinal.scale.y;

                tFinal.pos.x =
                    tLocal.pos.x * tFinal.scale.x + mParent->tFinal.pos.x;
                tFinal.pos.y =
                    tLocal.pos.y * tFinal.scale.y + mParent->tFinal.pos.y;
                ssvs::rotateRadAround(
                    tFinal.pos, mParent->tFinal.pos, tFinal.rot);

                for(auto& c : children) c->transformHierarchyImpl(this);
            }


        public:
            Transform tLocal, tFinal;


        public:
            template <typename T>
            void render(const T& mX);
            template <typename... TArgs>
            TimedAction& createTA(TArgs&&...);
            template <typename T, typename... TArgs>
            T& create(TArgs&&...);
            void simultaneously();
            void makeTACtx();
            void restoreTACtx();

            inline Widget(Ctx& mCtx) : ctx{&mCtx} {}

            inline virtual void update(FT) {}
            inline virtual void draw() {}

            inline void transformHierarchy()
            {
                for(auto& c : children) c->transformHierarchyImpl(this);
            }

            inline void updateHierarchy(FT mFT)
            {
                update(mFT);
                for(auto& c : children) c->updateHierarchy(mFT);
            }

            inline void drawHierarchy()
            {
                draw();
                for(auto& c : children) c->drawHierarchy();
            }
        };
    }
}

#endif