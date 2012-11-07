#ifndef PYTHONEDITORSTABWIDGET_H
#define PYTHONEDITORSTABWIDGET_H

#include <QtCore/QMap>
#include <QtGui/QTabWidget>

#include <tulip/tulipconf.h>

namespace tlp {

class PythonCodeEditor;

class TLP_PYTHON_SCOPE PythonEditorsTabWidget : public QTabWidget {

    Q_OBJECT

    int _fontZoom;
    bool _dontTreatFocusIn;
    bool reloadCodeInEditorIfNeeded(int index);

public:

    explicit PythonEditorsTabWidget(QWidget *parent = 0);

    int addEditor(const QString &fileName = "");

    PythonCodeEditor *getCurrentEditor() const;

    PythonCodeEditor *getEditor(int) const;

    void indicateErrors(const QMap<QString, QVector<int> > &errorLines);

    void clearErrorIndicators();

    bool eventFilter(QObject *, QEvent *);

    void saveCurrentEditorContentToFile();

    void saveEditorContentToFile(int);

    void increaseFontSize();

    void decreaseFontSize();

signals:

    void tabAboutToBeDeleted(int);

    void fileSaved(int);

    void filesReloaded();

public slots:

    void scriptTextChanged();

    void reloadCodeInEditorsIfNeeded();

    void closeTabRequested(int tab);

};

}

#endif // PYTHONEDITORSTABWIDGET_H
