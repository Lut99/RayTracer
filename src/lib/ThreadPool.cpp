/* THREAD POOL.cpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:10:34 PM
 * Last edited:
 *   1/27/2020, 3:34:21 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ThreadPool class is a collection of threads that are functioning
 *   as a thread pool. The idea is that they execute the same function
 *   multiple times with different data, until the stop signal is given.
 *   This particular file is the implementation file for ThreadPool.hpp.
**/

#include <iostream>

#include "include/ThreadPool.hpp"

using namespace std;
using namespace RayTracer;


ThreadPool::ThreadPool(int num_of_threads, int batch_size, int max_in_queue)
    : n_threads(num_of_threads),
    batch_size(batch_size),
    max_in_queue(max_in_queue)
{
    this->working = true;

    // Create the correct amount of threads
    this->pool.resize(this->n_threads);
    for (int i = 0; i < this->n_threads; i++) {
        this->pool[i] = thread(&ThreadPool::worker, this, i);
    }
}
ThreadPool::~ThreadPool() {
    this->stop();
}

void ThreadPool::worker(int id) {
    // Run while directed to do so
    while (this->working) {
        // Create a scope to indicate the mutex duration
        PixelBatch batch;
        {
            unique_lock<mutex> u_lock(this->batch_lock);
            vector<PixelBatch>* queue = &this->batch_queue;
            bool* working = &this->working;
            this->batch_cond.wait(u_lock, [working, queue](){ return !queue->empty() || !(*working); });

            // Acquire the first batch if and only if we didn't stop because of work
            if (this->working) {
                batch = *this->batch_queue.end();
                this->batch_queue.pop_back();
                cout << "Thread #" << id << " received a batch!" << endl;
            }
        }

        // Only do the work if we woke up because of work
        if (this->working) {
            this->render_batch(batch);
        }
        cout << "Thread #" << id << " completed batch, waiting for new ones" << endl;
    }
}

void ThreadPool::render_batch(const PixelBatch& batch) const {
    for (int y = batch.y2; y >= batch.y1; y--) {
        for (int x = batch.x1; x <= batch.x2; x++) {
            batch.out->operator[](y)[x] = batch.world->render_pixel(x, y);
        }
    }
}

PixelBatch ThreadPool::get_batch(int width, int height, unsigned long& batch_index) const {
    PixelBatch batch;

    batch.x1 = batch_index % width;
    batch.y1 = batch_index / width;

    batch_index += this->batch_size - 1;
    // Bind batch_index to a maximum of (w - 1) * (h - 1)
    if (batch_index > width * height) {
        batch_index = width *  height - 1;
    }

    batch.x2 = batch_index % width;
    batch.y2 = batch_index / width;

    batch_index++;

    return batch;
}

bool ThreadPool::batch_queue_full() const {
    return this->batch_queue.size() == this->max_in_queue;
}

void ThreadPool::add_batch(const PixelBatch& batch) {
    // Acquire the lock
    std::unique_lock<mutex> u_lock(this->batch_lock);
    if (this->batch_queue.size() >= this->max_in_queue) {
        return;
    }

    cout << "Adding new batch to the list" << endl;

    this->batch_queue.push_back(batch);

    // Wake a thread up
    this->batch_cond.notify_one();
}

void ThreadPool::stop() {
    // Try to stop the threads
    {
        unique_lock<mutex> u_lock(this->batch_lock);
        this->working = false;
    }

    this->batch_cond.notify_all();
    
    for (int i = 0; i < this->n_threads; i++) {
        if (this->pool[i].joinable()) {
            this->pool[i].join();
        }
    }
}