import json
import os
import subprocess
import shutil


magnet_params = {"shape": {'X_begin': 0.5, "X_end": 2., 'Y_begin':1., "Y_end": 3., 'Z': 5.},
                 "field": {'X': 0., 'Y': 4, 'Z':0}}

dirname = os.path.dirname(__file__)
with open(os.path.join(dirname, "magnet_params.json"), 'w') as f:
    json.dump(magnet_params, f)

# for sign in ["", "-"]:
#     f_name = "test"
#     f_name += "_plus" if sign == "" else "_minus"
#     print(sign == "")
#     command = "python $FAIRSHIP/macro/run_simScript.py --PG --pID {}13 -n 1000 --Estart 4 --Eend 10 --FastMuon -o {}".format(sign, f_name)
#     process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
#     for line in process.stdout:
#         print(line)
#     process.wait()
#     shutil.copyfile(os.path.join(dirname, "magnet_params.json"), os.path.join(f_name, "params.json"))

