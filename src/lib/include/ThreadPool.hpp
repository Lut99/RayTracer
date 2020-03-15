/* THREAD POOL.hpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:11:12 PM
 * Last edited:
 *   2/9/2020, 5:37:31 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ThreadPool class is a collection of threads that are functioning
 *   as a thread pool. The idea is that they execute the same function
 *   multiple times with different data, until the stop signal is given.
 *   This particular file is the header file for ThreadPool.cpp.
**/

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "RenderWorld.hpp"
#include "Image.hpp"
#include "Camera.hpp"

namespace RayTracer {
    struct PixelBatch {
        int x1;
        int y1;
        int x2;
        int y2;

        const Camera* camera;
        const RenderWorld* world;
        Image* out;
    };

    class ThreadPool {
        private:
            unsigned int width;
            unsigned int height;
            
            unsigned int max_in_queue;
            unsigned int n_threads;
            unsigned int batch_size;

            unsigned int threads_waiting;
            bool working;

            /* The list of thread objects */
            std::vector<std::thread> pool;

            /* Queue for the batches */
            std::vector<PixelBatch> batch_queue;
            /* Mutex object for to access the queue */
            std::mutex batch_lock;
            /* Condition variable to access the queue */
            std::condition_variable batch_cond;
            /* Mutex object for signalling the farmer thread */
            std::mutex wake_lock;
            /* Condition variable to wake up the farmer thread */
            std::condition_variable wake_cond;

            /* Thread worker function */
            void worker(unsigned int id);
            /* Function for rendering one batch of pixels */
            void render_batch(const PixelBatch& batch) const;
        public:
            /* The ThreadPool class is used for multicore rendering. */
            ThreadPool(unsigned int num_of_threads, unsigned int batch_size, unsigned int max_in_queue=5);
            ~ThreadPool();

            /* Returns whether the batch queue is full or not. */
            bool batch_queue_full() const;
            /* Adds a new batch to the queue, ready for processing. If the queue is full, returns without doing anything. */
            void add_batch(const PixelBatch& batch);
            /* Adds a new batch to the queue. Instead of supplying the batch, it is created and then populated using the given values. */
            void add_batch(unsigned int width, unsigned int height, const Camera& cam, const RenderWorld& world, Image& out, unsigned long& batch_index);
            /* Returns a the indices for a new batch */
            PixelBatch get_batch(unsigned int width, unsigned int height, unsigned long& batch_index) const;

            /* Uses a conditional variable to pause the thread until any of the worker threads wake it up */
            void wait();

            /* Stops the threadpool and waits until all threads have joined. */
            void stop();
            /* Stops the threadpool, but waits until the buffer is empty for it to happen. */
            void complete();
    };
}

#endif