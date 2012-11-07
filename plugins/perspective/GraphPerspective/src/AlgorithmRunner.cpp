/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "AlgorithmRunner.h"

#include "AlgorithmRunnerItem.h"
#include "ExpandableGroupBox.h"

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QToolButton>

#include <tulip/TulipMimes.h>
#include <tulip/TulipSettings.h>

struct FavoriteBox: public ExpandableGroupBox {
    bool _droppingFavorite;

    explicit FavoriteBox(QWidget *parent = NULL, const QString &title=QString()): ExpandableGroupBox(parent,title), _droppingFavorite(false) {
    }

protected:
    void paintEvent(QPaintEvent *event) {
        ExpandableGroupBox::paintEvent(event);
        QPainter painter(this);
        QPixmap px((_droppingFavorite ? ":/tulip/graphperspective/icons/16/favorite.png" : ":/tulip/graphperspective/icons/16/favorite-empty.png"));
        painter.drawPixmap(20,0,px);
    }
};

#include "ui_AlgorithmRunner.h"

using namespace tlp;

static ExpandableGroupBox *createGroupBox(QString name, bool root = false) {
    ExpandableGroupBox* groupBox = new ExpandableGroupBox(NULL,name);
    groupBox->setProperty("root",root);
    QWidget* groupWidget = new QWidget();
    groupWidget->setLayout(new QVBoxLayout);
    groupWidget->layout()->setContentsMargins(0,15,0,6);
    groupWidget->layout()->setSpacing(0);
    groupBox->setWidget(groupWidget);
    return groupBox;
}

void AlgorithmRunner::buildTreeUi(QWidget* w, PluginModel<tlp::Algorithm>* model, const QModelIndex& parent, bool root) {
    for (int i=0; i<model->rowCount(parent); ++i) {
        QModelIndex index = model->index(i,0,parent);
        QString name = model->data(index).toString();

        if (model->rowCount(index) > 0) {
            ExpandableGroupBox* groupBox = createGroupBox(name, root);
            w->layout()->addWidget(groupBox);
            buildTreeUi(groupBox->widget(),model,index);
        }
        else {
            AlgorithmRunnerItem* item = new AlgorithmRunnerItem(name);
            QObject::connect(_localModeButton,SIGNAL(clicked(bool)),item,SLOT(setLocalMode(bool)));
            w->layout()->addWidget(item);
        }
    }
}

void AlgorithmRunner::insertItem(QWidget* w, const QString& name) {
    const Plugin* plugin = PluginLister::pluginInformations(name.toStdString());
    QString category = plugin->category().c_str();
    QString group = plugin->group().c_str();
    delete plugin;

    ExpandableGroupBox* categoryBox = NULL, *groupBox = NULL;
    foreach(ExpandableGroupBox* box, w->findChildren<ExpandableGroupBox*>()) {
        if (box->title() ==  category) {
            categoryBox = box;
            break;
        }
    }

    if (categoryBox==NULL)
        return;

    if (!group.isEmpty()) {
        foreach(ExpandableGroupBox* box, categoryBox->findChildren<ExpandableGroupBox*>()) {
            if (box->title() == group) {
                groupBox = box;
                break;
            }
        }
    }

    if (!group.isEmpty() && groupBox == NULL) {
        groupBox = createGroupBox(group);
        QVBoxLayout *categoryLayout = static_cast<QVBoxLayout*>(categoryBox->widget()->layout());
        int index = 0;
        while (index < categoryLayout->count()) {
            ExpandableGroupBox *gb = dynamic_cast<ExpandableGroupBox*>(categoryLayout->itemAt(index)->widget());
            if (gb && group < gb->title()) {
                break;
            }
            ++index;
        }
        categoryLayout->insertWidget(index, groupBox);
    } else {
        groupBox = categoryBox;
    }

    AlgorithmRunnerItem* item = new AlgorithmRunnerItem(name);
    QObject::connect(_localModeButton,SIGNAL(clicked(bool)),item,SLOT(setLocalMode(bool)));
    QObject::connect(item,SIGNAL(favorized(bool)),this,SLOT(favorized(bool)));
    QVBoxLayout *groupLayout = static_cast<QVBoxLayout*>(groupBox->widget()->layout());
    int index = 0;
    while (index < groupLayout->count()) {
        AlgorithmRunnerItem* i = dynamic_cast<AlgorithmRunnerItem*>(groupLayout->itemAt(index)->widget());
        if (i && name < i->name()) {
            break;
        }
        ++index;
    }
    groupLayout->insertWidget(index, item);
}

