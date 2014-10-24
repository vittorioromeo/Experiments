#ifndef ALGOVEE_AVZ_WIDGETS
#define ALGOVEE_AVZ_WIDGETS

namespace avz
{
	namespace w
	{
		class Base : public Internal::Widget
		{
			public:
				inline void taAppear()
				{
					createTA(25.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(0.f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};
				}

				inline void taJump()
				{
					createTA(6.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(1.f, 1.3f));
						tLocal.scale = Vec2f{s, s};
					};

					createTA(6.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(1.3f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};
				}

				inline void taTranslate(const Vec2f& mPos)
				{
					createTA(35.f) += [this, mPos](auto& mTA, FT)
					{
						tLocal.pos.x = mTA.ease(tLocal.pos.x, mPos.x);
						tLocal.pos.y = mTA.ease(tLocal.pos.y, mPos.y);
					};
				}

				inline void taColorize(sf::Color& mColor, const sf::Color& mCStart, const sf::Color& mCEnd)
				{
					createTA(25.f) += [this, &mColor, mCStart, mCEnd](auto& mTA, FT)
					{
						mColor.r = mTA.ease(mCStart.r, mCEnd.r);
						mColor.g = mTA.ease(mCStart.g, mCEnd.g);
						mColor.b = mTA.ease(mCStart.b, mCEnd.b);
						mColor.a = mTA.ease(mCStart.a, mCEnd.a);
					};
				}

				template<typename T> inline void apply(T& mX)
				{
					mX.setOrigin(ssvs::getLocalCenter(mX));
					mX.setPosition(tFinal.pos);
					mX.setScale(tFinal.scale);
					mX.setRotation(ssvu::toDeg(tFinal.rot));
				}

			protected:
				inline Base(Ctx& mCtx, const Vec2f& mPos) : Internal::Widget{mCtx}
				{
					tLocal.pos = mPos;
					tLocal.scale = Vec2f{1.f, 1.f};
					tLocal.rot = 0.f;
				}
		};

		class Text : public Base
		{
			private:
				sf::Text impl;

				inline void setStr(const std::string& mStr)
				{
					if(impl.getString() == "")
					{
						impl.setString(mStr);
						taAppear();
						taJump();
						createTA(10.f) += [this, mStr](auto&, FT){ };

						return;
					}

					createTA() += [this, mStr](auto&, FT) { impl.setString(mStr); };
					taJump();
				}

			public:
				inline Text(Ctx& mCtx, const Vec2f& mPos)
					: Base{mCtx, mPos}, impl{"", *Assets::get().liberationSans, 30}
				{
					impl.setScale(0.f, 0.f);
					impl.setColor(sf::Color::White);
				}

				inline auto& operator=(const std::string& mStr) { setStr(mStr); return *this; }
				inline auto& operator+=(const std::string& mStr) { setStr(impl.getString() + mStr); return *this; }

				inline void update(FT) override { apply(impl); }
				inline void draw() override { render(impl); }
		};

		class TextSquare : public Base
		{
			private:
				sf::RectangleShape bg;
				sf::Color bgColorDef{95, 95, 95, 255};
				sf::Color bgColorHgl{175, 95, 175, 255};
				sf::Color bgColor{bgColorDef};
				Text* text;

			public:
				inline TextSquare(Ctx& mCtx, const Vec2f& mPos)
					: Base{mCtx, mPos}
				{
					text = &create<Text>(ssvs::zeroVec2f);
					bg.setSize(Vec2f{65, 65});
					bg.setOutlineColor(sf::Color::White);
					bg.setOutlineThickness(3);
				}

				inline void taShow()
				{
					taAppear();
					taJump();
				}

				inline void update(FT) override
				{
					apply(bg);
					bg.setFillColor(bgColor);
					//text->pos = pos;
				}
				inline void draw() override { render(bg); }

				inline auto& operator=(const std::string& mStr)		{ *text = mStr; return *this; }
				inline auto& operator+=(const std::string& mStr)	{ *text += mStr; return *this; }

				inline void taHightlight()		{ taColorize(bgColor, bgColorDef, bgColorHgl); }
				inline void taUnhightlight()	{ taColorize(bgColor, bgColorHgl, bgColorDef); }

				inline auto& getText() noexcept 			{ return *text; }
				inline const auto& getText() const noexcept	{ return *text; }

				inline auto& getBG() noexcept { return bg; }
				inline const auto& getBG() const noexcept { return bg; }
		};

