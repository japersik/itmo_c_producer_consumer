#include "producer_consumer.h"
#include <pthread.h>
#include <iostream>
#include <queue>

using namespace std;

// numbers Q
queue<int> int_queue;
pthread_mutex_t mutex_queue;

struct th_cons_param {
  int time_sleep = 0;
  bool debug = false;
};

__thread int* TLS_id;

int get_tid() {
  static int counter = 0;
  if (TLS_id == nullptr) {
    TLS_id = new int;
    *TLS_id = ++counter;
  }
  return *TLS_id;
}

void* producer_routine(void* arg) {
  cout << "Producer " << get_tid() << endl;
  istream* p_input_stream = static_cast<istream*>(arg);
  istream& input_stream = *p_input_stream;
  int value = 0;

  while (input_stream >> value && !input_stream.fail()) {
    pthread_mutex_lock(&mutex_queue);
    int_queue.push(value);
    pthread_mutex_unlock(&mutex_queue);
  }
  // read data, loop through each value and update the value, notify consumer,
  // wait for consumer to process
  return nullptr;
}

void* consumer_routine(void* arg) {
  cout << "Consumer " << get_tid() << endl;
  (void)arg;

  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
  th_cons_param* t_param = (th_cons_param*)arg;

  int* local_sum = new int;
  for (;;) {
    if (!int_queue.empty()) {
      pthread_mutex_lock(&mutex_queue);
      *local_sum += int_queue.front();
      int_queue.pop();
      if (t_param->debug) {
        cout << get_tid() << " " << *local_sum << endl;
      }
      pthread_mutex_unlock(&mutex_queue);
    }
    // sleep
  }
  // for every update issued by producer, read the value and add to sum
  // return pointer to result (for particular consumer)
  return local_sum;
}

void* consumer_interruptor_routine(void* arg) {
  cout << "Interruptor " << get_tid() << endl;
  (void)arg;
  // interrupt random consumer while producer is running
  return nullptr;
}

// the declaration of run threads can be changed as you like
int run_threads(int th_number, int time_sleep, bool debug,
                std::istream& in_stream) {
  // start N threads and wait until they're done
  // return aggregated sum of values

  // init mutex
  pthread_mutex_init(&mutex_queue, NULL);

  // start producer. arg - istream
  pthread_t th_producer;
  pthread_create(&th_producer, NULL, &producer_routine, &in_stream);

  // start consumers. arg - debug,delay
  pthread_t* th_consumers = new pthread_t[th_number];
  th_cons_param* args = new th_cons_param[th_number];
  for (int i = 0; i < th_number; ++i) {
    args[i].debug = debug;
    args[i].time_sleep = time_sleep;
    pthread_create(th_consumers + i, NULL, &consumer_routine, &args[i]);
  }

  // start interrupter. arg - consumer threads
  pthread_t th_interrupter;
  pthread_create(&th_interrupter, NULL, &consumer_interruptor_routine,
                 th_consumers);

  // wait and get all sum
  int all_sum = 0;
  int* sum = nullptr;
  for (int i = 0; i < th_number; ++i) {
    if (pthread_join(th_consumers[i], (void**)&sum)) {
      std::cout << "Error local sum reading" << std::endl;
    } else if (sum != nullptr) {
      all_sum += *sum;
    }
  }
  // clean data
  delete[] th_consumers;
  delete[] args;

  return all_sum;
}
