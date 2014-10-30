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

	float delayAttack;
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

	result.delayAttack = 15.f;

	return result;
}

class FGCFighter : public sses::Component
{
	public:	
		enum class Action
		{
			Stand, Crouch, Jump, Fall
		};

		enum class MovStatus
		{
			Move, Stop
		};	

		enum class AtkStatus
		{
			Attack, Idle
		};
		
		enum class Dir{Left, Right};

	private:
		FGGame& game;
		FGCPhys* cPhys{nullptr};
		Body* body{nullptr};

		Action action{Action::Stand};
		AtkStatus atkStatus{AtkStatus::Idle};
		MovStatus movStatus{MovStatus::Stop};

		FighterClass fc;
		int hp;

		Dir dir;
		bool crouching{false};

		float atkDelay{0.f};

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
		inline FGCFighter(Entity& mE, FGGame& mGame) : Component{mE}, game(mGame) 
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
			body = &cPhys->getBody();

			fc = getTestFC();
			hp = fc.hp;

			getEntity().addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroupsToCheck(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->setRestitutionX(0.3f);
			body->setRestitutionY(0.0f);
			body->setMass(1.f);
			body->setVelTransferMultX(0.6f);			
			body->setWidth(toCr(fc.width));		

			cPhys->getGroundSensor().getShape().setWidth(body->getWidth());
		}

		inline void update(FT mFT) override
		{
			if(isInAir())
			{
				action = (body->getVelocity().y > 0) ? Action::Fall : Action::Jump;
			}
			else
			{
				action = crouching ? Action::Crouch : Action::Stand;
			}

			movStatus = (std::abs(body->getVelocity().x) < 100) ? MovStatus::Stop : MovStatus::Move;

			if(atkDelay > 0.f) 
			{
				atkStatus = AtkStatus::Attack;
				atkDelay -= mFT;
			}
			else
			{
				atkStatus = AtkStatus::Idle;
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

		inline void attack()
		{
			if(atkDelay > 0.f) return;

			atkDelay = fc.delayAttack;

			auto offset(toCr(0.f, -60.f));
			if(crouching) offset = toCr(0.f, -25.f);

			auto speed(Vec2f(toCr(40.f, 0.f)));
			if(isFacingLeft()) speed.x *= -1.f;

			game.getFactory().createProjPunch(*body, cPhys->getPosI() + offset, toCr(20.f, 20.f), speed, 1.4f);
		}

		void damage(FGCProj& mCProj);

		inline bool isInAir() const noexcept 		{ return cPhys->isInAir(); }
		inline bool isFacingLeft() const noexcept 	{ return dir == Dir::Left; }
		inline auto getAction() const noexcept		{ return action; }
		inline auto getMovStatus() const noexcept	{ return movStatus; }
		inline auto getAtkStatus() const noexcept	{ return atkStatus; }
};