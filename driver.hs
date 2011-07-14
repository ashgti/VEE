{-# LANGUAGE ForeignFunctionInterface #-}
import Foreign
import Foreign.C.Types
import System.Posix.Unistd
import Control.Monad

-- foreign import ccall "arduino.h foo"
--     c_foo :: CInt

z :: Int -> Int
z y = if y > 1 then break y else x $ y - 1

x :: Int -> Int
x y = if y > 4 then y else z $ y + 2

main = do
    print (x 0)
    putStrLn "Done"

