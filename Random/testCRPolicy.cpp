#include <SSVUtils/Core/Core.hpp>

namespace Objs
{
    namespace Internal
    {
        struct Base
        {
            virtual void run() {}
        };
    }

    struct O1 : public Internal::Base
    {
        void run() override { ssvu::lo() << "O1 run()" << std::endl; }
    };

    struct O2 : public Internal::Base
    {
        void run() override { ssvu::lo() << "O2 run()" << std::endl; }
    };
}

namespace Exec
{
    class RunTime
    {
    private:
        ssvu::UPtr<Objs::Internal::Base> obj;

        template <typename T>
        RunTime(T&& mObj)
            : obj(std::forward<T>(mObj))
        {
        }

    public:
        template <typename T, typename... TArgs>
        static RunTime create(TArgs&&... mArgs)
        {
            RunTime temp{ssvu::makeUPtr<T>(std::forward<TArgs>(mArgs)...)};
            return temp;
        }

        auto get() -> decltype(*obj) { return *obj; }

        template <typename T, typename... TArgs>
        void reset(TArgs&&... mArgs)
        {
            obj = ssvu::makeUPtr<T>(std::forward<TArgs>(mArgs)...);
        }
    };

    template <typename T>
    class CompileTime
    {
    private:
        T obj;

    public:
        template <typename... TArgs>
        CompileTime(TArgs&&... mArgs)
            : obj(std::forward<TArgs>(mArgs)...)
        {
        }

        T& get() { return obj; }
    };
}

template <typename TExec>
class Manager
{
private:
    TExec exec;

public:
    template <typename... TArgs>
    Manager(TArgs&&... mArgs)
        : exec(std::forward<TArgs>(mArgs)...)
    {
    }

    void run() { exec.get().run(); }
    TExec& getExec() { return exec; }
};

int main()
{
    {
        Manager<Exec::CompileTime<Objs::O1>> manager;
        manager.run();
    }

    ssvu::lo() << std::endl;

    {
        Manager<Exec::CompileTime<Objs::O2>> manager;
        manager.run();
    }

    ssvu::lo() << std::endl;

    {
        Manager<Exec::RunTime> manager{Exec::RunTime::create<Objs::O1>()};
        manager.run();
        manager.getExec().reset<Objs::O2>();
        manager.run();
    }

    return 0;
}