#include "tulip/PluginProgressWidget.h"
#include "tulip/PluginProgressWidgetData.h"

using namespace tlp;
using namespace std;

PluginProgressWidget::PluginProgressWidget(QWidget *parent):QWidget(parent),SimplePluginProgress(),view(NULL),updateIterval(200),time(QTime::currentTime()),
ui(new Ui::PluginProgressWidget){
ui->setupUi(this);
connect(ui->stopPushButton,SIGNAL(clicked(bool)),this,SLOT(stopCompute()));
connect(ui->cancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelCompute()));
connect(ui->preview,SIGNAL(toggled(bool)),this,SLOT(changePreview(bool)));
}

PluginProgressWidget::PluginProgressWidget(View *view,int updateInterval,QWidget *parent) :
        QWidget(parent),SimplePluginProgress(),view(view),updateIterval(updateInterval),time(QTime::currentTime()),
    ui(new Ui::PluginProgressWidget)
{
    ui->setupUi(this);
    connect(ui->stopPushButton,SIGNAL(clicked(bool)),this,SLOT(stopCompute()));
    connect(ui->cancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelCompute()));
    connect(ui->preview,SIGNAL(toggled(bool)),this,SLOT(changePreview(bool)));
}

PluginProgressWidget::~PluginProgressWidget()
{
    delete ui;
}

//=====================================
void PluginProgressWidget::preview_handler(bool b) {
    if (ui->preview->isChecked() == b) return;
    ui->preview->setChecked(b);
}
//=====================================
void PluginProgressWidget::progress_handler(int i,int j) {
  if (state()!=TLP_CONTINUE) {
    return;
}
  if(time.msecsTo(QTime::currentTime())> updateIterval){
      ui->progressBar->setMaximum(j);
      ui->progressBar->setValue(i);
      qApp->processEvents();
      time = QTime::currentTime();
  }  
  if (view!=0 && isPreviewMode()) {
    view->init();
  }
}
//=====================================
void PluginProgressWidget::setComment(string msg) {
  ui->comment->setText(QString::fromUtf8(msg.c_str()));  
  qApp->processEvents();
}
//=====================================
void PluginProgressWidget::showPreview(bool flag) {
  if (flag)
    ui->preview->show();
  else
    ui->preview->hide();
}
//=====================================
void PluginProgressWidget::stopCompute(){
  SimplePluginProgress::stop();
}
//=====================================
void PluginProgressWidget::cancelCompute(){
  SimplePluginProgress::cancel();
}
//=====================================
void PluginProgressWidget::changePreview(bool b){
  SimplePluginProgress::setPreviewMode(b);
}
