#include <QApplication>
#include <iostream>
#include <memory>
#include <screen.h>
#include <comm_thread.hpp>

namespace {

    class App {
        int m_argc;
        char** m_argv;
        QApplication* m_qtApp;
        std::unique_ptr<gui_qt::Screen> m_screen;
        std::unique_ptr<comm::CommThread> m_commThread;

        public:
        App(int argc, char** argv, QApplication* const qtApp)
            : m_argc(argc), m_argv(argv), m_qtApp(qtApp) {}

        virtual ~App() = default;

        static const std::string& getVersion() {
            static const std::string ver = "1.0";
            return ver;
        }

        int run() {
            try {
                std::setlocale(LC_NUMERIC, "C");
                m_screen.reset(new gui_qt::Screen());
                m_commThread.reset(new comm::CommThread());
                m_commThread->start(std::ref(*m_screen));
                return m_qtApp->exec();
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
    };
}

int main(int argc, char** argv) {
    QApplication qtApp(argc, argv);
    App app(argc, argv, &qtApp);
    return app.run();
}
