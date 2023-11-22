#pragma once
#include <mutex>
#include <queue>
#include <list>
#include "Data.hpp"

namespace Network {
	// Simple mutex-guarded queue
	template<typename T1, typename T> class PairQueue
	{
	private:
		std::mutex mutex;
		std::vector<std::pair<Network::Digit, T>> queue;
	public:

        size_t size() {
            std::unique_lock<std::mutex> lock(mutex);
            return (queue.size());
        }

		void push(T1 id, T value)
		{
            //std::cout << "add msg client to queue" << std::endl;
			std::unique_lock<std::mutex> lock(mutex);
            queue.push_back({id, value});
		};

		// Get top message in the queue
		// Note: not exception-safe (will lose the message)
		std::pair<T1, T> pop()
		{
			std::unique_lock<std::mutex> lock(mutex);
			T value;
            Network::Digit id;
            std::swap(id, queue.front().first);
			std::swap(value, queue.front().second);
            if (queue.empty())
                queue.pop_back();
            else
                queue.erase(queue.begin());
            std::pair<Network::Digit, T> newPair = {id, value};
			return newPair;
		};

		bool empty() {
			std::unique_lock<std::mutex> lock(mutex);
			return queue.empty();
		}
	};
}
