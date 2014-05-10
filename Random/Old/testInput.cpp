#include <SSVUtils/SSVUtils.hpp>
#include <SSVUtilsJson/SSVUtilsJson.hpp>
#include <SSVStart/SSVStart.hpp>

enum class TNInputPieceType{TNKB, TNMB};
enum class TNInputRelationType{TNAnd, TNOr};

union TNInputUnion
{ 
	ssvs::KKey key; 
	ssvs::MBtn btn; 
	
	inline TNInputUnion(ssvs::KKey mKey) noexcept : key{mKey} { }
	inline TNInputUnion(ssvs::MBtn mBtn) noexcept : btn{mBtn} { }
};

class TNInputPiece
{
	private:
		TNInputPieceType type;
		TNInputUnion inputUnion;

	public:
		inline TNInputPiece(ssvs::KKey mKey) noexcept : type{TNInputPieceType::TNKB}, inputUnion{mKey} { }
		inline TNInputPiece(ssvs::MBtn mBtn) noexcept : type{TNInputPieceType::TNMB}, inputUnion{mBtn} { }

		inline bool isKKey() const noexcept { return type == TNInputPieceType::TNKB; }
		inline bool isMBtn() const noexcept { return type == TNInputPieceType::TNMB; }
};

class TNInputCombo
{

};

int main() 
{ 
	

	return 0; 
}