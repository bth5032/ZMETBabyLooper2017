#!/usr/bin/env python
import ROOT, sys, sets, os

mass_points = sets.Set()
files_in = []
output_filename = ""

def checkInputs():
  if (len(sys.argv) < 2) or (".root" in sys.argv[1]):
    print("Usage: ")
    print("./getMassSpectrum1D.py <sample_name> <path_to_baby_1> <path_to_baby_2> ... <path_to_baby_n>")
    exit(1)

  print("Getting mass spectrum for %d files" % len(sys.argv[2:]))

def fillMassSpectrumFromTChain():
  ch = ROOT.TChain("t")
  for i in files_in:
    ch.Add(i)

  ch.SetBranchStatus("*", 0)
  ch.SetBranchStatus("mass_chi", 1)

  n_entries = ch.GetEntries()
  for j_entry in range(n_entries):
    
    #if j_entry > 1000:
    #  break

    i_entry = ch.LoadTree(j_entry)
    if i_entry < 0:
      break
    nb = ch.GetEntry(j_entry)
    if nb <= 0:
      continue


    if j_entry % 10000 == 0:
      print("Processing entry %d of %d" % (j_entry, n_entries))

    if ((ch.mass_chi) not in mass_points):
      mass_points.add((ch.mass_chi))

  outfile = open(output_filename, 'w')

  for i in mass_points:
    outfile.write("mass_chi: %f\n" % i)

  outfile.close()

def fillMassSpectrumFromCache():
  """parse the mass binning file with output_filename as it's filename and fill the mass points set"""
  mass_file = open(output_filename, 'r')
  for line in mass_file:
    a=line.split()
    mass_points.add(int(float(a[1])))

def fillMassSpectrum():
  """checks if the sample name already has a mass binning file made, if it does, parse it and fill the mass spectrum, otherwise read the files in a build the mass spectrum on the fly."""
  if not os.path.isfile(output_filename):
    fillMassSpectrumFromTChain()
  else:
    fillMassSpectrumFromCache()

def getMassSpectrum(name, f_paths):
  """returns a list of pairs (mass_gluino, mass_LSP)"""
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
  files_in = f_paths
  
  fillMassSpectrum()

  return mass_points

def printScanChainCode():
  print("const int n_chi_points_%s = %d;" % (name, len(mass_points)-1))
  arr = "const double chi_points_%s[n_chi_points_%s+1] = {" % (name, name)
  for i in sorted(mass_points):
    arr+=("%d," % i)
  arr=arr[:-1]
  arr+="};"
  print(arr)

def printDataCardMakerCode():
  print("elif (signal_name == %s):" % name)
  print("histogram_Path=\"/nfs-7/userdata/bobak/ZMET2017_Hists/<FILL_IN_NAME>/CV/\"")
  arr = "mass_spectrum = ["
  for i in sorted(mass_points):
    arr+=("%d," % i)
  arr=arr[:-1]
  arr+="]"
  print(arr)

if __name__ == "__main__":
  name=sys.argv[1]
  output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
  if (sys.argv[2] == "scanchain"):
    fillMassSpectrumFromCache()
    printScanChainCode()
  elif (sys.argv[2] == "datacard"):
    fillMassSpectrumFromCache()
    printDataCardMakerCode()
  else: 
    checkInputs()
    files_in=sys.argv[2:]
    fillMassSpectrum()
    for i in mass_points:
      print("mass_chi: %f" % i)

