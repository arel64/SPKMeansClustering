import pandas as pd
import numpy as np
import csv
import sys
from enum import Enum
import  mykmeanssp

ERROR_MESSAGE = "An Error Has Occurred"
ARGV_LEN = len(sys.argv)
MAX_ARGV = 3
MIN_ARGV = 2
SPK = "spk"
WAM = "wam"
DDG = "ddg"
GL = "gl"
JACOBI = "jacobi"


def get_csv_datapoint_dimention(filename):
    with open(filename) as in_fp:
        first_line = next(csv.reader(in_fp))
        return len(first_line)


def get_dimentional_col(dimention):
    ret = []
    for i in range(dimention):
        ret += [i]
    return ret



if __name__ == "__main__":
    # try:
        np.random.seed(0)  # Important line for right pseudo-random behaviour.
        
        
        use_heuristic = len(sys.argv) == 4
        goal = sys.argv[len(sys.argv) - 2]
        data_file = sys.argv[len(sys.argv) - 1]

        # File processing

        dim = get_csv_datapoint_dimention(data_file)
        column_names = get_dimentional_col(dim)
        data_file_df = pd.read_csv(data_file, names=column_names)
        points_series = data_file_df.apply(lambda s: s.to_numpy(), axis=1)
        points_list = [[j for j in i] for i in points_series]

        # File processing done, only point list is relevent for the next parts.

        if(goal == WAM):
            printed_list = mykmeanssp.wam(points_list)
        elif(goal == DDG):
            printed_list = mykmeanssp.ddg(points_list)
        elif(goal == GL):
            printed_list = mykmeanssp.gl(points_list)
        elif(goal == JACOBI):
            printed_list = mykmeanssp.jacobi(points_list)


        i = (1 if goal != JACOBI else 2)
        while(i != 0):
            for point in (printed_list if goal != JACOBI else printed_list[len(printed_list) - i]):
                for coordinate in point[0: -1]:
                    print(f'{coordinate:.4f}', end= ',')
                print(f'{point[-1]:.4f}')
            i -= 1
            

    # except Exception:
    #     print(ERROR_MESSAGE)
