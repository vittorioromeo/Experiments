#include <vector>
#include <variant>
#include <iostream>
#include <boost/hana.hpp>

struct my_variant_wrapper;

using my_variant = 
    std::variant<int, float, std::vector<my_variant_wrapper>>;

struct my_variant_wrapper 
{
    my_variant _v;
    
    template <typename... Ts>
    my_variant_wrapper(Ts&&... xs) : _v(std::forward<Ts>(xs)...) { } 
};

struct struct_visitor
{
    void operator()(int x) const { std::cout << x << "i\n"; }
    void operator()(float x) const { std::cout << x << "f\n"; }
    
    void operator()(const std::vector<my_variant_wrapper>& x) const 
    { 
        for(const auto& y : x) std::visit(*this, y._v); 
    }
};

template<typename TRet, typename... TFs>
auto make_recursive_visitor(TFs&&... fs)
{
    namespace bh = boost::hana;
    return bh::fix([&fs...](auto self, const auto& x) -> TRet
        {
            bh::overload(std::forward<TFs>(fs)...)(self, x);
        });
    
    // std::visit(lambda_visitor, std::forward<TVariant>(v));
}

 
int main()
{
    my_variant v{
        std::vector<my_variant_wrapper>{
            my_variant_wrapper{45}, 
            std::vector<my_variant_wrapper>{
                my_variant_wrapper{1}, my_variant_wrapper{2}
            },
            my_variant_wrapper{33.f}
        }
    };

    namespace bh = boost::hana;
    /*auto lambda_visitor = bh::fix([](auto self, const auto& x) -> void
        {
            bh::overload(
                [](int y){ std::cout << y << "i\n"; },
                [](float y){ std::cout << y << "f\n"; },
                [&self](const std::vector<my_variant_wrapper>& y)
                {
                    for(const auto& z : y) std::visit(self, z._v);  
                })(x);
        });*/
    
    auto lambda_visitor = make_recursive_visitor<void>(
        [](auto, int y){ std::cout << y << "i\n"; },
        [](auto, float y){ std::cout << y << "f\n"; },
        [](auto self, const std::vector<my_variant_wrapper>& y)
        {
            for(const auto& z : y) std::visit(self, z._v);  
        }
    );
     
    std::visit(lambda_visitor, v);
}