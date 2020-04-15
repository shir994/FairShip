#!/usr/bin/env python2
from __future__ import division
import argparse
import numpy as np
import ROOT as r
# Fix https://root-forum.cern.ch/t/pyroot-hijacks-help/15207 :
r.PyConfig.IgnoreCommandLineOptions = True
import shipunit as u
import rootUtils as ut
import logger as log

BANNED_SET = set([12, -12, 14, -14, 16, -16])

def book_histograms(maxp, maxpt):
    h = {}
    # Define histograms
    for nplane in range(0, 23):
        ut.bookHist(h, 'NuTauMu_all_{}'.format(nplane),
                    'Rpc_{};x[cm];y[cm]'.format(
                        nplane), 100, -300, +300, 100, -300,
                    300)
        ut.bookHist(h, 'NuTauMu_mu_{}'.format(nplane),
                    'Rpc_{};x[cm];y[cm]'.format(
                        nplane), 100, -300, +300, 100, -300,
                    300)
    for suffix, title in [('mu', '#mu#pm hits'), ('all', 'All hits')]:
        ut.bookHist(h, 'muon_tiles_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 200, -1000, +1000, 90,
                    -900, 900)
        ut.bookHist(h, 'muon_bars_x_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 2, -300, +300, 240, -600,
                    600)
        ut.bookHist(h, 'muon_bars_y_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 120, -300, +300, 4, -600,
                    600)
        ut.bookHist(h, 'timing_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 3, -252, +252, 167, -501,
                    501)
        ut.bookHist(h, 'TargetTracker_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 120, -60, 60, 120, -60,
                    60)
        ut.bookHist(h, 'TargetTracker_yvsz_{}'.format(suffix),
                    '{};z[cm];y[cm]'.format(
                        title), 400, -3300, -2900, 120, -60,
                    60)
        ut.bookHist(h, 'TargetTracker_xvsz_{}'.format(suffix),
                    '{};z[cm];x[cm]'.format(
                        title), 400, -3300, -2900, 120, -60,
                    60)
        ut.bookHist(h, 'NuTauMu_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 100, -300, +300, 100, -300,
                    300)
        ut.bookHist(h, 'NuTauMu_yvsz_{}'.format(suffix),
                    '{};z[cm];y[cm]'.format(
                        title), 200, -2680, -2480, 600, -300,
                    300)
        ut.bookHist(h, 'NuTauMu_xvsz_{}'.format(suffix),
                    '{};z[cm];x[cm]'.format(
                        title), 200, -2680, -2480, 600, -300,
                    300)
        ut.bookHist(h, 'ECAL_TP_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 167, -501, +501, 334,
                    -1002, 1002)
        ut.bookHist(h, 'ECAL_Alt_{}'.format(suffix),
                    '{};x[cm];y[cm]'.format(title), 50, -500, +500, 100, -1000,
                    1000)
        ut.bookHist(h, 'SBT_Liquid_{}'.format(suffix),
                    '{};z[cm];#phi'.format(title), 100, -3000, +3000, 100,
                    -r.TMath.Pi(), r.TMath.Pi())
        ut.bookHist(h, 'SBT_Plastic_{}'.format(suffix),
                    '{};z[cm];#phi'.format(title), 100, -3000, +3000, 100,
                    -r.TMath.Pi(), r.TMath.Pi())
        ut.bookHist(h, 'SHIELD_veto_{}'.format(suffix),
                    '{};z[cm];#phi'.format(title), 100, -7100, -3200, 100,
                    -r.TMath.Pi(), r.TMath.Pi())

        for station in range(1, 5):
            ut.bookHist(h, 'T{}_{}'.format(station, suffix),
                        '{};x[cm];y[cm]'.format(title), 10, -250, +250, 20,
                        -500, 500)

    ut.bookHist(h, 'NuTauMu_mu_p', '#mu#pm;p[GeV];', 100, 0, maxp)
    ut.bookHist(h, 'NuTauMu_mu_pt', '#mu#pm;p_t[GeV];', 100, 0,
                maxpt)
    ut.bookHist(h, 'NuTauMu_mu_ppt', '#mu#pm;p[GeV];p_t[GeV];',
                100, 0, maxp, 100, 0, maxpt)
    ut.bookHist(h, 'NuTauMu_all_p', '#mu#pm;p[GeV];', 100, 0, maxp)
    ut.bookHist(h, 'NuTauMu_all_pt', '#mu#pm;p_t[GeV];', 100, 0,
                maxpt)
    ut.bookHist(h, 'NuTauMu_all_ppt', '#mu#pm;p[GeV];p_t[GeV];',
                100, 0, maxp, 100, 0, maxpt)

    for suffix in ['', '_original']:
        ut.bookHist(h, 'mu_p{}'.format(suffix), '#mu#pm;p[GeV];', 100, 0, maxp)
        ut.bookHist(h, 'mu_pt{}'.format(suffix), '#mu#pm;p_t[GeV];', 100, 0,
                    maxpt)
        ut.bookHist(h, 'mu_ppt{}'.format(suffix), '#mu#pm;p[GeV];p_t[GeV];',
                    100, 0, maxp, 100, 0, maxpt)
    ut.bookHist(h, 'ECAL_TP_e', 'e#pm with E#geq 250 MeV;x[cm];y[cm]', 167,
                -501, +501, 334, -1002, 1002)
    ut.bookHist(h, 'ECAL_Alt_e', 'e#pm with E#geq 250 MeV;x[cm];y[cm]', 50,
                -500, +500, 100, -1000, 1000)
    ut.bookHist(h, 'ECAL_TP_gamma', '#gamma;x[cm];y[cm]', 167, -501, +501, 334,
                -1002, 1002)
    ut.bookHist(h, 'ECAL_Alt_gamma', '#gamma;x[cm];y[cm]', 50, -500, +500, 100,
                -1000, 1000)
    ut.bookHist(h, 'ECAL_e_E', 'e#pm;E[GeV/c^{2}];', 100, 0, 1)
    ut.bookHist(h, 'ECAL_gamma_E', '#gamma;E[GeV/c^{2}];', 100, 0, 1)
    return h

