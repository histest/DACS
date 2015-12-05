#include <QtGui/QApplication>  

#include <osgViewer/Viewer>  
#include <osgGA/TrackballManipulator>  
#include <osgQt/GraphicsWindowQt>  

#include <osg/ShapeDrawable>  
#include <osg/MatrixTransform>  
#include <osgManipulator/TabBoxDragger>  
#include <osgGA/StateSetManipulator>   
#include <osgDB/ReadFile>  
#include <QTimer>
#include <QApplication>
#include <QGridLayout>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>
#include <osg/Group>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <QtGui/QIcon>
#include <QtCore/QTextCodec>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include "Tools/UnitConstant.h"
#include <osgDB/ReadFile>
#include <osgDB/ReadFile>
#include <osg/State>
#include <osg/Material>
#include <osg/Texture1D>
#include <osg/TexGen>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/BlendEquation>
#include <osg/PolygonOffset>
#include <osg/LineWidth>
#include "Tools/disEarth.h"
#include "Tools/UnitConstant.h"
#include "Tools/UserMath.h"
#include "Tools/UserMechanics.h"

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
	osg::ref_ptr<osg::Group> mRoot;

	QWidget* osgwidget;
	osgViewer::View* view;
	osg::ref_ptr<osgText::Text> updateText;
	ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::CullThreadPerCameraDrawThreadPerContext,osg::Group *scene = NULL) : QWidget()
	{
		mRoot=scene;
		setThreadingModel(threadingModel);
		setKeyEventSetsDone(0);
		updateText = new osgText::Text;
		osgwidget= addViewWidget( createGraphicsWindow(0,0,100,100),scene,updateText);///mRoot
		QGridLayout* grid = new QGridLayout;
		grid->addWidget( osgwidget, 0, 0 );
		setLayout( grid );

		connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
		_timer.start( 10 );
	}
	osg::Node* ViewerWidget ::createHUD(osgText::Text* updateText)
	{
		osg::Camera* hudCamera = new osg::Camera;
		hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		hudCamera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
		hudCamera->setViewMatrix(osg::Matrix::identity());
		hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
		hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

		std::string timesFont("fonts/times.ttf");

		// turn lighting off for the text and disable depth test to ensure its always ontop.
		osg::Vec3 position(80.0f,900.0f,0.0f);
		osg::Vec3 delta(0.0f,-35.0f,0.0f);

		{
			osg::Geode* geode = new osg::Geode();
			osg::StateSet* stateset = geode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
			geode->setName("simple");
			hudCamera->addChild(geode);

			osgText::Text* text = new  osgText::Text;
			geode->addDrawable( text );

			text->setFont(timesFont);
			text->setText("");
			text->setPosition(position);

			position += delta;
		}    


		//for (int i=0; i<20; i++) {
		//	osg::Vec3 dy(0.0f,-30.0f,0.0f);
		//	osg::Vec3 dx(60.0f,0.0f,0.0f);
		//	osg::Geode* geode = new osg::Geode();
		//	osg::StateSet* stateset = geode->getOrCreateStateSet();
		//	const char *opts[]={"One", "Two", "Three", "January", "Feb", "2003","","","","","","","","","","","","","",""};
		//	osg::Geometry *quad=new osg::Geometry;
		//	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		//	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
		//	std::string name="subOption";
		//	name += " ";
		//	name += std::string(opts[i]);
		//	geode->setName(name);
		//	osg::Vec3Array* vertices = new osg::Vec3Array(4); // 1 quad
		//	osg::Vec4Array* colors = new osg::Vec4Array;
		//	colors = new osg::Vec4Array;
		//	colors->push_back(osg::Vec4(2.0-0.1*i,0.1*i,0.2*i, 1.0));
		//	quad->setColorArray(colors);
		//	quad->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
		//	(*vertices)[0]=position;
		//	(*vertices)[1]=position+dx;
		//	(*vertices)[2]=position+dx+dy;
		//	(*vertices)[3]=position+dy;
		//	quad->setVertexArray(vertices);
		//	quad->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
		//	geode->addDrawable(quad);
		//	hudCamera->addChild(geode);

		//	position += delta;
		//}    


		{ // this displays what has been selected
			osg::Vec3 position2(180.0f,900.0f,0.0f);
			osg::Geode* geode = new osg::Geode();
			osg::StateSet* stateset = geode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
			geode->setName("The text label");
			geode->addDrawable( updateText );
			hudCamera->addChild(geode);

			updateText->setCharacterSize(34.0f);
			updateText->setFont(timesFont);
			updateText->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));//1.0f,1.0f,0.0f,1.0f
			updateText->setText("");
			updateText->setPosition(position2);
			updateText->setDataVariance(osg::Object::DYNAMIC);

			position2 += delta;
		}    

		return hudCamera;

	}
	QWidget* addViewWidget( osgQt::GraphicsWindowQt* gw, osg::Group* scene ,osg::ref_ptr<osgText::Text> updaText)
	{
		view= new osgViewer::View;
		addView( view );

		osg::Camera* camera = view->getCamera();
		camera->setGraphicsContext( gw );

		const osg::GraphicsContext::Traits* traits = gw->getTraits();

		camera->setClearColor(osg::Vec4(0.5,0.5,0.5,1) );//0.0, 0.0, 0.0, 1.0  
		camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
		camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );


		view->addEventHandler( new osgViewer::StatsHandler );
		view->setCameraManipulator( new osgGA::TrackballManipulator );

		//	scene->addChild(createHUD(updaText.get()));
		//	view->getEventHandlers().push_front(new CPickHandler(updaText.get()));//new CPickHandler(mViewer));
		view->setSceneData(scene);	


		return gw->getGLWidget();
	}
	osgQt::GraphicsWindowQt* createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false )
	{
		osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->windowName = name;
		traits->windowDecoration = windowDecoration;
		traits->x = x;
		traits->y = y;
		traits->width = w;
		traits->height = h;
		traits->doubleBuffer = true;
		traits->alpha = ds->getMinimumNumAlphaBits();
		traits->stencil = ds->getMinimumNumStencilBits();
		traits->sampleBuffers = ds->getMultiSamples();
		traits->samples = ds->getNumMultiSamples();

		return new osgQt::GraphicsWindowQt(traits.get());
	}
	void ViewerWidget::clearScene(osg::Group* scene)
	{
		osgViewer::View* view = new osgViewer::View;
		view->setSceneData(scene);	
		removeView(view);
	}
	virtual void paintEvent( QPaintEvent* event )
	{ frame(); }

protected:

	QTimer _timer;
};
