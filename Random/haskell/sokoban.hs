import Control.Concurrent
import Control.Monad
import Data.Maybe
import System.IO
import Data.List
import System.Console.ANSI

insertEvery2 :: Int -> a -> [a] -> [a]
insertEvery2 n y xs = insertEvery2Impl n xs where
    insertEvery2Impl 0 xs = y : insertEvery2Impl n xs
    insertEvery2Impl m (x:xs) = x : insertEvery2Impl (m - 1) xs
    insertEvery2Impl _ [] = []

swapElems :: Int -> Int -> [a] -> [a]
swapElems i j ls = [get k x | (k, x) <- zip [0..length ls - 1] ls]
    where get k x | k == i = ls !! j
                  | k == j = ls !! i
                  | otherwise = x

worldWidth = 28
world = "############################\
        \#..........................#\
        \#..........................#\
        \#..........................#\
        \#...................x......#\
        \############################"

isWall w i = if (w !! i) == '#' then True else False

moveh _ w Nothing = w
moveh o w (Just x)  = if (isWall w (x + o)) then w else (swapElems x (x + o) w)

movev _ w Nothing = w
movev o w (Just x) = if (isWall w ro) then w else (swapElems x ro w) where
    ro = (x + o * worldWidth)

movel = moveh (-1)
mover = moveh 1
moveu = movev (-1)
moved = movev 1

worldExec :: String -> Char -> String
worldExec w 'a' = movel w (findIndex (=='x') w)
worldExec w 'd' = mover w (findIndex (=='x') w)
worldExec w 'w' = moveu w (findIndex (=='x') w)
worldExec w 's' = moved w (findIndex (=='x') w)
worldExec w _ = world

readKey = do
    c <- newEmptyMVar    
    a <- getChar
    putMVar c a
    return a

loop w a = do
    {- putStrLn [a] -}
    let w' = worldExec w a
    clearScreen
    putStrLn $ "\n" ++ (insertEvery2 worldWidth '\n' w')

    c <- readKey
    loop w' c

main = do
    hSetBuffering stdin NoBuffering
    c <- readKey
    loop world c
    main

                    
