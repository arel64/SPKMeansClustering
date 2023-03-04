import sys



with open(sys.argv[1]) as first_file:
    with open(sys.argv[2]) as second_file:
        if(first_file.read() != second_file.read()):
            print("fail")
        else:
            print("great success!")