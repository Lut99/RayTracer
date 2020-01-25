/* THREAD POOL.hpp
 *   by Lut99
 *
 * Created:
 *   1/25/2020, 5:11:12 PM
 * Last edited:
 *   1/25/2020, 6:14:21 PM
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
#include <condition_variable>

#include "RenderObject.hpp"
#include "Image.hpp"
#include "Camera.hpp"

namespace RayTracer {
    struct PixelBatch {
        int x1;
        int y1;
        int x2;
        int y2;

        RenderObject* world;
        Image* out;
    };

    class ThreadPool {
        private:
            int width;
            int height;
            
            int max_in_queue;
            int batch_size;

            bool working;

            const Camera *cam;

            /* The list of thread objects */
            std::vector<std::thread> pool;

            /* Queue for the batches */
            std::vector<PixelBatch> batch_queue;
            /* Mutex object for to access the queue */
            std::mutex batch_lock;
            /* Condition variable to access the queue */
            std::condition_variable batch_cond;

            /* Thread worker function */
            void worker();
            /* Function for rendering one batch of pixels */
            void render_batch(const PixelBatch& batch) const;

            /* Returns a the indices for a new batch */
            PixelBatch get_batch(unsigned long& batch_index) const;
        public:
            /* The ThreadPool class is used for multicore rendering. */
            ThreadPool(int batch_size, const Camera& cam, int max_in_queue=5);
            ~ThreadPool();

            /* Render one image */
            Image render(const RenderObject& world);

            /* Stops the threadpool and waits until all threads have joined. */
            void stop();
    };
}

#endif