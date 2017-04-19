#!/usr/bin/env python
import ROOT, sys, sets, os


def sweep():
  ch = ROOT.TChain("t")
  for i in sys.argv[2:]:
    ch.Add(i)

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

def check():
  ch = ROOT.TChain("t")
  for i in sys.argv[2:]:
    ch.Add(i)

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

    a=ch.mass_gluino
    b=ch.mass_LSP

def store():
  ch = ROOT.TChain("t")
  for i in sys.argv[2:]:
    ch.Add(i)

  mass_points = sets.Set()
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

    a=ch.mass_gluino
    b=ch.mass_LSP

    if ((a,b) not in mass_points):
      mass_points.add((a,b))

if sys.argv[1] == "sweep":
  sweep()
elif sys.argv[1] == "check":
  check()
elif sys.argv[1] == "store":
  store()
else:
  print("unrecognized option %s, please use store, sweep, or check" % sys.argv[1])