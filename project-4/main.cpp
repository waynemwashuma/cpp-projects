#include <iostream>
#include <vector>
#include <thread>
#include "thread.cpp"

int parse_int(char *parsable, std::string error_message)
{
  int num;
  try
  {
    num = std::stoi(parsable);
  }
  catch (std::exception const &e)
  {
    throw std::runtime_error(error_message);
  }
  return num;
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cout << "Not enough parameters were passed!" << std::endl;
    return 1;
  }
  int thread_num = parse_int(argv[1], "Invalid argument 1! It should be an integer."),
      iter_num = parse_int(argv[2], "Invalid argument 2! It should be an integer.");
  bool enable_mutex = (bool)parse_int(argv[3], "Invalid argument 1! It should be `0` for false, `1` for true).");
  std::vector<std::thread> thread_pool;

  for (int i = 0; i < thread_num; i++)
  {
    thread_pool.push_back(std::thread(thread_callable, iter_num, enable_mutex));
  }
  for (int i = 0; i < thread_num; i++)
  {
    thread_pool.at(i).join();
  }

  std::cout << "Actual count: " << count << std::endl;
  std::cout << "Expected count: " << safe_count << std::endl;

  return 0;
}
