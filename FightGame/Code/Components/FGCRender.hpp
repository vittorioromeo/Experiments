#pragma once

class FGCRender : public sses::Component
{
	public:
		enum class Anchor{Center, Bottom};

	private:
		FGGame& game;
		FGCPhys* cPhys{nullptr};
		std::vector<sf::Sprite> sprites;
		std::vector<ssvs::Vec2f> offsets;
		bool flippedX{false}, flippedY{false}, scaleWithBody{false};
		ssvs::Vec2f globalOffset;

	public:
		Anchor anchor{Anchor::Center};
		float globalScale{1.f};

		inline FGCRender(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
		}

		inline void update(FT) override
		{
			Vec2f position;

			if(anchor == Anchor::Center) position = cPhys->getPosPx();
			else if(anchor == Anchor::Bottom) position = toPx(cPhys->getPosF() + Vec2f(0.f, cPhys->getHalfHeight()));
		
			const auto& size(cPhys->getSizePx());

			for(auto i(0u); i < sprites.size(); ++i)
			{
				auto& s(sprites[i]);

				const auto& rect(s.getTextureRect());
				s.setOrigin({rect.width / 2.f, rect.height / 2.f});
				s.setPosition(position + globalOffset + offsets[i]);
				s.setScale(flippedX ? -1 : 1, flippedY ? -1 : 1);
				s.setScale(s.getScale() * globalScale);

				if(scaleWithBody) s.setScale(size.x / rect.width, size.y / rect.height);
			}
		}
		void draw() override;

		template<typename... TArgs> inline void emplaceSprite(TArgs&&... mArgs) { sprites.emplace_back(ssvu::fwd<TArgs>(mArgs)...); offsets.emplace_back(); }

		inline void setRotation(float mDegrees)	noexcept					{ for(auto& s : sprites) s.setRotation(mDegrees); }
		inline void setFlippedX(bool mFlippedX)	noexcept					{ flippedX = mFlippedX; }
		inline void setFlippedY(bool mFlippedY)	noexcept					{ flippedY = mFlippedY; }
		inline void setScaleWithBody(bool mScale) noexcept					{ scaleWithBody = mScale; }
		inline void setGlobalOffset(const ssvs::Vec2f& mOffset) noexcept	{ globalOffset = mOffset; }

		inline bool isFlippedX() const noexcept							{ return flippedX; }
		inline bool isFlippedY() const noexcept							{ return flippedY; }
		inline const decltype(sprites)& getSprites() const noexcept		{ return sprites; }
		inline const decltype(offsets)& getOffsets() const noexcept		{ return offsets; }
		inline decltype(sprites)& getSprites() noexcept					{ return sprites; }
		inline decltype(offsets)& getOffsets() noexcept					{ return offsets; }
		inline sf::Sprite& operator[](unsigned int mIdx)				{ return sprites[mIdx]; }
		inline const sf::Sprite& operator[](unsigned int mIdx) const	{ return sprites[mIdx]; }
};