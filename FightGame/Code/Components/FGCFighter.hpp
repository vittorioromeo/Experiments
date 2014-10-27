#pragma once

struct FighterClass
{
	int hp;
	
	int speedStanding;
	int speedCrouching;
	
	int jumpStrength;

	int width;

	int heightStanding;
	int heightCrouching;
};

inline auto getTestFC()
{
	FighterClass result;

	result.hp = 100;
	
	result.speedStanding = 10;
	result.speedCrouching = 6;
	
	result.jumpStrength = 25;

	result.width = 60;

	result.heightStanding = 150;
	result.heightCrouching = 65;

	return result;
}

class FGCFighter : public sses::Component
{
	public:	
		enum class Action
		{
			Standing, StandAttacking, StandWalking, 
			Crouching, CrouchWalking, CrouchAttacking, 
			Jumping, Falling
		};
		
		enum class Dir{Left, Right};

	private:
		FGGame& game;
		FGCPhys* cPhys{nullptr};
		Body* body{nullptr};
		
		Sensor* groundSensor{nullptr};
		bool inAir{false};

		Action action{Action::Standing};

		FighterClass fc;
		int hp;

		Dir dir;
		bool crouching{true};

		inline auto getCrouchDiffVec()
		{
			return Vec2i{0, (toCr(fc.heightStanding) - toCr(fc.heightCrouching)) / 2};
		}	

		inline void moveImpl(int mDir)
		{
			auto speed(crouching ? fc.speedCrouching : fc.speedStanding);
			auto crSpd(toCr(speed));
			auto diff((crSpd * mDir) - body->getVelocity().x);
			
			ssvu::clamp(diff, -crSpd, crSpd);
			body->applyAccel(Vec2f(diff, 0.f));

			dir = mDir > 0 ? Dir::Right : Dir::Left;
		}

	public:
		inline FGCFighter(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
			body = &cPhys->getBody();

			groundSensor = &cPhys->getWorld().createSensor(cPhys->getPosI(), Vec2i(100, 100));
			groundSensor->addGroupsToCheck(FGGroup::FGGSolid);

			fc = getTestFC();
			hp = fc.hp;

			getEntity().addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroupsToCheck(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->setRestitutionX(0.3f);
			body->setRestitutionY(0.3f);
			body->setMass(1.f);
			body->setVelTransferMultX(0.6f);			
			body->setWidth(toCr(fc.width));

			body->onPreUpdate += [this]
			{
				
			};

			groundSensor->onPreUpdate += [this]
			{ 
				groundSensor->setPosition(body->getPosition() + Vec2i{0, body->getHeight() / 2}); 
				inAir = true; 
			};
			groundSensor->onDetection += [this](const DetectionInfo& mDI)
			{
				if(&mDI.body == this->body) return;
				inAir = false;				
			};
		}

		inline void update(FT) override
		{
			if(isInAir())
			{
				if(body->getVelocity().y > 0) action = Action::Falling;
				else action = Action::Jumping;
			}
			else if(!crouching)
			{
				if(std::abs(body->getVelocity().x) < 100) action = Action::Standing;
				else action = Action::StandWalking;
			}
			else
			{
				if(std::abs(body->getVelocity().x) < 100) action = Action::Crouching;
				else action = Action::CrouchWalking;	
			}
		}

		inline void draw() override { }	

		inline void moveLeft() { moveImpl(-1); }
		inline void moveRight() { moveImpl(1); }		

		inline void stop()
		{
			auto diff(0 - body->getVelocity().x);
			body->applyAccel(Vec2f(diff * 0.9f, 0.f));
		}
		
		inline void jump()
		{
			if(isInAir() || crouching) return;
			body->applyAccel(Vec2f(0.f, -toCr(fc.jumpStrength)));
		}

		inline void unCrouch()
		{
			if(!crouching) return;
			
			cPhys->setPos(cPhys->getPosI() - getCrouchDiffVec());
			cPhys->setHeight(toCr(fc.heightStanding));			

			crouching = false;
		}

		inline void crouch()
		{
			if(crouching || isInAir()) return;

			cPhys->setPos(cPhys->getPosI() + getCrouchDiffVec());
			cPhys->setHeight(toCr(fc.heightCrouching));
			
			crouching = true;
		}

		inline bool isInAir() const noexcept 		{ return inAir; }
		inline bool isFacingLeft() const noexcept 	{ return dir == Dir::Left; }
		inline auto getAction() const noexcept		{ return action; }

};