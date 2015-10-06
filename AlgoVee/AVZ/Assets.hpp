#ifndef ALGOVEE_AVZ_ASSETS
#define ALGOVEE_AVZ_ASSETS

namespace avz
{
    class Assets
    {
    private:
        ssvs::AssetManager assetManager;

    public:
        sf::Font* liberationSans;

    private:
        inline Assets()
        {
            assetManager.load<sf::Font>("liberationSans",
                "/usr/share/fonts/TTF/LiberationSans-Regular.ttf");
            liberationSans = &assetManager.get<sf::Font>("liberationSans");
        }

    public:
        inline static auto& get() noexcept
        {
            static Assets result;
            return result;
        }
    };
}

#endif