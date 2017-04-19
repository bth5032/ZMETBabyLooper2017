#!/usr/bin/env python

import argparse, sys, ROOT

def getCutsList(SR):
	cuts=["(nlep >= 2 && (nisoTrack_mt2+nlep <= 3))"]
	if (SR=="SRA"):
		cuts.append("((dilmass > 86) && (dilmass < 96))")
		cuts.append("(ht>= 500)")
		cuts.append("((njets == 2) || (njets == 3))")
		cuts.append("(mt2 >= 80)")
		cuts.append("((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))")
	elif (SR=="SRB"):
		cuts.append("((dilmass > 86) && (dilmass < 96))")
		cuts.append("(ht>= 500)")
		cuts.append("((njets == 4) || (njets == 5))")
		cuts.append("(mt2 >= 80)")
		cuts.append("((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))")
	elif (SR=="SRC"):
		cuts.append("((dilmass > 86) && (dilmass < 96))")
		cuts.append("(njets >= 6)")
		cuts.append("(mt2 >= 80)")
		cuts.append("((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))")
	elif (SR=="TChiHZ"):
		cuts.append("((dilmass > 86) && (dilmass < 96))")
		cuts.append("(njets >= 2)")
		cuts.append("(nBJetMedium == 2)")
		cuts.append("(mt2b >= 80)")
		cuts.append("(mbb_csv < 150)")
		cuts.append("((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))")
	elif (SR=="TChiWZ"):
		cuts.append("((dilmass > 86) && (dilmass < 96))")
		cuts.append("(njets >= 2)")
		cuts.append("(nBJetMedium == 0)")
		cuts.append("(mt2 >= 80)")
		cuts.append("(mjj_mindphi < 110)")
		cuts.append("((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))")
	return cuts

def makeT5ZZCutFlows(m_glu, m_lsp):
	ch = ROOT.TChain("t")
	if m_glu >= 1850:
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_mg1850_80x_v2_skim_*")
	else:
		ch.Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-16/skims/t5zz_orig_80x_v2_skim_*")

	cuts="evt_scale1fb*((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)
	#cuts="((mass_gluino == %f) && (mass_LSP == %f))" % (m_glu, m_lsp)

	print("SRA")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRATable(ch, cuts)
	
	print("SRB")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRBTable(ch, cuts)
	
	print("SRC")
	print("T5ZZ model, mass gluino: %.0f, mass LSP %.0f || Events in 35.9 fb$^{-1}$" % (m_glu, m_lsp))
	makeSRCTable(ch, cuts)

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
	
def makeSRATable(ch, cuts):
	n=ch.GetEntries(cuts)
	print("All Entries || %f" %n)
	
	cuts+="&& (nlep >= 2 && (nisoTrack_mt2+nlep <= 3) && (lep_pt[0] > 25) && (lep_pt[1] > 20))"
	n=ch.GetEntries(cuts)
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	cuts+=" && ((dilmass > 86) && (dilmass < 96))"
	n=ch.GetEntries(cuts)
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	cuts+=" && (ht>= 500)"
	n=ch.GetEntries(cuts)
	print("H_{T} > 500 GeV|| %f" %n)

	cuts+=" && ((njets == 2) || (njets == 3))"
	n=ch.GetEntries(cuts)
	print("2-3 Jets|| %f" %n)

	cuts+=" && (mt2 >= 80)"
	n=ch.GetEntries(cuts)
	print("MT2 > 80 GeV|| %f" %n)

	cuts+=" && ((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))"
	n=ch.GetEntries(cuts)
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	print("Btag requirement || B Veto || >= 1 Btag")
	
	h_bveto_met=ROOT.TH1D("h_bveto_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_bveto_met", cuts+" && (nBJetMedium == 0)")

	bveto_met_counts=[]
	bveto_met_counts.append(h_bveto_met.Integral(0,6001))
	bveto_met_counts.append(h_bveto_met.Integral(100,6001))
	bveto_met_counts.append(h_bveto_met.Integral(150,6001))
	bveto_met_counts.append(h_bveto_met.Integral(250,6001))
	#bveto_met_counts.append(h_bveto_met.Integral(100,149))
	#bveto_met_counts.append(h_bveto_met.Integral(150,249))
	#bveto_met_counts.append(h_bveto_met.Integral(250,6001))

	h_btag_met=ROOT.TH1D("h_btag_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_btag_met", cuts+" && ((nBJetMedium >= 1) && (mt2 > 100))")

	btag_met_counts=[]
	btag_met_counts.append(h_btag_met.Integral(0,6001))
	btag_met_counts.append(h_btag_met.Integral(100,6001))
	btag_met_counts.append(h_btag_met.Integral(150,6001))
	btag_met_counts.append(h_btag_met.Integral(250,6001))
	#btag_met_counts.append(h_btag_met.Integral(100,149))
	#btag_met_counts.append(h_btag_met.Integral(150,249))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))

	print("(btag column has MT2>100) || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))
	print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[3], btag_met_counts[3]))

