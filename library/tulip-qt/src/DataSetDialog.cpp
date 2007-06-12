
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

#if (QT_REL == 3)
#include <qvalidator.h>
#include <qdialog.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qtoolbutton.h>
#include <qtextbrowser.h>
#include <qtooltip.h>
#else
#include <QtGui/qvalidator.h>
#include <QtGui/qdialog.h>
#include <QtGui/qframe.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qtextbrowser.h>
#include "tulip/Qt3ForTulip.h"
#endif


using namespace std;
using namespace tlp;


#define	INFO_MSG				"The following parameters are requested :"
#define NO_HELP_AVAILABLE_MSG	"No help is available for this parameter."
#define HELP_WELCOME_MSG		"Move mouse cursor over a parameter label to display its documentation ..."
#define	HELP_HEIGHT				200


namespace {

#undef TN
#define	TN( T )		typeid(T).name()

  typedef vector<string> stringA;

  int getAllProperties(	stringA	&outA,
			Graph *inG,
			PropertyInterface *inCurrent = 0	) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      string s = propIt->next();
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
		    string inTypeName,
		    PropertyInterface *inCurrent = 0) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      string s = propIt->next();
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
  typedef vector<IParam> IParamA;
  typedef vector<QWidget*> QWidgetA;

  struct IParam {
    string name;
    string typeName;
    void* value;
    QLabel* label; // Label widget
    QWidgetA wA;   // Input widgets
    QWidget* opt;
    string helpText;
  };


  struct QParamDialog : public QDialog {

    const StructDef* sysDef;
    StructDef *inDef;

    QParamDialog(const StructDef *sDef, StructDef *iDef,
		 QWidget * parent = 0, const char * name = 0, bool modal = FALSE, Qt::WFlags f = 0 )
      : QDialog( parent, name, modal, f ), sysDef(sDef), inDef(iDef) {
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
      for( uint i = 0 ; i < iparamA.size(); i++ ) {
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
	  inDef->setDefValue(ip.name, le->text().ascii());
	}

	// string
	else if(ip.typeName == TN(string)) {
	  if (ip.name.find("text::") != string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    inDef->setDefValue(ip.name, te->text().ascii());
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    inDef->setDefValue( ip.name, le->text().ascii() );
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
	  string value = cb->currentText().ascii();
	  if (value != NONE_PROP)
	    inDef->setDefValue(ip.name, value);
	}
	
	// StringCollection
	else if (ip.typeName == TN(StringCollection)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  string current = cb->currentText().ascii();
	  string value(current);
	  for ( int i = 0; i < cb->count(); i++)
	    if (current != cb->text(i).ascii()){
	      value += ";";
	      value += cb->text(i).ascii();
	    }
	  inDef->setDefValue(ip.name, value);
	}
      }
    }

    void restoreSystemDefaults() {
      for( uint i = 0 ; i < iparamA.size(); i++ ) {
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
	else if(ip.typeName == TN(string)) {
	  if (ip.name.find("text::") != string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    te->setText(QString(sysDef->getDefValue(ip.name).c_str()));
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    le->setText(QString(sysDef->getDefValue(ip.name).c_str()));
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
	  ip.opt->setPaletteBackgroundColor(QColor(c.getR(), c.getG(), c.getB()));
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
	  string value = sysDef->getDefValue(ip.name);
	  if (sysDef->isMandatory(ip.name))
	    cb->setCurrentItem(0);
	  else {
	    if (value.empty())
	      value = NONE_PROP;
	  
	    for (int i = 0; i <  cb->count(); i++)
	      if (value == cb->text(i).ascii()) {
		cb->setCurrentItem(i);
		break;
	      }
	  }
	}
	
	// StringCollection
	else if (ip.typeName == TN(StringCollection)) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  StringCollection coll(sysDef->getDefValue(ip.name));
	  string current = coll.getCurrentString();
	  for ( int i = 0; i < cb->count(); i++)
	    if (current == cb->text(i).ascii()){
	      cb->setCurrentItem(i);
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
	for( uint i = 0 ; i < iparamA.size(); i++ ) {
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
#if (QT_REL == 3)  
	  helpBrowser->setText( ip->helpText.c_str() );
#else
	  helpBrowser->setHtml( ip->helpText.c_str() );
#endif
	else
	  helpBrowser->setText( NO_HELP_AVAILABLE_MSG );
      }

      if( e->type() == QEvent::MouseButtonRelease ) {
	IParam * ip = 0;
	for( uint i = 0 ; i < iparamA.size(); i++ ) {
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
	  QRgb rgb = qRgba( leR->text().toInt(),
			    leG->text().toInt(),
			    leB->text().toInt(),
			    leA->text().toInt()
			    );
	  bool ok = false;
	  rgb = QColorDialog::getRgba(rgb,&ok);
	  if( ok ) {
	    leR->setText( QString("%1").arg(qRed(rgb)));
	    leG->setText( QString("%1").arg(qGreen(rgb)));
	    leB->setText( QString("%1").arg(qBlue(rgb)));
	    leA->setText( QString("%1").arg(qAlpha(rgb)));
	  }
	  ((QWidget *)obj)->setPaletteBackgroundColor(QColor(rgb));
	  return false;
	} else if( ip->typeName == TN(string) ) {
	  QString s = QFileDialog::getOpenFileName();
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
	string::size_type pos = def.first.find("::");
	if (pos != string::npos)
	  ip.label = new QLabel(def.first.substr(pos + 2, def.first.length() - pos - 2).c_str(), this);
	else
	  ip.label    = new QLabel( ip.name.c_str(), this );
	ip.label->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
	ip.label->installEventFilter( this );
	ip.label->setMouseTracking( true );
	ip.opt	    = 0;

	// bool
	if(		ip.typeName == TN(bool) ) {
	  QCheckBox * cb = new QCheckBox( this );
#if (QT_REL > 3)
	  QSize size = cb->size();
	  size.setHeight(size.height() + 5);
	  cb->resize(size);
#endif
	  ip.wA.push_back( cb );
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
	    stringstream tmp;
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
	else if( ip.typeName == TN(string) ) {
	  // if text prefixed than create a QTextEdit
	  if (ip.name.find("text::") != string::npos) {
	    QTextEdit *te = new QTextEdit(QString(""),
#if (QT_REL == 3)
					  QString::null,
#endif
					  this);
	    te->resize(te->width() * 3, te->height()*3);
	    ip.wA.push_back(te);
	    if( inSet ) {
	      string v;
	      if( inSet->get
		  (ip.name, v) )
		te->setText( v.c_str() );
	    }
	  } else {
	    QLineEdit * le = new QLineEdit( "", this );
	    le->resize( le->width()*3, le->height() );
	    ip.wA.push_back( le );
	    if( inSet ) {
	      string v;
	      if( inSet->get
		  (ip.name, v) )
		le->setText( v.c_str() );
	    }
	    if (ip.name.find("file::") == 0) {
	      QToolButton * opt = new QToolButton( this );
	      opt->setText( "..." );
	      opt->adjustSize();
	      opt->resize( opt->width(), le->height() );
	      ip.opt = opt;
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
#if (QT_REL > 3)
	  lbR->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbG->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbB->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	  lbA->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
#endif
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
	  ip.wA.push_back( leG );
	  ip.wA.push_back( lbG );
	  ip.wA.push_back( leB );
	  ip.wA.push_back( lbB );
	  ip.wA.push_back( leA );
	  ip.wA.push_back( lbA );
	  ip.wA.push_back( opt );
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
	  opt->setPaletteBackgroundColor( QColor( v.getR(), v.getG(), v.getB() ) );
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
	  ip.wA.push_back( lbW );
	  ip.wA.push_back( leH );
	  ip.wA.push_back( lbH );
	  ip.wA.push_back( leD );
	  ip.wA.push_back( lbD );
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
	  if( proxyA.size() ) {
	    QComboBox * cb = new QComboBox( this );
	    for( uint i = 0 ; i < proxyA.size() ; i++ )
	      cb->insertItem( proxyA[i].c_str() );
	    ip.wA.push_back( cb );
	    // if property is not mandatory, insert None
	    if (!inDef->isMandatory(ip.name)) {
	      cb->insertItem(NONE_PROP, 0);
	      if ( curIdx >= 0 )
		++curIdx;
	    }
	    if (curIdx >= 0)
	      cb->setCurrentItem( curIdx );
	    else
	      cb->setCurrentItem(0);
	  }
	}
	// StringCollection
	else if(ip.typeName == TN (StringCollection) ) {
	  string valueCollection =  inDef->getDefValue(ip.name);
	  StringCollection stringCol(valueCollection);
	  QComboBox * cb = new QComboBox( this );
	  for(unsigned int i=0; i < stringCol.size(); i++ ) {
            cb->insertItem( stringCol[i].c_str());
	  }
	  cb->setCurrentItem(0);
	  ip.wA.push_back( cb );       
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
      for( uint i = 0 ; i < iparamA.size() ; i++ ) {
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
      QToolTip::add(setDefB, "Set the current values as the default ones for future use");
      setDefB->installEventFilter(this);
      size = setDefB->size();
#if defined(__APPLE__)
      size.setWidth(size.width() + 20);
#else
      size.setWidth(size.width() + 10);
#endif
      setDefB->resize(size);
      restoreSysDefB = new QPushButton("Restore System Defaults", this);
      QToolTip::add(restoreSysDefB, "ReInitialize the dialog with the original default values");
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

      int ix = 5, iy = 5;
      int y  = iy;
      int maxx = okB->width() + cancelB->width() +
	setDefB->width() + restoreSysDefB->width() + 4 * ix;

      QLabel * info = new QLabel( 0, this );
      info->setText( INFO_MSG );
      info->move( ix, y );
      y += info->height() + iy*4;

      for( uint i = 0 ; i < iparamA.size() ; i++ ) {
	IParam & ip = iparamA[i];

	int x = labelWidthMax+ix*2;
	int maxHeight = 0;
	for( uint j = 0 ; j < ip.wA.size() ; j++ ) {
	  ip.wA[j]->move( x, y );
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
      for( uint i = 0 ; i < iparamA.size(); i++ ) {
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
	else if(	ip.typeName == TN(string)	) {
	  if (ip.name.find("text::") != string::npos) {
	    QTextEdit *te = (QTextEdit *) ip.wA[0];
	    outSet.set<string>(ip.name, te->text().ascii());
	  } else {
	    QLineEdit * le = (QLineEdit *) ip.wA[0];
	    outSet.set<string>( ip.name, le->text().ascii() );
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
	  string propName(cb->currentText().latin1());
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
        std::vector<string> vectorTemp;
        for ( int i = 0; i < cb->count(); i++) {
            vectorTemp.push_back(cb->text(i).ascii());
        }
        outSet.set<StringCollection>( ip.name,
                StringCollection( vectorTemp, cb->currentText().latin1()));
        StringCollection toto;
        outSet.get<StringCollection>( ip.name,toto);
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
                        Graph * inG) {
  // DEBUG
  //	if( inSet )
  //		OutputDataSet( *inSet );
  /////

  if( inSet && inSet != &outSet )
    outSet = *inSet;

  if( !inName )
    inName = "Parameter's Dialog";
  QParamDialog *dlg = new QParamDialog(sysDef, inDef);
  dlg->setCaption( inName );

  if( !dlg->fillIn(inSet, inG) )
    return true;

  bool res = ( dlg->exec() == QDialog::Accepted );

  if( res )
    dlg->fillOut( outSet, inG );

  delete dlg;
  return res;
}
