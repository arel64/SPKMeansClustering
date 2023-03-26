import pandas as pd
import numpy as np
import csv
import sys
from enum import Enum
import  mykmeanssp
import kmeans as km
import math

ERROR_MESSAGE = "An Error Has Occurred"
ARGV_LEN = len(sys.argv)
MAX_ARGV = 3
MIN_ARGV = 2
SPK = "spk"
WAM = "wam"
DDG = "ddg"
GL = "gl"
JACOBI = "jacobi"
EPS = 0
MAX_ITER = 300


def get_csv_datapoint_dimention(filename):
    with open(filename) as in_fp:
        first_line = next(csv.reader(in_fp))
        return len(first_line)


def get_dimentional_col(dimention):
    ret = []
    for i in range(dimention):
        ret += [i]
    return ret

def find_huristic(first_row):
    retuned = 0
    max = 0
    for i in range(math.floor(float(len(first_row)) / 2)):
        value = abs(first_row[i + 1] - first_row[i])
        if max < value:
            returned = i
            max = value
    return returned

if __name__ == "__main__":
    # try:
        np.random.seed(0)
        
        goal = sys.argv[len(sys.argv) - 2]
        data_file = sys.argv[len(sys.argv) - 1]

        # File processing

        dim = get_csv_datapoint_dimention(data_file)
        column_names = get_dimentional_col(dim)
        data_file_df = pd.read_csv(data_file, names=column_names)
        points_series = data_file_df.apply(lambda s: s.to_numpy(), axis=1)
        points_list = [[j for j in i] for i in points_series]

        # File processing done, only point_list is relevent for the next parts.

        if goal != SPK:
            if(goal == WAM):
                printed_list = mykmeanssp.wam(points_list)
            elif(goal == DDG):
                printed_list = mykmeanssp.ddg(points_list)
            elif(goal == GL):
                printed_list = mykmeanssp.gl(points_list)
            elif(goal == JACOBI):
                printed_list = mykmeanssp.jacobi(points_list)
                printed_list = printed_list[0] + printed_list[1]

        else:
            # goal == spk
            USE_HUERISTIC = len(sys.argv) != 4
            gl = mykmeanssp.gl(points_list)
            jacobi_output = mykmeanssp.jacobi(gl)
            U = np.vstack(np.array(jacobi_output[0] + jacobi_output[1]))
            U = U[np.argsort(U[0]), :]
            
            k = find_huristic(U[0]) if USE_HUERISTIC else sys.argv[1]

            U = U[1:,:k]
            df = pd.DataFrame({'coordinates': [i for i in U]})

            the_chosen_boi = np.random.choice(df.index)
            centroids = pd.DataFrame(df["coordinates"].filter(items=[the_chosen_boi]))
            scatters = df.drop(the_chosen_boi, axis=0)

            while (len(centroids) != k):
                probs = []
                for scatter in scatters["coordinates"]:
                    def euc_dist(x): return np.linalg.norm(x - scatter)
                    probs += [min(centroids["coordinates"].map(euc_dist))]
                probs = np.array(probs)
                probs = probs / sum(probs)
                the_chosen_boi = (np.random.choice(scatters.index, p=probs))
                added_to_centroids = pd.DataFrame(
                    scatters["coordinates"].filter(items=[the_chosen_boi]))
                centroids = pd.concat([centroids, added_to_centroids])
                scatters = scatters.drop(the_chosen_boi, axis=0)

            dataPointList = [arr.tolist() for arr in scatters["coordinates"]]
            centroidsList = [arr.tolist() for arr in centroids["coordinates"]]
            dataPointList = dataPointList+centroidsList
            # Prints centroid's indexes.
            for i in centroids.index[0: -1]:
                print(i, end=",")
            print(centroids.index[len(centroids.index) - 1])

            printed_list = km.spk(dataPointList, centroidsList, MAX_ITER, EPS, len(dataPointList[0]))
        
        for row in printed_list:
            for coordinate in row[0: -1]:
                print(f'{coordinate:.4f}', end= ',')
            print(f'{row[-1]:.4f}')
    
    # except Exception:
    #     print(ERROR_MESSAGE)