// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVVM_SOURCEVEEASM
#define SSVVM_SOURCEVEEASM

namespace ssvvm
{
	class SourceVeeAsm
	{
		private:
			std::string contents;
			bool preprocessed{false};

		public:
			inline static SourceVeeAsm fromStringRaw(std::string mSourceRaw)
			{
				SourceVeeAsm result;
				result.contents = std::move(mSourceRaw);
				result.preprocessed = false;
				return result;
			}
			inline static SourceVeeAsm fromStringPreprocessed(std::string mSourcePreprocessed)
			{
				SourceVeeAsm result;
				result.contents = std::move(mSourcePreprocessed);
				result.preprocessed = true;
				return result;
			}

			inline bool isPreprocessed() const noexcept { return preprocessed; }

			inline const std::string& getSourceString() const noexcept { return contents; }
			inline void setSourceString(std::string mSource) noexcept { contents = std::move(mSource); }
			inline void setPreprocessed(bool mValue) noexcept { preprocessed = mValue; }
	};


}

#endif
