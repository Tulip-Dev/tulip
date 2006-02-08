
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/TlpQtTools.h"
#include "tulip/Reflect.h"
#include "tulip/Color.h"
#include "tulip/MetricProxy.h"
#include "tulip/StringProxy.h"
#include "tulip/SelectionProxy.h"
#include "tulip/LayoutProxy.h"
#include "tulip/IntProxy.h"
#include "tulip/ColorsProxy.h"
#include "tulip/SizesProxy.h"

#include <qapplication.h>
#include <qvalidator.h>
#include <qeventloop.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qframe.h>
#include <qpopupmenu.h>
#include <qbuttongroup.h>
#include <qhbox.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qtextbrowser.h>



using namespace std;


#define	INFO_MSG				"The following parameters are requested :"
#define NO_HELP_AVAILABLE_MSG	"No help is available for this parameter."
#define HELP_WELCOME_MSG		"Move mouse cursor over a parameter label to display its documentation ..."
#define	HELP_HEIGHT				200


namespace {

#undef TN
#define	TN( T )		typeid(T).name()

  typedef vector<string> stringA;

  int getAllProperties(	stringA	&outA,
			SuperGraph *inG,
			PProxy *inCurrent = 0	) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      string s = propIt->next();
      PProxy * proxy = inG->getProperty( s );
      if( inCurrent && proxy == inCurrent )
	curIdx = outA.size();
      outA.push_back( s );
    }
    delete propIt;
    return curIdx;
  }

  int getPropertyOf(stringA &outA,
		    SuperGraph *inG,
		    string inTypeName,
		    PProxy *inCurrent = 0) {
    assert( inG );
    outA.clear();
    Iterator< std::string > * propIt = inG->getProperties();
    int curIdx = -1;
    while( propIt->hasNext() ) {
      string s = propIt->next();
      PProxy * proxy = inG->getProperty( s );
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
      cout << p.first << " : " << int( p.second.value ) << endl;
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

    QParamDialog( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0 )
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
	  helpBrowser->setText( ip->helpText.c_str() );
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
	  QColor rgb( leR->text().toInt(),
		      leG->text().toInt(),
		      leB->text().toInt()	);
	  bool ok = false;
	  rgb.setRgb( QColorDialog::getRgba(rgb.rgb(),&ok) );
	  if( ok ) {
	    leR->setText( QString("%1").arg(rgb.red()) );
	    leG->setText( QString("%1").arg(rgb.green()) );
	    leB->setText( QString("%1").arg(rgb.blue()) );
	  }
	  ((QWidget *)obj)->setPaletteBackgroundColor(rgb);
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
		 SuperGraph* inG) {
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
	  QLineEdit * le = new QLineEdit( "", this );
	  le->resize( le->width()*3, le->height() );
	  QToolButton * opt = new QToolButton( this );
	  opt->setText( "..." );
	  opt->adjustSize();
	  opt->resize( opt->width(), le->height() );
	  ip.opt = opt;
	  opt->installEventFilter( this );
	  ip.wA.push_back( le );
	  ip.wA.push_back( opt );
	  if( inSet ) {
	    string v;
	    if( inSet->get
		(ip.name,v) )
	      le->setText( v.c_str() );
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

	// PProxy*
	else if( inG && ip.typeName == TN(PProxy*) ) {
	  stringA proxyA;
	  PProxy * curProxy = 0;
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
		 ( ip.typeName == TN(SelectionProxy)
		   || ip.typeName == TN(MetricProxy)
		   || ip.typeName == TN(LayoutProxy)
		   || ip.typeName == TN(StringProxy)
		   || ip.typeName == TN(IntProxy)
		   || ip.typeName == TN(SizesProxy)
		   || ip.typeName == TN(ColorsProxy) ) ) {
	  stringA proxyA;
	  PProxy * curProxy = 0;
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
      for( uint i = 0 ; i < iparamA.size() ; i++ )
	if (labelWidthMax < iparamA[i].label->width())
	  labelWidthMax = iparamA[i].label->width();

      int ix = 5, iy = 5;
      int y  = iy;
      int maxx = 0;

      QLabel * info = new QLabel( 0, this );
      info->setText( INFO_MSG );
      info->move( ix, y );
      y += info->height() + iy*4;

      for( uint i = 0 ; i < iparamA.size() ; i++ ) {
	IParam & ip = iparamA[i];

	ip.label->move( ix, y );

	int x = labelWidthMax+ix*2;
	for( uint j = 0 ; j < ip.wA.size() ; j++ ) {
	  ip.wA[j]->move( x, y );
	  x += ip.wA[j]->width() + ix;
	}
	if (maxx < x) maxx = x; //maxx = maxx >? x;

	y += ip.label->height() + iy;
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
			SuperGraph *		inG			) {
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
	  QLineEdit * le = (QLineEdit *) ip.wA[0];
	  outSet.set<string>( ip.name, le->text().ascii() );
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

	// PProxy*
	else if( inG && ip.typeName == TN(PProxy*) ) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  outSet.set<PProxy*>( ip.name, inG->getProperty( cb->currentText().latin1() ) );
	}

	// Typed Proxy
	else if(	inG &&
			(	ip.typeName == TN(SelectionProxy)
				||	ip.typeName == TN(MetricProxy)
				||	ip.typeName == TN(LayoutProxy)
				||	ip.typeName == TN(StringProxy)
				||	ip.typeName == TN(IntProxy)
				||	ip.typeName == TN(SizesProxy)
				||	ip.typeName == TN(ColorsProxy) )		) {
	  QComboBox * cb = (QComboBox*) ip.wA[0];
	  outSet.set<PProxy*>( ip.name, inG->getProperty( cb->currentText().latin1() ) );
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
                        SuperGraph * inG) {
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



