#!/usr/bin/env python
import ROOT, sys, sets, os

def checkInputs():
  if (len(sys.argv) < 2) or (".root" in sys.argv[1]):
    print("Usage: ")
    print("./getMassSpectrum.py <sample_name> <path_to_baby_1> <path_to_baby_2> ... <path_to_baby_n>")
    exit(1)

  print("Getting mass spectrum for %d files" % len(sys.argv[1:]))

def fillMassSpectrumFromTChain():
  ch = ROOT.TChain("t")
  for i in sys.argv[2:]:
    ch.Add(i)

  ch.SetBranchStatus("*", 0)
  ch.SetBranchStatus("mass_LSP", 1)
  ch.SetBranchStatus("mass_gluino", 1)

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

    if ((ch.mass_gluino,ch.mass_LSP) not in mass_points):
      mass_points.add((ch.mass_gluino,ch.mass_LSP))

  outfile = open(output_filename, 'w')

  for i in mass_points:
    outfile.write("mass gluino: %f \t mass_LSP: %f \n" % (i[0], i[1]))

  outfile.close()

def fillMassSpectrumFromCache():
  mass_file = open(output_filename, 'r')
  for line in mass_file:
    a=line.split()
    mass_points.add((int(float(a[2])), int(float(a[4]))))

def fillMassSpectrum():
  if not os.path.isfile(output_filename):
    fillMassSpectrumFromTChain()
  else:
    fillMassSpectrumFromCache()

name=sys.argv[1]
checkInputs()
output_filename = "SMSScans/Spectra/mass_spectrum_%s.txt" % name
mass_points = sets.Set()
fillMassSpectrum()

for i in mass_points:
    print("mass gluino: %f \t mass_LSP: %f \n" % (i[0], i[1]))

