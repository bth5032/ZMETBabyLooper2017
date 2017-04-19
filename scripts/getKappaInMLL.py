#!/usr/bin/env python

import ROOT, argparse, sys, math

def errIndependentRatio(num, Dnum, denom, Ddenom):
  return math.sqrt( ((float(Dnum)**2)/(float(denom)**2)) + ((float(Ddenom)**2 * float(num)**2)/(float(denom)**4)))

def getIntegralAndError(hist, low, high):
  """Returns a tuple of (integral, error) in value range for histogram"""
  err = ROOT.Double(0.)
  count = hist.IntegralAndError(hist.FindBin(low), hist.FindBin(high-0.001), err)
  return (count, float (err))

def getIntegralAndErrorOffZ(hist, mll_low, mll_high): 
  (count_low, err_low) = getIntegralAndError(hist, mll_low, 86)
  (count_high, err_high) = getIntegralAndError(hist, 96, mll_high)

  return (count_low+count_high, math.sqrt(err_low*err_low + err_high+err_high))

def getIntegralAndErrorOnZ(hist):
  return getIntegralAndError(hist, 86, 96)

def deriveKappa(mll_low, mll_high, dir_path):
  """Reads in histgrams from the directory and computes kappa in the Mll range specified..."""
  
  print("Computing Kappa in Data")
  data_file = ROOT.TFile(dir_path+"Data.root","r")
  data_hist = data_file.Get("dilmass").Clone("dilmass_data")
  data_onZ = data_hist.Integral(data_hist.FindBin(86),data_hist.FindBin(96))
  data_offZ = data_hist.Integral(data_hist.FindBin(mll_low),data_hist.FindBin(mll_high))
  print("Data: onZ %f, offZ %f, Kappa %f" % (data_onZ, data_offZ, data_onZ/data_offZ))

  mc_names=["Rares","SingleTop","TTBar_Dilep","TTBar_SingleLep","TTW","WW"]
  mc_files=[ROOT.TFile(dir_path+x+".root","r") for x in mc_names]
  mc_hists=[f[0].Get("dilmass").Clone("dilmass_%s" % f[1]) for f in zip(mc_files, mc_names)]
  mc_onZ_counts = [x.Integral(x.FindBin(86),x.FindBin(96)) for x in mc_hists]
  mc_offZ_counts = [x.Integral(x.FindBin(mll_low),x.FindBin(mll_high)) for x in mc_hists]
  
  mc_onZ=sum(mc_onZ_counts)
  mc_offZ=sum(mc_offZ_counts)

  print("MC: onZ %f, offZ %f, Kappa %f" % (mc_onZ, mc_offZ, mc_onZ/mc_offZ))
  sys.stdout.write("MC computed with: ")
  sys.stdout.write(str(mc_names))
  sys.stdout.write("\n")
  sys.stdout.flush()

def deriveKappaWithErrors(mll_low, mll_high, dir_path):
  """Reads in histgrams from the directory and computes kappa in the Mll range specified..."""
  
  print("Computing Kappa in Data")
  data_file = ROOT.TFile(dir_path+"Data.root","r")
  data_hist = data_file.Get("dilmass").Clone("dilmass_data")
  data_onZ = getIntegralAndErrorOnZ(data_hist)
  data_offZ = getIntegralAndError(data_hist, mll_low, mll_high)
  print("Data: onZ %.0f+/-%.4f, offZ %.0f+/-%.4f, Kappa %.4f+/-%.4f" % (data_onZ[0],data_onZ[1], data_offZ[0], data_offZ[1], data_onZ[0]/data_offZ[0], errIndependentRatio(data_onZ[0], data_onZ[1], data_offZ[0], data_offZ[1])))

  mc_names=["Rares","SingleTop","TTBar_Dilep","TTBar_SingleLep","TTW","WW"]
  mc_files=[ROOT.TFile(dir_path+x+".root","r") for x in mc_names]
  mc_hists=[f[0].Get("dilmass").Clone("dilmass_%s" % f[1]) for f in zip(mc_files, mc_names)]
  mc_onZ_counts = [getIntegralAndErrorOnZ(x) for x in mc_hists]
  mc_offZ_counts = [getIntegralAndError(x, mll_low, mll_high) for x in mc_hists]
  
  mc_onZ=sum([a[0] for a in mc_onZ_counts])
  mc_offZ=sum([a[0] for a in mc_offZ_counts])
  mc_err_onZ = math.sqrt(sum([a[1]**2 for a in mc_onZ_counts]))
  mc_err_offZ = math.sqrt(sum([a[1]**2 for a in mc_offZ_counts]))

  print("MC: onZ %.4f+/-%.4f, offZ %.4f+/-%.4f, Kappa %.4f+/-%.4f" % (mc_onZ, mc_err_onZ, mc_offZ, mc_err_offZ, mc_onZ/mc_offZ, errIndependentRatio(mc_onZ, mc_err_onZ, mc_offZ, mc_err_offZ)))
  sys.stdout.write("MC computed with: ")
  sys.stdout.write(str(mc_names))
  sys.stdout.write("\n")
  sys.stdout.flush()

def main():
  parser = argparse.ArgumentParser(add_help=False)
  
  parser.add_argument("-l", "--mll_low", help="Derive Kappa from the low mll value", type=float)
  parser.add_argument("-g", "--mll_high", help="Derive Kappa up to the high mll value", type=float)
  parser.add_argument("-p", "--path", help="Path to directory that holds histograms.", type=str, default="/nfs-7/userdata/bobak/ZMET2017_Hists/topoffcheck_emu/edge/")
  parser.add_argument("--help", help="Print help message and quit", action="store_true")
  
  args=parser.parse_args()


  if args.help:
    parser.print_help()
    exit(0);
  elif (not args.mll_low) or (not args.mll_high):
    print("You must give a low and high mll value")
    parser.print_help()
    exit(1);
  else:
    deriveKappaWithErrors(args.mll_low, args.mll_high, args.path)

if __name__ == "__main__":
  main()