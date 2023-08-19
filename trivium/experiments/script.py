import subprocess
import random
import os
import sys
from sys import argv

N = int(argv[1])      # Number of attempts
tm = argv[2]          # Timeout for determination of satisfiability in seconds
prefix = argv[3]+"/"  # Path to dir with files of current experiment
name_res = argv[4]    # Name of file with result

len_reg = 288
len_stream = 300

path_use_trivium = "../use_trivium_noinit"
path_rand_input = ".rand_input"
path_rand_output = ".rand_output"
path_trivium_noinit = "../trivium_noinit.c"
path_trivium_assume = ".trivium_noinit_assume.c"
path_cnf = prefix + ".current.cnf"
path_temp_cnf = ".temp.cnf"
path_cbmc = "cbmc"
path_kissat = "/home/denis/SATsolvers/kissat-3.1.0-linux-amd64"
path_answer = ".SAT_answer"
path_res = prefix + name_res


def err_exit(msg):
  print(msg, file = sys.stderr)
  exit(1)

def generate_c_with_random():
  rand_list = [random.randint(0,1) for _ in range(len_reg)]
  with open(path_rand_input, "w") as file:
    for num in rand_list:
      file.write(str(num) + "\n")

  ret = subprocess.run([path_use_trivium, path_rand_input,  path_rand_output]) 
  if ret.returncode != 0:
    err_exit("Error when calling " + path_use_trivium)

  with open(path_trivium_noinit, "r") as in_file:
    with open(path_trivium_assume, "w") as out_file:
      for line in in_file:
        out_file.write(line)
        if line=="  /* Setting assume for all input bits (or some part of the bits) */\n":
          for i, num in enumerate(rand_list, 0):
            out_file.write(f"  __CPROVER_assume(input[{i}] == "+str(num)+");\n")
        if line=="   /* Setting assume for all stream bits (or some part of the bits) */\n":
          with open(path_rand_output, "r") as stream_file:
            nums = map(int, map(str.strip, stream_file.readlines()))
            for i, num in enumerate(nums, 0):
              out_file.write(f"  __CPROVER_assume(stream[{i}] == "+str(num)+");\n")

def remove_comments_cnf():
  with open(path_cnf, "r") as old_file:
    with open(path_temp_cnf, "w") as new_file:
      for line in old_file:
        if line.startswith("c"):
          break
        else:
          new_file.write(line)
  os.remove(path_cnf)
  os.rename(path_temp_cnf, path_cnf)

def generate_cnf():
  ret = subprocess.run([path_cbmc, path_trivium_assume, "--dimacs", "--outfile",
                        path_cnf], stdout = subprocess.DEVNULL)
  if ret.returncode != 0:
    err_exit("Error when calling " + path_cbmc)
  remove_comments_cnf()

def reduce_reg_in_cnf():
  with open(path_cnf, "r") as old_file:
    with open(path_temp_cnf, "w") as new_file:
      first = next(old_file).split()
      num_eq = int(first[-1])
      first[-1] = str(num_eq - 1)
      new_file.write(" ".join(first) + "\n")
      for i, line in enumerate(old_file, 1):
        if i != num_eq - len_stream:
          new_file.write(line)
  os.remove(path_cnf)
  os.rename(path_temp_cnf, path_cnf)
      
def check_satisfiability():
  with open(path_answer, "w") as file:
    ret = subprocess.run([path_kissat, "--time="+tm, path_cnf],
                          stdout = file)
    # 0: not found, 1: error, 10: SAT, 20: UNSAT
    if ret.returncode ==  1:
      err_exit("Error when calling " + path_kissat)
    elif ret.returncode == 0 or ret.returncode == 10 or ret.returncode == 20:
      return ret.returncode
    else:
      err_exit("Error: unknown code from kissat")

def parse_time():
  with open(path_answer, "r") as file:
    for line in file:
      if line.startswith("c process-time"):
        tokens = line.split()
        return tokens[-2]

def clean():
  os.remove(path_rand_input)
  os.remove(path_rand_output)
  os.remove(path_trivium_assume)
  os.remove(path_cnf)
  os.remove(path_answer)

def main():
  if not os.path.exists(prefix):
    os.mkdir(prefix)
  if os.path.exists(path_res):
    os.remove(path_res)

  total = []
  with open(path_res, "w") as res_file:
    for k in range(1, N + 1):
      res_file.write(f"==================== INPUT {k} ====================\n")
      print(f"==================== INPUT {k} ====================" )
      
      print("Generating .c file...")
      generate_c_with_random()

      print("Generating .cnf file...")
      generate_cnf()

      print("Running the solver on partial signification...")
      cnt = 0
      while (st := check_satisfiability()) != 0:
        if st == 20:
          err_exit(f"Error: UNSAT on step {cnt}")
        res_file.write(f"step {cnt:3}: {parse_time()} seconds\n")
        reduce_reg_in_cnf()
        cnt+=1
      res_file.write(f"step {cnt:3}: {parse_time()} seconds\n")
      print(f"Stop on step {cnt}")
      total.append(len_reg - cnt + 1)
 
    total = list(map(str, total)) 
    print(f"===================== TOTAL =====================")
    print(f"It was possible to solve in {int(tm)} seconds signify the first:")
    print(" ".join(total))
    res_file.write(f"===================== TOTAL =====================\n")
    res_file.write(f"It was possible to solve in {int(tm)} seconds signify the first:\n")
    res_file.write(" ".join(total) + "\n")
  clean()

main()

def test():
  #generate_c_with_random()
  #generate_cnf()
  check_satisfiability()
  #print(parse_time())

#test()
