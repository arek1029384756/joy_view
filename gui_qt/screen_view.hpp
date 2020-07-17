#ifndef SCREEN_VIEW_HPP
#define SCREEN_VIEW_HPP

#include <QGraphicsView>
#include <QWheelEvent>
#include <qmath.h>
#include <screen_params.hpp>

namespace gui_qt {

    class GraphicsView : public QGraphicsView {
        bool m_gridEnable;

        protected:
            void wheelEvent(QWheelEvent* e) override {
                if(e->modifiers() & Qt::ControlModifier) {
                    auto exp = (e->delta() > 0) ? g_scaleExp : -g_scaleExp;
                    auto factor = qPow(qreal(2), exp);
                    scale(factor, factor);
                    e->accept();
                } else {
                    QGraphicsView::wheelEvent(e);
                }
            }

            void drawBackground(QPainter* painter, const QRectF& rect) override {
                if(m_gridEnable) {
                    painter->setClipRect(rect);
                    painter->setPen(QPen(Qt::lightGray, 0));
                    const qreal details = transform().m11();
                    const std::size_t delta = (details > 0.5) ? 1 : 10;

                    for(std::size_t i = 0; i <= g_logScreenWidth; i += delta) {
                        painter->drawLine(L2P(i), 0, L2P(i), L2P(g_logScreenHeight));
                    }

                    for(std::size_t i = 0; i <= g_logScreenHeight; i += delta) {
                        painter->drawLine(0, L2P(i), L2P(g_logScreenWidth), L2P(i));
                    }
                }
            }

        public:
            GraphicsView(QGraphicsScene* scene)
                : QGraphicsView(scene), m_gridEnable(true) {}
            virtual ~GraphicsView() = default;

            void toggleGrid() {
                auto* const v = viewport();
                m_gridEnable = !m_gridEnable;
                v->update();
            }
    };

}

#endif
