import sys


# with open(sys.argv[1]) as first_file:
#     with open(sys.argv[2]) as second_file:
#         i = 0
#         j = 0
#         first_reader = csv.reader(first_file)
#         second_reader = csv.reader(second_file)
#         for first_row in first_reader:
#             i += 1
#             j = 0
#             second_reader = r

with open(sys.argv[1]) as first_file:
    with open(sys.argv[2]) as second_file:
        print(first_file.read() == second_file.read())
