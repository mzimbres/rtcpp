#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mutex;
std::condition_variable cv;
bool has_new_msg;
std::queue<std::string> msg_queue;

void logger()
{
  std::ofstream ofs("log.txt");
  for (;;) {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&] { return has_new_msg; });

    if (msg_queue.empty())
      break;

    while (!msg_queue.empty()) {
      ofs << msg_queue.front() << std::endl;
      msg_queue.pop();
    }
    has_new_msg = false;
  }
}

void post_msg(std::string msg = {})
{
    std::unique_lock<std::mutex> lock(mutex);
    msg_queue.push(msg);
    if (msg.empty())
      std::queue<std::string>{}.swap(msg_queue);
    has_new_msg = true;
    lock.unlock();
    cv.notify_one();
}

int main()
{
  using namespace std::literals::chrono_literals;

  std::thread logging_thread(logger);

  for (auto i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(20ms);
    post_msg("Message: " + std::to_string(i));
  }
  post_msg();
  logging_thread.join();
}

