#include <future>

volatile unsigned int state = 0;
unsigned int get_value()
{
    return state;
}

int main()
{
    return std::async(std::launch::async, get_value).get();
}