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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/TlpQtTools.h"
#include "tulip/Reflect.h"
#include "tulip/Color.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"

#include "tulip/ColorScaleWidget.h"
#include "tulip/ColorScaleConfigDialog.h"

#include <QtGui/qvalidator.h>
#include <QtGui/qdialog.h>
#include <QtGui/qframe.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qtextbrowser.h>
#include <QtGui/qevent.h>


using namespace tlp;


#define	INFO_MSG				"The following parameters are requested :"
#define NO_HELP_AVAILABLE_MSG	"No help is available for this parameter."
#define HELP_WELCOME_MSG		"Move mouse cursor over a parameter label to display its documentation ..."
#define	HELP_HEIGHT				200


namespace {

#undef TN
#define	TN( T )		typeid(T).name()

  typedef std::vector<std::string> stringA;

  class ColorScalePushButton : public QWidget {

  public :

    ColorScalePushButton(const QString &text, QWidget *parent):QWidget(parent){

      QHBoxLayout *layout = new QHBoxLayout;
      button=new QPushButton(text);
      colorScaleWidget= new ColorScaleWidget;
      colorScaleWidget->setColorScale(&colorScale);
      layout->addWidget(colorScaleWidget);
      layout->addWidget(button);
      setLayout(layout);
      layout->setMargin(0);

      dialog=new ColorScaleConfigDialog(&colorScale,this);
      connect(button,SIGNAL(clicked()),dialog,SLOT(exec()));
      connect(dialog,SIGNAL(accepted()),colorScaleWidget,SLOT(repaint()));
    }

    ColorScale *getColorScale(){return &colorScale;}

  protected :

    ColorScaleWidget *colorScaleWidget;
    QPushButton *button;
    ColorScale colorScale;
    ColorScaleConfigDialog *dialog;

  };

