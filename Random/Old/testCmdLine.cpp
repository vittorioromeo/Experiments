#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/CmdLine/CmdLine.hpp>

int main(int argc, char* argv[])
{
    using namespace ssvu::CmdLine;
    Ctx ctx;

    auto& cmdMain(ctx.getCmdMain());
    auto& fUpper(cmdMain.create<Flag>("u", "upper"));
    auto& fvTitle(cmdMain.create<FlagValue<std::string>>("t", "title"));
    auto& fvoMsg(
        cmdMain.create<FlagValueOpt<std::string>>("m", "msg", "Hello world!"));

    cmdMain += [&fUpper, &fvTitle, &fvoMsg]
    {
        ssvu::lo(fvTitle.get())
            << (fUpper ? ssvu::toUpper(fvoMsg.get()) : fvoMsg.get())
            << std::endl;
    };

    try
    {
        ctx.process(argc, argv);
    }
    catch(const std::runtime_error& mEx)
    {
        ssvu::lo() << mEx.what() << std::endl;
        return 1;
    }

    return 0;
}
