#pragma once

class FGCPlayerControl : public sses::Component
{
	public:	
		FGGame& game;
		FGCFighter* cFighter{nullptr};

	public:
		inline FGCPlayerControl(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cFighter = &getEntity().getComponent<FGCFighter>();
		}

		inline void update(FT)
		{
			const auto& gi(game.getInput());

			if(gi.getX() == 1) cFighter->moveRight();		
			else if(gi.getX() == -1) cFighter->moveLeft();	
			else cFighter->stop();
			
			if(gi.getCrouch()) cFighter->crouch();
			else cFighter->unCrouch();
			
			if(gi.getAttack()) cFighter->attack();

			if(gi.getJump()) cFighter->jump();
		}
};