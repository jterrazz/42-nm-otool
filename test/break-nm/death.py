#!/usr/bin/env python3

from colorama import Fore
import os
import sys
import subprocess
import signal

def kill(prog):
    dirs = ["corrupted_binaries/", "corrupted_archive/", "weird_binaries/", "weird_architecture/", "weird_archive/"]
    ok = 1
    for d in dirs:
        bombs = os.listdir(d)
        for b in bombs:
            #debug
            #print(b)
            r = subprocess.run(["../" + prog, d + b], capture_output=True)
            if (r.returncode == -11):
                print(Fore.RED + "SIGSEGV" + Fore.RESET + f": Your {prog} exploded on {b}")
                ok = 0

    if ok == 1:
        print(Fore.GREEN + f"Your {prog} seems to hold the pressure." + Fore.RESET)

def diff_dir(prog, directory):
    sysprog = "nm" if prog == "ft_nm" else "otool -t"
    files = os.listdir(directory)
    files = [directory + f for f in files]
    r = subprocess.run(["../" + prog] + files, capture_output=True)
    sys_r = subprocess.run([sysprog] + files, capture_output=True)
    if r.stdout != sys_r.stdout:
        print(Fore.RED + f"KO for {directory}" + Fore.RESET)
    else:
        print(Fore.GREEN + f"OK for {directory}" + Fore.RESET)

def diff_file(prog, f):
    sysprog = "nm" if prog == "ft_nm" else "otool -t"
    r = subprocess.run(["../" + prog, f], capture_output=True)
    sys_r = subprocess.run([sysprog, f], capture_output=True)
    if r.stdout != sys_r.stdout:
        print(Fore.RED + f"KO for {f}" + Fore.RESET)
    else:
        print(Fore.GREEN + f"OK for {f}" + Fore.RESET)
    
if __name__ == "__main__":
    #kill("ft_nm")
    #kill("ft_otool")
    #diff("ft_nm", "/bin/")
    diff_file("ft_nm", "../libft/libft.a")
    diff_file("ft_nm", "weird_archive/libO3.a")
    diff_dir("ft_nm", "feed_the_nm")
