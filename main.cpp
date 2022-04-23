#include <cstring>
#include <iostream>

#include "producer_consumer.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "Usage: " << argv[0]
         << " [number of customers] [max sleep time(ms)] {-debug}" << endl;
    exit(1);
  }

  int th_number = atoi(argv[1]);
  int time_sleep = atoi(argv[2]);

  bool debug = false;
  if (argc == 4 && !strncmp(argv[3], "-debug", 6)) {
    debug = true;
  }
  // input params
  cout << th_number << endl << time_sleep << endl << debug << endl;
  cout << run_threads(th_number, time_sleep, debug) << std::endl;

  return 0;
}
