#!/usr/bin/env python

import argparse, sys, re, getSignalNumbers

templates_path="SMSScans/Templates/"
signal_name = None
mass_spectrum = None
histogram_Path = None
output_path = None
n_parms = {}

def properSpacing(key, param):
  """return param padded with spaces so that it's length {key} to preserve tabbing in templates"""

  delta=len(key)-len(param)

  if (len(param) < len(key)):
    param=(" "*delta)+param

  return param

def addSignalYields(d, SR, mass_gluino, mass_lsp):
  """Pulls and computes CV. yields, stat uncertainty, btag light SF unc, btag heavy SF unc, and ISR SF unc from the signal scan histogram at the proper mass point."""

  avg_yields, RecoMET_yields, stat_uncs, bl_yields, bh_yields, isr_yields, JES = getSignalNumbers.getSignalYields(SR, mass_gluino, mass_lsp, "%s%s/%s.root" % (histogram_Path, SR, signal_name))

  for i,y in enumerate(RecoMET_yields):
    stat_nuisence = 0
    bl_nuisence = 0
    bh_nuisence = 0
    isr_nuisence = 0
    JES_nuisence = 0
    avg_y = avg_yields[i]
    met_nuisence = 0

    #make sure we don't divide by 0
    if y != 0:
      stat_nuisence = (1 + stat_uncs[i]/float(y))
      bl_nuisence = bl_yields[i]/float(y)
      bh_nuisence = bh_yields[i]/float(y)
      isr_nuisence = isr_yields[i]/float(y)
      JES_nuisence = 1+(JES[i]/float(y))
    if avg_y != 0:
      met_nuisence = 1+(abs(y-avg_y)/float(avg_y))

    d["BGbin%d_sig" % i] = properSpacing("{BGbin1_sig}", "%.3f" % avg_y)
    d["sig_stat_syst_bin%d" % i] = properSpacing("{sig_stat_syst_bin1}","%.3f" % stat_nuisence)

    d["sig_btaglight_syst_bin%d" % i] = properSpacing("{sig_btaglight_syst_bin1}", "%.3f" % bl_nuisence)  
    d["sig_btagheavy_syst_bin%d" % i] = properSpacing("{sig_btagheavy_syst_bin1}", "%.3f" % bh_nuisence)  

    d["sig_isr_syst_bin%d" % i] = properSpacing("{sig_isr_syst_bin1}", "%.4f" % isr_nuisence)

    d["sig_JES_syst_bin%d" % i] = properSpacing("{sig_JES_syst_bin1}","%.3f" % JES_nuisence)

    d["sig_metfromFS_syst_bin%d" % i] = properSpacing("{sig_metfromFS_syst_bin1}","%.3f" % met_nuisence)

def addConstantVals(d):
  d["sig_trig_syst"] = properSpacing("{sig_trig_syst}","1.03")
  d["sig_leptonFS_syst"] = properSpacing("{sig_leptonFS_syst}","1.072")
  d["sig_lumi_syst"] = properSpacing("{sig_lumi_syst}","1.026")
  d["sig_pileup_syst"] = properSpacing("{sig_pileup_syst}","1.03")
  d["sig_refacAndNorm_syst"] = properSpacing("{sig_refacAndNorm_syst}","1.03")

def getNuisenceParameters(SR):
  """Reads in the output of the plot maker for the signal region and collects all the key value pairs of nuisance parameters."""
  f = open("outputs/configs_prediction_Final_%s.plots_out" % SR, "r")
  
  n_dict = {}
  for line in f:
    if re.match("{.*} [0-9]*\.[0-9]*\s$", line):
      toks=line.split()
      if "rsfof*kappa" in toks[0]:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%.5f" % float(toks[1]))
      elif "count_" in toks[0] and "_fsbkg" in toks[0]:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%d" % int(float(toks[1])))
      else:
        n_dict[toks[0][1:-1]] = properSpacing(toks[0],"%.3f" % float(toks[1]))

  addConstantVals(n_dict)

  return n_dict

def makeDataCard(sp, SR):
  if SR not in n_parms.keys():
    n_parms[SR] = getNuisenceParameters(SR)

  addSignalYields(n_parms[SR], SR, sp[0], sp[1])

  #for x in n_parms[SR].keys():
  #  print("%s : %s" % (x, n_parms[SR][x]))

  f_template=open("%s%s.txt" % (templates_path, SR), 'r')
  f_out=open("%sdatacard_%s_mGluino_%d_mLSP_%d_.txt" % (output_path, SR, sp[0], sp[1]), 'w+')

  f_out.write(f_template.read().format(**n_parms[SR]))

  f_out.close()
  f_template.close()

def launch():
  """Launches datacard making for each signal region depending on signal name"""
  for sp in mass_spectrum:
    if signal_name == "t5zz":
      makeDataCard(sp, "SRA")
      makeDataCard(sp, "SRAb")
      makeDataCard(sp, "SRB")
      makeDataCard(sp, "SRBb")
      makeDataCard(sp, "SRC")
      makeDataCard(sp, "SRCb")
    elif signal_name == "tchiwz":
      makeDataCard(sp, "TChiHZ")
      makeDataCard(sp, "TChiWZ")
    else:
      print("Do not know how to run on signal model %s. Please use t5zz or tchiwz." % signal_name)
      exit(1)

