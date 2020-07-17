#include <QtWidgets>
#include <iostream>
#include <screen.h>

namespace gui_qt {

    qreal Joystick::bodyThick = 0.0;
    qreal Joystick::termThick = 0.0;

    Screen::Screen(QWidget *parent)
        : QDialog(parent) {

        m_scene = new QGraphicsScene();
        m_view = new GraphicsView(m_scene);

        m_scene->setSceneRect(0, 0, L2P(g_logScreenWidth), L2P(g_logScreenHeight));
        m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        m_view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
        m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

        m_view->showMaximized();

        setThickness(Qt::Key_1);
        grabKeyboard();

        initScreen();

        connect(this,
                SIGNAL(signalSetParams(const QPointF, qreal)),
                this,
                SLOT(slotSetParams(const QPointF, qreal)));
    }

    Screen::~Screen() {
        delete m_view;
        delete m_scene;
    }

    void Screen::initScreen() {
        m_j1 = new Joystick(50, 50, 1);
        m_j1->setComponentPos(QPointF(0, 0));
        m_j1->setFlag(QGraphicsItem::ItemIsMovable);
        m_scene->addItem(m_j1);

        m_j2 = new Joystick(50, 50, 1);
        m_j2->setComponentPos(QPointF(50, 0));
        m_scene->addItem(m_j2);
    }

    void Screen::toggleGrid(int /* key */) const {
        m_view->toggleGrid();
    }

    void Screen::setThickness(int key) const {
        if(key == Qt::Key_1) {
            Joystick::setThickness(0.3, 0.1);
        } else if(key == Qt::Key_2) {
            Joystick::setThickness(0.4, 0.2);
        } else if(key == Qt::Key_3) {
            Joystick::setThickness(0.6, 0.3);
        }
        m_scene->update();
    }

    void Screen::zoomScreen(int key) const {
        auto factor = qPow(qreal(2),
                (key == Qt::Key_PageUp) ? g_scaleExp  : -g_scaleExp);
        m_view->scale(factor, factor);
    }

    void Screen::scrollScreen(int key) const {
        constexpr int delta = 20;
        auto valH = m_view->horizontalScrollBar()->value();
        auto valV = m_view->verticalScrollBar()->value();

        if(key == Qt::Key_Left) {
            valH -= delta;
        } else if(key == Qt::Key_Right) {
            valH += delta;
        } else if(key == Qt::Key_Up) {
            valV -= delta;
        } else if(key == Qt::Key_Down) {
            valV += delta;
        }

        m_view->horizontalScrollBar()->setValue(valH);
        m_view->verticalScrollBar()->setValue(valV);
    }

    void Screen::showAll(int /* key */) const {
        m_view->fitInView(0,
                          0,
                          L2P(g_logScreenWidth),
                          L2P(g_logScreenHeight),
                          Qt::KeepAspectRatio);
    }

    void Screen::showOptimal(int /* key */) const {
        m_view->fitInView(m_scene->itemsBoundingRect(),
                          Qt::KeepAspectRatio);
    }

    void Screen::keyPressEvent(QKeyEvent* e) {
        const auto key = e->key();
        auto it = m_keyCommands.find(key);
        if(it != m_keyCommands.end()) {
            auto fun = std::bind(it->second, this, key);
            fun();
        }

        QDialog::keyPressEvent(e);
    }

    void Screen::notify(const QPointF& pos, qreal size) const {
        emit signalSetParams(pos, size);
    }

    void Screen::slotSetParams(const QPointF pos, qreal size) {
        m_j1->setDotParams(pos, size);
    }

}
