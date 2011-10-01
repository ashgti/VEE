import vee

def main():
    a = vee.Ardulator()
    a.length = 50.0
    
    a.signals.append(vee.signals.SquareWave(5, 2.0, 1.0))
    
    print a.run(25)
    print a.run()

if __name__ == '__main__':
    main()

