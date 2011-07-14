import vee

def main():
    a = vee.Ardulator()
    a.length = 50.0
    
    s = vee.signals.Determenistic(5)
    
    print a.run(25)
    print a.run()
    print a.register_signals(5)

if __name__ == '__main__':
    main()

