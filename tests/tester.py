import os

suffixes = ["ddg", "gl", "wam"]

for functionality in suffixes:
    for i in range(1, 11):
        output = lambda x: "tests_output/" + x +"/test" + str(i) +"_" + functionality + ".txt"
        os.system("python ../spkmeans.py " + functionality + " tests_input/test" + str(i) +".txt >" + output("python"))
        with open(output("python")) as python_out:
            with open(output("C")) as C_out:
                if python_out.read() == C_out.read():
                    print("Great suceess! Fuck yeah!")
                else:
                    print("Oi, failure on input:" + str(i) + "\nfor functionality:" + functionality)
