#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

namespace skyfall
{
	class ThreadPool
	{
		typedef std::function<void()> work;

		enum class MessageType
		{
			NewJob,
			Terminate
		};
	private:
		std::vector<std::thread> threads;

		std::queue<std::pair<MessageType, work>> works;
		std::mutex mtx;

	public:
		explicit ThreadPool(std::size_t number_of_threads)
		{
			for (std::size_t i = 0; i < number_of_threads; i++)
			{
				this->threads.push_back(std::thread([](std::queue<std::pair<MessageType, work>>& messages, std::mutex& mtx) {

					while (true)
					{
						std::unique_lock<std::mutex> ul(mtx);

						if (!messages.empty()) //check if there is any job
						{
							if (messages.front().first == ThreadPool::MessageType::Terminate)
								break;
							else if (messages.front().first == ThreadPool::MessageType::NewJob)
							{
								std::function<void()> work = messages.front().second;
								messages.pop();
								ul.unlock();

								work();
							}
						}
					}

					}, std::ref(this->works), std::ref(this->mtx)));
			}
		}

		~ThreadPool()
		{
			this->works.push(std::make_pair(MessageType::Terminate, nullptr)); //push message for terminate all threads

			for (std::thread& thr : this->threads)
				thr.join(); //wait threads for end up all work 
		}

	public:
		void execute(std::function<void()> function)
		{
			works.push(std::make_pair(MessageType::NewJob, function));
		}
	};
}