void AlgorithmRunner::refreshTreeUi(QWidget* w) {
    QStringList visibleItems;
    foreach(AlgorithmRunnerItem* i, w->findChildren<AlgorithmRunnerItem*>()) {
        if (PluginLister::instance()->pluginExists(i->name().toStdString()))
            visibleItems.push_back(i->name());
        else {
            _favorites.removeAll(i);
            delete i;
        }
    }

    foreach(ExpandableGroupBox* gb, w->findChildren<ExpandableGroupBox*>()) {
        if (!gb->property("root").toBool() && gb->findChildren<AlgorithmRunnerItem*>().empty()) {
            delete gb;
        }
    }

    std::list<std::string> installedPlugins = PluginLister::instance()->availablePlugins<tlp::Algorithm>();
    for(std::list<std::string>::iterator it = installedPlugins.begin(); it != installedPlugins.end(); ++it) {
        std::string name = *it;
        if (!visibleItems.contains(name.c_str())) {
            insertItem(w, name.c_str());
        }
    }
}

AlgorithmRunner::AlgorithmRunner(QWidget* parent): QWidget(parent), _ui(new Ui::AlgorithmRunner), _graph(NULL) {
    _ui->setupUi(this);
    _ui->favoritesBox->setWidget(new QWidget());
    _ui->favoritesBox->widget()->setAcceptDrops(true);
    _ui->favoritesBox->widget()->setMinimumHeight(75);
    _ui->favoritesBox->widget()->setLayout(new QVBoxLayout);
    _ui->favoritesBox->widget()->layout()->setContentsMargins(0,15,0,15);
    _ui->favoritesBox->widget()->layout()->setSpacing(5);
    _ui->favoritesBox->widget()->installEventFilter(this);

    _ui->contents->setEnabled(false);
    _localModeButton = new QToolButton(_ui->header);
    _localModeButton->setMaximumSize(25,25);
    _localModeButton->setMinimumSize(25,25);
    _localModeButton->setCheckable(true);
    _localModeButton->setChecked(true);
    _localModeButton->setIcon(QIcon(":/tulip/graphperspective/icons/16/hierarchy_add.png"));
    _localModeButton->setIconSize(QSize(22,22));
    _localModeButton->setToolTip(trUtf8("Always store result in local property"));
    _ui->header->mainFrame()->layout()->addWidget(_localModeButton);

    PluginModel<tlp::Algorithm> model;
    buildTreeUi(_ui->contents, &model, QModelIndex(), true);
    _ui->contents->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));
    foreach(AlgorithmRunnerItem* i, findChildren<AlgorithmRunnerItem*>()) {
        connect(i,SIGNAL(favorized(bool)),this,SLOT(favorized(bool)));
    }
    foreach(QString a, TulipSettings::instance().favoriteAlgorithms()) {
        addFavorite(a);
    }
}

AlgorithmRunner::~AlgorithmRunner() {
    delete _ui;
}

void AlgorithmRunner::setGraph(Graph* g) {
    _ui->contents->setEnabled(g != NULL);
    _graph = g;
    foreach(AlgorithmRunnerItem* item, findChildren<AlgorithmRunnerItem*>()) {
        if (item->graph() != g) {
            item->setGraph(g);
        }
    }
}

void AlgorithmRunner::findPlugins() {
    _ui->searchBox->selectAll();
    _ui->searchBox->setFocus(Qt::ShortcutFocusReason);
}

void AlgorithmRunner::refreshPluginsList() {
    refreshTreeUi(_ui->contents);
    setGraph(_graph);
}

void AlgorithmRunner::favorized(bool f) {
    AlgorithmRunnerItem* item = static_cast<AlgorithmRunnerItem*>(sender());

    if (f)
        addFavorite(item->name(),item->data());
    else
        removeFavorite(item->name());
}

// A not recursive implementation of QObject::findChildren
template<typename T>
QList<T> childrenObj(QObject* obj) {
    QList<T> result;
    foreach(QObject* o, obj->children()) {
        T var = dynamic_cast<T>(o);

        if (var != NULL)
            result+=var;
    }
    return result;
}
bool filterGroup(ExpandableGroupBox* group, QString filter) {
    QList<ExpandableGroupBox*> subGroups = childrenObj<ExpandableGroupBox*>(group->widget());
    QList<AlgorithmRunnerItem*> subItems = childrenObj<AlgorithmRunnerItem*>(group->widget());

    if (group->title().contains(filter,Qt::CaseInsensitive)) {
        group->show();
        foreach(ExpandableGroupBox* g, subGroups) {
            g->show();
            subItems+=childrenObj<AlgorithmRunnerItem*>(g->widget());
        }
        foreach(AlgorithmRunnerItem* i, subItems)
            i->show();
        return true;
    }

    bool groupVisible = false;
    foreach(ExpandableGroupBox* g, subGroups) {
        groupVisible |= filterGroup(g,filter);
    }
    foreach(AlgorithmRunnerItem* i, subItems) {
        bool itemVisible = i->name().contains(filter,Qt::CaseInsensitive);
        i->setVisible(itemVisible);
        groupVisible |= itemVisible;
    }
    group->setVisible(groupVisible);
    return groupVisible;
}

