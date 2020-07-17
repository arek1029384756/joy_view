#ifndef COMM_THREAD_HPP
#define COMM_THREAD_HPP

#include <string>
#include <base_thread.hpp>
#include <com.hpp>
#include <screen.h>

namespace comm {

    class CommThread : public task::BaseThread<const gui_qt::Screen&> {
        com m_com;

        void run(const gui_qt::Screen& screen) {
            try {
                constexpr qreal R = 90;
                qreal angle = 0;
                while(isRunning()) {
                    screen.notify(QPointF(R * std::cos(angle),
                                          R * std::sin(angle)),
                                 angle);
                    angle += 0.006;
                    msSleep(2);
                }
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
            }
        }

        public:
        virtual ~CommThread() = default;
    };

}

#endif
