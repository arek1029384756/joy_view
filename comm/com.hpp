#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <cstddef>
#include <array>

namespace comm {

    class com {

        static constexpr std::size_t cbInQueue  = 1024;
        static constexpr std::size_t cbOutQueue = 16;

        int m_fd;
        std::array<std::uint8_t, cbInQueue>  m_rxBuffer;
        std::array<std::uint8_t, cbOutQueue> m_txBuffer;

        int open(char *port, std::size_t baud) {
            close();

            m_fd = ::open(port, O_RDWR | O_NOCTTY | O_NDELAY);
            if(m_fd < 0) {
                return 0;
            } else {
                struct termios options;

                fcntl(m_fd, F_SETFL, 0);
                tcgetattr(m_fd, &options);

                switch(baud) {
                    case 9600:
                        cfsetispeed(&options, B9600);
                        cfsetospeed(&options, B9600);
                        break;
                    case 19200:
                        cfsetispeed(&options, B19200);
                        cfsetospeed(&options, B19200);
                        break;
                    case 38400:
                        cfsetispeed(&options, B38400);
                        cfsetospeed(&options, B38400);
                        break;
                    case 57600:
                        cfsetispeed(&options, B57600);
                        cfsetospeed(&options, B57600);
                        break;
                    case 115200:
                        cfsetispeed(&options, B115200);
                        cfsetospeed(&options, B115200);
                        break;
                    default:
                        cfsetispeed(&options, B300);
                        cfsetospeed(&options, B300);
                        break;
                }

                //8N1 frame
                options.c_cflag &= ~PARENB;
                options.c_cflag &= ~CSTOPB;
                options.c_cflag &= ~CSIZE;
                options.c_cflag |= CS8;

                //disable hardware flow control
                options.c_cflag &= ~CRTSCTS;

                //Enable receiver and local mode
                options.c_cflag |= (CLOCAL | CREAD);

                //Raw input
                options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

                //Disable parity checking
                options.c_iflag &= ~(INPCK | ISTRIP);

                //Don't map CR to NL or NL to CR
                options.c_iflag &= ~(ICRNL | INLCR);

                //Don't map uppercase to lowercase
                options.c_iflag &= ~IUCLC;

                //Don't ignore CR
                options.c_iflag &= ~IGNCR;

                //Ignore BREAK condition
                options.c_iflag |= IGNBRK;

                //Disable software flow control
                options.c_iflag &= ~(IXON | IXOFF | IXANY);

                //Raw output - other c_oflag bits ignored
                options.c_oflag &= ~OPOST;

                //Set new settings
                tcsetattr(m_fd, TCSANOW, &options);
            }

            return 1;
        }


        void close(void) {
            if(m_fd > 0) {
                ::close(m_fd);
            }
        }


        std::int32_t write(const std::list<std::uint8_t>& data) {
            std::int32_t iOutWrite;
            if(m_fd < 1) {
                return 0;
            }

            std::copy_n(data.begin(), data.size(), m_txBuffer.begin());
            iOutWrite = ::write(m_fd, m_txBuffer.data(), data.size());
            if(iOutWrite < 0) {
                return 0;
            }

            return iOutWrite;
        }

        int read(std::list<std::uint8_t>& data) {
            std::int32_t iInRead;

            if(m_fd < 1) {
                return 0;
            }

            iInRead = ::read(m_fd, m_rxBuffer.data(), m_rxBuffer.size());
            if(iInRead < 0) {
                return 0;
            } else {
                std::copy_n(m_rxBuffer.begin(),
                            iInRead,
                            data.end());
            }

            return 1;
        }

        void checkCommInput(std::size_t *lpInQue) {
            ::ioctl(m_fd, FIONREAD, lpInQue);
        }

    };

}