void AlgorithmRunner::setFilter(QString filter) {
    foreach(ExpandableGroupBox* group, childrenObj<ExpandableGroupBox*>(_ui->contents)) {
        if (group != _ui->favoritesBox)
            filterGroup(group,filter);
    }
    filterGroup(_ui->favoritesBox,filter);
}

bool AlgorithmRunner::eventFilter(QObject* obj, QEvent* ev) {
    bool draggableObject = obj == _ui->favoritesBox->widget() || _favorites.contains(dynamic_cast<AlgorithmRunnerItem*>(obj));

    if (ev->type() == QEvent::Paint) {
        if (obj == _ui->favoritesBox->widget() && _favorites.empty()) {
            QPainter painter(_ui->favoritesBox->widget());
            QPixmap px((_ui->favoritesBox->_droppingFavorite ? ":/tulip/graphperspective/icons/32/favorite.png" : ":/tulip/graphperspective/icons/32/favorite-empty.png"));
            painter.drawPixmap(_ui->favoritesBox->widget()->width()/2 - px.width()/2, 8, px);
            QFont f;
            f.setBold(true);
            painter.setFont(f);
            painter.setBrush(QColor(107,107,107));
            painter.setPen(QColor(107,107,107));
            painter.drawText(0,45,_ui->favoritesBox->widget()->width(),65535,Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap,trUtf8("Put your favorite algorithms here"));
        }
    }
    else if ((ev->type() == QEvent::DragEnter || ev->type() == QEvent::DragMove) && draggableObject) {
        QDropEvent* dropEv = static_cast<QDropEvent*>(ev);

        if (dynamic_cast<const AlgorithmMimeType*>(dropEv->mimeData()) != NULL) {
            _ui->favoritesBox->_droppingFavorite = true;
            ev->accept();
            _ui->favoritesBox->repaint();
        }

        return true;
    }
    else if (ev->type() == QEvent::DragLeave && draggableObject) {
        _ui->favoritesBox->_droppingFavorite = false;
        _ui->favoritesBox->repaint();
    }
    else if (ev->type() == QEvent::Drop && draggableObject) {
        QDropEvent* dropEv = static_cast<QDropEvent*>(ev);
        const AlgorithmMimeType* mime = dynamic_cast<const AlgorithmMimeType*>(dropEv->mimeData());

        if (mime != NULL)
            addFavorite(mime->algorithm(),mime->params());

        _ui->favoritesBox->_droppingFavorite = false;
        _ui->favoritesBox->repaint();
    }

    return false;
}

void AlgorithmRunner::removeFavorite(const QString &algName) {
    foreach(AlgorithmRunnerItem* i, _favorites) {
        if (i->name() == algName) {
            _favorites.removeAll(i);
            i->deleteLater();
            foreach(AlgorithmRunnerItem* item, findChildren<AlgorithmRunnerItem*>())

                if (item != i && item->name() == algName)
                    item->setFavorite(false);

            break;
        }
    }
    TulipSettings::instance().removeFavoriteAlgorithm(algName);

    if (_favorites.isEmpty())
        _ui->favoritesBox->widget()->setMinimumHeight(75);
}

void AlgorithmRunner::addFavorite(const QString &algName, const DataSet &data) {
    if (!PluginLister::pluginExists(algName.toStdString()))
        return;

    TulipSettings::instance().addFavoriteAlgorithm(algName);

    foreach(AlgorithmRunnerItem* i, _favorites) {
        if (i->name() == algName)
            return;
    }

    _ui->favoritesBox->widget()->setMinimumHeight(0);
    AlgorithmRunnerItem* item = new AlgorithmRunnerItem(algName);
    item->setGraph(_graph);

    if (!data.empty()) {
        item->setData(data);
    }

    item->setFavorite(true);
    int itemPos = 0;
    foreach(AlgorithmRunnerItem* i, _ui->favoritesBox->widget()->findChildren<AlgorithmRunnerItem*>()) {
        if (i->name() > item->name()) {
            break;
        }

        ++itemPos;
    }
    static_cast<QBoxLayout*>(_ui->favoritesBox->widget()->layout())->insertWidget(itemPos,item);
    _favorites+=item;
    item->installEventFilter(this);
    item->setAcceptDrops(true);
    connect(item,SIGNAL(favorized(bool)),this,SLOT(favorized(bool)));

    foreach(AlgorithmRunnerItem* i, findChildren<AlgorithmRunnerItem*>()) {
        if (i != item && i->name() == algName)
            i->setFavorite(true);
    }
}
