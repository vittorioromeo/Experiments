// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVVM_OPCODES
#define SSVVM_OPCODES

namespace ssvvm
{
	SSVU_FATENUM_MGR(ReflectedEnum);

	#define SSVVM_CREATE_MFPTR(mIdx, mData, mArg) & SSVPP_EXPAND(T) :: SSVPP_EXPAND(mArg) SSVPP_COMMA_IF(mIdx)

	#define SSVVM_CREATE_OPCODE_DATABASE(...)	\
		SSVU_FATENUM_DEFS(ReflectedEnum, OpCode, std::size_t, __VA_ARGS__) \
		template<typename T> inline VMFnPtr<T> getVMFnPtr(OpCode mOpCode) noexcept \
		{ \
			static VMFnPtr<T> fnPtrs[] \
			{ \
				SSVPP_FOREACH(SSVVM_CREATE_MFPTR, SSVPP_EMPTY(), __VA_ARGS__) \
			}; \
			return fnPtrs[std::size_t(mOpCode)]; \
		}

	SSVVM_CREATE_OPCODE_DATABASE
	(
		// Virtual machine control
		halt,

		// Register instructions
		loadIntCVToR,
		loadFloatCVToR,
		moveRVToR,

		// Register-stack instructions
		pushRVToS,
		popSVToR,
		moveSBOVToR,

		// Stack instructions
		pushIntCVToS,
		pushFloatCVToS,
		pushSVToS,
		popSV,

		// Program logic
		goToPI,
		goToPIIfIntRV,
		goToPIIfCompareRVGreater,
		goToPIIfCompareRVSmaller,
		goToPIIfCompareRVEqual,
		callPI,
		returnPI,

		// Register basic arithmetic
		incrementIntRV,
		decrementIntRV,

		// Stack basic arithmetic
		addInt2SVs,
		addFloat2SVs,
		subtractInt2SVs,
		subtractFloat2SVs,
		multiplyInt2SVs,
		multiplyFloat2SVs,
		divideInt2SVs,
		divideFloat2SVs,

		// Comparisons
		compareIntRVIntRVToR,
		compareIntRVIntSVToR,
		compareIntSVIntSVToR,
		compareIntRVIntCVToR,
		compareIntSVIntCVToR
	)

	inline const std::string& getOpCodeStr(OpCode mOpCode) noexcept { return ReflectedEnum<OpCode>::getAsString(mOpCode); }
}

#endif
