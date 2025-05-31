#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

// Hàm thực hiện trong thread
void* threadfunc(void* thread_param)
{
    struct thread_data* data = (struct thread_data *) thread_param;

    // Ngủ trước khi lấy mutex
    usleep(data->wait_to_obtain_ms * 1000);

    if (pthread_mutex_lock(data->mutex) != 0) {
        ERROR_LOG("Failed to lock mutex\n");
        data->thread_complete_success = false;
        return data;
    }

    // Ngủ sau khi đã lock mutex
    usleep(data->wait_to_release_ms * 1000);

    if (pthread_mutex_unlock(data->mutex) != 0) {
        ERROR_LOG("Failed to unlock mutex\n");
        data->thread_complete_success = false;
        return data;
    }

    data->thread_complete_success = true;
    return data;
}

// Hàm khởi động thread
bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,
                                  int wait_to_obtain_ms, int wait_to_release_ms)
{
    struct thread_data* data = (struct thread_data*) malloc(sizeof(struct thread_data));
    if (!data) {
        ERROR_LOG("Failed to allocate memory for thread_data\n");
        return false;
    }

    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;

    int rc = pthread_create(thread, NULL, threadfunc, data);
    if (rc != 0) {
        ERROR_LOG("Failed to create thread\n");
        free(data);
        return false;
    }

    return true;
}

