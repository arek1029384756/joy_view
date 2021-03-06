#ifndef BASE_THREAD_HPP
#define BASE_THREAD_HPP

#include <thread>
#include <atomic>
#include <chrono>

namespace task {

    template<class... TArgs>
    class ThreadManagerInterface {
        public:
        virtual void start(TArgs&... args) = 0;
        virtual void stop() = 0;
    };

    class ThreadUserInterface {
        public:
        virtual bool isRunning() const = 0;
        virtual void msSleep(std::intmax_t ms) const = 0;
    };

    template<class... TArgs>
    class BaseThread : public ThreadManagerInterface<TArgs...>,
                       public ThreadUserInterface {
        std::atomic<bool> m_run;
        std::thread m_th;

        void join() {
            if(m_th.joinable()) {
                m_th.join();
            }
        }

        protected:
        virtual void run(TArgs&... args) = 0;

        bool isRunning() const override {
            return m_run.load();
        }

        void msSleep(std::intmax_t ms) const override {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

        ThreadUserInterface * getThreadUserInterface() {
            return this;
        }

        public:
        BaseThread() {
            m_run.store(false);
        }

        virtual ~BaseThread() {
            stop();
        }

        ThreadManagerInterface<TArgs...> * getThreadManagerInterface() {
            return this;
        }

        void start(TArgs&... args) override {
            if(!m_th.joinable()) {
                m_run.store(true);
                m_th = std::thread([&](TArgs&... args){ run(args...); }, std::ref(args)...);
            } else {
                throw std::runtime_error("Thread already running");
            }
        }

        void stop() override {
            m_run.store(false);
            join();
        }
    };

}

#endif