def extract_kinematics(event, hit):
    if hit and hit.GetEnergyLoss() > 0:
        kinematics = {}
        kinematics["trid"] = hit.GetTrackID()
        assert kinematics["trid"] > 0
        kinematics["weight"] = event.MCTrack[kinematics["trid"]].GetWeight()
        kinematics["x"] = hit.GetX()
        kinematics["y"] = hit.GetY()
        kinematics["z"] = hit.GetZ()
        kinematics["pt"] = np.hypot(hit.GetPx(), hit.GetPy())
        kinematics["P"] = np.hypot(hit.GetPz(), kinematics["pt"])
        kinematics["pid"] = hit.PdgCode()
        assert kinematics["pid"] not in BANNED_SET
        return kinematics
    else:
        return None

def fill_muon_histograms(h, muon_ids, kinematics):
    muon_ids.add(kinematics["trid"])
    h['mu_p'].Fill(kinematics["P"], kinematics['weight'])
    h['mu_pt'].Fill(kinematics["pt"], kinematics['weight'])
    h['mu_ppt'].Fill(kinematics["P"], kinematics["pt"], kinematics['weight'])

def main():
    parser = argparse.ArgumentParser(description='Script to create flux maps.')
    parser.add_argument(
        '-f',
        '--inputfile',
        help='''Simulation results to use as input. '''
        '''Supports retrieving files from EOS via the XRootD protocol.''')
    parser.add_argument(
        '-o',
        '--outputfile',
        default='flux_map.root',
        help='''File to write the flux maps to. '''
        '''Will be recreated if it already exists.''')
    args = parser.parse_args()
    f = r.TFile.Open(args.outputfile, 'recreate')
    f.cd()
    maxpt = 10. * u.GeV
    maxp = 360. * u.GeV
    h = book_histograms(maxp, maxpt)

    ch = r.TChain('cbmsim')
    ch.Add(args.inputfile)
    n = ch.GetEntries()
    log.info(n)
    
    for index, event in enumerate(ch):
        if index % 10000 == 0:
            log.info('{}/{}'.format(index, n))
        muon_ids = set()

        for hit in event.strawtubesPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            station = hit.GetDetectorID() // 10000000
            h['T{}_all'.format(station)].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            if abs(kinematics['pid']) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                h['T{}_mu'.format(station)].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])

        for hit in event.EcalPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            h['ECAL_TP_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            h['ECAL_Alt_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            if abs(kinematics["pid"]) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                h['ECAL_TP_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['ECAL_Alt_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            elif abs(kinematics["pid"]) == 11:
                Esq = px ** 2 + py ** 2 + pz ** 2 + 0.000511 ** 2
                h['ECAL_e_E'].Fill(np.sqrt(Esq), kinematics['weight'])
                h['ECAL_TP_e'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['ECAL_Alt_e'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            elif abs(kinematics["pid"]) == 22:
                Esq = px ** 2 + py ** 2 + pz ** 2
                h['ECAL_gamma_E'].Fill(np.sqrt(Esq), kinematics['weight'])
                h['ECAL_TP_gamma'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['ECAL_Alt_gamma'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])

        for hit in event.TTPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            if hit.GetDetectorID() == 0:
                h['TargetTracker_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            h['TargetTracker_xvsz_all'].Fill(kinematics['z'], kinematics['x'], kinematics['weight'])
            h['TargetTracker_yvsz_all'].Fill(kinematics['z'], kinematics['y'], kinematics['weight'])
            if abs(kinematics["pid"]) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                if hit.GetDetectorID() == 0:
                    h['TargetTracker_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['TargetTracker_xvsz_mu'].Fill(kinematics['z'], kinematics['x'], kinematics['weight'])
                h['TargetTracker_yvsz_mu'].Fill(kinematics['z'], kinematics['y'], kinematics['weight'])

        for hit in event.ShipRpcPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            nplane = hit.GetDetectorID() - 10000
            h['NuTauMu_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            if nplane >= 0:
                h['NuTauMu_all_{}'.format(nplane)].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            h['NuTauMu_xvsz_all'].Fill(kinematics['z'], kinematics['x'], kinematics['weight'])
            h['NuTauMu_yvsz_all'].Fill(kinematics['z'], kinematics['y'], kinematics['weight'])
            if hit.GetDetectorID() == 10000:
                h['NuTauMu_all_p'].Fill(kinematics["P"], kinematics['weight'])
                h['NuTauMu_all_pt'].Fill(kinematics["pt"], kinematics['weight'])
                h['NuTauMu_all_ppt'].Fill(kinematics["P"], kinematics["pt"], kinematics['weight'])
            if abs(kinematics["pid"]) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                h['NuTauMu_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                if nplane >= 0:
                    # fill the histogram corresponding to nplane
                    h['NuTauMu_mu_{}'.format(nplane)].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                if hit.GetDetectorID() == 10000:
                    h['NuTauMu_mu_p'].Fill(kinematics["P"], kinematics['weight'])
                    h['NuTauMu_mu_pt'].Fill(kinematics["pt"], kinematics['weight'])
                    h['NuTauMu_mu_ppt'].Fill(kinematics["P"], kinematics["pt"], kinematics['weight'])
                h['NuTauMu_xvsz_mu'].Fill(kinematics['z'], kinematics['x'], kinematics['weight'])
                h['NuTauMu_yvsz_mu'].Fill(kinematics['z'], kinematics['y'], kinematics['weight'])

        for hit in event.TimeDetPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            h['timing_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            if abs(kinematics["pid"]) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                h['timing_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])

        for hit in event.muonPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            h['muon_tiles_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            h['muon_bars_x_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            h['muon_bars_y_all'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
            if abs(kinematics["pid"]) == 13:
                fill_muon_histograms(h, muon_ids, kinematics)
                h['muon_tiles_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['muon_bars_y_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])
                h['muon_bars_x_mu'].Fill(kinematics['x'], kinematics['y'], kinematics['weight'])

        for hit in event.vetoPoint:
            kinematics = extract_kinematics(event, hit)
            if not kinematics:
                continue
            phi = r.TMath.ATan2(kinematics['y'], kinematics['x'])
            if 99999 < hit.GetDetectorID() < 999999:
                h['SBT_Liquid_all'].Fill(kinematics['z'], phi, kinematics['weight'])
                if abs(kinematics["pid"]) == 13:
                    fill_muon_histograms(h, muon_ids, kinematics)
                    h['SBT_Liquid_mu'].Fill(kinematics['z'], phi, kinematics['weight'])
            elif hit.GetDetectorID() > 999999:
                h['SBT_Plastic_all'].Fill(kinematics['z'], phi, kinematics['weight'])
                if abs(kinematics["pid"]) == 13:
                    fill_muon_histograms(h, muon_ids, kinematics)
                    h['SBT_Plastic_mu'].Fill(kinematics['z'], phi, kinematics['weight'])
            else:
                h['SHIELD_veto_all'].Fill(kinematics['z'], phi, kinematics['weight'])
                if abs(kinematics["pid"]) == 13:
                    fill_muon_histograms(h, muon_ids, kinematics)
                    h['SHIELD_veto_mu'].Fill(kinematics['z'], phi, kinematics['weight'])

                #log.warn('Unidentified vetoPoint.')

        for muonid in muon_ids:
            original_muon = event.MCTrack[muonid]
            weight = original_muon.GetWeight()
            h['mu_p_original'].Fill(original_muon.GetP(), weight)
            h['mu_pt_original'].Fill(original_muon.GetPt(), weight)
            h['mu_ppt_original'].Fill(original_muon.GetP(),
                                      original_muon.GetPt(), weight)
            # NOTE: muons are counted several times if they create several hits
            #       But the original muon is only counted once.
    log.info('Event loop done')

    for key in h:
        classname = h[key].Class().GetName()
        if 'TH' in classname or 'TP' in classname:
            h[key].Write()
    f.Close()


if __name__ == '__main__':
    r.gErrorIgnoreLevel = r.kWarning
    r.gROOT.SetBatch(True)
    main()
