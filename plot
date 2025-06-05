#!/usr/bin/python3
import sys
import matplotlib.pyplot as plt
import csv

bins = []
mags = []

# ik this script is shit. I hate python.

with open(sys.argv[2], 'r') as f:
    reader = csv.reader(f)
    for row in reader:
        if len(row) < 2:
            continue
        bin_val = int(row[0])
        mag_str = row[1].strip()
        if sys.argv[1] == "-f":
            mag_val = float(mag_str) if mag_str != '' else 0  # convert empty to 0
        elif sys.argv[1] == "-i":
            mag_val = int(mag_str) if mag_str != '' else 0  # convert empty to 0
        else:
            print("You must supply a type argument! -f for float and -i for integers!")
            exit()
        bins.append(bin_val)
        mags.append(mag_val)

plt.plot(bins, mags)
plt.xlabel('Bin')
plt.ylabel('Magnitude')
plt.title('DFT Magnitude Spectrum')
plt.grid(True)
plt.show()
