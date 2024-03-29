#include "producer_consumer.h"
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>
using namespace std;

// numbers Q
queue<int> int_queue;
pthread_mutex_t mutex_queue;
pthread_cond_t cond_queue;
bool can_finish;

pthread_barrier_t done_barrier;

struct th_cons_param {
  int time_sleep = 0;
  bool debug = false;
};

struct th_interrupter_param {
  int n;
  pthread_t* consumers;
};

__thread int* TLS_id;

int get_tid() {
  static int counter = 0;
  if (TLS_id == nullptr) {
    // create id on first call
    TLS_id = new int;
    *TLS_id = ++counter;
  }
  return *TLS_id;
}

void* producer_routine(void* arg) {
  //  cout << "Producer " << get_tid() << endl;
  istream* p_input_stream = static_cast<istream*>(arg);
  istream& input_stream = *p_input_stream;
  int value = 0;

  for (; input_stream >> value && !input_stream.fail();) {
    pthread_mutex_lock(&mutex_queue);
    pthread_cond_signal(&cond_queue);
    int_queue.push(value);
    pthread_mutex_unlock(&mutex_queue);
  }
  can_finish = true;
  pthread_mutex_lock(&mutex_queue);
  pthread_cond_broadcast(&cond_queue);
  pthread_mutex_unlock(&mutex_queue);
  return nullptr;
}

void* consumer_routine(void* arg) {
  //  cout << "Consumer " << get_tid() << endl;
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
  th_cons_param* t_param = (th_cons_param*)arg;

  int* local_sum = new int;
  *local_sum = 0;
  for (; !can_finish || !int_queue.empty();) {
    pthread_mutex_lock(&mutex_queue);
    while (!can_finish && int_queue.empty()) {
      pthread_cond_wait(&cond_queue, &mutex_queue);
    }
    if (!int_queue.empty()) {
      *local_sum += int_queue.front();
      int_queue.pop();
      if (t_param->debug) {
        cout << get_tid() << " " << *local_sum << endl;
      }
    }
    pthread_mutex_unlock(&mutex_queue);

    // random time [1;time_sleep] sleep
    if (t_param->time_sleep > 0) {
      usleep((rand() % t_param->time_sleep + 1) * 1000);  // 1 ms = 1000 mcs
    }
  }
  pthread_barrier_wait(&done_barrier);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  pthread_exit(static_cast<void*>(local_sum));
}

void* consumer_interruptor_routine(void* arg) {
  //  cout << "Interrupter " << get_tid() << endl;

  th_interrupter_param consumers_info = *((th_interrupter_param*)arg);
  // try to cancel
  while (!can_finish) {
    pthread_cancel(consumers_info.consumers[rand() % consumers_info.n]);
  }
  // interrupt random consumer while producer is running
  pthread_barrier_wait(&done_barrier);
  return nullptr;
}

// the declaration of run threads can be changed as you like
int run_threads(int th_number, int time_sleep, bool debug,
                std::istream& in_stream) {
  // init mutex
  pthread_mutex_init(&mutex_queue, NULL);
  pthread_cond_init(&cond_queue, NULL);
  can_finish = false;

  pthread_barrier_init(&done_barrier, NULL, th_number + 1);
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
  th_interrupter_param i_param = th_interrupter_param{th_number, th_consumers};
  pthread_create(&th_interrupter, NULL, &consumer_interruptor_routine,
                 &i_param);

  // wait and get all sum
  int all_sum = 0;
  int* sum = nullptr;
  for (int i = 0; i < th_number; ++i) {
    if (pthread_join(th_consumers[i], (void**)&sum)) {
      std::cout << "Error local sum reading" << std::endl;
    } else if (sum != nullptr) {
      all_sum += *sum;
      delete sum;
    }
  }
  // clean data
  delete[] th_consumers;
  delete[] args;
  pthread_mutex_destroy(&mutex_queue);
  pthread_cond_destroy(&cond_queue);
  pthread_barrier_destroy(&done_barrier);
  return all_sum;
}
