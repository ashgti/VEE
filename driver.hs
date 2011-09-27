#!/usr/bin/env runghc
{-# LANGUAGE ForeignFunctionInterface #-}
module Main where

import Foreign.C
import Foreign.C.String
import Control.Monad
import Control.Monad.Random

-- convenience functions since we are dealing with binary logic
on, off :: Bool
on = True
off = False

data SignalValue = Float Double
                 | Comm String
                 | Digital Bool
                 | Analog Int
    deriving (Show)

type Signal = (Double, SignalValue)

data Simulation = Ardulator { signals :: [[Signal]]
                            }
    deriving (Show)

data SignalReport = SignalReport { signalsDispatched :: Int
                                 , signalsMissed :: Int
                                 }
    deriving (Show)

data SimulationReport = Report { ranTimeDuration :: Double
                               , signalReports :: [SignalReport]
                               }
    deriving (Show)

runSimulation :: Simulation -> Double -> IO SimulationReport
runSimulation (Ardulator { signals = sigs }) startTime = do
    let reports = [SignalReport 3 5]
    return $ Report startTime reports

squareWave :: Double -> Double -> Double -> [Signal]
squareWave duration high ratio =
    zip (takeWhile (< duration) change_times)
        (cycle [Digital on, Digital off])
    where change_times = scanl (+) 0 (cycle [high, high * ratio])

squareWaveWithNoise :: (RandomGen g) => Double -> Double -> Double -> Double -> Rand g [Signal]
squareWaveWithNoise duration high ratio noiseLevel =
    zipWithM
        (\changeTime pinState -> do
            nextSignal <- getRandomR (changeTime * noiseLevel, changeTime * (2 - noiseLevel))
            return (nextSignal, pinState))
        (takeWhile (< duration) change_times)
        (cycle [Digital on, Digital off])
    where change_times = scanl (+) 0 (cycle [high, high * ratio])

main :: IO ()
main = do
    let x = Ardulator [squareWave 10.4 3.0 1.4]
    print x
    let dur = 10.5 :: Double
    let high = 3.0 :: Double 
    let ratio = 2.0 :: Double
    print $ squareWave dur high ratio
    values <- evalRandIO (squareWaveWithNoise dur high ratio 0.85)
    let y = Ardulator [values]
    y' <- runSimulation y 5.0
    print y'
    putStrLn "Done"
