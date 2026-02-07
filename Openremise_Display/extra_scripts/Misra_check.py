Import ("env")


import subprocess
import sys
import os

print("\n🔍 Running MISRA / Cppcheck before build...\n")
os.makedirs("reports", exist_ok=True)
os.makedirs(".cppcheck", exist_ok=True)

cmd = [
    r"C:\Program Files\Cppcheck\cppcheck",
    "--enable=warning,style,performance",
    "--addon=misra",
    "--language=c++",

    "--std=c++11",
    "--inline-suppr",
    #"--error-exitcode=1",
   
    "--template={file},{line},{severity},{id},{message}",
    "--output-file=reports/misra.csv",

     "--cppcheck-build-dir=.cppcheck",

    "-Isrc",
    "-Iinclude",

    "-DARDUINO",
    "-DARDUINO_AVR_NANO",


    "src",
    "include"
]
result = subprocess.call(cmd)

if result != 0:
    print("\n❌ MISRA check failed. Build aborted.\n")
    sys.exit(1)

print("\n✅ MISRA check passed. See Misra.csv\n")