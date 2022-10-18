import subprocess


f_high = 4426 
f_very_high = 44   

#############################################################
########################### Blade ###########################
#############################################################
VNFs_base_blade = 50
K_base_blade = 50 
failure_base_blade = 46080
boottime_base_blade = 1 
boottimes_blade = (1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.2, 4.5)
failuretimes_blade = (23040, 27648, 32256, 36864, 41472, 46080, 50688, 55296, 59904, 64512, 69120)
K_blade = (16, 20, 23, 26, 29, 32, 36, 39, 42, 45, 48) 
VNFs_blade = (7, 8, 10, 11, 12, 13, 15, 16, 17, 19, 20) 

file_boottime_blade_high = 'blade_boottime_70.txt'
file_boottime_blade_very_high = 'blade_boottime_20.txt'

file_failuretime_blade_high = 'blade_failuretime_70.txt'
file_failuretime_blade_very_high = 'blade_failuretime_20.txt'

file_K_blade_high = 'blade_K_70.txt'
file_K_blade_very_high = 'blade_K_20.txt'

file_N_blade_high = 'blade_N_70.txt'
file_N_blade_very_high = 'blade_N_20.txt' 


for a in boottimes_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, K_base_blade, a, failure_base_blade, seed, f_high, file_boottime_blade_high), shell=True) 
 
for a in boottimes_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, K_base_blade, a, failure_base_blade, seed, f_very_high, file_boottime_blade_very_high), shell=True)  

for a in failuretimes_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, K_base_blade, boottime_base_blade, a, seed, f_high, file_failuretime_blade_high), shell=True) 
 
for a in failuretimes_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, K_base_blade, boottime_base_blade, a, seed, f_very_high, file_failuretime_blade_very_high), shell=True)       

for a in K_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, a, boottime_base_blade, failure_base_blade, seed, f_high, file_K_blade_high), shell=True) 
 
for a in K_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_blade, a, boottime_base_blade, failure_base_blade, seed, f_very_high, file_K_blade_very_high), shell=True) 

for a in VNFs_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_blade, boottime_base_blade, failure_base_blade, seed, f_high, file_N_blade_high), shell=True) 
 
for a in VNFs_blade:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_blade, boottime_base_blade, failure_base_blade, seed, f_very_high, file_N_blade_very_high), shell=True)         
        

#############################################################
########################### NUC ###########################
#############################################################

VNFs_base_nuc = 200
K_base_nuc = 50
failure_base_nuc = 34560
boottime_base_nuc = 0.06666667 
boottimes_nuc = (0.06666667, 0.08000000, 0.09333333, 0.10666667, 0.12000000, 0.13333333, 0.14666667, 0.16000000, 0.17333333, 0.18666667, 0.20000000)  
failuretimes_nuc = (11520, 13824, 16128, 18432, 20736, 23040, 25344, 27648, 29952, 32256, 34560) 
K_nuc = (2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6)  
VNFs_nuc = (52, 63, 73, 84, 94, 104, 115, 125, 136, 146, 156)

file_boottime_nuc_high = 'nuc_boottime_70.txt'
file_boottime_nuc_very_high = 'nuc_boottime_20.txt'

file_failuretime_nuc_high = 'nuc_failuretime_70.txt'
file_failuretime_nuc_very_high = 'nuc_failuretime_20.txt'

file_K_nuc_high = 'nuc_K_70.txt'
file_K_nuc_very_high = 'nuc_K_20.txt'

file_N_nuc_high = 'nuc_N_70.txt'
file_N_nuc_very_high = 'nuc_N_20.txt'  


for a in boottimes_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, K_base_nuc, a, failure_base_nuc, seed, f_high, file_boottime_nuc_high), shell=True) 
 
for a in boottimes_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, K_base_nuc, a, failure_base_nuc, seed, f_very_high, file_boottime_nuc_very_high), shell=True)  

for a in failuretimes_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, K_base_nuc, boottime_base_nuc, a, seed, f_high, file_failuretime_nuc_high), shell=True) 
 
for a in failuretimes_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, K_base_nuc, boottime_base_nuc, a, seed, f_very_high, file_failuretime_nuc_very_high), shell=True)       

for a in K_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, a, boottime_base_nuc, failure_base_nuc, seed, f_high, file_K_nuc_high), shell=True) 
 
for a in K_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_nuc, a, boottime_base_nuc, failure_base_nuc, seed, f_very_high, file_K_nuc_very_high), shell=True) 

for a in VNFs_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_nuc, boottime_base_nuc, failure_base_nuc, seed, f_high, file_N_nuc_high), shell=True) 
 
for a in VNFs_nuc:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_nuc, boottime_base_nuc, failure_base_nuc, seed, f_very_high, file_N_nuc_very_high), shell=True)  


#############################################################
########################### RPi ###########################
#############################################################

VNFs_base_rpi = 400
K_base_rpi = 50 
failure_base_rpi = 17280
boottime_base_rpi = 0.15
boottimes_rpi = (0.15, 0.18, 0.21, 0.24, 0.27, 0.30, 0.33, 0.36, 0.39, 0.42, 0.45)
failuretimes_rpi = (5760, 6912, 8064, 9216, 10368, 11520, 12672, 13824, 14976, 16128, 17280)  
K_rpi = (1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3) 
VNFs_rpi = (104 ,125, 146, 167, 188, 208, 229, 250, 271, 292, 312) 

file_boottime_rpi_high = 'rpi_boottime_70.txt'
file_boottime_rpi_very_high = 'rpi_boottime_20.txt'

file_failuretime_rpi_high = 'rpi_failuretime_70.txt'
file_failuretime_rpi_very_high = 'rpi_failuretime_20.txt'

file_K_rpi_high = 'rpi_K_70.txt'
file_K_rpi_very_high = 'rpi_K_20.txt'

file_N_rpi_high = 'rpi_N_70.txt'
file_N_rpi_very_high = 'rpi_N_20.txt'


for a in boottimes_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, K_base_rpi, a, failure_base_rpi, seed, f_high, file_boottime_rpi_high), shell=True) 
 
for a in boottimes_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, K_base_rpi, a, failure_base_rpi, seed, f_very_high, file_boottime_rpi_very_high), shell=True)  

for a in failuretimes_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, K_base_rpi, boottime_base_rpi, a, seed, f_high, file_failuretime_rpi_high), shell=True) 
 
for a in failuretimes_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, K_base_rpi, boottime_base_rpi, a, seed, f_very_high, file_failuretime_rpi_very_high), shell=True)       

for a in K_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, a, boottime_base_rpi, failure_base_rpi, seed, f_high, file_K_rpi_high), shell=True)  
 
for a in K_rpi:
    for seed in range (1, 51): 
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(VNFs_base_rpi, a, boottime_base_rpi, failure_base_rpi, seed, f_very_high, file_K_rpi_very_high), shell=True) 

for a in VNFs_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_rpi, boottime_base_rpi, failure_base_rpi, seed, f_high, file_N_rpi_high), shell=True) 
 
for a in VNFs_rpi:
    for seed in range (1, 51):
        subprocess.call('./a.out {} {} {} {} {} {} {}'.format(a, K_base_rpi, boottime_base_rpi, failure_base_rpi, seed, f_very_high, file_N_rpi_very_high), shell=True)  

        