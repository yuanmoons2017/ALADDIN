#include "file_func.h"
#include "Datapath.h"
#include "Scratchpad.h"
#include "dddg.h"
#include <stdio.h>

int main( int argc, const char *argv[])
{
   const char *logo="     ________                                                    \n"  
                    "    /\\ ____  \\    ___   _       ___  ______ ______  _____  _   _ \n"
                    "   /  \\    \\  |  / _ \\ | |     / _ \\ |  _  \\|  _  \\|_   _|| \\ | |\n"
                    "  / /\\ \\    | | / /_\\ \\| |    / /_\\ \\| | | || | | |  | |  |  \\| |\n"
                    " | |  | |   | | |  _  || |    |  _  || | | || | | |  | |  | . ` |\n"
                    " \\ \\  / /__/  | | | | || |____| | | || |/ / | |/ /  _| |_ | |\\  |\n"
                    "  \\_\\/_/ ____/  \\_| |_/\\_____/\\_| |_/|___/  |___/  |_____|\\_| \\_/\n"
                    "                                                                 \n";

   std::cout << logo << endl;


  if(argc < 4)
  {
    std::cerr << "-------------------------------" << std::endl;
    std::cerr << "Aladdin takes:                 " << std::endl;
    std::cerr << "./aladdin <bench> <dynamic trace> <config file>" << endl;
    std::cerr << "-------------------------------" << std::endl;
    exit(0);
  }
  std::cerr << "-------------------------------" << std::endl;
  std::cerr << "      Starts Aladdin           " << std::endl;
  std::cerr << "-------------------------------" << std::endl;

  string bench(argv[1]);
  string trace_file(argv[2]);
  string config_file(argv[3]);
  
  cout << bench << "," << trace_file << "," << config_file <<  endl;
  /*Build Initial DDDG*/
  if (build_initial_dddg(bench, trace_file))
  {
    std::cerr << "-------------------------------" << std::endl;
    std::cerr << "       Aladdin Ends..          " << std::endl;
    std::cerr << "-------------------------------" << std::endl;
    exit(0);
  }
  parse_config(bench, config_file);
  
  
  Datapath *acc;
  Scratchpad *spad;

  spad = new Scratchpad(1); 
  acc = new Datapath(bench, CYCLE_TIME);
  acc->setScratchpad(spad);
  //get the complete graph
  acc->setGlobalGraph();
  acc->globalOptimizationPass();
  /*Profiling*/
  acc->clearGlobalGraph();
  acc->setGraphForStepping();
  
  //Scheduling
  while(!acc->step())
    spad->step();
  int cycles = acc->clearGraph();
  
  acc->dumpStats();
  delete acc;
  delete spad;
  return 0;
}
