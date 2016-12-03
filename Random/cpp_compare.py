import subprocess
import sys
import itertools

def run_cmd(cmd):
    return subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

def main():
    flags = "-std=c++17 -Ofast"
    time = "/usr/bin/time -f %e"
    cpp0 = sys.argv[1]
    cpp1 = sys.argv[2]

    cmdbase = "{} {}".format("g++", flags)

    cmd0 = "{} {}".format(cmdbase, "-o /tmp/x0.x")
    run_cmd(cmd0)
    et0 = float(run_cmd("{} /tmp/x0.x".format(time)).stderr)

    cmd1 = "{} {}".format(cmdbase, "-o /tmp/x1.x")
    run_cmd(cmd1)
    et1 = float(run_cmd("{} /tmp/x1.x".format(time)).stderr)





if __name__ == "__main__":
    sys.exit(main())
