#include <tulip/DataSet.h>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QVariantMap>
#include <tulip/Color.h>
#include <sstream>
#include <tulip/ForEach.h>

#include <sys/time.h>
#include <sys/resource.h>

#include <proc/readproc.h>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(tlp::Color)


#define START_TIMER(n) QDateTime n = QDateTime::currentDateTime()
#define DISP_TIMER(n) qWarning() << "Timer " << __STRING(n) << "= " << n.msecsTo(QDateTime::currentDateTime()) << "ms"
#define DISP_TIMER_AND_RESET(n) qWarning() << "Timer " << __STRING(n) << "= " << n.msecsTo(QDateTime::currentDateTime()) << "ms"; n = QDateTime::currentDateTime()
#define RESET_TIMER(n) n = QDateTime::currentDateTime()
#define DISP_MEM struct proc_t usage; look_up_our_self(&usage); qWarning() << "Memory: " << usage.vsize/1024/1024;
#define N 10000

int main() {

  qWarning() << "===========================";
  qWarning() << "       USING DATASETS";
  qWarning() << "===========================";

  {
    START_TIMER(storing_ints);
    tlp::DataSet data;
    for (int i=0;i<N;++i) {
      std::stringstream ss;
      ss << i;
      data.set<int>(ss.str(),i);
    }
    DISP_TIMER(storing_ints);

    START_TIMER(iterating_ints);
    std::pair<std::string,tlp::DataType *> val;
    forEach(val, data.getValues()) {
      int i;
      data.get<int>(val.first,i);
    }
    DISP_TIMER(iterating_ints);

    DISP_MEM;
  }

  {
    START_TIMER(storing_strings);
    tlp::DataSet data;
    for (int i=0;i<N;++i) {
      std::stringstream ss;
      ss << i;
      data.set<std::string>(ss.str(),ss.str());
    }
    DISP_TIMER(storing_strings);

    START_TIMER(iterating_strings);
    std::pair<std::string,tlp::DataType *> val;
    forEach(val, data.getValues()) {
      std::string i;
      data.get<std::string>(val.first,i);
    }
    DISP_TIMER(iterating_strings);

    DISP_MEM;
  }

  {
    START_TIMER(storing_colors);
    tlp::DataSet data;
    for (int i=0;i<N;++i) {
      std::stringstream ss;
      ss << i;
      data.set<tlp::Color>(ss.str(),tlp::Color(0,0,0));
    }
    DISP_TIMER(storing_colors);

    START_TIMER(iterating_colors);
    std::pair<std::string,tlp::DataType *> val;
    forEach(val, data.getValues()) {
      tlp::Color i;
      data.get<tlp::Color>(val.first,i);
    }
    DISP_TIMER(iterating_colors);

    DISP_MEM;
  }

  qWarning() << "===========================";
  qWarning() << "         USING QT";
  qWarning() << "===========================";
  {
    START_TIMER(storing_ints);
    QVariantMap map;
    for (int i=0;i<N;++i)
      map[QString::number(i)] = i;
    DISP_TIMER(storing_ints);

    START_TIMER(iterating_ints);
    foreach(QString k,map.keys())
      map[k];
    DISP_TIMER(iterating_ints);

    DISP_MEM;
  }


  {
    START_TIMER(storing_strings);
    QVariantMap map;
    for (int i=0;i<N;++i) {
      QVariant val;
      val.setValue(QString::number(i).toStdString());
      map[QString::number(i)] = val;
     }
    DISP_TIMER(storing_strings);

    START_TIMER(iterating_strings);
    foreach(QString k,map.keys())
      map[k];
    DISP_TIMER(iterating_strings);

    DISP_MEM;
  }

  {
    START_TIMER(storing_colors);
    QVariantMap map;
    for (int i=0;i<N;++i) {
      QVariant val;
      val.setValue(tlp::Color(0,0,0));
      map[QString::number(i)] = val;
     }
    DISP_TIMER(storing_colors);

    START_TIMER(iterating_colors);
    foreach(QString k,map.keys())
      map[k];
    DISP_TIMER(iterating_colors);

    DISP_MEM;
  }
}
