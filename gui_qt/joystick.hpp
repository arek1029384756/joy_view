#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

namespace gui_qt {

    class Joystick : public QGraphicsItem {

        using QGraphicsItem::setX;
        using QGraphicsItem::setY;
        using QGraphicsItem::setPos;

        qreal m_length;
        qreal m_width;
        qreal m_margin;

        QPointF m_dotPos;
        qreal m_dotSize;

        static constexpr qreal MAXDOT = 25;
        static qreal bodyThick;
        static qreal termThick;

        static QPen getBodyPen() {
            return QPen(Qt::black, L2P(bodyThick), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        static QPen getTerminalPen() {
            return QPen(Qt::black, L2P(termThick), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }

        qreal getL() const {
            return m_length;
        }

        qreal getW() const {
            return m_width;
        }

        qreal getM() const {
            return m_margin;
        }

        static QPen getDrawingPen_T1() {
            return getTerminalPen();
        }

        public:
        static void setThickness(qreal thBody, qreal thTerm) {
            bodyThick = thBody;
            termThick = thTerm;
        }

        Joystick(qreal logLength,
                qreal logWidth,
                qreal logMargin)
            : m_length(L2P(logLength)),
            m_width(L2P(logWidth)),
            m_margin(L2P(logMargin)),
            m_dotPos(QPointF(0, 0)),
            m_dotSize(0) {

            setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
        }

        virtual ~Joystick() = default;

        void setComponentPos(const QPointF& position) {
            setPos(L2P(position));
        }

        void setDotParams(const QPointF& pos, const qreal& size) {
            m_dotPos = pos;
            m_dotSize = size;
            update();
        }

        virtual QRectF boundingRect() const override {
            return QRectF(0, 0, m_length, m_width);
        }

        virtual void paint(QPainter *painter,
                    const QStyleOptionGraphicsItem * opt,
                    QWidget *) override {
            painter->setClipRect(opt->exposedRect);
            painter->setBrush(Qt::NoBrush);
            painter->setPen(getBodyPen());
            painter->drawEllipse(QPointF(getL() / 2, getW() / 2), getL() / 2 - getM(), getW() / 2 - getM());
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawEllipse(QPointF(getL() / 2, getW() / 2), 1, 1);

            painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawEllipse(QPointF(getL() / 2, getW() / 2) + (m_dotPos * (getL() / 2 - getM())) / 100,
                                 (m_dotSize * MAXDOT) / 100 + 1,
                                 (m_dotSize * MAXDOT) / 100 + 1);
        }

    };

}

#endif
