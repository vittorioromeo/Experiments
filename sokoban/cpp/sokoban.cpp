#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <tuple>
#include <type_traits>

// ANNOYANCE: have to write this myself
template <typename T>
struct [[nodiscard]] range
{
    T d_begin;
    T d_end;

    struct [[nodiscard]] iterator
    {
        T d_value;

        iterator& operator++() noexcept
        {
            ++d_value;
            return *this;
        }

        [[nodiscard]] T operator*() const noexcept
        {
            return d_value;
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept
        {
            return d_value == rhs.d_value;
        }

        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept
        {
            return !(*this == rhs);
        }
    };

    iterator begin() const noexcept
    {
        return {d_begin};
    }
    iterator end() const noexcept
    {
        return {d_end};
    }
};

template <typename A, typename B>
range(A, B)->range<std::common_type_t<A, B>>;

// ANNOYANCE: non-exhaustive switches require this
[[noreturn]] void unreachable()
{
    assert(false);
    __builtin_unreachable();
}

enum class [[nodiscard]] Tile : std::uint8_t{None, Wall, Goal};
enum class [[nodiscard]] Obj : std::uint8_t{None, Player, Box};

constexpr std::size_t boardWidth = 8;
constexpr std::size_t boardHeight = 8;

template <typename T>
using Layer = std::array<T, boardWidth * boardHeight>;

using Index = std::size_t;
using Coord = std::size_t;
using Vec2D = std::tuple<Coord, Coord>;

[[nodiscard]] constexpr Index toIndex(const Vec2D c) noexcept
{
    const auto [x, y] = c;
    return (y * boardWidth) + x;
}

[[nodiscard]] constexpr Vec2D toVec2D(const Index i) noexcept
{
    return {i % boardWidth, i / boardWidth};
}

[[nodiscard]] constexpr const char* tileChar(const Tile tile)
{
    // clang-format off
    switch (tile) {
        case Tile::None: return " ";
        case Tile::Wall: return "▒";
        case Tile::Goal: return "○";
        default:         unreachable(); // ANNOYANCE: non-exhaustive switch
    };
    // clang-format on
}

[[nodiscard]] constexpr const char* objChar(const Obj obj, const Tile tile)
{
    // clang-format off
    switch (obj) {
        case Obj::None:   return tileChar(tile);
        case Obj::Player: return "☻";
        case Obj::Box:    return tile == Tile::Goal ? "◙" : "■";
        default:          unreachable(); // ANNOYANCE: non-exhaustive switch
    }
    // clang-format on
}

struct [[nodiscard]] Board
{
    Layer<Tile> d_tiles;
    Layer<Obj> d_objects;

    void print()
    {
        for(const auto y : range{0, boardHeight})
        {
            for(const auto x : range{0, boardWidth})
            {
                const auto i = toIndex({x, y});
                std::cout << objChar(d_objects[i], d_tiles[i]);
            }
            std::cout << '\n';
        }
    }

    [[nodiscard]] Coord findPlayer() const noexcept
    {
        for(const auto i : range{0, d_objects.size()})
        {
            if(d_objects[i] == Obj::Player)
            {
                return i;
            }
        }

        unreachable();
    }

    [[nodiscard]] std::size_t countGoals() const noexcept
    {
        return std::count_if(std::begin(d_tiles), std::end(d_tiles),
            [](const Tile t) { return t == Tile::Goal; });
    }
};

class [[nodiscard]] Game
{
private:
    Board d_board;
    Coord d_playerIndex;
    std::size_t d_goalsLeft;

    [[nodiscard]] auto& objAt(const Index i) noexcept
    {
        return d_board.d_objects[i];
    }

    [[nodiscard]] auto& tileAt(const Index i) noexcept
    {
        return d_board.d_tiles[i];
    }

    [[nodiscard]] bool moveBox(const Vec2D& pos, const Vec2D& offset)
    {
        const auto [px, py] = pos;
        const auto [ox, oy] = offset;
        const Index source = toIndex(pos);
        const Index target = toIndex({px + ox, py + oy});

        if(tileAt(target) == Tile::Wall || objAt(target) != Obj::None)
        {
            return false;
        }

        if(tileAt(source) == Tile::Goal)
        {
            ++d_goalsLeft;
        }

        if(tileAt(target) == Tile::Goal)
        {
            --d_goalsLeft;
        }

        std::swap(objAt(target), objAt(source));
        return true;
    }

public:
    Game(const Board& board)
        : d_board{board}, d_playerIndex{d_board.findPlayer()},
          d_goalsLeft{d_board.countGoals()}
    {
    }

    bool movePlayer(const Vec2D& offset)
    {
        const auto [px, py] = toVec2D(d_playerIndex);
        const auto [ox, oy] = offset;
        const Vec2D targetVec2D{px + ox, py + oy};
        const Index target = toIndex(targetVec2D);

        const bool couldntPushBox =
            objAt(target) == Obj::Box && !moveBox(targetVec2D, offset);

        if(tileAt(target) == Tile::Wall || couldntPushBox)
        {
            return false;
        }

        std::swap(objAt(target), objAt(d_playerIndex));
        d_playerIndex = target;
        return true;
    }

    void print()
    {
        d_board.print();
        std::cout << '\n' << "Goals left: " << d_goalsLeft << '\n';
    }
};

constexpr auto tileLayer = []() -> Layer<Tile> {
    const auto [_, H, X] = std::tuple{Tile::None, Tile::Wall, Tile::Goal};

    // clang-format off
    return {H,H,H,H,H,H,H,H,
            H,H,_,_,_,_,_,H,
            H,_,_,_,_,_,_,H,
            H,_,_,_,_,_,_,H,
            H,_,_,_,H,_,X,H,
            H,_,_,_,_,_,X,H,
            H,_,_,_,X,X,X,H,
            H,H,H,H,H,H,H,H};
    // clang-format on
}();

constexpr auto objectLayer = []() -> Layer<Obj> {
    const auto [_, P, B] = std::tuple{Obj::None, Obj::Player, Obj::Box};

    // clang-format off
    return {_,_,_,_,_,_,_,_,
            _,_,_,_,_,_,_,_,
            _,_,B,B,_,_,_,_,
            _,_,B,_,B,_,_,_,
            _,_,_,_,_,_,_,_,
            _,_,_,_,B,_,_,_,
            _,P,_,_,_,_,_,_,
            _,_,_,_,_,_,_,_};
    // clang-format on
}();

[[nodiscard]] bool restart()
{
    Game game{Board{tileLayer, objectLayer}};

    while(true)
    {
        system("clear");
        game.print();

        // clang-format off
        switch(std::getchar()) {
            // TOOD: possible bug -1
            case 'w': game.movePlayer({ 0, -1}); break;
            case 's': game.movePlayer({ 0,  1}); break;
            case 'a': game.movePlayer({-1,  0}); break;
            case 'd': game.movePlayer({ 1,  0}); break;

            case 'q': return false;
            case 'r': return true;
        }
        // clang-format on
    }
}

int main()
{
    while(restart())
    {
    }
}
