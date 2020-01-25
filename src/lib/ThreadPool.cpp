/* THREAD POOL.cpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:10:34 PM
 * Last edited:
 *   1/25/2020, 6:19:23 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ThreadPool class is a collection of threads that are functioning
 *   as a thread pool. The idea is that they execute the same function
 *   multiple times with different data, until the stop signal is given.
 *   This particular file is the implementation file for ThreadPool.hpp.
**/

#include "include/ThreadPool.hpp"

using namespace std;
using namespace RayTracer;


ThreadPool::ThreadPool(int batch_size, const Camera& cam, int max_in_queue)
    : batch_size(batch_size),
    max_in_queue(max_in_queue)
{
    this->cam = &cam;

    this->working = true;

    // Create the correct amount of threads
    for (int i = 0; i < CAMERA_THREADS; i++) {
        this->pool.push_back(thread(this->worker));
    }
}
ThreadPool::~ThreadPool() {
    this->stop();
}

void ThreadPool::worker() {
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
            batch.out[0][y][x] = cam->render_pixel(x, y, *batch.world);
        }
    }
}

PixelBatch ThreadPool::get_batch(unsigned long& batch_index) const {
    PixelBatch batch;

    batch.x1 = batch_index % this->width;
    batch.y1 = batch_index / this->width;

    batch_index += this->batch_size - 1;
    // Bind batch_index to a maximum of (w - 1) * (h - 1)
    if (batch_index > this->width * this->height) {
        batch_index = this->width *  this->height - 1;
    }

    batch.x2 = batch_index % this->width;
    batch.y2 = batch_index / this->width;

    batch_index++;

    return batch;
}


Image ThreadPool::render(const RenderObject& world) {
    // Keep pumping out batches in queue until we don't need to anymore
    unsigned long pixels_done = 0;
    unsigned long batch_index = 0;
    unsigned long to_do = this->width * this->height;
    while (pixels_done < to_do) {
        // If a the queue is full, continue
        if (this->batch_queue.size() >= this->max_in_queue) {
            continue;
        }

        // Create a new batch and append it
        this->batch_queue
    }
}


void ThreadPool::stop() {
    // Try to stop the threads
    {
        unique_lock<mutex> u_lock(this->batch_lock);
        this->working = false;
    }

    this->batch_cond.notify_all();
    
    for (int i = 0; i < CAMERA_THREADS; i++) {
        if (this->pool[i].joinable()) {
            this->pool[i].join();
        }
    }
}