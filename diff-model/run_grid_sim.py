import json
import os
import subprocess
import shutil
import time

for n_magnets in range(15, 16):
    print("N_MAGNETS", n_magnets)
    magnet_params = {"shape": {'X': 5., 'Y':5., 'Z': 1.},
                     "field": {'X': 0., 'Y': 0., 'Z':0.},
                     "n_magnets": n_magnets}

    with open(os.path.join(os.getenv("FAIRSHIP"), "diff-model/magnet_params.json"), 'w') as f:
        json.dump(magnet_params, f)

    for sign in [""]:#, "-"]:
        f_name = str(n_magnets) #str(i) + "_" + str(j)
        f_name += "_plus" if sign == "" else "_minus"
        print("SIGN:", "-" if sign == "" else "+")
        s_time = time.time()
        command = "python $FAIRSHIP/macro/run_simScript.py --PG --pID {}13 -n 100000 --Estart 25 --Eend 25 --FastMuon -o {}".format(sign, f_name)
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        for line in process.stdout:
            print(line)
        process.wait()
        print("GENERATION TIME: {} min".format((time.time() - s_time) / 60))
        shutil.copyfile(os.path.join(os.getenv("FAIRSHIP"), "diff-model/magnet_params.json"),
                        os.path.join(f_name, "params.json"))


