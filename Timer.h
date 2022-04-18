#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>
#include <thread>
#include <functional>

class Timer
{ 
	//Our new thread
	std::thread newThread;

	bool timerRunning = false;
	
	//How many times should the function be run

	long executionCount = -1L;

	//How many times has the function been run in total
	
	long historicalCount = -1L;

	//How long should the wait be before executions
	
	std::chrono::milliseconds executionInterval = std::chrono::milliseconds(0);
	
	//A new container (and a function to return it) that can store any callable objects including functions, lambdas, etc...

	std::function<void(void)> functionContainer = nullptr;

	const std::function<void(void)> &getFunctionContainer() const
	{
		return functionContainer;
	}

	//Checks if the timer is running, retrieves the function container, and executes the first function inside it

	void sleepNRun()
	{
		//Go to sleep while the main thread logs keys and fills up workingKeyLog

		std::this_thread::sleep_for(executionInterval);

		if (timerRunning)
		{
			getFunctionContainer()();
		}
	}

	//threadFunction will be executed on the new thread

	void threadFunction()
	{
		if (executionCount == Infinite)
		{
			while (timerRunning)
			{
				sleepNRun();
			}
		}
		else
		{
			while (historicalCount--)
			{
				sleepNRun();
			}
		}
	}

	public:
		
		static const long Infinite = -1L;

		Timer() {};

		Timer(const std::function<void()> &functionContainerP) :
			functionContainer(functionContainerP) {}
		
		Timer(
			const std::function<void()> &functionContainerP,
			const unsigned long &milisecondsP,
			const long executionCountP = Timer::Infinite) :

			functionContainer(functionContainerP),
			executionInterval(std::chrono::milliseconds(milisecondsP)),
			executionCount(executionCountP) {}
		
		void startTimer(bool asynch = true)
		{
			if (timerRunning)
			{
				return;
			}
			else
			{
				timerRunning = true;
				historicalCount = executionCount;
			}

			if (asynch)
			{
				newThread = std::thread(&Timer::threadFunction, this);
			}
			else
			{
				this->threadFunction();
			}
		}

		void stopTimer()
		{
			timerRunning = false;

			//Resync execution by waiting for the newly created thread to complete execution

			newThread.join();
		}

		void setContainerFunction(const std::function<void()> &functionContainerP)
		{
			functionContainer = functionContainerP;
		}

		void setExecutionCount(long executionCountP)
		{
			if (timerRunning)
			{
				return;
			}
			else
			{
				executionCount = executionCountP;
			}

		}

		void setHistoricalCount(long historicalCountP)
		{
			if (timerRunning)
			{
				return;
			}
			else
			{
				historicalCount = historicalCountP;
			}

		}

		void setExecutionInterval(const unsigned long& executionIntervalP)
		{
			if (timerRunning)
			{
				return;
			}
			else
			{
				executionInterval = std::chrono::milliseconds(executionIntervalP);
			}
		}

		long getHistoricalCount() { return historicalCount; }
		long getExecutionCount() { return executionCount; }
		bool isTimerRunning() { return timerRunning; }
		unsigned long getexecutionIntervalAsLong() const { return executionInterval.count(); }
};

#endif // TIMER_H_INCLUDED