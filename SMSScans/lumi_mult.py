#!/usr/bin/env python

import sys, math

multiplier=2

def main(file_list):
	"""Reads the datacards and multiplies each yield by the multiplier factor, whilst dividing each statistical uncertainty by sqrt(multiplier). It does this in two passes, the first pass does what was described in the previous sentence. The second pass ensures the count for gamma uncertainties match the value given in the count times the weight, but this is currently broken because combine changes the order of the rows vs. columns and this program assumes that the first nuisance to appear in the top row is the first nuisance to appear in the columns."""

	#should get folder inside DataCards/<folder>/card.txt
	sample_name = file_list[0][file_list[0].find("DataCards/")+10:file_list[0].find("/", file_list[0].find("DataCards/")+10)]

	print("Building x%d cards for %s" %(multiplier, sample_name))
	for f in file_list:
		fs_values=[]
		f_in = open(f, "r+")
		
		tmp_filename=f.replace(sample_name, "%s_tmp_x%d" % (sample_name, multiplier)) #Does not have double check on fsbkg 'rate'
		new_filename=f.replace(sample_name, "%s_x%d" % (sample_name, multiplier))

		print("file: %s \t tmp: %s \t new: %s" % (f, tmp_filename, new_filename))

		f_out = open(tmp_filename, "w+")

		#First Replace all the numbers with the either multiplied or divided value
		#we also need to save the numbers that should go in the rate column for the fsbkg
		#since combine checks that the value is the exact same as RSFOF*Kappa*GammaCount
		for line in f_in:		
			toks = line.split()
			if (len(toks) < 1):
				f_out.write(line)
				continue	
			if (toks[0] == "observation") or  (toks[0] == "rate"):
				"""Observations need to be multiplied by the multiplier"""
				l2=toks[0]+"   "
				for t in toks[1:]:
					if t != '-':
						#print("mutliplying %f to %f" % (float(t), float(t)*multiplier))
						l2+="%f     " % (float(t)*multiplier)
					else:
						l2+="%s     " % t
				if l2[-1] != '\n':
					l2+='\n'
				f_out.write(l2)
			elif "fsbkg_stat_syst" in toks[0]:
				"""FS Stat needs count to be multiplied by the multiplier"""
				l2=toks[0]+"   "+toks[1]+"  "
				l2+="%d      " % math.floor(float(toks[2])*multiplier)
				fs_values.append(math.floor(float(toks[2])*multiplier))
				for t in toks[3:]:
					if (t != '-'):
						fs_values[-1] *= float(t)
					l2+="%s     " % t
				if l2[-1] != '\n':
					l2+='\n'
				f_out.write(l2)
			elif "stat_syst" in toks[0]:
				"""The rest of the log normal stat uncs need to be divided by the sqrt(multiplier)"""
				l2=toks[0]+"   "+toks[1]+"  "
				for t in toks[2:]:
					if t != '-':
						l2+="%f     " % (1 + ((abs(1-float(t)))/math.sqrt(multiplier)))
					else:
						l2+="%s     " % t
				if l2[-1] != '\n':
					l2+='\n'
				f_out.write(l2)
			else:
				f_out.write(line)

		f_in.close()
		f_out.close()
		f_in = open(tmp_filename, "r")
		f_out = open(new_filename, "w+")

		fs_values_locations = []
		
		for line in f_in:
			#print(line)
			toks = line.split()
			if (len(toks) < 1):
				f_out.write(line)
				continue
			if (toks[0] == "rate"):
				l2=""
				for ind,t in enumerate(toks):
					if ((ind - 3) % 4) == 0:
						l2+="%s     " % fs_values[ind/4] #gives 0 for the first case, 1 for the second case, ect...
					else:
						l2+="%s     " % t
				if l2[-1] != '\n':
					l2+='\n'
				f_out.write(l2)
			else:
				f_out.write(line)

		f_in.close()
		f_out.close()	

if __name__ == "__main__":
	if (len(sys.argv) < 2):
		print("Usage: ./lumi_mult.py <path_to_file1> <path_to_file_2> ... ") 
		print("Also be sure that the cards are in SMSScans/DataCards/<sample_name>/") 
		print("And that there exists a directory <sample_name>_x2_tmp and <sample_name>_x2, replace 2 with the multiplier.")
	main(sys.argv[1:])