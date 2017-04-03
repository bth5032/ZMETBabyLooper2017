#!/usr/bin/env python

import argparse, sys, ROOT

basedir="/nfs-7/userdata/bobak/ZMET2017_Hists/SignalCutFlows/"
lumi_fb=35.9

def getXSec(model, mass_point):
	"""Loads the proper TChain for the model and loops through events until it finds the mass point given and returns evt_xsec, if the mass point can not be found, it returns None"""
	m_glu=None
	m_lsp=None
	m_chi=None

	ch = ROOT.TChain("t")

	if model == "t5zz":
		m_glu = mass_point[0]
		m_lsp = mass_point[1]
		if m_glu >= 1850:
			ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
		else:
			ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")
	elif model == "tchiwz":
		m_glu = mass_point[0]
		m_lsp = mass_point[1]
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/tchiwz_80x_v2*")
	elif model == "tchihz":
		m_chi = mass_point
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-17/skims/tchihz_80x_v2*")
	elif model == "tchizz":
		m_chi = mass_point
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
			  return ch.evt_xsec

	return None

def makeT5ZZCutFlows(m_glu, m_lsp):
	
	if m_glu >= 1850:
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
	else:
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")

	cuts="evt_scale1fb*((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)
	#cuts="((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)

	print("SRA")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRATable(m_glu, m_lsp)
	
	print("SRB")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRBTable(m_glu, m_lsp)
	
	print("SRC")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRCTable(m_glu, m_lsp)

def makeTChiWZCutFlows(m_glu, m_lsp):
	ch = ROOT.TChain("t")
	ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/tchiwz_80x_v2*")

	cuts="evt_scale1fb*((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)
	#cuts="((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)

	print("TChiWZ")
	print("TChiWZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeTChiWZTable(ch, cuts)
	
	print("TChiHZ")
	print("TChiWZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeTChiHZTable(ch, cuts)
	
def makeSRATable(mass_point):
	hists_path = basedir+"T5ZZ/SRA/mglu%d_mlsp_%d_" % (m_glu, m_lsp)
	
	n = getXSec([m_glu, m_lsp])
	print("All Entries || %f" % (n*1000*lumi_fb))
	
	hp = hists_path+"2lep.root"
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	hp = hists_path+"2lep_dilmass.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("2-3 Jets|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets_dphi.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	# ==========================
	# Btagging Start
	# ==========================
	print("Btag requirement || B Veto || >= 1 Btag")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
	n_btag=h_met.Integral(1,6001)
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
	n_bveto=h_met.Integral(1,6001)
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	# ==========================
	# MT2 Start
	# ==========================
	print("MT2 > || 80 GeV || 100 GeV")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
	n_btag=h_met.Integral(1,6001)
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
	n_bveto=h_met.Integral(1,6001)
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	# ==========================
	# HT and MET Start
	# ==========================
	print("$H_{T}$ > || 500 GeV || 200 GeV")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
	n_btag=h_met.Integral(1,6001)
	btag_met_counts=[]
	btag_met_counts.append(h_met.Integral(100,6001))
	btag_met_counts.append(h_met.Integral(150,6001))
	btag_met_counts.append(h_met.Integral(250,6001))
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
	n_bveto=h_met.Integral(1,6001)
	bveto_met_counts=[]
	bveto_met_counts.append(h_met.Integral(100,6001))
	bveto_met_counts.append(h_met.Integral(150,6001))
	bveto_met_counts.append(h_met.Integral(250,6001))
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeSRBTable(ch, cuts):
	hists_path = basedir+"T5ZZ/SRB/mglu%d_mlsp_%d_" % (m_glu, m_lsp)
	
	n = getXSec([m_glu, m_lsp])
	print("All Entries || %f" % (n*1000*lumi_fb))
	
	hp = hists_path+"2lep.root"
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	hp = hists_path+"2lep_dilmass.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("4-5 Jets|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets_dphi.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	# ==========================
	# Btagging Start
	# ==========================
	print("Btag requirement || B Veto || >= 1 Btag")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
	n_btag=h_met.Integral(1,6001)
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
	n_bveto=h_met.Integral(1,6001)
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	# ==========================
	# MT2 Start
	# ==========================
	print("MT2 > || 80 GeV || 100 GeV")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
	n_btag=h_met.Integral(1,6001)
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
	n_bveto=h_met.Integral(1,6001)
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	# ==========================
	# HT and MET Start
	# ==========================
	print("$H_{T}$ > || 500 GeV || 200 GeV")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2_ht.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2_ht")
	n_btag=h_met.Integral(1,6001)
	btag_met_counts=[]
	btag_met_counts.append(h_met.Integral(100,6001))
	btag_met_counts.append(h_met.Integral(150,6001))
	btag_met_counts.append(h_met.Integral(250,6001))
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2_ht.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2_ht")
	n_bveto=h_met.Integral(1,6001)
	bveto_met_counts=[]
	bveto_met_counts.append(h_met.Integral(100,6001))
	bveto_met_counts.append(h_met.Integral(150,6001))
	bveto_met_counts.append(h_met.Integral(250,6001))
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeSRCTable(ch, cuts):
	hists_path = basedir+"T5ZZ/SRC/mglu%d_mlsp_%d_" % (m_glu, m_lsp)
	
	n = getXSec([m_glu, m_lsp])
	print("All Entries || %f" % (n*1000*lumi_fb))
	
	hp = hists_path+"2lep.root"
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	hp = hists_path+"2lep_dilmass.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("6+ Jets|| %f" %n)

	hp = hists_path+"2lep_dilmass_njets_dphi.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi")
	n=h_met.Integral(1,6001)
	f_met.close()
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	# ==========================
	# Btagging Start
	# ==========================
	print("Btag requirement || B Veto || >= 1 Btag")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag")
	n_btag=h_met.Integral(1,6001)
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto")
	n_bveto=h_met.Integral(1,6001)
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))

	# ==========================
	# MT2 and MET Start
	# ==========================
	print("MT2 > || 80 GeV || 100 GeV")
	hp = hists_path+"2lep_dilmass_njets_dphi_btag_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_btag_MT2")
	n_btag=h_met.Integral(1,6001)
	btag_met_counts=[]
	btag_met_counts.append(h_met.Integral(100,6001))
	btag_met_counts.append(h_met.Integral(150,6001))
	btag_met_counts.append(h_met.Integral(250,6001))
	f_met.close()

	hp = hists_path+"2lep_dilmass_njets_dphi_bveto_MT2.root" 
	f_met = ROOT.TFile(hp, 'r')
	h_met = f_met.Get("type1MET").Clone("met_2lep_dilmass_njets_dphi_bveto_MT2")
	n_bveto=h_met.Integral(1,6001)
	bveto_met_counts=[]
	bveto_met_counts.append(h_met.Integral(100,6001))
	bveto_met_counts.append(h_met.Integral(150,6001))
	bveto_met_counts.append(h_met.Integral(250,6001))
	f_met.close()
	print(" || %f || %f" % (n_bveto, n_btag))


	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))

