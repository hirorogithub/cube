#ifndef __CUBE_EVENT_LOOP_H__
#define __CUBE_EVENT_LOOP_H__

#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>

namespace cube {

#define CUBE_OK 0
#define CUBE_ERR -1

typedef uint64_t TimerId;

class Eventor;
class Poller;
class TimerQueue;

class EventLoop {
    public:
        typedef std::function<void()> Task;

        EventLoop();
        ~EventLoop();

        // thread-safety
        void Post(const Task &task);

        void Loop();
        void RunOnce();

        void Stop();

        // wrap for poller
        void UpdateEvents(Eventor *e);
        void RemoveEvents(Eventor *e);

        // wrap for timer queue
        TimerId AddOneShotTimer(const Task &task, int64_t expiration_ms);
        TimerId AddRepeatedTimer(const Task &task, int64_t interval_ms);

        bool IsLoopThread() { return m_thread_id == std::this_thread::get_id(); }

        void WakeUp();

    private:
        void HandleEvents(int revents);

    private:
        // thread id
        std::thread::id m_thread_id;

        // wake up 
        int m_wakeup_fd;
        std::unique_ptr<Eventor> m_wakeup_eventor;

        // poller
        std::unique_ptr<Poller> m_poller;

        // timer queue
        std::unique_ptr<TimerQueue> m_timer_queue;
        
        // task
        std::mutex m_tasks_mutex;
        std::vector<Task> m_tasks;
        
        // running flag
        bool m_running;
};

}

#endif