  int getAllProperties(	stringA	&outA,
			Graph *inG,
			PropertyInterface *inCurrent = 0	) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      std::string s = propIt->next();
      PropertyInterface * proxy = inG->getProperty( s );
      if( inCurrent && proxy == inCurrent )
	curIdx = outA.size();
      outA.push_back( s );
    }
    delete propIt;
    return curIdx;
  }

  int getPropertyOf(stringA &outA,
		    Graph *inG,
		    std::string inTypeName,
		    PropertyInterface *inCurrent = 0) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      std::string s = propIt->next();
      PropertyInterface * proxy = inG->getProperty( s );
      if( typeid((*proxy)).name() == inTypeName ) {
	if( inCurrent && proxy == inCurrent )
	  curIdx = outA.size();
	outA.push_back( s );
      }
    }
    delete propIt;
    return curIdx;
  }


  /*
    void OutputDataSet( const DataSet & inSet ) {
    Iterator< pair<string,DataType*> > * it = inSet.getValues();
    while( it->hasNext() ) {
      pair<string,DataType*> p;
      p = it->next();
      cout << p.first << " : " << (unsigned long)p.second->value << endl;
    } delete it;
  }
  */


  struct  IParam;
  typedef std::vector<IParam> IParamA;
  typedef std::vector<QWidget*> QWidgetA;

  struct IParam {
    std::string name;
    std::string typeName;
    void* value;
    QLabel* label; // Label widget
    QWidgetA wA;   // Input widgets
    std::vector<int> offY;
    QWidget* opt;
    std::string helpText;
  };


  struct QParamDialog : public QDialog {

    const StructDef* sysDef;
    StructDef *inDef;

    QParamDialog(const StructDef *sDef, StructDef *iDef,
		 QWidget * parent = 0)
      : QDialog( parent), sysDef(sDef), inDef(iDef) {
      helpBrowser = 0;
      curHelpParam = -1;
    }

    IParamA iparamA;
    QTextBrowser *helpBrowser;
    QPushButton *setDefB;
    QPushButton *restoreSysDefB;
    int	curHelpParam;

#define NONE_PROP " None"

    void setDefaults() {
      for( unsigned int i = 0 ; i < iparamA.size(); i++ ) {
	IParam & ip = iparamA[i];

	// bool
	if(ip.typeName == TN(bool)) {
	  QCheckBox * cb = (QCheckBox*) ip.wA[0];
	  inDef->setDefValue(ip.name, BooleanType::toString(cb->isChecked()));
	}

	// int
	// unsigned int
	// float
	// double
	else if(ip.typeName == TN(int) ||
		ip.typeName == TN(unsigned int) ||
		ip.typeName == TN(float) ||
		ip.typeName == TN(double)) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  inDef->setDefValue(ip.name, le->text().toUtf8().data());
	}

	// string
	else if(ip.typeName == TN(std::string)) {
	  if (ip.name.find("text::") != std::string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    inDef->setDefValue(ip.name, te->toPlainText().toUtf8().data());
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    inDef->setDefValue( ip.name, le->text().toUtf8().data() );
	  }
	}

	// Color
	else if( ip.typeName == TN(Color) )	{
	  QLineEdit * leR = (QLineEdit*) ip.wA[0];
	  QLineEdit * leG = (QLineEdit*) ip.wA[2];
	  QLineEdit * leB = (QLineEdit*) ip.wA[4];
	  QLineEdit * leA = (QLineEdit*) ip.wA[6];
	  int R			= leR->text().toInt();
	  int G			= leG->text().toInt();
	  int B			= leB->text().toInt();
	  int A			= leA->text().toInt();
	  Color c( R, G, B, A );
	  inDef->setDefValue(ip.name, ColorType::toString(c));
	}

	// Size
	else if( ip.typeName == TN(Size) )	{
	  QLineEdit * leW = (QLineEdit*) ip.wA[0];
	  QLineEdit * leH = (QLineEdit*) ip.wA[2];
	  QLineEdit * leD = (QLineEdit*) ip.wA[4];
	  float W	= leW->text().toFloat();
	  float H	= leH->text().toFloat();
	  float D	= leD->text().toFloat();
	  inDef->setDefValue(ip.name, SizeType::toString(Size(W,H,D)));
	}

	// PropertyInterface*
	else if (ip.typeName == TN(PropertyInterface*)
		 || ip.typeName == TN(BooleanProperty)
		 || ip.typeName == TN(DoubleProperty)
		 || ip.typeName == TN(LayoutProperty)
		 || ip.typeName == TN(StringProperty)
		 || ip.typeName == TN(IntegerProperty)
		 || ip.typeName == TN(SizeProperty)
		 || ip.typeName == TN(ColorProperty)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  std::string value = cb->currentText().toUtf8().data();
	  if (value != NONE_PROP)
	    inDef->setDefValue(ip.name, value);
	}
	
	// StringCollection
	else if (ip.typeName == TN(StringCollection)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  std::string current = cb->currentText().toUtf8().data();
	  std::string value(current);
	  for ( int i = 0; i < cb->count(); i++)
	    if (current != cb->itemText(i).toUtf8().data()){
	      value += ";";
	      value += cb->itemText(i).toUtf8().data();
	    }
	  inDef->setDefValue(ip.name, value);
	}
      }
    }

    void restoreSystemDefaults() {
      for( unsigned int i = 0 ; i < iparamA.size(); i++ ) {
	IParam & ip = iparamA[i];

	// bool
	if(ip.typeName == TN(bool)) {
	  QCheckBox * cb = (QCheckBox*) ip.wA[0];
	  bool isOn;
	  BooleanType::fromString(isOn, sysDef->getDefValue(ip.name));
	  cb->setChecked(isOn);
	}
	// int
	// unsigned int
	// float
	// double
	else if(ip.typeName == TN(int) ||
		ip.typeName == TN(unsigned int) ||
		ip.typeName == TN(float) ||
		ip.typeName == TN(double)) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  le->setText(QString(sysDef->getDefValue(ip.name).c_str()));
	}

	// string
	else if(ip.typeName == TN(std::string)) {
	  if (ip.name.find("text::") != std::string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    te->setText(QString::fromUtf8(sysDef->getDefValue(ip.name).c_str()));
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    le->setText(QString::fromUtf8(sysDef->getDefValue(ip.name).c_str()));
	  }
	}

	// Color
	else if( ip.typeName == TN(Color))	{
	  QLineEdit * leR = (QLineEdit*) ip.wA[0];
	  QLineEdit * leG = (QLineEdit*) ip.wA[2];
	  QLineEdit * leB = (QLineEdit*) ip.wA[4];
	  QLineEdit * leA = (QLineEdit*) ip.wA[6];
	  Color c;
	  ColorType::fromString(c, sysDef->getDefValue(ip.name));
	  leR->setText(QString("%1").arg(c.getR()));
	  leG->setText(QString("%1").arg(c.getG()));
	  leB->setText(QString("%1").arg(c.getB()));
	  leA->setText(QString("%1").arg(c.getA()));
	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(c.getR(), c.getG(), c.getB()));
	  ip.opt->setPalette(palette);
	}

	// Size
	else if( ip.typeName == TN(Size))	{
	  QLineEdit * leW = (QLineEdit*) ip.wA[0];
	  QLineEdit * leH = (QLineEdit*) ip.wA[2];
	  QLineEdit * leD = (QLineEdit*) ip.wA[4];
	  Size s;
	  SizeType::fromString(s, sysDef->getDefValue(ip.name));
	  leW->setText(QString("%1").arg(s.getW()));
	  leH->setText(QString("%1").arg(s.getH()));
	  leD->setText(QString("%1").arg(s.getD()));
	}

	// PropertyInterface*
	else if (ip.typeName == TN(PropertyInterface*)
		 || ip.typeName == TN(BooleanProperty)
		 || ip.typeName == TN(DoubleProperty)
		 || ip.typeName == TN(LayoutProperty)
		 || ip.typeName == TN(StringProperty)
		 || ip.typeName == TN(IntegerProperty)
		 || ip.typeName == TN(SizeProperty)
		 || ip.typeName == TN(ColorProperty)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  std::string value = sysDef->getDefValue(ip.name);
	  if (sysDef->isMandatory(ip.name))
	    cb->setCurrentIndex(0);
	  else {
	    if (value.empty())
	      value = NONE_PROP;
	  
	    for (int i = 0; i <  cb->count(); i++)
	      if (value == cb->itemText(i).toUtf8().data()) {
		cb->setCurrentIndex(i);
		break;
	      }
	  }
	}
	
	// StringCollection
	else if (ip.typeName == TN(StringCollection)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  StringCollection coll(sysDef->getDefValue(ip.name));
	  std::string current = coll.getCurrentString();
	  for ( int i = 0; i < cb->count(); i++)
	    if (current == cb->itemText(i).toUtf8().data()){
	      cb->setCurrentIndex(i);
	      break;
	    }
	}
      }
      *inDef = *sysDef;
    }

    bool eventFilter(QObject *obj, QEvent *e) {
      if (obj == setDefB) {
	if (e->type() == QEvent::MouseButtonRelease)
	  setDefaults();
	return false;
      } else if (obj == restoreSysDefB) {
	if (e->type() == QEvent::MouseButtonRelease)
	  restoreSystemDefaults();
	return false;
      }	
      if( e->type() == QEvent::MouseMove ) {
	IParam * ip = 0;
	for( unsigned int i = 0 ; i < iparamA.size(); i++ ) {
	  if( iparamA[i].label == obj ) {
	    ip = &iparamA[i];
	    break;
	  }
	}
	if( !ip )
	  return false;

	int ipIdx = ip - &iparamA[0];
	if( curHelpParam == ipIdx )
	  return false;
	curHelpParam = ipIdx;

	if( ip->helpText.size() )
	  helpBrowser->setHtml( ip->helpText.c_str() );
	else
	  helpBrowser->setText( NO_HELP_AVAILABLE_MSG );
      }

      if( e->type() == QEvent::MouseButtonRelease ) {
	IParam * ip = 0;
	for( unsigned int i = 0 ; i < iparamA.size(); i++ ) {
	  if( iparamA[i].opt == obj ) {
	    ip = &iparamA[i];
	    break;
	  }
	}
	if( !ip )
	  return false;

	if( ip->typeName == TN(Color) ) {
	  QLineEdit * leR = (QLineEdit*) ip->wA[0];
	  QLineEdit * leG = (QLineEdit*) ip->wA[2];
	  QLineEdit * leB = (QLineEdit*) ip->wA[4];
	  QLineEdit * leA = (QLineEdit*) ip->wA[6];
    QColor baseColor( leR->text().toInt(),leG->text().toInt(),leB->text().toInt(),leA->text().toInt());
    QColor color=baseColor;
    if(getColorDialog(baseColor,NULL,"Color chooser",color)) {
      leR->setText( QString("%1").arg(color.red()));
      leG->setText( QString("%1").arg(color.green()));
      leB->setText( QString("%1").arg(color.blue()));
      leA->setText( QString("%1").arg(color.alpha()));
	  }
	  QPalette palette;
    palette.setColor(QPalette::Button, color);
	  ((QWidget *)obj)->setPalette(palette);
	  return false;
	} else if( ip->typeName == TN(std::string) ) {
	  QString s;
	  if (ip->name.find("file::") == 0)
	    s = QFileDialog::getOpenFileName();
	  else // dir::
	    s = QFileDialog::getExistingDirectory();
	  if( s != QString::null ) {
	    QLineEdit * le = (QLineEdit*) ip->wA[0];
	    le->setText( s );
	  }
	}
      }
      return false;
    }

    bool fillIn(const DataSet *inSet, Graph* inG) {
      //
      // Parse inDef

      Iterator< std::pair<std::string,std::string> > * defIt;
      defIt = inDef->getField();

      while( defIt->hasNext() ) {
	std::pair<std::string,std::string> def;
	def = defIt->next();
	IParam ip;
	ip.name     = def.first;
	ip.typeName = def.second;
	ip.helpText = inDef->getHelp(ip.name);
	// first part of the parameter name may be used
	// to indicate a subtype
	std::string::size_type pos = def.first.find("::");
	if (pos != std::string::npos)
	  ip.label = new QLabel(def.first.substr(pos + 2, def.first.length() - pos - 2).c_str(), this);
	else
	  ip.label    = new QLabel( ip.name.c_str(), this );
	ip.label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
	ip.label->installEventFilter( this );
	ip.label->setMouseTracking( true );
	ip.opt	    = 0;
	// default offset
	ip.offY.push_back(4);

	//cout << "type fi : " << ip.typeName << endl;

	// bool
	if(		ip.typeName == TN(bool) ) {
	  QCheckBox * cb = new QCheckBox( this );
	  QSize size = cb->size();
	  size.setHeight(size.height() + 5);
	  cb->resize(size);
	  ip.wA.push_back( cb );
	  // no offset
	  ip.offY[ip.offY.size() - 1] = 0;
	  if( inSet ) {
	    bool isOn;
	    if( inSet->get
		(ip.name,isOn) )
	      cb->setChecked( isOn );
	  }
	}

	// int
	if(ip.typeName == TN(int)
	   || ip.typeName == TN(unsigned int)	) {
	  QIntValidator * intValidator = new QIntValidator( this );
	  if( ip.typeName == TN(unsigned int) )
	    intValidator->setBottom( 0 );
	  QLineEdit * le = new QLineEdit( "0", this );
	  le->setValidator( intValidator );
	  ip.wA.push_back( le );
	  if( inSet ) {
	    if( ip.typeName == TN(unsigned int) ) {
	      unsigned int v;
	      if( inSet->get(ip.name,v) )
		le->setText( QString("%1").arg(v) );
	    } else {
	      int v;
	      if( inSet->get(ip.name,v) )
		le->setText( QString("%1").arg(v) );
	    }
	  }
	}

	// fp
	if (ip.typeName == TN(float)
	    || ip.typeName == TN(double)) {
	  QValidator* fpValidator = new QDoubleValidator( this );
	  
	  QLineEdit * le = new QLineEdit( "0", this );
	  le->setValidator( fpValidator );
	  ip.wA.push_back( le );
	  if( inSet ) {
	    std::stringstream tmp;
	    if (ip.typeName == TN(float)) {
	      float v;
	      if( inSet->get(ip.name,v) ) {
		tmp << v;
		le->setText( tmp.str().c_str() );
	      }
	    } else {
	      double d;
	      if (inSet->get(ip.name, d)){
		tmp << d;
		le->setText( tmp.str().c_str()  );
	      }
	    }
	  }
	}
	// string
	else if( ip.typeName == TN(std::string) ) {
	  // if text prefixed than create a QTextEdit
	  if (ip.name.find("text::") != std::string::npos) {
	    QTextEdit *te = new QTextEdit(QString(""),
					  this);
	    te->resize(te->width() * 3, te->height()*3);
	    ip.wA.push_back(te);
	    if( inSet ) {
	      std::string v;
	      if( inSet->get
		  (ip.name, v) )
		te->setText(QString::fromUtf8(v.c_str()));
	    }
	  } else {
	    QLineEdit * le = new QLineEdit( "", this );
	    le->resize( le->width()*3, le->height() );
	    ip.wA.push_back( le );
	    // no offset
	    ip.offY[ip.offY.size() - 1] = 0;
	    if( inSet ) {
	      std::string v;
	      if( inSet->get
		  (ip.name, v) )
		le->setText(QString::fromUtf8(v.c_str()));
	    }
	    if ((ip.name.find("file::") == 0) ||
		(ip.name.find("dir::") == 0)) {
	      QToolButton * opt = new QToolButton( this );
	      opt->setText( "..." );
	      opt->adjustSize();
	      opt->resize( opt->width(), le->height() );
	      ip.opt = opt;
	      // offset
	      ip.offY.push_back(0);
	      opt->installEventFilter( this );
	      ip.wA.push_back( opt );
	    }
	  }
	}

	// Color
	else if( ip.typeName == TN(Color) ) {
	  QValidator* intValidator = new QIntValidator( 0, 255, this );
	  QLabel    * lbR = new QLabel( "R", this );
	  QLabel    * lbG = new QLabel( "G", this );
	  QLabel    * lbB = new QLabel( "B", this );
	  QLabel    * lbA = new QLabel( "A", this );
	  lbR->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbG->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbB->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbA->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbR->resize( 16, lbR->height() );
	  lbG->resize( 16, lbG->height() );
	  lbB->resize( 16, lbB->height() );
	  lbA->resize( 16, lbA->height() );
	  QLineEdit * leR = new QLineEdit( 0, this );
	  QLineEdit * leG = new QLineEdit( 0, this );
	  QLineEdit * leB = new QLineEdit( 0, this );
	  QLineEdit * leA = new QLineEdit( 0, this );
	  leR->setValidator( intValidator );
	  leG->setValidator( intValidator );
	  leB->setValidator( intValidator );
	  leA->setValidator( intValidator );
	  QToolButton * opt = new QToolButton( this );
	  opt->setText( "..." );
	  opt->adjustSize();
	  opt->resize( opt->width(), lbR->height() );
	  ip.wA.push_back( leR );
	  ip.wA.push_back( lbR );
	  ip.offY.push_back(4);
	  ip.wA.push_back( leG );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbG );
	  ip.offY.push_back(4);
	  ip.wA.push_back( leB );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbB );
	  ip.offY.push_back(4);
	  ip.wA.push_back( leA );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbA );
	  ip.offY.push_back(4);
	  ip.wA.push_back( opt );
	  // no offset
	  ip.offY.push_back(0);
	  ip.opt = opt;
	  opt->installEventFilter( this );
	  Color v(255,255,255,255);
	  if( !inSet || !inSet->get
	      (ip.name,v) )
	    v = Color(255,255,255,255);
	  leR->setText( QString("%1").arg(v.getR()) );
	  leG->setText( QString("%1").arg(v.getG()) );
	  leB->setText( QString("%1").arg(v.getB()) );
	  leA->setText( QString("%1").arg(v.getA()) );
	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(v.getR(), v.getG(), v.getB()));
	  opt->setPalette(palette);
	}

	// Size
	else if( ip.typeName == TN(Size) ) {
	  QValidator* intValidator = new QDoubleValidator( this );
	  QLabel    * lbW = new QLabel( "W", this );
	  QLabel    * lbH = new QLabel( "H", this );
	  QLabel    * lbD = new QLabel( "D", this );
	  lbW->resize( 16, lbW->height() );
	  lbH->resize( 16, lbH->height() );
	  lbD->resize( 16, lbD->height() );
	  QLineEdit * leW = new QLineEdit( 0, this );
	  QLineEdit * leH = new QLineEdit( 0, this );
	  QLineEdit * leD = new QLineEdit( 0, this );
	  leW->setValidator( intValidator );
	  leH->setValidator( intValidator );
	  leD->setValidator( intValidator );
	  ip.wA.push_back( leW );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbW );
	  ip.offY.push_back(4);
	  ip.wA.push_back( leH );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbH );
	  ip.offY.push_back(4);
	  ip.wA.push_back( leD );
	  ip.offY.push_back(4);
	  ip.wA.push_back( lbD );
	  ip.offY.push_back(4);
	  Size v(0,0,0);
	  if( !inSet || !inSet->get
	      (ip.name,v) )
	    v = Size(0,0,0);
	  leW->setText( QString("%1").arg(v.getW()) );
	  leH->setText( QString("%1").arg(v.getH()) );
	  leD->setText( QString("%1").arg(v.getD()) );
	}

	// PropertyInterface* or Typed Proxy
	else if (inG &&
		 (ip.typeName == TN(PropertyInterface*)
		  || ip.typeName == TN(BooleanProperty)
		  || ip.typeName == TN(DoubleProperty)
		  || ip.typeName == TN(LayoutProperty)
		  || ip.typeName == TN(StringProperty)
		  || ip.typeName == TN(IntegerProperty)
		  || ip.typeName == TN(SizeProperty)
		  || ip.typeName == TN(ColorProperty))) {
	  stringA proxyA;
	  PropertyInterface * curProxy = 0;
	  if( !inSet || !inSet->get
	      (ip.name,curProxy) )
	    curProxy = 0;
	  int curIdx;
	  if (ip.typeName == TN(PropertyInterface*))
	    curIdx = getAllProperties( proxyA, inG, curProxy );
	  else
	    curIdx = getPropertyOf( proxyA, inG, ip.typeName, curProxy );
	  if( !proxyA.empty() ) {
	    QComboBox * cb = new QComboBox( this );
	    for( unsigned int i = 0 ; i < proxyA.size() ; i++ )
	      cb->addItem(QString::fromUtf8(proxyA[i].c_str()));
	    ip.wA.push_back( cb );
	    // if property is not mandatory, insert None
	    if (!inDef->isMandatory(ip.name)) {
	      cb->insertItem(0, QString(NONE_PROP));
	      if ( curIdx >= 0 )
		++curIdx;
	    }
	    if (curIdx >= 0)
	      cb->setCurrentIndex( curIdx );
	    else
	      cb->setCurrentIndex(0);
	  }
	}
	// StringCollection
	else if(ip.typeName == TN (StringCollection) ) {
	  std::string valueCollection =  inDef->getDefValue(ip.name);
	  StringCollection stringCol(valueCollection);
	  QComboBox * cb = new QComboBox( this );
	  for(unsigned int i=0; i < stringCol.size(); i++ ) {
            cb->addItem(QString::fromUtf8(stringCol[i].c_str()));
	  }
	  cb->setCurrentIndex(0);
	  ip.wA.push_back( cb );       
	}

  // ColorScale
  else if(ip.typeName == TN (ColorScale) ) {
    ColorScalePushButton *button=new ColorScalePushButton("Color Scale",this);
    ip.wA.push_back(button);
  }
	
	if( ip.wA.size() )
	  iparamA.push_back( ip );
	else
	  delete ip.label;
      }
      delete defIt;

      if( iparamA.size() == 0 )
	return false;


      // Layout'ing

      int labelWidthMax = 0;
      for( unsigned int i = 0 ; i < iparamA.size() ; i++ ) {
	iparamA[i].label->adjustSize();
	int lWidth = iparamA[i].label->width();
	if (labelWidthMax < lWidth)
	  labelWidthMax = lWidth;
      }

      QPushButton * okB = new QPushButton("OK", this);
      QSize size = okB->size();
      size.setWidth(size.width() - 20);
      okB->resize(size);
      QPushButton * cancelB = new QPushButton("Cancel", this );
      size = cancelB->size();
      size.setWidth(size.width() - 10);
      cancelB->resize(size);
      setDefB = new QPushButton("Set as Defaults", this);
      setDefB->setToolTip("Set the current values as the default ones for future use");
      setDefB->installEventFilter(this);
      size = setDefB->size();
