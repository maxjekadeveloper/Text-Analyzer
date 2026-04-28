#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <queue>
#include <vector>
#include <functional>

namespace FileHandling {
// Thread pool for concurrent processing
class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    
    ~ThreadPool();
    
    /**
     * Enqueues a task to be executed by the thread pool.
     * 
     * @tparam F Function type
     * @tparam Args Argument types
     * @param f Function to execute
     * @param args Arguments to pass to the function
     * @return A future containing the result of the function
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) /*-> std::future<typename std::invoke_result_t<F, Args...>>*/ {
        using return_type = typename std::invoke_result_t<F, Args...>;
        
        // TODO: Implement task enqueuing
        // 1. Create a packaged task with the function and arguments
        // 2. Get the future from the task
        // 3. Add the task to the queue
        // 4. Notify a worker thread

        std::packaged_task<return_type()> task{std::bind(std::forward<F>(f), std::forward<Args>(args)...)};
        std::future<return_type> future = task.get_future();
        auto shared_task = std::make_shared<std::packaged_task<return_type()>>(std::move(task));
        {
            std::lock_guard<std::mutex> lock{queueMutex};
            tasks.emplace([shared_task]{ (*shared_task)(); });
            condition.notify_one();
        }
        return future;
    }
    
private:
    void work();

    std::vector<std::jthread> workers;
    std::queue<std::function<void()>> tasks;
    
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<char> stop;
};
}