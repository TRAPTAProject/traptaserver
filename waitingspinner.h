#ifndef WAITINGSPINNER_H
#define WAITINGSPINNER_H

#include <QWidget>

#include <QTimer>
#include <QColor>

class WaitingSpinner : public QWidget {
  Q_OBJECT
public:
  /*! Constructor for "standard" widget behaviour - use this
   * constructor if you wish to, e.g. embed your widget in another. */
  WaitingSpinner(QWidget *parent = 0);

  /*! Constructor - use this constructor to automatically create a modal
   * ("blocking") spinner on top of the calling widget/window.  If a valid
   * parent widget is provided, "centreOnParent" will ensure that
   * QtWaitingSpinner automatically centres itself on it, if not,
   * "centreOnParent" is ignored. */
    WaitingSpinner(Qt::WindowModality modality, QWidget *parent = 0,
                   bool centreOnParent = true);

public Q_SLOTS:
  void start();
  void stop();

public:
  void setColor(QColor color);
  void setRoundness(qreal roundness);
  void setMinimumTrailOpacity(qreal minOpacity);
  void setTrailFadePercentage(qreal trail);
  void setRevolutionsPerSecond(int rps);
  void setNumberOfLines(int lines);
  void setLineLength(int length);
  void setLineWidth(int width);
  void setInnerRadius(int radius);

  bool isSpinning() const;

private Q_SLOTS:
  void rotate();

protected:
  void paintEvent(QPaintEvent *ev);

private:
  static int calculateTimerInterval(int lines, int speed);
  static int lineCountDistanceFromPrimary(int current, int primary,
                                          int totalNrOfLines);
  static QColor currentLineColor(int distance, int totalNrOfLines,
                                 qreal trailFadePerc, qreal minOpacity,
                                 QColor color);

  void initialise();
  void updateSize();
  void updateTimer();
  void updatePosition();

private:
  // Configurable settings.
  QColor m_color;
  qreal m_roundness; // 0..100
  qreal m_minTrailOpacity;
  qreal m_trailFadePercentage;
  int m_revPerSec; // revolutions per second
  int m_numberOfLines;
  int m_lineLength;
  int m_lineWidth;
  int m_innerRadius;

private:
  WaitingSpinner(const WaitingSpinner&);
  WaitingSpinner& operator=(const WaitingSpinner&);
  QTimer *m_timer;
  QWidget *m_parent;
  bool m_centreOnParent;
  int m_currentCounter;
  bool m_isSpinning;
};

#endif // WAITINGSPINNER_H
