#ifndef __THREAD_CPP__
#define __THREAD_CPP__

#include <iostream>
#include <mutex>
#include <cmath>
#include <windows.h>

static unsigned int count = 0; // Actual count after update by threads
std::mutex mutex_lock;
static unsigned int safe_count = 0;
std::mutex safe_mutex_lock;

void thread_callable(int iter_num, bool mutex_enabled)
{
  // calculate actual count.

  for (int i = 0; i < iter_num; i++)
  {
    if (mutex_enabled)
    {
      mutex_lock.lock();
    }
    // Critical section. The following loop is the CS. Do not change its’ code.
    for (int j = 0; j < 10; j++)
      count = count + 1; // + std::abs( (int)GetCurrentThreadId());
    if (mutex_enabled)
    {
      mutex_lock.unlock();
    }
  }

  // calculate expected count.
  safe_mutex_lock.lock();
  for (int i = 0; i < iter_num; i++)
  {
    // Critical section. The following loop is the CS. Do not change its’ code.
    for (int j = 0; j < 10; j++)
      safe_count += 1; // + std::abs( (int)GetCurrentThreadId());
  }
  safe_mutex_lock.unlock();
}

#endif