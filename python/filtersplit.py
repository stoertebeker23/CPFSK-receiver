import csv

with open('../matlab/filter.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
       values = row[0].split(",")
    coeff = []
    for i in range (0,526+1):
    	coeff.append([])
    for j in range(0,3+1):
	    for i in range(0, 526+1):
	    	try:
	    		coeff[i].append(values[i + j * 527])
	    	except:
	    		print("fuggedi")
	    		coeff[i].append('0')

    print(coeff)

    for i in range (0,526+1):
    	print("const short FIR_BANDPASS_" + str(i+1) + "[4] = {", end = " ")
    	for elem in coeff[i]:
    		print(str(elem), end = ", ")
    	print("};")
