/* THREAD POOL.cpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:10:34 PM
 * Last edited:
 *   2/8/2020, 1:23:49 PM
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
    max_in_queue(max_in_queue),
    wakeup(false)
{
    this->working = true;
    this->threads_waiting = 0;

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
            // Acquire the lock
            unique_lock<mutex> u_lock(this->batch_lock);

            // Get some references to class variables
            vector<PixelBatch>* queue = &this->batch_queue;
            bool* working = &this->working;

            // Signal the main thread that we're ready for more
            this->wake_cond.notify_all();

            // Wait until work is available OR we're stopping
            this->threads_waiting++;
            this->batch_cond.wait(u_lock, [working, queue](){ return !queue->empty() || !(*working); });
            this->threads_waiting--;

            // Acquire the first batch if and only if we didn't stop because of work
            if (this->working) {
                batch = this->batch_queue.at(this->batch_queue.size() - 1);
                this->batch_queue.pop_back();
            }
        }

        // Only do the work if we woke up because of work
        if (this->working) {
            this->render_batch(batch);
        }
    }
}

void ThreadPool::render_batch(const PixelBatch& batch) const {
    for (int y = batch.y2; y >= batch.y1; y--) {
        for (int x = batch.x1; x <= batch.x2; x++) {
            batch.out->operator[](y)[x] = batch.world->render_pixel(x, y, *batch.camera);
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

    this->batch_queue.push_back(batch);

    // Wake a thread up
    this->batch_cond.notify_one();
}
void ThreadPool::add_batch(int width, int height, const Camera& cam, const RenderWorld& world, Image& out, unsigned long& batch_index) {
    PixelBatch batch = this->get_batch(width, height, batch_index);
    batch.camera = &cam;
    batch.world = &world;
    batch.out = &out;

    this->add_batch(batch);
}


void ThreadPool::wait() {
    unique_lock<mutex> u_lock(this->batch_lock);

    // Wait for the conditional variable to be triggered
    bool& wakeup = this->wakeup;
    this->wake_cond.wait(u_lock, [wakeup](){ return wakeup; });
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
void ThreadPool::complete() {
    // Wait until all threads are waiting
    while (true) {
        {
            unique_lock<mutex> u_lock(this->batch_lock);
            // cout << "Batch empty? " << this->batch_queue.empty() << endl;
            // cout << "Threads waiting = " << this->threads_waiting << endl;
            if (this->batch_queue.empty() && this->threads_waiting == this->n_threads) {
                return;
            }
        }
    }
}