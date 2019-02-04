#pragma once

#include <pthread.h>

#include <atomic>
#include <queue>

namespace pebble {

template<typename T>
class Queue {
public:
    Queue();
    explicit Queue(int max_size);
    ~Queue();

    // default blocking queue
    bool push(const T& obj, bool try_push = false);
    bool pop(T& obj, bool try_pop = false);
private:
    void init(int max_size);

    pthread_cond_t _q_no_empty;
    pthread_cond_t _q_no_full;
    pthread_mutex_t _qlock;
    std::queue<T> _queue;
    std::atomic_int _queue_size;
    int _max_size;
};

}
