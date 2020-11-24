import json
import os
import subprocess
import shutil


for n_magnets in range(1, 2):
    print("N_MAGNETS", n_magnets)
    magnet_params = {"shape": {'X': 5., 'Y':5., 'Z': 1.},
                     "field": {'X': 0., 'Y': 0., 'Z':0.},
                     "n_magnets": n_magnets}

    dirname = os.path.dirname(__file__)
    with open(os.path.join(dirname, "magnet_params.json"), 'w') as f:
        json.dump(magnet_params, f)

    for sign in ["", "-"]:
        f_name = str(n_magnets) #str(i) + "_" + str(j)
        f_name += "_plus" if sign == "" else "_minus"
        print(sign == "")
        command = "python $FAIRSHIP/macro/run_simScript.py --PG --pID {}13 -n 1000 --Estart 25 --Eend 25 --FastMuon -o {}".format(sign, f_name)
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        for line in process.stdout:
            print(line)
        process.wait()
        shutil.copyfile(os.path.join(dirname, "magnet_params.json"), os.path.join(f_name, "params.json"))