#if defined(__APPLE__)
      size.setWidth(size.width() + 20);
#else
      size.setWidth(size.width() + 10);
#endif
      setDefB->resize(size);
      restoreSysDefB = new QPushButton("Restore System Defaults", this);
      restoreSysDefB->setToolTip("ReInitialize the dialog with the original default values");
      restoreSysDefB->installEventFilter(this);
      size = restoreSysDefB->size();
#if defined(__APPLE__)
      size.setWidth(size.width() + 80);
#else
      size.setWidth(size.width() + 60);
#endif
      restoreSysDefB->resize(size);
      if (!sysDef) {
	setDefB->hide();
	restoreSysDefB->hide();
      }

      int ix = 10, iy = 5;
      int y  = iy;
      int maxx = okB->width() + cancelB->width() +
	setDefB->width() + restoreSysDefB->width() + 4 * ix;

      QLabel * info = new QLabel( 0, this );
      info->setText( INFO_MSG );
      info->move( ix, y );
      y += info->height() + iy*4;

      for( unsigned int i = 0 ; i < iparamA.size() ; i++ ) {
	IParam & ip = iparamA[i];

	int x = labelWidthMax+ix*2;
	int maxHeight = 0;
	for( unsigned int j = 0 ; j < ip.wA.size() ; j++ ) {
	  ip.wA[j]->move( x, y + ip.offY[j]);
	  x += ip.wA[j]->width() + ix;
	  if (ip.wA[j]->height() > maxHeight)
	    maxHeight = ip.wA[j]->height();
	}
	if (maxx < x) maxx = x; //maxx = maxx >? x;

	ip.label->resize(labelWidthMax, ip.label->height());
	ip.label->move( ix, y + (maxHeight -  ip.label->height())/2);

	y += /* ip.label->height()*/ maxHeight + iy;
      }

      y += iy;
      if( maxx < 400 )
	maxx = 400;
      int parentw = maxx + ix;

      QTextBrowser * tb = new QTextBrowser( this );
      tb->move( ix, y );
      tb->resize( parentw-ix*2, HELP_HEIGHT );
      tb->setText( HELP_WELCOME_MSG );
      y += tb->height() + iy;
      helpBrowser = tb;

      QFrame * f = new QFrame( this );
      f->setFrameShape( QFrame::HLine );
      f->setFrameShadow( QFrame::Sunken );
      f->move( ix, y );
      f->resize( parentw-ix*2, 2 );
      y += f->height() + iy*2;

      int x = parentw - ix;
      x -= okB->width();
      okB->move( x, y );
      x -= cancelB->width() + ix;
      cancelB->move( x, y );
      x -= setDefB->width() + ix;
      setDefB->move( x, y );
      x -= restoreSysDefB->width() + ix;
      restoreSysDefB->move( x, y );
      y += cancelB->height() + iy;

      info->resize( parentw-ix*2, info->height() );
      setMinimumSize( maxx+ix, y );
      setMaximumSize( maxx+ix, y );

      QWidget::connect( okB, SIGNAL( clicked() ), this, SLOT( accept() ) );
      QWidget::connect( cancelB, SIGNAL( clicked() ), this, SLOT( reject() ) );

      return true;
    }


    void fillOut(	DataSet &			outSet,
			Graph *		inG			) {
      for( unsigned int i = 0 ; i < iparamA.size(); i++ ) {
	IParam & ip = iparamA[i];

	// bool
	if(	ip.typeName == TN(bool) ) {
	  QCheckBox * cb = (QCheckBox*) ip.wA[0];
	  outSet.set<bool>( ip.name, cb->isChecked() );
	}

	// int
	else if(	ip.typeName == TN(int) ) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  outSet.set<int>( ip.name, le->text().toInt() );
	}

	// unsigned int
	else if(	ip.typeName == TN(unsigned int) ) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  outSet.set<unsigned int>( ip.name, (unsigned int)(le->text().toInt()) );
	}

	// float
	else if(	ip.typeName == TN(float) ) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  outSet.set<float>( ip.name, le->text().toFloat() );
	}

	// double
	else if(	ip.typeName == TN(double) ) {
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  outSet.set<double>( ip.name, le->text().toDouble() );
	}

	// string
	else if(	ip.typeName == TN(std::string)	) {
	  if (ip.name.find("text::") != std::string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    outSet.set<std::string>(ip.name, te->toPlainText().toUtf8().data());
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    outSet.set<std::string>( ip.name, le->text().toUtf8().data() );
	  }
	}

	// Color
	else if( ip.typeName == TN(Color) )	{
	  QLineEdit * leR = (QLineEdit*) ip.wA[0];
	  QLineEdit * leG = (QLineEdit*) ip.wA[2];
	  QLineEdit * leB = (QLineEdit*) ip.wA[4];
	  QLineEdit * leA = (QLineEdit*) ip.wA[6];
	  int R			= leR->text().toInt();
	  int G			= leG->text().toInt();
	  int B			= leB->text().toInt();
	  int A			= leA->text().toInt();
	  Color c( R, G, B, A );
	  outSet.set<Color>( ip.name, c );
	}

	// Size
	else if( ip.typeName == TN(Size) )	{
	  QLineEdit * leW = (QLineEdit*) ip.wA[0];
	  QLineEdit * leH = (QLineEdit*) ip.wA[2];
	  QLineEdit * leD = (QLineEdit*) ip.wA[4];
	  float W	= leW->text().toFloat();
	  float H	= leH->text().toFloat();
	  float D	= leD->text().toFloat();
	  outSet.set<Size>( ip.name, Size(W,H,D) );
	}

	// PropertyInterface*
	else if (inG &&
		 (ip.typeName == TN(PropertyInterface*)
		  || ip.typeName == TN(BooleanProperty)
		  || ip.typeName == TN(DoubleProperty)
		  || ip.typeName == TN(LayoutProperty)
		  || ip.typeName == TN(StringProperty)
		  || ip.typeName == TN(IntegerProperty)
		  || ip.typeName == TN(SizeProperty)
		  || ip.typeName == TN(ColorProperty))) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  std::string propName(cb->currentText().toUtf8().data());
	  if (propName != NONE_PROP)
	    outSet.set<PropertyInterface*>( ip.name, inG->getProperty(propName) );
	  else {
	    PropertyInterface * curProxy;
	    outSet.getAndFree(ip.name, curProxy);
	  }
	}
	
	// StringCollection
	else if(ip.typeName == TN(StringCollection) ) {
        QComboBox * cb = (QComboBox*) ip.wA[0];
        std::vector<std::string> vectorTemp;
        for ( int i = 0; i < cb->count(); i++) {
            vectorTemp.push_back(cb->itemText(i).toUtf8().data());
        }
        outSet.set<StringCollection>( ip.name,
                StringCollection( vectorTemp, cb->currentText().toUtf8().data()));
        StringCollection toto;
        outSet.get<StringCollection>( ip.name,toto);
	}

  // ColorScale
  else if(ip.typeName == TN(ColorScale) ) {
        ColorScalePushButton* colorScalePushButton = (ColorScalePushButton*) ip.wA[0];
        ColorScale *colorScale=colorScalePushButton->getColorScale();
        outSet.set<ColorScale>(ip.name,*colorScale);
      }
      }
    }

  };

}

bool
tlp::openDataSetDialog(	DataSet & outSet,
			const StructDef *sysDef,
                        StructDef *inDef,
                        const DataSet *inSet,
                        const char * inName,
                        Graph * inG,
			QWidget* parent) {
  // DEBUG
  //	if( inSet )
  //		OutputDataSet( *inSet );
  /////

  if( inSet && inSet != &outSet )
    outSet = *inSet;

  if( !inName )
    inName = "Parameter's Dialog";
  QParamDialog *dlg = new QParamDialog(sysDef, inDef, parent);
  dlg->setWindowTitle( inName );

  if( !dlg->fillIn(inSet, inG) ){
    delete dlg;
    return true;
  }

  bool res = ( dlg->exec() == QDialog::Accepted );

  if( res )
    dlg->fillOut( outSet, inG );

  delete dlg;
  return res;
}
