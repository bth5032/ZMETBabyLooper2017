#!/usr/bin/env python

import argparse, sys, ROOT

basedir="/nfs-7/userdata/bobak/ZMET2017_Hists/SignalCutFlows/"
lumi_fb=35.9
Xsecs = {}

def getXSec(model, mass_point):
  """Loads the proper TChain for the model and loops through events until it finds the mass point given and returns evt_xsec, if the mass point can not be found, it returns None"""
  m_glu=None
  m_lsp=None
  m_chi=None
  global Xsecs

  ch = ROOT.TChain("t")

  if model == "t5zz":
    m_glu = mass_point[0]
    m_lsp = mass_point[1]
    
    if (("%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)) in Xsecs.keys()):
      return Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)]
    
    if m_glu >= 1850:
      ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
    else:
      ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")
  elif model == "tchiwz":
    m_glu = mass_point[0]
    m_lsp = mass_point[1]
    if (("%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)) in Xsecs.keys()):
      return Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/tchiwz_80x_v2*")
  elif model == "tchihz":
    m_chi = mass_point
    if (("%s_mchi%d" % (model, m_chi)) in Xsecs.keys()):
      return Xsecs["%s_mchi%d" % (model, m_chi)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchihz_80x_v2*")
  elif model == "tchizz":
    m_chi = mass_point
    if (("%s_mchi%d" % (model, m_chi)) in Xsecs.keys()):
      return Xsecs["%s_mchi%d" % (model, m_chi)]
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchizz_80x_v2*")

  if m_glu:
    ch.SetBranchStatus("*", 0)
    ch.SetBranchStatus("mass_LSP", 1)
    ch.SetBranchStatus("mass_gluino", 1)
    ch.SetBranchStatus("evt_xsec", 1)

    n_entries = ch.GetEntries()
    for j_entry in range(n_entries):

      i_entry = ch.LoadTree(j_entry)
      if i_entry < 0:
        break
      nb = ch.GetEntry(j_entry)
      if nb <= 0:
        continue


      if ((ch.mass_gluino == m_glu) and (ch.mass_LSP == m_lsp)):
        Xsecs["%s_mglu%d_mlsp%d" % (model, m_glu, m_lsp)] = ch.evt_xsec
        return ch.evt_xsec
  
  elif m_chi:
    ch.SetBranchStatus("*", 0)
    ch.SetBranchStatus("mass_chi", 1)
    ch.SetBranchStatus("evt_xsec", 1)

    n_entries = ch.GetEntries()
    for j_entry in range(n_entries):

      i_entry = ch.LoadTree(j_entry)
      if i_entry < 0:
        break
      nb = ch.GetEntry(j_entry)
      if nb <= 0:
        continue


      if (ch.mass_chi == m_chi):
        Xsecs["%s_mchi%d" % (model, m_chi)] = ch.evt_xsec
        return ch.evt_xsec

  return None

def makeT5ZZCutFlows(m_glu, m_lsp):
  ch = ROOT.TChain("t")
  if m_glu >= 1850:
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
  else:
    ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")

  cuts="((mass_gluino == %f) && (mass_LSP == %f))*evt_scale1fb*%f" % (m_glu, m_lsp, lumi_fb)
  h_baseline_2lep = ROOT.TH1D("h_baseline_2lep", "2 lep baseline", 1,0,10)
  ch.Draw("nlep>>h_baseline_2lep", cuts)
  n_baseline_2lep = h_baseline_2lep.Integral(0,-1)

  """cuts="((lep_pt[0] > 25) && (lep_pt[1] > 20) && %s" % cuts[1:]
  #print(cuts)
  h_baseline_pt = ROOT.TH1D("h_baseline_pt", "lepton pt", 1,0,10)
  ch.Draw("nlep>>h_baseline_pt", cuts)
  n_baseline_pt = h_baseline_pt.Integral(0,-1)

  cuts="(((nisoTrack_mt2+nlep) < 3) && (nveto_leptons < 1) && %s" % cuts[1:]
  #print(cuts)
  h_baseline_lepveto = ROOT.TH1D("h_baseline_lepveto", "extra lepton vetos", 1,0,10)
  ch.Draw("nlep>>h_baseline_lepveto", cuts)
  n_baseline_lepveto = h_baseline_lepveto.Integral(0,-1)"""

  print("\\begin{tabular}{l|c|c}")
  print("\\hline")
  print("\\multicolumn{3}{c}{\\textbf{SRA}} \\\\ \\hline")
  print("T5ZZ model, mass gluino: %.0f GeV, mass LSP: %.0f GeV &  \\multicolumn{2}{c}{Events in %.1f fb$^{-1}$} \\\\ \\hline" % (m_glu, m_lsp, lumi_fb))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("Expected production rate & \multicolumn{2}{c}{%.2f} \\\\" % (n*1000*lumi_fb))
  makeSRATable(m_glu, m_lsp)
 
  print("")

  print("\\begin{tabular}{l|c|c}")
  print("\\hline")
  print("\\multicolumn{3}{c}{\\textbf{SRB}} \\\\ \\hline")
  print("T5ZZ model, mass gluino: %.0f GeV, mass LSP: %.0f GeV &  \\multicolumn{2}{c}{Events in %.1f fb$^{-1}$} \\\\ \\hline" % (m_glu, m_lsp, lumi_fb))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("Expected production rate & \multicolumn{2}{c}{%.2f} \\\\" % (n*1000*lumi_fb))
  makeSRBTable(m_glu, m_lsp)
  
  print("")

  print("\\begin{tabular}{l|c|c}")
  print("\\hline")
  print("\\multicolumn{3}{c}{\\textbf{SRC}} \\\\ \\hline")
  print("T5ZZ model, mass gluino: %.0f GeV, mass LSP: %.0f GeV &  \\multicolumn{2}{c}{Events in %.1f fb$^{-1}$} \\\\ \\hline" % (m_glu, m_lsp, lumi_fb))
  n = getXSec("t5zz", [m_glu, m_lsp])
  print("Expected production rate & \multicolumn{2}{c}{%.2f} \\\\" % (n*1000*lumi_fb))
  makeSRCTable(m_glu, m_lsp)

def makeTChiWZCutFlows(m_glu, m_lsp):
  ch = ROOT.TChain("t")
  ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/tchiwz_80x_v2*")

  cuts="((mass_gluino == %f) && (mass_LSP == %f))*evt_scale1fb*%f" % (m_glu, m_lsp, lumi_fb)
  h_baseline_2lep = ROOT.TH1D("h_baseline_2lep", "2 lep baseline", 1,0,10)
  ch.Draw("nlep>>h_baseline_2lep", cuts)
  n_baseline_2lep = h_baseline_2lep.Integral(0,-1)

  """cuts="((lep_pt[0] > 25) && (lep_pt[1] > 20) && %s" % cuts[1:]
  h_baseline_pt = ROOT.TH1D("h_baseline_pt", "lepton pt", 1,0,10)
  ch.Draw("nlep>>h_baseline_pt", cuts)
  n_baseline_pt = h_baseline_pt.Integral(0,-1)

  cuts="(((nisoTrack_mt2+nlep) < 3) && (nveto_leptons < 1) && %s" % cuts[1:]
  h_baseline_lepveto = ROOT.TH1D("h_baseline_lepveto", "extra lepton vetos", 1,0,10)
  ch.Draw("nlep>>h_baseline_lepveto", cuts)
  n_baseline_lepveto = h_baseline_lepveto.Integral(0,-1)"""

  print("\\begin{tabular}{l|r}")
  print("\\hline")
  print("\\multicolumn{2}{c}{\\textbf{TChiWZ}} \\\\ \\hline")
  print("TChiWZ model, mass gluino: %.0f GeV, mass LSP: %.0f GeV &  Events in %.1f fb$^{-1}$ \\\\ \\hline" % (m_glu, m_lsp, lumi_fb))
  n = getXSec("tchiwz", [m_glu, m_lsp])
  print("Expected production rate & %.2f \\\\" % (n*1000*lumi_fb))
  makeTChiWZTable(m_glu, m_lsp, "tchiwz")
 
  """ 
  print("")

  print("\\begin{tabular}{l|r}")
  print("\\hline")
  print("\\multicolumn{2}{c}{\\textbf{TChiHZ}} \\\\ \\hline")
  print("TChiWZ model, mass gluino: %.0f GeV, mass LSP: %.0f GeV &  Events in %.1f fb$^{-1}$ \\\\ \\hline" % (m_glu, m_lsp, lumi_fb))
  n = getXSec("tchiwz", [m_glu, m_lsp])
  print("All events at mass point & %f \\\\" % (n*1000*lumi_fb))
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with p$_{T} >$ 10 GeV & %f \\\\" % (n_baseline_2lep))
  print("(Sub)Leading lepton p$_{T} > 25(20)$ GeV & %f \\\\" % (n_baseline_pt))
  print("Extra lepton vetos & %f \\\\" % (n_baseline_lepveto))
  makeTChiHZTable(m_glu, m_lsp, "tchiwz")"""
  
def makeTChiZZCutFlows(m_chi):
  ch = ROOT.TChain("t")
  ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchizz_80x_v2*")

  cuts="(mass_chi == %f)*evt_scale1fb*%f" % (m_chi, lumi_fb)
  h_baseline_2lep = ROOT.TH1D("h_baseline_2lep", "2 lep baseline", 1,0,10)
  ch.Draw("nlep>>h_baseline_2lep", cuts)
  n_baseline_2lep = h_baseline_2lep.Integral(0,-1)

  """cuts="((lep_pt[0] > 25) && (lep_pt[1] > 20) && %s" % cuts[1:]
  h_baseline_pt = ROOT.TH1D("h_baseline_pt", "lepton pt", 1,0,10)
  ch.Draw("nlep>>h_baseline_pt", cuts)
  n_baseline_pt = h_baseline_pt.Integral(0,-1)

  cuts="(((nisoTrack_mt2+nlep) < 3) && (nveto_leptons < 1) && %s" % cuts[1:]
  h_baseline_lepveto = ROOT.TH1D("h_baseline_lepveto", "extra lepton vetos", 1,0,10)
  ch.Draw("nlep>>h_baseline_lepveto", cuts)
  n_baseline_lepveto = h_baseline_lepveto.Integral(0,-1)"""

  print("\\begin{tabular}{l|r}")
  print("\\hline")
  print("\\multicolumn{2}{c}{\\textbf{TChiWZ}} \\\\ \\hline")
  print("TChiZZ model, mass chi: %.0f GeV &  Events in %.1f fb$^{-1}$ \\\\ \\hline" % (m_chi, lumi_fb))
  n = getXSec("tchizz", m_chi)
  print("Expected production rate & %.2f \\\\" % (n*1000*lumi_fb))
  makeTChiWZTable(-1,-1,"tchizz", m_chi)
  
  print("\\begin{tabular}{l|r}")
  print("\\hline")
  print("\\multicolumn{2}{c}{\\textbf{TChiHZ}} \\\\ \\hline")
  print("TChiZZ model, mass chi: %.0f GeV &  Events in %.1f fb$^{-1}$ \\\\ \\hline" % (m_chi, lumi_fb))
  n = getXSec("tchizz", m_chi)
  print("Expected production rate & %.2f \\\\" % (n*1000*lumi_fb))
  makeTChiHZTable(-1,-1,"tchizz", m_chi)

def makeTChiHZCutFlows(m_chi):
  ch = ROOT.TChain("t")
  ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchihz_80x_v2*")

  cuts="(mass_chi == %f)*evt_scale1fb*%f" % (m_chi, lumi_fb)
  h_baseline_2lep = ROOT.TH1D("h_baseline_2lep", "2 lep baseline", 1,0,10)
  ch.Draw("nlep>>h_baseline_2lep", cuts)
  n_baseline_2lep = h_baseline_2lep.Integral(0,-1)

  """cuts="((lep_pt[0] > 25) && (lep_pt[1] > 20) && %s" % cuts[1:]
  h_baseline_pt = ROOT.TH1D("h_baseline_pt", "lepton pt", 1,0,10)
  ch.Draw("nlep>>h_baseline_pt", cuts)
  n_baseline_pt = h_baseline_pt.Integral(0,-1)

  cuts="(((nisoTrack_mt2+nlep) < 3) && (nveto_leptons < 1) && %s" % cuts[1:]
  h_baseline_lepveto = ROOT.TH1D("h_baseline_lepveto", "extra lepton vetos", 1,0,10)
  ch.Draw("nlep>>h_baseline_lepveto", cuts)
  n_baseline_lepveto = h_baseline_lepveto.Integral(0,-1)"""

  print("\\begin{tabular}{l|r}")
  print("\\hline")
  print("\\multicolumn{2}{c}{\\textbf{TChiHZ}} \\\\ \\hline")
  print("TChiHZ model, mass chi: %.0f GeV &  Events in %.1f fb$^{-1}$ \\\\ \\hline" % (m_chi, lumi_fb))
  n = getXSec("tchihz", m_chi)
  print("Expected production rate & %.2f \\\\" % (n*1000*lumi_fb))
  makeTChiHZTable(-1,-1,"tchihz", m_chi)

def makeSRATable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRA/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2pluslep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2pluslep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with (sub)leading p$_{T} > 25 (20)$ GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Extra lepton vetos & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window (86,96) GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2-3 Jets & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\Delta \Phi$ between MET and two highest p$_{T}$ jets $> 0.4$ rad & \multicolumn{2}{c}{%.2f} \\\\ \\hline" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{Btag requirement}} & \\textbf{B Veto} & $\\boldmath{\geq 1}$ \\textbf{Btag} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  # ==========================
  # MT2 Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{MT2} $>$} & \\textbf{80 GeV} & \\textbf{100 GeV} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  # ==========================
  # HT and MET Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{H$_{T} >$}} & \\textbf{500 GeV} & \\textbf{200 GeV} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  bveto_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  print("$E^{miss}_{T} > 100$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[1], btag_met_counts[1]))
  print("$E^{miss}_{T} > 250$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[2], btag_met_counts[2]))
  print("\end{tabular}")

def makeSRBTable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRB/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2pluslep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2pluslep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with (sub)leading p$_{T} > 25 (20)$ GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Extra lepton vetos & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window (86,96) GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("4-5 Jets & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\Delta \Phi$ between MET and two highest p$_{T}$ jets $> 0.4$ rad & \multicolumn{2}{c}{%.2f} \\\\ \\hline" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{Btag requirement}} & \\textbf{B Veto} & $\\boldmath{\geq 1}$ \\textbf{Btag} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  # ==========================
  # MT2 Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{MT2} $>$} & \\textbf{80 GeV} & \\textbf{100 GeV} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  # ==========================
  # HT and MET Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{H$_{T} >$}} & \\textbf{500 GeV} & \\textbf{200 GeV} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  bveto_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  print("$E^{miss}_{T} > 100$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[1], btag_met_counts[1]))
  print("$E^{miss}_{T} > 250$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[2], btag_met_counts[2]))
  print("\end{tabular}")

def makeSRCTable(m_glu, m_lsp):
  hists_path = basedir+"T5ZZ/SRC/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  
  hp = hists_path+"2pluslep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2pluslep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with (sub)leading p$_{T} > 25 (20)$ GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Extra lepton vetos & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window (86,96) GeV & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("6+ Jets & \multicolumn{2}{c}{%.2f} \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\Delta \Phi$ between MET and two highest p$_{T}$ jets $> 0.4$ rad & \multicolumn{2}{c}{%.2f} \\\\ \hline" %n)

  # ==========================
  # Btagging Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{Btag requirement}} & \\textbf{B Veto} & $\\boldmath{\geq 1}$ \\textbf{Btag} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n_btag=h_met.Integral(1,6001)
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
  n_bveto=h_met.Integral(1,6001)
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))

  # ==========================
  # MT2 and MET Start
  # ==========================
  print("\multicolumn{1}{c|}{\\textbf{MT2} $>$} & \\textbf{80 GeV} & \\textbf{100 GeV} \\\\ \cline{2-3}")
  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n_btag=h_met.Integral(1,6001)
  btag_met_counts=[]
  btag_met_counts.append(h_met.Integral(100,6001))
  btag_met_counts.append(h_met.Integral(150,6001))
  btag_met_counts.append(h_met.Integral(250,6001))
  f_met.Close()

  hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
  n_bveto=h_met.Integral(1,6001)
  bveto_met_counts=[]
  bveto_met_counts.append(h_met.Integral(100,6001))
  bveto_met_counts.append(h_met.Integral(150,6001))
  f_met.Close()
  print(" & %.2f & %.2f \\\\ \hline" % (n_bveto, n_btag))


  print("$E^{miss}_{T} > 100$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[0], btag_met_counts[0]))
  print("$E^{miss}_{T} > 150$ GeV & %.2f & %.2f \\\\" % (bveto_met_counts[1], btag_met_counts[1]))
  print("\end{tabular}")

def makeTChiHZTable(m_glu, m_lsp, model, m_chi=None):
  if model == "tchiwz":
    hists_path = basedir+"TChiWZ/TChiHZ/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  elif model == "tchihz":
    hists_path = basedir+"TChiHZ/TChiHZ/mchi%d_" % (m_chi)
  elif model == "tchizz":
    hists_path = basedir+"TChiZZ/TChiHZ/mchi%d_" % (m_chi)
  else:
    print ("No proper model given for TChiHZ table. Got: %s" % model)
    return None

  hp = hists_path+"2pluslep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2pluslep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with (sub)leading p$_{T} > 25 (20)$ GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Extra lepton vetos & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window (86,96) GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2+ Jets & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\Delta \Phi$ between MET and two highest p$_{T}$ jets $> 0.4$ rad & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Exactly 2 btags & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2b.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2b")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("MT2b $>$ 200 GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2b_Mbb.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2b_Mbb")
  n=h_met.Integral(1,6001)
  met_counts=[]
  met_counts.append(h_met.Integral(100,6001))
  met_counts.append(h_met.Integral(150,6001))
  met_counts.append(h_met.Integral(250,6001))
  f_met.Close()
  print("M$_{bb} <$  150 GeV & %.2f \\\\ \hline" %n)

  print("$E^{miss}_{T} > 100$ GeV & %.2f \\\\" % met_counts[0])
  print("$E^{miss}_{T} > 150$ GeV & %.2f \\\\" % met_counts[1])
  print("$E^{miss}_{T} > 250$ GeV & %.2f \\\\" % met_counts[2])
  print("\end{tabular}")

def makeTChiWZTable(m_glu, m_lsp, model, m_chi=None):
  if model == "tchiwz":
    hists_path = basedir+"TChiWZ/TChiWZ/mglu%d_mlsp%d_" % (m_glu, m_lsp)
  elif model == "tchizz":
    hists_path = basedir+"TChiZZ/TChiWZ/mchi%d_" % (m_chi)
  else:
    print ("No proper model given for TChiWZ table. Got: %s" % model)
    return None

  hp = hists_path+"2pluslep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2pluslep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\geq$ 2 Leptons (e$^{\pm}$ e$^{\mp}$ or $\mu^{\pm}\mu^{\mp}$) with (sub)leading p$_{T} > 25 (20)$ GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep.root"
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Extra lepton vetos & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("Dilepton mass $\in$ Z mass window (86,96) GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("2-3 Jets & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("$\Delta \Phi$ between MET and two highest p$_{T}$ jets $> 0.4$ rad & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("No btags & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
  n=h_met.Integral(1,6001)
  f_met.Close()
  print("MT2 $>$ 80 GeV & %.2f \\\\" %n)

  hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_Mjj.root" 
  f_met = ROOT.TFile(hp, 'r')
  h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_Mjj")
  n=h_met.Integral(1,6001)
  met_counts=[]
  met_counts.append(h_met.Integral(100,6001))
  met_counts.append(h_met.Integral(150,6001))
  met_counts.append(h_met.Integral(250,6001))
  met_counts.append(h_met.Integral(350,6001))
  f_met.Close()
  print("M$_{jj}$ for min $\Delta \Phi$ jets $<$ 150 GeV & %.2f \\\\ \hline" %n)

  print("$E^{miss}_{T} > 100$ GeV & %.2f \\\\" % met_counts[0])
  print("$E^{miss}_{T} > 150$ GeV & %.2f \\\\" % met_counts[1])
  print("$E^{miss}_{T} > 250$ GeV & %.2f \\\\" % met_counts[2])
  print("$E^{miss}_{T} > 350$ GeV & %.2f \\\\" % met_counts[3])
  print("\end{tabular}")

if __name__ == "__main__":
  #makeT5ZZCutFlows(1400,700)
  #makeTChiWZCutFlows(550,200)
  #makeTChiZZCutFlows(350)
  #makeTChiHZCutFlows(350)
  parser = argparse.ArgumentParser()

  parser.add_argument("--t5zz", help="print cutflow table for T5ZZ sample (can specify mass_gluino and mass_lsp)", action="store_true")
  parser.add_argument("--tchiwz", help="print cutflow table for TChiWZ sample (can specify mass_gluino and mass_lsp)", action="store_true")
  parser.add_argument("--tchizz", help="print cutflow table for TChiZZ sample (can specify mass_chi)", action="store_true")
  parser.add_argument("--tchihz", help="print cutflow table for TChiHZ sample (can specify mass_chi)", action="store_true")
  parser.add_argument("--all", help="print latex cutflow table for all samples (uses default masses)", action="store_true")
  parser.add_argument("--mass_chi", help="choose chi mass for TChiHZ or TChiZZ sample, default is 350 for both TChiHZ and TChiZZ", type=int)
  parser.add_argument("--mass_gluino", help="choose gluino mass for T5ZZ or TChiWZ sample, defualt is 1400 for T5ZZ and 550 for TChiWZ", type=int)
  parser.add_argument("--mass_lsp", help="choose chi mass for T5ZZ or TChiWZ sample, defualt is 700 for T5ZZ and 200 for TChiWZ", type=int)
  
  args=parser.parse_args()

  if (args.t5zz):
    if args.mass_gluino and args.mass_lsp:
      makeT5ZZCutFlows(args.mass_gluino, args.mass_lsp)
    else:
      makeT5ZZCutFlows(1400,700)
  elif (args.tchiwz):
    if args.mass_gluino and args.mass_lsp:
      makeTChiWZCutFlows(args.mass_gluino, args.mass_lsp)
    else:
      makeTChiWZCutFlows(550,200)
  elif (args.tchizz):
    if args.mass_chi:
      makeTChiZZCutFlows(args.mass_chi)
    else:
      makeTChiZZCutFlows(350)
  elif (args.tchihz):
    if args.mass_chi:
      makeTChiHZCutFlows(args.mass_chi)
    else:
      makeTChiHZCutFlows(350)
  elif (args.all):
    print("\subsection*{T5ZZ}")
    print("\\begin{table}[ht!]")
    print("\\begin{center}")
    makeT5ZZCutFlows(1400,700)
    print("\end{center}")
    print("\end{table}")
    print("\clearpage")
    
    print("\subsection*{TChiWZ}")
    print("\\begin{table}[ht!]")
    print("\\begin{center}")
    makeTChiWZCutFlows(550,200)
    print("\end{center}")
    print("\end{table}")

    print("\subsection*{TChiZZ}")
    print("\\begin{table}[ht!]")
    print("\\begin{center}")
    makeTChiZZCutFlows(350)
    print("\end{center}")
    print("\end{table}")
    print("\clearpage")

    print("\subsection*{TChiHZ}")
    print("\\begin{table}[ht!]")
    print("\\begin{center}")
    makeTChiHZCutFlows(350)
    print("\end{center}")
    print("\end{table}")
    print("\clearpage")

  else:
    parser.print_help()
    exit()

