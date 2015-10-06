#pragma once

class FGCRender : public sses::Component
{
public:
    enum class Anchor
    {
        Center,
        Bottom
    };

private:
    FGGame& game;
    FGCPhys* cPhys{nullptr};
    std::vector<sf::Sprite> sprites;
    std::vector<ssvs::Vec2f> offsets;
    bool flippedX{false}, flippedY{false}, scaleWithBody{false};
    ssvs::Vec2f globalOffset;

    Anchor anchor{Anchor::Center};
    float globalScale{1.f};

public:
    inline FGCRender(Entity& mE, FGGame& mGame) : Component{mE}, game(mGame)
    {
        cPhys = &getEntity().getComponent<FGCPhys>();
    }

    inline void update(FT) override
    {
        auto position(cPhys->getPosPx());

        if(anchor == Anchor::Bottom)
            position += toPx(Vec2f(0.f, cPhys->getHalfHeight()));

        const auto& size(cPhys->getSizePx());

        for(auto i(0u); i < sprites.size(); ++i)
        {
            auto& s(sprites[i]);

            const auto& rect(s.getTextureRect());
            s.setOrigin({rect.width / 2.f, rect.height / 2.f});
            s.setPosition(position + globalOffset + offsets[i]);
            s.setScale(flippedX ? -1 : 1, flippedY ? -1 : 1);
            s.setScale(s.getScale() * globalScale);

            if(scaleWithBody)
                s.setScale(size.x / rect.width, size.y / rect.height);
        }
    }
    void draw() override;

    template <typename... TArgs>
    inline void emplaceSprite(TArgs&&... mArgs)
    {
        sprites.emplace_back(ssvu::fwd<TArgs>(mArgs)...);
        offsets.emplace_back();
    }

    inline void setRotation(float mDegrees) noexcept
    {
        for(auto& s : sprites) s.setRotation(mDegrees);
    }
    inline void setFlippedX(bool mFlippedX) noexcept { flippedX = mFlippedX; }
    inline void setFlippedY(bool mFlippedY) noexcept { flippedY = mFlippedY; }
    inline void setScaleWithBody(bool mScale) noexcept
    {
        scaleWithBody = mScale;
    }
    inline void setGlobalOffset(const ssvs::Vec2f& mOffset) noexcept
    {
        globalOffset = mOffset;
    }
    inline void setGlobalScale(float mScale) noexcept { globalScale = mScale; }
    inline void setAnchor(Anchor mAnchor) noexcept { anchor = mAnchor; }

    inline bool isFlippedX() const noexcept { return flippedX; }
    inline bool isFlippedY() const noexcept { return flippedY; }
    inline const auto& getSprites() const noexcept { return sprites; }
    inline const auto& getOffsets() const noexcept { return offsets; }
    inline auto& getSprites() noexcept { return sprites; }
    inline auto& getOffsets() noexcept { return offsets; }
    inline auto& operator[](SizeT mIdx) { return sprites[mIdx]; }
    inline const auto& operator[](SizeT mIdx) const { return sprites[mIdx]; }
};