		template<typename T> class Vector : public Base
		{
			public:
				std::vector<TextSquare*> tss;
				std::vector<T> data;

				inline void refreshPositions()
				{
					if(tss.empty()) return;

					tss[0]->tLocal.pos = ssvs::zeroVec2f;
					for(auto i(1u); i < tss.size(); ++i)
					{
						tss[i]->tLocal.pos = tss[i - 1]->tLocal.pos + Vec2f{65.f, 0.f};
					}
				}

			public:
				inline Vector(Ctx& mCtx, const Vec2f& mPos) : Base{mCtx, mPos} { }

				inline void taPushFront(const T& mX)
				{
					// Instant
					data.emplace(std::begin(data), mX);
					auto& ts(create<TextSquare>(ssvs::zeroVec2f));
					auto xStr(ssvu::toStr(mX));

					// Timed
					this->createTA() += [this, &ts](auto&, FT){ tss.emplace(std::begin(tss), &ts); };
					this->createTA() += [this](auto&, FT){ this->refreshPositions(); };
					ts.taShow();
					ts = xStr;
				}
				inline void taPushBack(const T& mX)
				{
					// Instant
					data.emplace_back(mX);
					auto& ts(create<TextSquare>(ssvs::zeroVec2f));
					auto xStr(ssvu::toStr(mX));

					// Timed
					this->createTA() += [this, &ts](auto&, FT){ tss.emplace_back(&ts); };
					this->createTA() += [this](auto&, FT){ this->refreshPositions(); };
					ts.taShow();
					ts = xStr;
				}

				inline void update(FT) override
				{
					//tLocal.rot += 0.01f * mFT;
				}

				inline auto& operator[](SizeT mI) noexcept { return data[mI]; }
				inline const auto& operator[](SizeT mI) const noexcept { return data[mI]; }

				inline void taHightlight(SizeT mI) 
				{  
					this->createTA() += [this, mI](auto&, FT){ tss[mI]->taHightlight(); };
				}
				inline void taUnhightlight(SizeT mI) 
				{  
					this->createTA() += [this, mI](auto&, FT){ tss[mI]->taUnhightlight(); };
				}

				inline void taSet(SizeT mI, T mX)
				{
					this->createTA() += [this, mI, mX](auto&, FT) 
					{
						(*tss[mI]) = ssvu::toStr(mX);
					};
				}

				inline void taTranslateTo(SizeT mI, const Vec2f& mP)
				{
					this->createTA() += [this, mI, mP](auto&, FT) 
					{
						tss[mI]->taTranslate(mP);
					};
				}

				inline void taTranslateBy(SizeT mI, const Vec2f& mP)
				{
					this->createTA() += [this, mI, mP](auto&, FT) 
					{
						tss[mI]->taTranslate(tss[mI]->tLocal.pos + mP);
					};
				}

				inline void taSwap(SizeT mA, SizeT mB)
				{
					// Instant
					std::swap(data[mA], data[mB]);

					// Timed
					this->createTA() += [this, mA, mB](auto&, FT)
					{
						ssvu::lo("make") << "\n";
						makeTACtx();

						ssvu::lo("taswapstart") << mA << " <-> " << mB << "\n";

						auto& tsA(*tss[mA]);
						auto& tsB(*tss[mB]);

						auto pTSA(tsB.tLocal.pos);
						auto pTSB(tsA.tLocal.pos);
	
						tsA.taHightlight();
						tsB.taHightlight();

						tsA.taTranslate(tsA.tLocal.pos + Vec2f{0, -100});
						tsB.taTranslate(tsB.tLocal.pos + Vec2f{0, -100});

						tsA.taTranslate(Vec2f{pTSA.x, tsA.tLocal.pos.y - 100});
						this->simultaneously();
						tsB.taTranslate(Vec2f{pTSB.x, tsB.tLocal.pos.y - 100});

						tsA.taTranslate(pTSA);
						tsB.taTranslate(pTSB);

						tsA.taUnhightlight();
						simultaneously();
						tsB.taUnhightlight();

						ssvu::lo("pre-restore") << "\n";
						this->createTA() += [this](auto&, FT)
						{
							ssvu::lo("restore") << "\n";
							restoreTACtx();
						};
					};

					this->createTA() += [this, mA, mB](auto&, FT)
					{
						ssvu::lo("taswap") << mA << " <-> " << mB << "\n";
						std::swap(tss[mA], tss[mB]);
						ssvu::lo() << "\n\n";
					};
				}

				inline auto& getTS(SizeT mI) noexcept { return tss[mI]; }

				inline auto getSize() const noexcept { return data.size(); }
				inline auto& getData() noexcept { return data; }
				inline const auto& getData() const noexcept { return data; }
		};
	}

}	

#endif