def makeTChiHZTable(ch, cuts):
	n=ch.GetEntries(cuts)
	print("All Entries || %f" %n)
	
	cuts+="&& (nlep >= 2 && (nisoTrack_mt2+nlep <= 3) && (lep_pt[0] > 25) && (lep_pt[1] > 20))"
	n=ch.GetEntries(cuts)
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	cuts+=" && ((dilmass > 86) && (dilmass < 96))"
	n=ch.GetEntries(cuts)
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	cuts+=" && (njets >= 2)"
	n=ch.GetEntries(cuts)
	print("2+ Jets|| %f" %n)

	cuts+=" && (nBJetMedium == 2)"
	n=ch.GetEntries(cuts)
	print("==2 Btags|| %f" %n)

	cuts+=" && (mt2b >= 200)"
	n=ch.GetEntries(cuts)
	print("MT2b > 200 GeV|| %f" %n)

	cuts+=" && (mbb_csv < 150)"
	n=ch.GetEntries(cuts)
	print("M_{bb} < 150 GeV|| %f" %n)

	cuts+=" && ((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))"
	n=ch.GetEntries(cuts)
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	h_btag_met=ROOT.TH1D("h_btag_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_btag_met", cuts)

	btag_met_counts=[]
	btag_met_counts.append(h_btag_met.Integral(0,6001))
	btag_met_counts.append(h_btag_met.Integral(100,6001))
	btag_met_counts.append(h_btag_met.Integral(150,6001))
	btag_met_counts.append(h_btag_met.Integral(250,6001))
	#btag_met_counts.append(h_btag_met.Integral(100,149))
	#btag_met_counts.append(h_btag_met.Integral(150,249))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))

	print("$E^{miss}_{T} > 100$ GeV || %f" % btag_met_counts[1])
	print("$E^{miss}_{T} > 150$ GeV || %f" % btag_met_counts[2])
	print("$E^{miss}_{T} > 250$ GeV || %f" % btag_met_counts[3])

def makeTChiWZTable(ch, cuts):
	n=ch.GetEntries(cuts)
	print("All Entries || %f" %n)
	
	cuts+="&& (nlep >= 2 && (nisoTrack_mt2+nlep <= 3) && (lep_pt[0] > 25) && (lep_pt[1] > 20))"
	n=ch.GetEntries(cuts)
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	cuts+=" && ((dilmass > 86) && (dilmass < 96))"
	n=ch.GetEntries(cuts)
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	cuts+=" && (njets >= 2)"
	n=ch.GetEntries(cuts)
	print("2+ Jets|| %f" %n)

	cuts+=" && (nBJetMedium == 0)"
	n=ch.GetEntries(cuts)
	print("No Btags|| %f" %n)

	cuts+=" && (mt2 >= 80)"
	n=ch.GetEntries(cuts)
	print("MT2b > 200 GeV|| %f" %n)

	cuts+=" && (mjj_mindphi < 110)"
	n=ch.GetEntries(cuts)
	print("$M_{jj}$ for min $\Delta \Phi$ jets < 150 GeV|| %f" %n)

	cuts+=" && ((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))"
	n=ch.GetEntries(cuts)
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	h_btag_met=ROOT.TH1D("h_btag_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_btag_met", cuts)

	btag_met_counts=[]
	btag_met_counts.append(h_btag_met.Integral(0,6001))
	btag_met_counts.append(h_btag_met.Integral(100,6001))
	btag_met_counts.append(h_btag_met.Integral(150,6001))
	btag_met_counts.append(h_btag_met.Integral(250,6001))
	#btag_met_counts.append(h_btag_met.Integral(100,149))
	#btag_met_counts.append(h_btag_met.Integral(150,249))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))

	print("$E^{miss}_{T} > 100$ GeV || %f" % btag_met_counts[1])
	print("$E^{miss}_{T} > 150$ GeV || %f" % btag_met_counts[2])
	print("$E^{miss}_{T} > 250$ GeV || %f" % btag_met_counts[3])

if __name__ == "__main__":
	makeT5ZZCutFlows(1400,700)
	#makeTChiWZCutFlows(550,200)

