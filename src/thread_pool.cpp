#include "thread_pool.hpp"
#include <iostream>

namespace FileHandling {
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
        while(numThreads > 0)
        {
            workers.emplace_back([this]{ work(); });
            numThreads--;
        }
    }
    
ThreadPool::~ThreadPool() {
        stop.store(1); // finish thread pool
        condition.notify_all();
}

void ThreadPool::work()
{
    while(true) {
        decltype(tasks)::value_type task;
        {
            std::unique_lock<std::mutex> lock{queueMutex};
            condition.wait(lock, [this]{ return !tasks.empty() || stop.load(); });

            if(tasks.empty() && stop.load())
                break;

            if(!tasks.empty()) {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }

        if(task) { // if we have the task 
            try {
                task();
            }
            catch(const std::exception& ex)
            {
                std::cout << "task " << std::this_thread::get_id() << " has failed with the error: " << ex.what() << std::endl;
            }
        }
    }
}
}