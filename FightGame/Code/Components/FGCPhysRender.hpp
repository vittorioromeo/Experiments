#pragma once

class FGCPhysRender : public sses::Component
{
	public:	
		FGGame& game;
		const FGCPhys* cPhys{nullptr};

	private:
		sf::RectangleShape shp;

	public:
		inline FGCPhysRender(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
		}

		inline void update(FT mFT) override
		{
			shp.setPosition(cPhys->getPosPx());
			shp.setSize(cPhys->getSizePx());
			shp.setOrigin(cPhys->getHalfSizePx());
		}

		void draw() override;
};