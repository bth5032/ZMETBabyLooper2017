#!/usr/bin/env python

import argparse, sys

def resultsTable(infile, regions, print_header=True):
  """Parses and pretty prints the Templates Error output tables"""

  SR_LINE = "50-100" #thing to check for the line that gets the sample name 
  
  num_rows_std = 3
  row_insert = "l |"

  if (regions == "ewk"):
    num_rows_std = 4
    row_insert = "l | l |"

  if (print_header):
    print("\\documentclass[a4paper,landscape]{article}")
    print("\\usepackage{fullpage}")
    print("\\usepackage{float}")
    print("\\usepackage{multicol}")
    print("\\usepackage{adjustbox}")
    print("\\begin{document}")


  #print("\\begin{table}[H]")
  print("\\begin{table}[ht!]")
  print("\\begin{center}")

  tabular_header_string=""
  tabular_header_printed=False
  sr_name=""

  for line in infile:
    if "{table}" in line:
      continue
    if "{center}" in line:
      continue
    if "{document}" in line:
      continue
    if "\\end{tabular}" in line:
      continue
    if "\\textbf" in line:
      #Get Signal Region Name
      sr_name=line[8:-3]
      continue
    if "\\begin{tabular}" in line:
      #Add extra c| for Signal Region Title
      if not tabular_header_printed:
        print("%s %s%s" % (line[:line.index("{l |")+4], row_insert, line[line.index("{l |")+4:-1]))
        tabular_header_printed=True
      continue

    if SR_LINE in line:
      if line.count('&') == num_rows_std:
        toks=line.split('&')
        sys.stdout.write("%s & %s" % (sr_name, toks[0]))
        for col in toks[1:-1]:
          sys.stdout.write("& %s" % col)
        sys.stdout.write("& \\multicolumn{2}{l}{%s} \\\\ \\hline \n" % toks[-1][:toks[-1].index("\\\\")])
      else:
        print("%s & %s" % (sr_name, line[:-1]))

      #print("%s & %s" % (sr_name, line[:-1]))
      continue

    line=line.replace("\\hline", "")


    #Add newline and horizontal line after data rows
    if "Data" in line:
      line=line.replace(".0", "")
      line="%s \\\\ \\hline \n" % line[:-1]

    if ("\\\\" in line):
      if line.count('&') == num_rows_std:
        toks=line.split('&')
        for col in toks[:-1]:
          sys.stdout.write("& %s" % col)
        if "Data" in line:
          sys.stdout.write("& \\multicolumn{2}{l}{%s} \\\\ \\hline \n" % toks[-1][:toks[-1].index("\\\\")])
        else:
          sys.stdout.write("& \\multicolumn{2}{l}{%s} \\\\ \n" % toks[-1][:toks[-1].index("\\\\")])
      else:
        print("& %s" % line[:-1])
      continue      

    print("%s" % line[:-1])

  print("\\end{tabular}")
  print("\\end{center}")
  print("\\end{table}")

  if (print_header):
    print("\\end{document}")


def templatesError(infile, print_header=True):
  """Parses and pretty prints the Templates Error output tables"""

  SR_LINE = "50-100" #thing to check for the line that gets the sample name

  if (print_header):
    print("\\documentclass[a4paper,landscape]{article}")
    print("\\usepackage{fullpage}")
    print("\\usepackage{float}")
    print("\\usepackage{multicol}")
    print("\\usepackage{adjustbox}")
    print("\\begin{document}")


  #print("\\begin{table}[H]")
  print("\\begin{table}[ht!]")
  print("\\begin{center}")

  tabular_header_string=""
  tabular_header_printed=False
  sr_name=""

  for line in infile:
    if "{table}" in line:
      continue
    if "{center}" in line:
      continue
    if "{document}" in line:
      continue
    if "\\end{tabular}" in line:
      continue
    if "\\textbf" in line:
      #Get Signal Region Name
      sr_name=line[8:-3]
      #print("\\textbf{%s}" % sr_name)
      continue
    if "\\begin{tabular}" in line:
      #Add extra c| for Signal Region Title
      if not tabular_header_printed:
        print("%sl |%s" % (line[:line.index("{l |")+4], line[line.index("{l |")+4:-1]))
        tabular_header_printed=True
        line=next(infile)
        print("SR & %s" % line[:-1])
      else:
        line=next(infile)
      continue
    if SR_LINE in line:
      print("%s & %s" % (sr_name, line[:-1]))
      continue

    if ("\\\\" in line):
      print("& %s" % line[:-1])
      continue      

    print("%s" % line[:-1])

  print("\\end{tabular}")
  print("\\end{center}")
  print("\\end{table}")

  if (print_header):
    print("\\end{document}")


def templatesClosure(infile, print_header=True):
  """Parses and pretty prints the templates closure tables"""

  if (print_header):
    print("\\documentclass[a4paper,landscape]{article}")
    print("\\usepackage{fullpage}")
    print("\\usepackage{float}")
    print("\\usepackage{multicol}")
    print("\\usepackage{adjustbox}")
    print("\\begin{document}")


  #print("\\begin{table}[H]")
  print("\\begin{table}[ht!]")
  print("\\begin{center}")

  tabular_header_string=""
  tabular_header_printed=False
  sr_name=""

  for line in infile:
    if "{table}" in line:
      continue
    if "{center}" in line:
      continue
    if "{document}" in line:
      continue
    if "\\end{tabular}" in line:
      continue
    if "\\section" in line:
      #Get Signal Region Name
      toks=line.split()
      sr_name=toks[toks.index("statsplots") - 1]
      #print("\\textbf{%s}" % sr_name)
      continue
    if "\\begin{tabular}" in line:
      #Add extra c| for Signal Region Title
      if not tabular_header_printed:
        print("%sc|%s" % (line[:line.index("{c|")+3], line[line.index("{c|")+3:-1]))
        tabular_header_printed=True
        line=next(infile)
        print("SR & sample%s" % line[:-1])
      else:
        line=next(infile)
      continue
    if "Z Jets" in line:
      print("%s & %s" % (sr_name, line[:-1]))
      continue

    if "\\\\" in line:
      print("& %s" % line[:-1])
      continue      

    print(line[:-1])

  print("\\end{tabular}")
  print("\\end{center}")
  print("\\end{table}")

  if (print_header):
    print("\\end{document}")


def main():
  parser = argparse.ArgumentParser(add_help=False)

  parser.add_argument("-c", "--closure", help="parse and pretty print closure table", action="store_true")
  parser.add_argument("-s", "--strong_results", help="parse and pretty print strong results table", action="store_true")
  parser.add_argument("-w", "--ewk_results", help="parse and pretty print ewk results table", action="store_true")
  parser.add_argument("-e", "--templates_err", help="parse and pretty print templates error table", action="store_true")
  parser.add_argument("-h", "--header", help="Print latex headers", action="store_true")
  parser.add_argument("--help", help="Print help message and quit", action="store_true")
  parser.add_argument("infile", nargs='?', type=argparse.FileType('r'), default=sys.stdin)

  args=parser.parse_args()

  if (args.closure):
    templatesClosure(args.infile, args.header)
  elif (args.strong_results):
    resultsTable(args.infile, "strong", args.header)
  elif (args.ewk_results):
    resultsTable(args.infile, "ewk" ,args.header)
  elif (args.templates_err):
    templatesError(args.infile, args.header)
  else:
    parser.print_help()



if __name__ == "__main__":
  main();