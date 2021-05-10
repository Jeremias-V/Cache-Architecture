from random import randint

def main():
    with open('RAM-Data.txt', 'w') as f:
        for i in range(2048):
            randStr = format(randint(0, 255), '08b')
            if(i == 2047):
                f.write('{}'.format(randStr))
            else:
                f.write('{}\n'.format(randStr))
main()
