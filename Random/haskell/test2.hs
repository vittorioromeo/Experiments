-- Types of currency pieces
pieceTypes = [20, 10, 5, 1]

-- Typedefs
type Money = Int
type Target = Money
type Piece = Money
type Used = [Money]

-- Computation: can be ongoing/success/failure
data CurrComp = OnGoing Target Used | Success Used | Failure 
    deriving(Show)

-- Compute possible combinations
compute :: Int -> CurrComp -> [CurrComp]
compute max (OnGoing target used)   
            
            -- Used more pieces than the max? -> Failure
            | length used > max = []

            -- Negative target? -> Failure
            | target < 0 = [] 

            -- Reached target? -> Success
            | target == 0 = [Success used]

            -- Recursive step -> try one of the pieces
            | otherwise = do
                piece <- pieceTypes
                compute max (OnGoing (target - piece) (used ++ [piece]))

main = mapM print $ [z | z <- compute 7 (OnGoing 120 [])]