def makeSRBTable(ch, cuts):
	n=ch.GetEntries(cuts)
	print("All Entries || %f" %n)
	
	cuts+="&& (nlep >= 2 && (nisoTrack_mt2+nlep <= 3) && (lep_pt[0] > 25) && (lep_pt[1] > 20))"
	n=ch.GetEntries(cuts)
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	cuts+=" && ((dilmass > 86) && (dilmass < 96))"
	n=ch.GetEntries(cuts)
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	cuts+=" && (ht>= 500)"
	n=ch.GetEntries(cuts)
	print("H_{T} > 500 GeV|| %f" %n)

	cuts+=" && ((njets == 4) || (njets == 5))"
	n=ch.GetEntries(cuts)
	print("4-5 Jets|| %f" %n)

	cuts+=" && (mt2 >= 80)"
	n=ch.GetEntries(cuts)
	print("MT2 > 80 GeV|| %f" %n)

	cuts+=" && ((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))"
	n=ch.GetEntries(cuts)
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	print("Btag requirement || B Veto || >= 1 Btag")
	
	h_bveto_met=ROOT.TH1D("h_bveto_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_bveto_met", cuts+" && (nBJetMedium == 0)")

	bveto_met_counts=[]
	bveto_met_counts.append(h_bveto_met.Integral(0,6001))
	bveto_met_counts.append(h_bveto_met.Integral(100,6001))
	bveto_met_counts.append(h_bveto_met.Integral(150,6001))
	bveto_met_counts.append(h_bveto_met.Integral(250,6001))
	#bveto_met_counts.append(h_bveto_met.Integral(100,149))
	#bveto_met_counts.append(h_bveto_met.Integral(150,249))
	#bveto_met_counts.append(h_bveto_met.Integral(250,6001))

	h_btag_met=ROOT.TH1D("h_btag_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_btag_met", cuts+" && ((nBJetMedium >= 1) && (mt2 > 100))")

	btag_met_counts=[]
	btag_met_counts.append(h_btag_met.Integral(0,6001))
	btag_met_counts.append(h_btag_met.Integral(100,6001))
	btag_met_counts.append(h_btag_met.Integral(150,6001))
	btag_met_counts.append(h_btag_met.Integral(250,6001))
	#btag_met_counts.append(h_btag_met.Integral(100,149))
	#btag_met_counts.append(h_btag_met.Integral(150,249))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))

	print("(btag column has MT2>100) || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))
	print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[3], btag_met_counts[3]))

def makeSRCTable(ch, cuts):
	n=ch.GetEntries(cuts)
	print("All Entries || %f" %n)
	
	cuts+="&& (nlep >= 2 && (nisoTrack_mt2+nlep <= 3) && (lep_pt[0] > 25) && (lep_pt[1] > 20))"
	n=ch.GetEntries(cuts)
	print("2 Leptons (e^{\pm} e^{\mp} or \mu^{\pm}\mu^{\mp}), with p_{T} > 25 (20) GeV || %f" %n)

	cuts+=" && ((dilmass > 86) && (dilmass < 96))"
	n=ch.GetEntries(cuts)
	print("dilepton mass $\in$ Z mass window == (86,96) GeV|| %f" %n)

	cuts+=" && (njets >= 6)"
	n=ch.GetEntries(cuts)
	print("6+ Jets|| %f" %n)

	cuts+=" && (mt2 >= 80)"
	n=ch.GetEntries(cuts)
	print("MT2 > 80 GeV|| %f" %n)

	cuts+=" && ((dphi_metj1 > 0.4) && (dphi_metj2 > 0.4))"
	n=ch.GetEntries(cuts)
	print("\Delta \Phi between MET and two highest p_{T} jets > 0.4 rad || %f" %n)

	print("Btag requirement || B Veto || >= 1 Btag")
	
	h_bveto_met=ROOT.TH1D("h_bveto_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_bveto_met", cuts+" && (nBJetMedium == 0)")

	bveto_met_counts=[]
	bveto_met_counts.append(h_bveto_met.Integral(0,6001))
	bveto_met_counts.append(h_bveto_met.Integral(100,6001))
	bveto_met_counts.append(h_bveto_met.Integral(150,6001))
	#bveto_met_counts.append(h_bveto_met.Integral(250,6001))
	#bveto_met_counts.append(h_bveto_met.Integral(100,149))
	#bveto_met_counts.append(h_bveto_met.Integral(150,249))
	#bveto_met_counts.append(h_bveto_met.Integral(250,6001))

	h_btag_met=ROOT.TH1D("h_btag_met", "h_bveto_met", 6000,0,6000)
	ch.Draw("met_T1CHS_miniAOD_CORE_pt>>h_btag_met", cuts+" && ((nBJetMedium >= 1) && (mt2 > 100))")

	btag_met_counts=[]
	btag_met_counts.append(h_btag_met.Integral(0,6001))
	btag_met_counts.append(h_btag_met.Integral(100,6001))
	btag_met_counts.append(h_btag_met.Integral(150,6001))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))
	#btag_met_counts.append(h_btag_met.Integral(100,149))
	#btag_met_counts.append(h_btag_met.Integral(150,249))
	#btag_met_counts.append(h_btag_met.Integral(250,6001))

	print("(btag column has MT2>100) || %f || %f" % (bveto_met_counts[0], btag_met_counts[0]))
	print("$E^{miss}_{T} > 100$ GeV || %f || %f" % (bveto_met_counts[1], btag_met_counts[1]))
	print("$E^{miss}_{T} > 150$ GeV || %f || %f" % (bveto_met_counts[2], btag_met_counts[2]))
	#print("$E^{miss}_{T} > 250$ GeV || %f || %f" % (bveto_met_counts[3], btag_met_counts[3]))

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

