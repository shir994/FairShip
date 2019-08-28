import json
import os
import subprocess
import shutil


for i, Z in enumerate(range(1, 15, 1)):
    for j, X in enumerate(range(1, 10, 1)):
        print("kek", i, Z, j, X)
        magnet_params = {"shape": {'X': X, 'Y':5., 'Z': Z},
                         "field": {'X': 0., 'Y': 4, 'Z':0}}

        dirname = os.path.dirname(__file__)
        with open(os.path.join(dirname, "magnet_params.json"), 'w') as f:
            json.dump(magnet_params, f)

        for sign in ["", "-"]:
            f_name = str(i) + "_" + str(j)
            f_name += "_plus" if sign == "" else "_minus"
            print(sign == "")
            command = "python $FAIRSHIP/macro/run_simScript.py --PG --pID {}13 -n 1000 --Estart 4 --Eend 10 --FastMuon -o {}".format(sign, f_name)
            process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            for line in process.stdout:
                print(line) 
            process.wait()
            shutil.copyfile(os.path.join(dirname, "magnet_params.json"), os.path.join(f_name, "params.json"))

