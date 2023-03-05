import sys



with open(sys.argv[1]) as first_file:
    with open(sys.argv[2]) as second_file:
        print(first_file.read() == second_file.read())