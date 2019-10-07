#!/usr/bin/python

import ROOT
import os
import argparse
import numpy as np
from collections import defaultdict
import math
import sys


def process_file(DATA_DIR, input_file):
    file = ROOT.TFile(os.path.join(DATA_DIR, input_file))


    tree = file.Get("cbmsim")
    print("Total events:{}".format(tree.GetEntries()))


    MUON = 13
    muons = []
    veto_points = []

    for index, event in enumerate(tree):
        if index % 50000 == 0:
            print(index)
        muon = []
        for hit in event.MCTrack:
            if abs(hit.GetPdgCode()) == MUON and hit.GetMotherId() == -1:
                muon.append([
                    hit.GetPx(),
                    hit.GetPy(),
                    hit.GetPz(),
                    hit.GetPdgCode()
                ])
                break

        muon_veto_points = []
        for hit in event.vetoPoint:
            if hit.GetTrackID() == 0 and\
                -13001 <= hit.GetZ() <= -12998:
                # Middle or inital stats??
                pos_begin = ROOT.TVector3()
                hit.Position(pos_begin)
                mom = ROOT.TVector3()
                hit.Momentum(mom)
                muon_veto_points.append([pos_begin.X(), pos_begin.Y(),
                                         pos_begin.Z(), mom.Mag()])
        if len(muon_veto_points) > 1:
            continue
        elif len(muon_veto_points) == 0:
            muon_veto_points = [[0, 0, 0, 0]]
        muons.extend(muon)
        veto_points.extend(muon_veto_points)

    np.save(os.path.join(DATA_DIR, "muons_mom"), np.array(muons))
    np.save(os.path.join(DATA_DIR, "veto_points"), np.array(veto_points))

if __name__ == "__main__":
    for folder in os.listdir(sys.argv[1]):
        if "Untitled.ipynb" not in folder:
            filename = [file for file in os.listdir(os.path.join(sys.argv[1], folder)) if "ship.conical.PG" in file][0]
            print(folder, filename)
            process_file(os.path.join(sys.argv[1],folder), filename)
