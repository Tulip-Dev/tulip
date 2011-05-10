#ifndef FILENAMEEDITORWIDGET_H
#define FILENAMEEDITORWIDGET_H

#include <QWidget>
#include <tulip/tulipconf.h>

namespace Ui {
    class FileNameEditorWidget;
}



namespace tlp{
    /**
  * @brief File selection widget.
  **/
    class TLP_QT_SCOPE FileNameEditorWidget : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
                Q_PROPERTY(QString filter READ filter WRITE setFilter)

    public:
                explicit FileNameEditorWidget(QWidget *parent = 0);
        ~FileNameEditorWidget();

        QString fileName() const;
        QString filter() const;
        QString getBasePath() const;

    public slots:
        void setFileName(const QString &);
        void setFilter(const QString &);
        void setBasePath(const QString &);

    signals:
        void fileNameChanged(const QString &);

    private slots:
        void buttonPressed();

    private:
        Ui::FileNameEditorWidget *ui;
        QString fileFilter;
        QString basePath;

    };
}
#endif // FILENAMEEDITORWIDGET_H