def setupVars():
  """Clears n_parms, sets histogram paths, mass spectrum, and output path for signal name"""
  global n_parms
  global mass_spectrum
  global histogram_Path
  global output_path
  n_parms = {}

  if signal_name == "t5zz":
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/T5ZZScan/CV/"
    mass_spectrum=[(2000.000000,1990.000000),(1600.000000,200.000000),(1950.000000,800.000000),(1550.000000,1200.000000),(1150.000000,450.000000),(1600.000000,550.000000),(1300.000000,700.000000),(1250.000000,300.000000),(1600.000000,100.000000),(1050.000000,400.000000),(1550.000000,1000.000000),(2100.000000,400.000000),(1150.000000,1100.000000),(1750.000000,25.000000),(1850.000000,1000.000000),(2100.000000,1300.000000),(1700.000000,300.000000),(1900.000000,1700.000000),(1450.000000,500.000000),(1450.000000,800.000000),(2000.000000,1100.000000),(1300.000000,750.000000),(1850.000000,850.000000),(1850.000000,1300.000000),(1850.000000,250.000000),(1900.000000,1050.000000),(1000.000000,800.000000),(1850.000000,700.000000),(1650.000000,250.000000),(2100.000000,1350.000000),(1250.000000,25.000000),(1850.000000,1840.000000),(1700.000000,1200.000000),(1950.000000,1400.000000),(1150.000000,550.000000),(2100.000000,250.000000),(1650.000000,450.000000),(1950.000000,500.000000),(2000.000000,200.000000),(2100.000000,1150.000000),(1000.000000,450.000000),(1550.000000,550.000000),(1650.000000,1350.000000),(900.000000,600.000000),(1000.000000,550.000000),(1300.000000,850.000000),(1050.000000,550.000000),(1350.000000,600.000000),(2000.000000,1350.000000),(1950.000000,1050.000000),(1250.000000,1200.000000),(2000.000000,1950.000000),(1150.000000,350.000000),(2100.000000,1600.000000),(1650.000000,400.000000),(1400.000000,1250.000000),(1800.000000,900.000000),(1900.000000,950.000000),(1800.000000,1700.000000),(1100.000000,450.000000),(1400.000000,50.000000),(1250.000000,800.000000),(1350.000000,1340.000000),(1800.000000,800.000000),(1200.000000,850.000000),(1800.000000,1790.000000),(1150.000000,650.000000),(1000.000000,50.000000),(1450.000000,1050.000000),(1950.000000,100.000000),(800.000000,200.000000),(1250.000000,100.000000),(1300.000000,300.000000),(1050.000000,200.000000),(1700.000000,900.000000),(1900.000000,1350.000000),(1200.000000,700.000000),(1450.000000,350.000000),(1750.000000,50.000000),(1350.000000,950.000000),(1600.000000,1000.000000),(1400.000000,300.000000),(1700.000000,350.000000),(1600.000000,1050.000000),(1150.000000,50.000000),(1950.000000,750.000000),(2050.000000,500.000000),(1600.000000,300.000000),(1450.000000,750.000000),(2050.000000,950.000000),(2000.000000,1850.000000),(2100.000000,200.000000),(2050.000000,1400.000000),(1550.000000,950.000000),(1600.000000,450.000000),(2050.000000,1850.000000),(1450.000000,400.000000),(1500.000000,1490.000000),(2100.000000,1100.000000),(1650.000000,1400.000000),(1850.000000,100.000000),(1950.000000,700.000000),(1900.000000,450.000000),(1350.000000,450.000000),(1800.000000,1400.000000),(1800.000000,50.000000),(1550.000000,1300.000000),(1900.000000,1250.000000),(1750.000000,450.000000),(1700.000000,550.000000),(1350.000000,550.000000),(1550.000000,100.000000),(2000.000000,1750.000000),(1950.000000,1450.000000),(1950.000000,350.000000),(1450.000000,450.000000),(1350.000000,400.000000),(1250.000000,1100.000000),(1000.000000,600.000000),(1050.000000,500.000000),(1300.000000,800.000000),(1750.000000,750.000000),(2050.000000,800.000000),(2050.000000,1250.000000),(1450.000000,850.000000),(2050.000000,1700.000000),(1950.000000,300.000000),(1600.000000,800.000000),(1250.000000,700.000000),(1850.000000,1050.000000),(800.000000,25.000000),(1050.000000,150.000000),(1600.000000,50.000000),(1650.000000,200.000000),(1900.000000,1150.000000),(1750.000000,1740.000000),(1550.000000,500.000000),(1300.000000,350.000000),(1450.000000,1300.000000),(1500.000000,850.000000),(1100.000000,300.000000),(1500.000000,1150.000000),(1600.000000,250.000000),(1450.000000,550.000000),(1100.000000,25.000000),(1750.000000,1200.000000),(1600.000000,1250.000000),(2000.000000,1250.000000),(1650.000000,950.000000),(2000.000000,1050.000000),(1000.000000,150.000000),(1000.000000,400.000000),(2000.000000,1650.000000),(1900.000000,1890.000000),(1300.000000,100.000000),(1900.000000,1550.000000),(1100.000000,800.000000),(2100.000000,450.000000),(1700.000000,450.000000),(1050.000000,100.000000),(1600.000000,1450.000000),(1700.000000,1350.000000),(1650.000000,25.000000),(900.000000,200.000000),(2100.000000,1950.000000),(1950.000000,1750.000000),(1650.000000,550.000000),(1050.000000,900.000000),(1250.000000,450.000000),(1850.000000,550.000000),(1650.000000,350.000000),(1750.000000,1300.000000),(1550.000000,400.000000),(1050.000000,450.000000),(1400.000000,1200.000000),(1050.000000,700.000000),(1850.000000,300.000000),(1400.000000,600.000000),(1900.000000,1450.000000),(900.000000,500.000000),(2000.000000,300.000000),(1450.000000,200.000000),(1650.000000,750.000000),(2000.000000,900.000000),(800.000000,600.000000),(1450.000000,1100.000000),(2000.000000,1500.000000),(1450.000000,1400.000000),(1250.000000,950.000000),(1500.000000,450.000000),(1500.000000,800.000000),(1500.000000,550.000000),(1500.000000,1100.000000),(1100.000000,150.000000),(1950.000000,1350.000000),(1200.000000,500.000000),(1150.000000,800.000000),(1800.000000,1100.000000),(1350.000000,700.000000),(1950.000000,450.000000),(1100.000000,600.000000),(1800.000000,200.000000),(1600.000000,1150.000000),(1500.000000,600.000000),(1050.000000,850.000000),(2050.000000,250.000000),(1750.000000,500.000000),(2050.000000,700.000000),(1250.000000,550.000000),(800.000000,790.000000),(1950.000000,1300.000000),(2050.000000,1150.000000),(1550.000000,300.000000),(1600.000000,400.000000),(900.000000,25.000000),(1950.000000,400.000000),(2100.000000,1000.000000),(1350.000000,1100.000000),(1800.000000,1200.000000),(1400.000000,900.000000),(2100.000000,1900.000000),(1350.000000,200.000000),(1600.000000,950.000000),(1650.000000,600.000000),(1500.000000,1200.000000),(1200.000000,450.000000),(2000.000000,800.000000),(1750.000000,200.000000),(1200.000000,100.000000),(1700.000000,850.000000),(1150.000000,900.000000),(2000.000000,1400.000000),(1900.000000,1750.000000),(1300.000000,250.000000),(1250.000000,1150.000000),(800.000000,500.000000),(1850.000000,800.000000),(1450.000000,25.000000),(1800.000000,350.000000),(1000.000000,200.000000),(1000.000000,850.000000),(2100.000000,850.000000),(1400.000000,800.000000),(1750.000000,1550.000000),(2050.000000,100.000000),(1400.000000,500.000000),(1950.000000,1900.000000),(2050.000000,550.000000),(2100.000000,1750.000000),(2000.000000,100.000000),(1950.000000,1000.000000),(2000.000000,700.000000),(1800.000000,700.000000),(1150.000000,850.000000),(1350.000000,850.000000),(1500.000000,50.000000),(1950.000000,1550.000000),(1750.000000,300.000000),(2100.000000,500.000000),(1600.000000,1100.000000),(1900.000000,1650.000000),(1250.000000,1000.000000),(1950.000000,650.000000),(1200.000000,25.000000),(1150.000000,100.000000),(1850.000000,1650.000000),(1350.000000,50.000000),(1450.000000,900.000000),(1350.000000,300.000000),(1450.000000,1200.000000),(1250.000000,850.000000),(1350.000000,800.000000),(1850.000000,450.000000),(1900.000000,100.000000),(1850.000000,900.000000),(1550.000000,1250.000000),(1950.000000,600.000000),(1000.000000,100.000000),(1550.000000,250.000000),(1300.000000,25.000000),(1250.000000,50.000000),(1800.000000,300.000000),(2100.000000,2050.000000),(1500.000000,700.000000),(2100.000000,350.000000),(1150.000000,950.000000),(900.000000,890.000000),(1550.000000,1050.000000),(1750.000000,1000.000000),(1650.000000,850.000000),(2000.000000,600.000000),(1600.000000,1500.000000),(900.000000,700.000000),(1750.000000,150.000000),(1300.000000,450.000000),(1150.000000,200.000000),(2100.000000,800.000000),(1650.000000,1200.000000),(1500.000000,750.000000),(1500.000000,650.000000),(2100.000000,1700.000000),(1500.000000,1350.000000),(1700.000000,150.000000),(1400.000000,1150.000000),(2100.000000,600.000000),(1400.000000,550.000000),(1850.000000,1750.000000),(1400.000000,250.000000),(1100.000000,850.000000),(1850.000000,1150.000000),(1450.000000,250.000000),(1950.000000,150.000000),(1150.000000,1050.000000),(1700.000000,1690.000000),(1500.000000,1050.000000),(1000.000000,990.000000),(1250.000000,150.000000),(1200.000000,1050.000000),(1900.000000,400.000000),(1800.000000,1350.000000),(1650.000000,1550.000000),(2100.000000,1550.000000),(1150.000000,300.000000),(1550.000000,850.000000),(1800.000000,450.000000),(1600.000000,25.000000),(1600.000000,1400.000000),(2000.000000,450.000000),(1650.000000,900.000000),(1200.000000,800.000000),(1200.000000,750.000000),(1350.000000,1050.000000),(1050.000000,300.000000),(1750.000000,1150.000000),(2050.000000,1200.000000),(2100.000000,300.000000),(2050.000000,1500.000000),(2050.000000,1650.000000),(1000.000000,950.000000),(2050.000000,1950.000000),(1400.000000,650.000000),(1100.000000,900.000000),(1700.000000,1600.000000),(1350.000000,250.000000),(1200.000000,300.000000),(1150.000000,250.000000),(1350.000000,1000.000000),(1750.000000,950.000000),(1900.000000,300.000000),(1800.000000,1450.000000),(1650.000000,650.000000),(1700.000000,800.000000),(1900.000000,500.000000),(1700.000000,950.000000),(1950.000000,1700.000000),(1800.000000,1500.000000),(2000.000000,1550.000000),(1400.000000,100.000000),(1750.000000,1600.000000),(1650.000000,1450.000000),(1800.000000,600.000000),(2100.000000,150.000000),(1400.000000,1350.000000),(2100.000000,1400.000000),(1200.000000,950.000000),(1400.000000,450.000000),(2050.000000,450.000000),(1750.000000,650.000000),(2050.000000,900.000000),(1850.000000,1400.000000),(2050.000000,1350.000000),(2050.000000,1800.000000),(1100.000000,550.000000),(1450.000000,300.000000),(1850.000000,200.000000),(1100.000000,100.000000),(1300.000000,550.000000),(1850.000000,650.000000),(1650.000000,50.000000),(1100.000000,1000.000000),(1250.000000,400.000000),(1550.000000,1450.000000),(1500.000000,350.000000),(1350.000000,750.000000),(1600.000000,600.000000),(1500.000000,950.000000),(1450.000000,50.000000),(1750.000000,1700.000000),(1850.000000,350.000000),(2000.000000,250.000000),(1450.000000,950.000000),(1150.000000,400.000000),(1700.000000,1400.000000),(1450.000000,1250.000000),(1650.000000,100.000000),(1900.000000,600.000000),(2100.000000,2090.000000),(1450.000000,1150.000000),(1600.000000,500.000000),(1550.000000,750.000000),(1550.000000,900.000000),(1500.000000,200.000000),(1250.000000,200.000000),(1750.000000,100.000000),(1300.000000,900.000000),(1100.000000,1090.000000),(1050.000000,250.000000),(1750.000000,1350.000000),(1850.000000,1500.000000),(1550.000000,200.000000),(1400.000000,1390.000000),(1700.000000,25.000000),(1650.000000,1500.000000),(1300.000000,1000.000000),(2050.000000,2040.000000),(2100.000000,100.000000),(1500.000000,150.000000),(1700.000000,700.000000),(1600.000000,900.000000),(1850.000000,150.000000),(1950.000000,1800.000000),(1550.000000,1350.000000),(1550.000000,650.000000),(1400.000000,1100.000000),(1950.000000,900.000000),(1700.000000,400.000000),(1600.000000,1200.000000),(1000.000000,25.000000),(1000.000000,750.000000),(1400.000000,200.000000),(1100.000000,1050.000000),(2100.000000,650.000000),(1800.000000,650.000000),(1850.000000,600.000000),(1450.000000,600.000000),(1650.000000,1150.000000),(1750.000000,850.000000),(2000.000000,1300.000000),(1500.000000,300.000000),(1350.000000,1150.000000),(2000.000000,1900.000000),(1500.000000,900.000000),(1750.000000,600.000000),(1350.000000,150.000000),(1050.000000,1000.000000),(1550.000000,350.000000),(1900.000000,900.000000),(1800.000000,1550.000000),(1800.000000,100.000000),(2100.000000,1450.000000),(1550.000000,25.000000),(2050.000000,350.000000),(1750.000000,400.000000),(1650.000000,1300.000000),(1950.000000,1500.000000),(1800.000000,1000.000000),(1050.000000,650.000000),(1200.000000,600.000000),(1750.000000,1650.000000),(2050.000000,2000.000000),(1800.000000,750.000000),(1150.000000,1140.000000),(1700.000000,1050.000000),(1800.000000,250.000000),(1750.000000,700.000000),(1050.000000,25.000000),(1650.000000,1600.000000),(1700.000000,1500.000000),(1650.000000,300.000000),(1900.000000,800.000000),(1800.000000,1750.000000),(2000.000000,1800.000000),(1250.000000,650.000000),(1800.000000,1150.000000),(800.000000,400.000000),(1800.000000,850.000000),(800.000000,100.000000),(1850.000000,400.000000),(1150.000000,25.000000),(1950.000000,1650.000000),(1900.000000,150.000000),(1400.000000,1300.000000),(1950.000000,1100.000000),(1400.000000,1000.000000),(2100.000000,950.000000),(2050.000000,200.000000),(900.000000,100.000000),(1950.000000,200.000000),(2050.000000,650.000000),(1400.000000,25.000000),(1200.000000,150.000000),(1700.000000,50.000000),(2050.000000,1100.000000),(2050.000000,1550.000000),(1250.000000,250.000000),(1750.000000,1400.000000),(2000.000000,1700.000000),(1200.000000,900.000000),(1700.000000,1000.000000),(1700.000000,250.000000),(1600.000000,1350.000000),(1750.000000,550.000000),(1900.000000,700.000000),(1700.000000,1150.000000),(2100.000000,1200.000000),(1300.000000,500.000000),(1450.000000,100.000000),(1200.000000,550.000000),(1500.000000,1250.000000),(1150.000000,500.000000),(1500.000000,250.000000),(1300.000000,1150.000000),(1850.000000,1250.000000),(1850.000000,1700.000000),(1050.000000,1040.000000),(1500.000000,1300.000000),(1100.000000,700.000000),(2000.000000,1200.000000),(1000.000000,300.000000),(2100.000000,1250.000000),(1600.000000,1550.000000),(1900.000000,1100.000000),(1550.000000,1100.000000),(1500.000000,500.000000),(1200.000000,1150.000000),(1750.000000,1500.000000),(1300.000000,1200.000000),(1550.000000,800.000000),(1800.000000,25.000000),(2100.000000,1050.000000),(1750.000000,1250.000000),(1200.000000,650.000000),(1300.000000,1050.000000),(2000.000000,350.000000),(1250.000000,350.000000),(1200.000000,250.000000),(2000.000000,950.000000),(2000.000000,1600.000000),(1600.000000,650.000000),(2100.000000,900.000000),(1150.000000,600.000000),(1600.000000,1590.000000),(1950.000000,1850.000000),(1500.000000,1450.000000),(1200.000000,1000.000000),(1400.000000,1050.000000),(1950.000000,950.000000),(1400.000000,750.000000),(2100.000000,700.000000),(1100.000000,350.000000),(1900.000000,1850.000000),(1400.000000,150.000000),(1850.000000,1350.000000),(900.000000,800.000000),(1550.000000,1500.000000),(1850.000000,750.000000),(1550.000000,1400.000000),(1150.000000,750.000000),(1450.000000,650.000000),(1550.000000,1540.000000),(1000.000000,700.000000),(1250.000000,500.000000),(800.000000,300.000000),(1600.000000,700.000000),(1900.000000,350.000000),(1300.000000,50.000000),(1000.000000,500.000000),(2100.000000,750.000000),(1550.000000,150.000000),(1250.000000,600.000000),(1050.000000,750.000000),(1800.000000,1250.000000),(1300.000000,400.000000),(1800.000000,1050.000000),(1150.000000,700.000000),(1700.000000,1650.000000),(1400.000000,400.000000),(1500.000000,25.000000),(2000.000000,850.000000),(1950.000000,550.000000),(2050.000000,400.000000),(2000.000000,1450.000000),(2050.000000,850.000000),(1750.000000,1050.000000),(2050.000000,1300.000000),(1350.000000,350.000000),(1300.000000,650.000000),(2050.000000,1750.000000),(1100.000000,400.000000),(1650.000000,1000.000000),(1250.000000,900.000000),(1800.000000,950.000000),(1000.000000,250.000000),(1700.000000,650.000000),(1800.000000,400.000000),(1400.000000,850.000000),(1900.000000,250.000000),(1700.000000,200.000000),(1700.000000,1550.000000),(1150.000000,150.000000),(1650.000000,1250.000000),(2000.000000,150.000000),(900.000000,300.000000),(1800.000000,150.000000),(2000.000000,750.000000),(1250.000000,1050.000000),(1900.000000,1300.000000),(1700.000000,600.000000),(1000.000000,650.000000),(1800.000000,500.000000),(1050.000000,600.000000),(1950.000000,1150.000000),(1900.000000,650.000000),(1350.000000,1200.000000),(1650.000000,1100.000000),(1400.000000,350.000000),(800.000000,700.000000),(1200.000000,400.000000),(2050.000000,1000.000000),(1750.000000,250.000000),(1600.000000,150.000000),(2050.000000,1450.000000),(1500.000000,400.000000),(2050.000000,1600.000000),(1700.000000,1100.000000),(2050.000000,1900.000000),(1850.000000,1450.000000),(1100.000000,50.000000),(1100.000000,950.000000),(1050.000000,50.000000),(1200.000000,1190.000000),(1100.000000,500.000000),(1300.000000,150.000000),(2000.000000,550.000000),(1950.000000,1940.000000),(1050.000000,950.000000),(1700.000000,750.000000),(1400.000000,700.000000),(1900.000000,1200.000000),(1600.000000,350.000000),(1450.000000,150.000000),(2100.000000,2000.000000),(1300.000000,1100.000000),(2000.000000,650.000000),(1700.000000,1250.000000),(1300.000000,600.000000),(1900.000000,550.000000),(2100.000000,550.000000),(1800.000000,1600.000000),(2000.000000,1150.000000),(1650.000000,800.000000),(1250.000000,1240.000000),(1750.000000,350.000000),(1700.000000,1450.000000),(1700.000000,100.000000),(1900.000000,1600.000000),(1300.000000,200.000000),(1550.000000,600.000000),(1050.000000,350.000000),(1750.000000,1450.000000),(1650.000000,1640.000000),(1250.000000,750.000000),(1850.000000,1100.000000),(2100.000000,1850.000000),(1850.000000,1550.000000),(1850.000000,500.000000),(1350.000000,1300.000000),(1050.000000,800.000000),(1850.000000,950.000000),(1650.000000,500.000000),(1100.000000,250.000000),(1000.000000,900.000000),(1750.000000,800.000000),(1200.000000,1100.000000),(1550.000000,50.000000),(1600.000000,1300.000000),(1200.000000,200.000000),(1600.000000,850.000000),(900.000000,400.000000),(1350.000000,500.000000),(2100.000000,1500.000000),(1500.000000,1400.000000),(2000.000000,500.000000),(1800.000000,550.000000),(1900.000000,1500.000000),(1450.000000,700.000000),(1950.000000,1600.000000),(1450.000000,1000.000000),(1500.000000,100.000000),(1300.000000,950.000000),(1750.000000,1100.000000),(1650.000000,700.000000),(1450.000000,1350.000000),(1500.000000,1000.000000),(1900.000000,850.000000),(1350.000000,25.000000),(1350.000000,1250.000000),(1900.000000,1000.000000),(1600.000000,750.000000),(1100.000000,650.000000),(1700.000000,1300.000000),(1100.000000,200.000000),(1350.000000,900.000000),(1350.000000,650.000000),(1950.000000,1250.000000),(1700.000000,500.000000),(1750.000000,900.000000),(2050.000000,150.000000),(1400.000000,950.000000),(2050.000000,600.000000),(1550.000000,450.000000),(2050.000000,1050.000000),(1350.000000,100.000000),(1800.000000,1300.000000),(1200.000000,350.000000),(1900.000000,1400.000000),(1950.000000,1200.000000),(2100.000000,1800.000000),(1450.000000,1440.000000),(1900.000000,200.000000),(2000.000000,400.000000),(1900.000000,750.000000),(1550.000000,700.000000),(1300.000000,1250.000000),(2000.000000,1000.000000),(1150.000000,1000.000000),(1850.000000,1800.000000),(1950.000000,250.000000),(1800.000000,1650.000000),(1850.000000,1200.000000),(1950.000000,850.000000),(1900.000000,1800.000000),(1650.000000,150.000000),(1100.000000,750.000000),(1300.000000,1290.000000),(1550.000000,1150.000000),(1650.000000,1050.000000),(1000.000000,350.000000),(2050.000000,300.000000),(1850.000000,1600.000000),(2100.000000,1650.000000),(2050.000000,750.000000),(1200.000000,50.000000)]  
  elif signal_name == "tchiwz":
    histogram_Path="/nfs-7/userdata/bobak/ZMET2017_Hists/TChiWZScan/CV/"
    mass_spectrum=[(350.000000,1.000000),(600.000000,200.000000),(550.000000,1.000000),(225.000000,125.000000),(700.000000,25.000000),(575.000000,225.000000),(675.000000,25.000000),(575.000000,275.000000),(250.000000,75.000000),(350.000000,240.000000),(125.000000,35.000000),(150.000000,10.000000),(425.000000,225.000000),(700.000000,75.000000),(400.000000,260.000000),(275.000000,145.000000),(500.000000,225.000000),(150.000000,70.000000),(500.000000,300.000000),(300.000000,270.000000),(125.000000,75.000000),(600.000000,1.000000),(325.000000,225.000000),(675.000000,100.000000),(600.000000,275.000000),(700.000000,100.000000),(450.000000,200.000000),(500.000000,75.000000),(475.000000,75.000000),(475.000000,150.000000),(375.000000,25.000000),(200.000000,185.000000),(500.000000,150.000000),(425.000000,150.000000),(400.000000,25.000000),(550.000000,100.000000),(175.000000,85.000000),(300.000000,290.000000),(575.000000,100.000000),(375.000000,75.000000),(275.000000,185.000000),(675.000000,250.000000),(650.000000,200.000000),(400.000000,100.000000),(200.000000,160.000000),(150.000000,80.000000),(700.000000,200.000000),(325.000000,175.000000),(475.000000,50.000000),(225.000000,135.000000),(575.000000,250.000000),(250.000000,100.000000),(425.000000,50.000000),(575.000000,75.000000),(325.000000,100.000000),(200.000000,193.000000),(500.000000,275.000000),(100.000000,20.000000),(700.000000,275.000000),(225.000000,155.000000),(650.000000,275.000000),(450.000000,1.000000),(550.000000,25.000000),(650.000000,1.000000),(150.000000,60.000000),(275.000000,225.000000),(625.000000,50.000000),(225.000000,75.000000),(650.000000,250.000000),(550.000000,175.000000),(200.000000,180.000000),(550.000000,300.000000),(250.000000,140.000000),(450.000000,50.000000),(475.000000,175.000000),(375.000000,245.000000),(200.000000,70.000000),(100.000000,60.000000),(200.000000,50.000000),(325.000000,1.000000),(600.000000,250.000000),(500.000000,25.000000),(475.000000,25.000000),(275.000000,100.000000),(300.000000,285.000000),(250.000000,210.000000),(550.000000,275.000000),(675.000000,150.000000),(700.000000,125.000000),(275.000000,260.000000),(225.000000,210.000000),(125.000000,45.000000),(300.000000,125.000000),(100.000000,1.000000),(475.000000,225.000000),(250.000000,180.000000),(225.000000,205.000000),(275.000000,135.000000),(200.000000,110.000000),(200.000000,170.000000),(175.000000,168.000000),(525.000000,200.000000),(375.000000,225.000000),(200.000000,90.000000),(150.000000,140.000000),(375.000000,150.000000),(450.000000,75.000000),(400.000000,250.000000),(325.000000,150.000000),(525.000000,150.000000),(350.000000,200.000000),(100.000000,90.000000),(150.000000,50.000000),(350.000000,210.000000),(625.000000,175.000000),(150.000000,20.000000),(300.000000,250.000000),(250.000000,220.000000),(350.000000,125.000000),(275.000000,175.000000),(150.000000,135.000000),(125.000000,25.000000),(125.000000,115.000000),(250.000000,160.000000),(150.000000,90.000000),(325.000000,185.000000),(350.000000,260.000000),(325.000000,195.000000),(175.000000,55.000000),(525.000000,250.000000),(350.000000,100.000000),(150.000000,1.000000),(150.000000,30.000000),(525.000000,75.000000),(375.000000,100.000000),(400.000000,75.000000),(475.000000,275.000000),(500.000000,125.000000),(350.000000,25.000000),(425.000000,275.000000),(625.000000,200.000000),(300.000000,180.000000),(425.000000,1.000000),(350.000000,220.000000),(525.000000,25.000000),(375.000000,285.000000),(500.000000,1.000000),(300.000000,210.000000),(700.000000,1.000000),(300.000000,75.000000),(100.000000,50.000000),(275.000000,215.000000),(350.000000,230.000000),(275.000000,50.000000),(600.000000,50.000000),(525.000000,1.000000),(375.000000,125.000000),(250.000000,200.000000),(125.000000,5.000000),(575.000000,50.000000),(200.000000,1.000000),(400.000000,290.000000),(600.000000,100.000000),(575.000000,300.000000),(375.000000,175.000000),(350.000000,280.000000),(100.000000,93.000000),(175.000000,35.000000),(350.000000,290.000000),(525.000000,125.000000),(500.000000,250.000000),(225.000000,195.000000),(425.000000,285.000000),(375.000000,1.000000),(625.000000,250.000000),(300.000000,170.000000),(575.000000,1.000000),(275.000000,255.000000),(325.000000,275.000000),(525.000000,275.000000),(250.000000,240.000000),(375.000000,295.000000),(350.000000,250.000000),(675.000000,75.000000),(600.000000,175.000000),(175.000000,125.000000),(175.000000,135.000000),(250.000000,25.000000),(550.000000,50.000000),(350.000000,175.000000),(225.000000,25.000000),(275.000000,125.000000),(300.000000,260.000000),(400.000000,175.000000),(600.000000,225.000000),(200.000000,100.000000),(350.000000,300.000000),(325.000000,235.000000),(625.000000,1.000000),(250.000000,235.000000),(300.000000,100.000000),(325.000000,245.000000),(650.000000,225.000000),(500.000000,200.000000),(200.000000,80.000000),(625.000000,125.000000),(350.000000,150.000000),(425.000000,125.000000),(450.000000,150.000000),(550.000000,150.000000),(550.000000,225.000000),(650.000000,25.000000),(100.000000,80.000000),(350.000000,75.000000),(225.000000,95.000000),(375.000000,275.000000),(575.000000,175.000000),(275.000000,165.000000),(400.000000,225.000000),(175.000000,45.000000),(600.000000,300.000000),(400.000000,270.000000),(200.000000,140.000000),(650.000000,175.000000),(675.000000,50.000000),(450.000000,300.000000),(500.000000,175.000000),(350.000000,50.000000),(675.000000,275.000000),(200.000000,120.000000),(625.000000,275.000000),(250.000000,50.000000),(475.000000,250.000000),(450.000000,275.000000),(175.000000,95.000000),(275.000000,235.000000),(125.000000,118.000000),(325.000000,25.000000),(475.000000,1.000000),(100.000000,70.000000),(675.000000,1.000000),(300.000000,293.000000),(150.000000,40.000000),(275.000000,205.000000),(175.000000,155.000000),(225.000000,218.000000),(150.000000,100.000000),(125.000000,15.000000),(700.000000,50.000000),(250.000000,1.000000),(125.000000,105.000000),(250.000000,150.000000),(650.000000,50.000000),(300.000000,50.000000),(275.000000,75.000000),(175.000000,25.000000),(425.000000,200.000000),(175.000000,160.000000),(625.000000,25.000000),(475.000000,200.000000),(450.000000,250.000000),(250.000000,120.000000),(225.000000,175.000000),(175.000000,75.000000),(275.000000,245.000000),(375.000000,200.000000),(700.000000,150.000000),(650.000000,150.000000),(650.000000,75.000000),(100.000000,10.000000),(150.000000,143.000000),(650.000000,300.000000),(675.000000,200.000000),(250.000000,190.000000),(425.000000,100.000000),(700.000000,300.000000),(450.000000,100.000000),(125.000000,85.000000),(250.000000,130.000000),(500.000000,50.000000),(575.000000,200.000000),(375.000000,255.000000),(250.000000,110.000000),(100.000000,40.000000),(425.000000,75.000000),(700.000000,225.000000),(175.000000,115.000000),(175.000000,165.000000),(300.000000,1.000000),(225.000000,185.000000),(225.000000,165.000000),(400.000000,150.000000),(450.000000,25.000000),(125.000000,110.000000),(425.000000,25.000000),(525.000000,225.000000),(325.000000,75.000000),(300.000000,240.000000),(650.000000,125.000000),(250.000000,230.000000),(525.000000,175.000000),(275.000000,155.000000),(450.000000,225.000000),(250.000000,170.000000),(425.000000,295.000000),(225.000000,105.000000),(200.000000,25.000000),(200.000000,130.000000),(350.000000,270.000000),(475.000000,300.000000),(325.000000,205.000000),(125.000000,65.000000),(100.000000,85.000000),(325.000000,215.000000),(400.000000,50.000000),(650.000000,100.000000),(550.000000,125.000000),(375.000000,235.000000),(400.000000,200.000000),(175.000000,1.000000),(275.000000,25.000000),(625.000000,150.000000),(425.000000,250.000000),(325.000000,265.000000),(600.000000,125.000000),(625.000000,75.000000),(400.000000,125.000000),(575.000000,150.000000),(575.000000,25.000000),(375.000000,265.000000),(300.000000,200.000000),(525.000000,100.000000),(300.000000,230.000000),(550.000000,75.000000),(275.000000,268.000000),(275.000000,195.000000),(625.000000,300.000000),(675.000000,125.000000),(175.000000,145.000000),(625.000000,225.000000),(150.000000,110.000000),(525.000000,50.000000),(400.000000,280.000000),(225.000000,1.000000),(425.000000,175.000000),(200.000000,150.000000),(600.000000,25.000000),(700.000000,175.000000),(250.000000,243.000000),(150.000000,130.000000),(575.000000,125.000000),(550.000000,200.000000),(325.000000,255.000000),(600.000000,75.000000),(100.000000,30.000000),(450.000000,125.000000),(300.000000,160.000000),(300.000000,25.000000),(225.000000,145.000000),(175.000000,65.000000),(400.000000,1.000000),(225.000000,115.000000),(300.000000,190.000000),(325.000000,285.000000),(325.000000,295.000000),(450.000000,175.000000),(125.000000,55.000000),(700.000000,250.000000),(325.000000,125.000000),(625.000000,100.000000),(300.000000,220.000000),(675.000000,300.000000),(150.000000,120.000000),(200.000000,190.000000),(475.000000,100.000000),(200.000000,60.000000),(500.000000,100.000000),(375.000000,50.000000),(400.000000,300.000000),(675.000000,175.000000),(525.000000,300.000000),(225.000000,85.000000),(125.000000,95.000000),(325.000000,50.000000),(300.000000,280.000000),(600.000000,150.000000),(550.000000,250.000000),(275.000000,265.000000),(225.000000,215.000000),(675.000000,225.000000),(225.000000,50.000000),(175.000000,105.000000),(275.000000,1.000000),(300.000000,150.000000),(475.000000,125.000000)]
  else:
    print("Do not know how to run on signal model %s. Please use t5zz or tchiwz." % signal_name)
    exit(1)

  output_path="SMSScans/DataCards/%s/" % signal_name

def main():
  global signal_name
  parser = argparse.ArgumentParser()
  
  parser.add_argument("--t5zz", help="make datacards for t5zz sample", action="store_true")
  parser.add_argument("--tchiwz", help="make datacards for TChiWZ sample", action="store_true")
  parser.add_argument("--tchizz", help="make datacards for TChiZZ sample", action="store_true")
  parser.add_argument("--tchihz", help="make datacards for TChiHZ sample", action="store_true")
  
  args=parser.parse_args()

  if (args.t5zz):
    signal_name = "t5zz"
    setupVars()
    launch()
  elif (args.tchiwz):
    signal_name = "tchiwz"
    setupVars()
    launch()
  else:
    parser.print_help()




if __name__=="__main__":
  main()