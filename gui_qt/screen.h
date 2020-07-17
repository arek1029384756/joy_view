#ifndef SCREEN_H
#define SCREEN_H

#include <QDialog>
#include <QGraphicsScene>
#include <functional>
#include <map>
#include <screen_view.hpp>
#include <joystick.hpp>

namespace gui_qt {

    class Screen : public QDialog {
        Q_OBJECT

        private slots:
        void slotSetParams(const QPointF, qreal);

        public:
            Screen(QWidget *parent = 0);
            void keyPressEvent(QKeyEvent* e) override;
            void notify(const QPointF&, qreal) const;
            virtual ~Screen();

        signals:
        void signalSetParams(const QPointF, qreal) const;

        private:
            QGraphicsScene* m_scene;
            GraphicsView* m_view;
            Joystick *m_j1;
            Joystick *m_j2;

            void toggleGrid(int key) const;
            void setThickness(int key) const;
            void zoomScreen(int key) const;
            void scrollScreen(int key) const;
            void showAll(int key) const;
            void showOptimal(int key) const;

            void initScreen();

            inline static const std::map<int, std::function<void(Screen* const, int)>> m_keyCommands = {
                { Qt::Key_G,        &Screen::toggleGrid },
                { Qt::Key_A,        &Screen::showAll },
                { Qt::Key_O,        &Screen::showOptimal },
                { Qt::Key_1,        &Screen::setThickness },
                { Qt::Key_2,        &Screen::setThickness },
                { Qt::Key_3,        &Screen::setThickness },
                { Qt::Key_PageUp,   &Screen::zoomScreen },
                { Qt::Key_PageDown, &Screen::zoomScreen },
                { Qt::Key_Left,     &Screen::scrollScreen },
                { Qt::Key_Right,    &Screen::scrollScreen },
                { Qt::Key_Up,       &Screen::scrollScreen },
                { Qt::Key_Down,     &Screen::scrollScreen }
            };
    };

}

#endif
