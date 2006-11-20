
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
#include <qwidget.h>
#include <qtextbrowser.h>
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
#include <QtGui/qwidget.h>
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


  void	OutputDataSet( const DataSet & inSet ) {
    Iterator< pair<string,DataType> > * it = inSet.getValues();
    while( it->hasNext() ) {
      pair<string,DataType> p;
      p = it->next();
      cout << p.first << " : " << (unsigned long)p.second.value << endl;
    } delete it;
  }


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

    QParamDialog( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, Qt::WFlags f = 0 )
      : QDialog( parent, name, modal, f ) {
      helpBrowser = 0;
      curHelpParam = -1;
    }

    IParamA iparamA;
    QTextBrowser *helpBrowser;
    int	curHelpParam;

    bool eventFilter(QObject *obj, QEvent *e) {
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

    bool fillIn( const StructDef& inDef,
		 const DataSet* inSet,
		 Graph* inG) {
      //
      // Parse inDef

      Iterator< std::pair<std::string,std::string> > * defIt;
      defIt = ((StructDef*)&inDef)->getField();

      while( defIt->hasNext() ) {
	std::pair<std::string,std::string> def;
	def = defIt->next();
	IParam ip;
	ip.name     = def.first;
	ip.typeName = def.second;
	ip.helpText = inDef.getHelp(ip.name);
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

	// PropertyInterface*
	else if( inG && ip.typeName == TN(PropertyInterface*) ) {
	  stringA proxyA;
	  PropertyInterface * curProxy = 0;
	  if( !inSet || !inSet->get
	      (ip.name,curProxy) )
	    curProxy = 0;
	  int curIdx = getAllProperties( proxyA, inG, curProxy );
	  if( proxyA.size() ) {
	    QComboBox * cb = new QComboBox( this );
	    for( uint i = 0 ; i < proxyA.size() ; i++ )
	      cb->insertItem( proxyA[i].c_str() );
	    ip.wA.push_back( cb );
	    if( curIdx >= 0 )
	      cb->setCurrentItem( curIdx );
	  }
	}

	// Typed Proxy
	else if( inG &&
		 ( ip.typeName == TN(BooleanProperty)
		   || ip.typeName == TN(DoubleProperty)
		   || ip.typeName == TN(LayoutProperty)
		   || ip.typeName == TN(StringProperty)
		   || ip.typeName == TN(IntegerProperty)
		   || ip.typeName == TN(SizeProperty)
		   || ip.typeName == TN(ColorProperty) ) ) {
	  stringA proxyA;
	  PropertyInterface * curProxy = 0;
	  if( !inSet || !inSet->get
	      (ip.name,curProxy) )
	    curProxy = 0;
	  int curIdx = getPropertyOf( proxyA, inG, ip.typeName, curProxy );
	  if( proxyA.size() ) {
	    QComboBox * cb = new QComboBox( this );
	    for( uint i = 0 ; i < proxyA.size() ; i++ )
	      cb->insertItem( proxyA[i].c_str() );
	    ip.wA.push_back( cb );
	    if( curIdx >= 0 )
	      cb->setCurrentItem( curIdx );
	  }
	}
	// StringCollection
	else if(ip.typeName == TN (StringCollection) ) {
	  string valueCollection =  inDef.getDefValue(ip.name);
	  StringCollection stringCol(valueCollection);
	  QComboBox * cb = new QComboBox( this );
	  for(unsigned int i=0; i < stringCol.size(); i++ ) {
            cb->insertItem( stringCol[i].c_str());
	  }
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

      int ix = 5, iy = 5;
      int y  = iy;
      int maxx = 0;

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
      QPushButton * okB     = new QPushButton( "OK",     this );
      x -= okB->width();
      okB->move( x, y );
      QPushButton * cancelB = new QPushButton( "Cancel", this );
      x -= cancelB->width() + ix;
      cancelB->move( x, y );
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
	else if( inG && ip.typeName == TN(PropertyInterface*) ) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  outSet.set<PropertyInterface*>( ip.name, inG->getProperty( cb->currentText().latin1() ) );
	}

	// Typed Proxy
	else if(	inG &&
			(	ip.typeName == TN(BooleanProperty)
				||	ip.typeName == TN(DoubleProperty)
				||	ip.typeName == TN(LayoutProperty)
				||	ip.typeName == TN(StringProperty)
				||	ip.typeName == TN(IntegerProperty)
				||	ip.typeName == TN(SizeProperty)
				||	ip.typeName == TN(ColorProperty) )		) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  outSet.set<PropertyInterface*>( ip.name, inG->getProperty( cb->currentText().latin1() ) );
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
                        const StructDef & inDef,
                        const DataSet *	inSet,
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
    QParamDialog * dlg = new QParamDialog();
    dlg->setCaption( inName );

    if( !dlg->fillIn(inDef,inSet,inG) )
        return true;

    bool res = ( dlg->exec() == QDialog::Accepted );

    if( res )
        dlg->fillOut( outSet, inG );

    ///// DEBUG
    //	OutputDataSet( outSet );
    /////

    delete dlg;
    return res;
}



