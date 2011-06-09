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
#include "TulipItemDelegate.h"

#include <tulip/Color.h>

#include "tulip/ColorButton.h"
#include "tulip/TlpQtTools.h"
#include "tulip/FileNameEditorWidget.h"
#include "tulip/CoordWidget.h"
#include "tulip/SizeWidget.h"
#include "tulip/ColorButton.h"

#include "VectorEditionWidget.h"
#include "ElementCollection.h"
#include "TulipQVariantBuilder.h"

#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QTextLayout>

using namespace tlp;
using namespace std;

QWidget* TulipItemDelegate::createEditor(QWidget* p, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    QVariant data = index.data(Qt::EditRole);
    //Create the right editor in function of the data type.
    //If the data are int check if there is Glyph, Edge Extremity or Edge shape
    if(data.userType() == QVariant::Double){
        QLineEdit *lineEdit = new QLineEdit(p);
        lineEdit->setValidator(new QDoubleValidator(lineEdit));
        lineEdit->setText(QString::number(data.toDouble()));
        return lineEdit;
    }else
        //If data are QString check if there is file.
        if(data.userType() == QVariant::String){
            return QStyledItemDelegate::createEditor(p, option, index);
        }else
            //Create editor for user defined type.
            if(data.userType() == qMetaTypeId< Color >()){
                ColorButton* button =  new ColorButton(p);
                button->setColor(colorToQColor(data.value<Color>()));
                button->setFocusPolicy(Qt::StrongFocus);
                return button;
            }else
                if(data.userType() == qMetaTypeId< Coord >()){
                    CoordWidget *editor = new CoordWidget(p);
                    editor->setCoord(data.value<Coord>());
                    editor->setAutoFillBackground(true);
                    editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
                    editor->setFocusPolicy(Qt::StrongFocus);
                    return editor;
                }else
                    if(data.userType() == qMetaTypeId< Size >()){
                        SizeWidget * editor = new  SizeWidget(p);
                        editor->setSize(data.value<Size>());
                        editor->setAutoFillBackground(true);
                        editor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
                        editor->setFocusPolicy(Qt::StrongFocus);
                        return editor;
                    }else
                        if(data.userType() == qMetaTypeId< vector<bool > >()){
                            VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                            vectorWidget->setInterface(BOOLEAN_PROPERTY_RTTI,new ListPropertyWidgetTypeManger<BooleanType>(data.value<vector<bool> >()));
                            return vectorWidget;
                        }else
                            if(data.userType() == qMetaTypeId< vector<Color > >()){
                                VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                vectorWidget->setInterface(COLORPROPERTY_RTTI,new ListPropertyWidgetTypeManger<ColorType>(data.value<vector<Color> >()));
                                return vectorWidget;
                            }else
                                if(data.userType() == qMetaTypeId< vector<Coord > >()){
                                    VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                    vectorWidget->setInterface(LAYOUTPROPERTY_RTTI,new ListPropertyWidgetTypeManger<PointType>(data.value<vector<Coord> >()));
                                    return vectorWidget;
                                }else
                                    if(data.userType() == qMetaTypeId< vector<double > >()){
                                        VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                        vectorWidget->setInterface(DOUBLEPROPERTY_RTTI,new ListPropertyWidgetTypeManger<DoubleType>(data.value<vector<double> >()));
                                        return vectorWidget;
                                    }else
                                        if(data.userType() == qMetaTypeId< vector<int > >()){
                                            VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                            vectorWidget->setInterface(INTEGER_PROPERTY_RTTI,new ListPropertyWidgetTypeManger<IntegerType>(data.value<vector<int> >()));
                                            return vectorWidget;
                                        }else
                                            if(data.userType() == qMetaTypeId< vector<Size > >()){
                                                VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                                vectorWidget->setInterface(SIZEPROPERTY_RTTI,new ListPropertyWidgetTypeManger<SizeType>(data.value<vector<Size> >()));
                                                return vectorWidget;
                                            }else
                                                if(data.userType() == qMetaTypeId< vector<string > >()){
                                                    VectorEditionWidget *vectorWidget = new VectorEditionWidget(p);
                                                    vectorWidget->setInterface(STRINGPROPERTY_RTTI,new ListPropertyWidgetTypeManger<StringType>(data.value<vector<string> >()));
                                                    return vectorWidget;
                                                }else if(data.userType() == qMetaTypeId< ElementCollection >()){
                                                    ElementCollection *collection = new ElementCollection(data.value< ElementCollection >());
                                                    QComboBox *comboBox = new QComboBox(p);
                                                    comboBox->setModel(collection);
                                                    collection->setParent(comboBox);
                                                    QList<int> selection = collection->selectedElement();
                                                    if(!selection.empty()){
                                                        comboBox->setCurrentIndex(selection.front());
                                                    }
                                                    return comboBox;
                                                } else if(data.userType() == qMetaTypeId< FilteredUrl >()){
                                                    FilteredUrl url = data.value<FilteredUrl>();
                                                    return createFileNameEditor(p,url.path(),url.extensionsFilters());
                                                }  else{
                                                    return QStyledItemDelegate::createEditor(p, option, index);
                                                }
    return QStyledItemDelegate::createEditor(p, option, index);
}

void TulipItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                     const QModelIndex& index) const {
    QVariant data = index.data(Qt::EditRole);
    if(data.userType() == QVariant::Double){
        QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
        model->setData(index,QVariant(lineEdit->text().toDouble()));
    }else
        if(data.userType() == qMetaTypeId< Color >()){
            QVariant v;
            v.setValue<Color>(QColorToColor(((ColorButton*)editor)->color()));
            model->setData(index,v);
        }else
            if(data.userType() == qMetaTypeId< Coord >()){
                QVariant v;
                v.setValue<Coord>(((CoordWidget*)editor)->coord());
                model->setData(index,v);
            }else
                if(data.userType() == qMetaTypeId< Size >()){
                    QVariant v;
                    v.setValue<Size>(((SizeWidget*)editor)->size());
                    model->setData(index,v);
                }else
                    if(data.userType() == qMetaTypeId< vector<bool > >()){
                        QVariant v;
                        v.setValue<vector<bool > >(((ListPropertyWidgetTypeManger<BooleanType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                        model->setData(index, v);
                    }else
                        if(data.userType() == qMetaTypeId< vector<Color > >()){
                            QVariant v;
                            v.setValue<vector<Color > >(((ListPropertyWidgetTypeManger<ColorType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                            model->setData(index, v);
                        }else
                            if(data.userType() == qMetaTypeId< vector<Coord > >()){
                                QVariant v;
                                v.setValue<vector<Coord > >(((ListPropertyWidgetTypeManger<PointType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                                model->setData(index, v);
                            }else
                                if(data.userType() == qMetaTypeId< vector<double > >()){
                                    QVariant v;
                                    v.setValue<vector<double > >(((ListPropertyWidgetTypeManger<DoubleType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                                    model->setData(index, v);
                                }else
                                    if(data.userType() == qMetaTypeId< vector<int > >()){
                                        QVariant v;
                                        v.setValue<vector<int > >(((ListPropertyWidgetTypeManger<IntegerType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                                        model->setData(index, v);
                                    }else
                                        if(data.userType() == qMetaTypeId< vector<Size > >()){
                                            QVariant v;
                                            v.setValue<vector<Size > >(((ListPropertyWidgetTypeManger<SizeType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                                            model->setData(index, v);
                                        }else
                                            if(data.userType() == qMetaTypeId< vector<string > >()){
                                                QVariant v;
                                                v.setValue<vector<string > >(((ListPropertyWidgetTypeManger<StringType>*)((VectorEditionWidget*)editor)->getInterface())->getResultValue());
                                                model->setData(index, v);
                                            }else if(data.userType() == qMetaTypeId< ElementCollection >()){
                                                QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
                                                ElementCollection collection(*qobject_cast<ElementCollection*>(comboBox->model()));
                                                collection.setAllElementSelection(false);
                                                collection.setElementSelection(comboBox->currentIndex(),true);
                                                model->setData(index,QVariant::fromValue<ElementCollection>(collection));
                                            }else if(data.userType() == qMetaTypeId< FilteredUrl >()){
                                                FileNameEditorWidget *fileEditor = qobject_cast<FileNameEditorWidget*>(editor);
                                                FilteredUrl url(fileEditor->fileName());
                                                model->setData(index,QVariant::fromValue<FilteredUrl>(url));
                                            }else{
                                                QStyledItemDelegate::setModelData(editor, model, index);
                                            }
}

QWidget* TulipItemDelegate::createFileNameEditor(QWidget* parent, const QString& defaultFileName, const QString& filenameFilter) const
{
    FileNameEditorWidget *editor = new FileNameEditorWidget(parent);
    editor->setFileName(defaultFileName);
    editor->setFilter(filenameFilter);
    editor->setAutoFillBackground(true);
    return editor;
}

void TulipItemDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    QVariant data = index.data();
    //Get the normalized value of the element.
    QVariant normalizedValue = index.data(NormalizedValueRole);
    //If we have a normalized value represent it as an histogram.
    if(normalizedValue.isValid() && normalizedValue.type()==QVariant::Double){
        QStyleOptionViewItemV4 opt = static_cast<QStyleOptionViewItemV4>(option);
        initStyleOption(&opt,index);
        painter->save();

        //Change the color of the background if the element is selected
        if (opt.state & QStyle::State_Selected)
            painter->fillRect(opt.rect, opt.palette.highlight());

        //Draw histogram
        double normalization = normalizedValue.toDouble();
        QRect histogramFrame = opt.rect;
        QRect histogramRect(histogramFrame.topLeft(),QSize(histogramFrame.width()*normalization,histogramFrame.height()));
        painter->fillRect(histogramRect,QBrush(Qt::lightGray));
        //Draw value
        QString text = data.toString();
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect boundingRect = opt.rect;
        boundingRect.adjust(textMargin, 0, -textMargin, 0);
        //Change the color of the text if the element is selected
        QPen pen = painter->pen();
        if (opt.state & QStyle::State_Selected)
            pen.setColor(opt.palette.color(QPalette::HighlightedText));
        else
            pen.setColor(opt.palette.color(QPalette::Text));
        painter->setPen(pen);
        QTextOption textOption;
        const bool wrapText = opt.features & QStyleOptionViewItemV2::WrapText;
        textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
        textOption.setTextDirection(opt.direction);
        textOption.setAlignment(QStyle::visualAlignment(opt.direction, opt.displayAlignment));
        painter->drawText(boundingRect,text,textOption);
        painter->restore();
    }else{
        QStyledItemDelegate::paint(painter,option,index);
    }
}
