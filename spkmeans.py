import pandas as pd
import csv
import sys
from enum import Enum
import  mykmeanssp

ERROR_MESSAGE = "An Error Has Occurred"
ARGV_LEN = len(sys.argv)
MAX_ARGV = 3
MIN_ARGV = 2

class goal(Enum):
    full_spectral_kmeans = "spk"
    weighted_adjacency_matrix = "wam"
    diagonal_degree_matrix = "ddg"
    graph_laplacian = "gl"
    jacobi = "jacobi"


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
    try:
        use_heuristic = False # May be unuseful.

        goal = sys.argv[len(sys.argv) - 2]
        data_file = sys.argv[len(sys.argv) - 1]

        # resolve file dimention and construct header
        dimention = get_csv_datapoint_dimention(data_file)
        col_names = get_dimentional_col(dimention)

        data_file_df = pd.read_csv(data_file, names=col_names)

        points_series = data_file.apply(lambda s: s.to_numpy(), axis=1)

        # File processing done, now points is a series of the different points given in the txt file.
        
        points_list = [i for i in points_series]

        print(points_list)




    except Exception:
        print(ERROR_MESSAGE)
