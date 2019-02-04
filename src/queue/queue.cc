#include "queue.h"

namespace pebble {

template<typename T>
Queue<T>::Queue() {
    init(0);
}

template<typename T>
Queue<T>::Queue(int max_size) {
    init(max_size);
}

template<typename T>
Queue<T>::~Queue() {
    pthread_cond_destroy(&_q_no_empty);
    pthread_cond_destroy(&_q_no_full);
    pthread_mutex_destroy(&_qlock);
}

template<typename T>
void Queue<T>::init(int max_size) {
    _max_size = max_size;
    _queue_size.store(0);
    pthread_cond_init(&_q_no_empty, NULL);
    pthread_cond_init(&_q_no_full, NULL);
    pthread_mutex_init(&_qlock, NULL);
}

template<typename T>
bool Queue<T>::push(const T& obj, bool try_push) {
    pthread_mutex_lock(&_qlock);
    if (_max_size > 0) {
        // if _max_size <= 0, then queue size is unlimit
        while (_queue_size.load() >= _max_size) {
            if (try_push) {
                pthread_mutex_unlock(&_qlock);
                return false;
            }
            pthread_cond_wait(&_q_no_full, &_qlock);
        }
    }
    _queue.push(obj);
    _queue_size.fetch_add(1);
    pthread_cond_signal(&_q_no_empty);
    pthread_mutex_unlock(&_qlock);
    return true;
}

template<typename T>
bool Queue<T>::pop(T& obj, bool try_pop) {
    pthread_mutex_lock(&_qlock);
    while (_queue_size.load() <= 0) {
        if (try_pop) {
            pthread_mutex_unlock(&_qlock);
            return false;
        }
        pthread_cond_wait(&_q_no_empty, &_qlock);
    }
    obj = _queue.front();
    _queue.pop();
    _queue_size.fetch_sub(1);
    pthread_cond_signal(&_q_no_full);
    pthread_mutex_unlock(&_qlock);
    return true;
}

}
