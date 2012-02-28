import Data.List
import System.IO
import Control.Monad(when)
import System.Exit
import System.Environment(getArgs)

main = do
    args <- getArgs
    when (length args /= 2) $ do
        putStrLn "Syntax: passwd-al filename uid"
        exitFailure

    content <- readFile (args !! 0)
    let username = findByUID content (read (args !! 1))
    case username of
         Just x -> putStrLn x
         Nothing -> putStrLn "UID not found"

findByUID :: String -> Integer -> Maybe String
findByUID content uid = 
    lookup uid al
        where al = map parseLine. lines $ content

parseLine :: String -> (Integer, String)
parseLine input =
    (read (fields !! 2), fields !! 0)
        where fields = split ':' input

split :: Eq a => a -> [a] -> [[a]]
split _ [] = [[]]
split delim str =
    let (before, remainder) = span (/= delim) str
    in before : case remainder of
                     [] -> []
                     x -> split delim (tail x)
