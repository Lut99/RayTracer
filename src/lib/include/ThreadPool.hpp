/* THREAD POOL.hpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:11:12 PM
 * Last edited:
 *   2/2/2020, 6:12:28 PM
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

#ifndef CAMERA_THREADS
#define CAMERA_THREADS 0
#endif

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
            int width;
            int height;
            
            int max_in_queue;
            int n_threads;
            int batch_size;

            int threads_waiting;
            bool working;

            /* The list of thread objects */
            std::vector<std::thread> pool;

            /* Queue for the batches */
            std::vector<PixelBatch> batch_queue;
            /* Mutex object for to access the queue */
            std::mutex batch_lock;
            /* Condition variable to access the queue */
            std::condition_variable batch_cond;

            /* Thread worker function */
            void worker(int id);
            /* Function for rendering one batch of pixels */
            void render_batch(const PixelBatch& batch) const;
        public:
            /* The ThreadPool class is used for multicore rendering. */
            ThreadPool(int num_of_threads, int batch_size, int max_in_queue=5);
            ~ThreadPool();

            /* Returns whether the batch queue is full or not. */
            bool batch_queue_full() const;
            /* Adds a new batch to the queue, ready for processing. If the queue is full, returns without doing anything. */
            void add_batch(const PixelBatch& batch);
            /* Adds a new batch to the queue. Instead of supplying the batch, it is created and then populated using the given values. */
            void add_batch(int width, int height, const Camera& cam, const RenderWorld& world, Image& out, unsigned long& batch_index);
            /* Returns a the indices for a new batch */
            PixelBatch get_batch(int width, int height, unsigned long& batch_index) const;

            /* Stops the threadpool and waits until all threads have joined. */
            void stop();
            /* Stops the threadpool, but waits until the buffer is empty for it to happen. */
            void complete();
    };
}

#endif