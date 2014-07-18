#!/usr/bin/env python 
import os
import sys
import operator
import gzip
import math
from collections import defaultdict
from subprocess import Popen
from subprocess import PIPE
from subprocess import STDOUT
import shlex

kernels = {
'bb_gemm' : 'bb_gemm',
'fft' :
'fft1D_512,step1,step2,step3,step4,step5,step6,step7,step8,step9,step10,step11',
'md' : 'md,md_kernel',
'pp_scan' : 'pp_scan,local_scan,sum_scan,last_step_scan',
'reduction' : 'reduction',
'ss_sort' : 'ss_sort,init,hist,local_scan,sum_scan,last_step_scan,update',
'stencil' : 'stencil',
'triad' : 'triad',
}

def main (directory, source):
  
  os.chdir(directory)
  obj = source + '.llvm'
  opt_obj = source + '-opt.llvm'
  executable = source + '-instrumented'
  os.environ['WORKLOAD']=kernels[source]

  source_file = source + '.c'
  print directory
  os.system('clang -g -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -emit-llvm -o ' + obj + ' '  + source_file)
  os.system('opt -S -load=/group/vlsiarch/shao/Projects/llvm-trace/full-trace/full_trace.so -fulltrace ' + obj + ' -o ' + opt_obj)
  os.system('llvm-link -o full.llvm ' + opt_obj + ' /group/vlsiarch/shao/Projects/llvm-trace/profileFunc/trace_logger.llvm')
  os.system('llc -filetype=asm -o full.s full.llvm')
  os.system('gcc -fno-inline -o ' + executable + ' full.s -lm')
  os.system('./' + executable)

if __name__ == '__main__':
  directory = sys.argv[1]
  source = sys.argv[2]
  print directory, source
  main(directory, source)
