import csv
with open('filter.csv', newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in spamreader:
       values = row[0].split(",")
    lol = []
    for i in range (0,526+1):
    	lol.append([])
    for j in range(0,3+1):
	    for i in range(0, 526+1):
	    	try:
	    		lol[i].append(values[i+j*527])
	    	except:
	    		print("fuggedi")
	    		lol[i].append('0')

    print(lol)

    for i in range (0,526+1):
    	print("const short FIR_BANDPASS_" + str(i+1) + "[4] = {", end = " ")
    	for elem in lol[i]:
    		print(str(elem), end = ", ")
    	print("};")
