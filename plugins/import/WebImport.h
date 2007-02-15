#include <string>
#include <qhttp.h>
#include <qeventloop.h>

class HttpContext :public QHttp {
  Q_OBJECT
public:
  bool status;
  int rqid;
  bool processed;
  bool redirected;
  bool isHtml;
  std::string newLocation;

  HttpContext();
  void setTimer(QTimer *timer);

public slots:
  void finished(int id, bool error);
  void headerReceived(const QHttpResponseHeader &);
  void timeout();
};
