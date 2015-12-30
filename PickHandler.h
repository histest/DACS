#include <sstream>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include "Satellite.h"
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ShapeDrawable>
#include <Windows.h>
#include <locale.h>
#include <osgText/Text>
#include <osgFX/Scribe>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

#include <Windows.h>

#include <osg/Geode>

#include <osg/Geometry>

#include <osgText/Text>
#include <osgText/TextBase>
#include <osgViewer/Viewer>

#include <locale.h>

#include <osgDB/ReadFile>
class CPickHandler : public osgGA::GUIEventHandler {
public: 

	CPickHandler(osgText::Text* updateText):
	  _updateText(updateText) {}

	  ~CPickHandler() {}

	  void createContent(osgText::Text& textObject,const char* string);
	  void gb2312ToUtf8(const string& src, string& result);
	  void  unicodeToUTF8(const wstring &src, string& result);
	  void  gb2312ToUnicode(const string& src, wstring& result);
	  bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	  virtual void Pick(osgViewer::View* view,float MouseX, float MouseY); 

	  void setLabel(const std::string& name)
	  {
		//  std::string timesFont("fonts/simsun.ttf");
		//  osgText::Font* fontHei=osgText::readFontFile("C:\\WINDOWS\\Fonts\\simhei.ttf");
		//  _updateText->setFont("simhei.ttf");
		//  setlocale(LC_ALL,".936");//　配置地域化信息
		//  osgText::String textstr;
		//  textstr.set(name, osgText::String::ENCODING_UTF8);


		//const char* titleString="木兰辞\n拟古决绝词简友";
		//const char* string1;
		//int requiredSize=mbstowcs(NULL,titleString,0);//如果mbstowcs第一参数为NULL那么返回字符串的数目

		//wchar_t* wText=new wchar_t[requiredSize+1];

		//mbstowcs(wText,string1,requiredSize+1);//由char转换成wchar类型
		//  
		//char *m_char;
		//int len= WideCharToMultiByte(CP_ACP,0,wText,wcslen(wText),NULL,0,NULL,NULL);  
		//m_char=new char[len+1];  
		//WideCharToMultiByte(CP_ACP,0,wText,wcslen(wText),m_char,len,NULL,NULL);  
		//m_char[len]='\0';  
		//  if (_updateText.get())
		//	  _updateText->setText(name);

		  QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

		  QTextCodec* code = QTextCodec::codecForName("UTF-8");
		  QString qstr = QObject::tr("中国");
		  string str = code->fromUnicode(qstr).data();
		  osgText::Text* text = new osgText::Text;
		  _updateText->setFont("Stsong.ttf");//设置中文字体C:/WINDOWS/Fonts/STSONG.ttf
		  _updateText->setText(name, osgText::String::ENCODING_UTF8);
			// createContent(*_updateText,wText);// _updateText->setText(wText);//
	  }
	  osg::Node* lastTubeWellNode ;
	  osg::Node* tubeWellNode ;

	//  osg::StateAttribute tubeWellNodeMaterial;
public:
	int pickcount;
protected:

	osg::ref_ptr<osgText::Text>  _updateText;
};

void CPickHandler::createContent(osgText::Text& textObject,const char* string)

{

	int requiredSize=mbstowcs(NULL,string,0);//如果mbstowcs第一参数为NULL那么返回字符串的数目

	wchar_t* wText=new wchar_t[requiredSize+1];

	mbstowcs(wText,string,requiredSize+1);//由char转换成wchar类型

	//textObject.setText(wText);
	//return wText;
	delete wText;

}
bool CPickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) 
{ 
	switch(ea.getEventType()) 
	{ 
		//如果为鼠标左键按下 
	case(osgGA::GUIEventAdapter::PUSH): 
		if (ea.getButton() & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) 
		{ 
			//执行Pick 
			osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
			if (view) Pick(view,ea.getX(),ea.getY());
		} 
		return true;
	} 
	return false;
}

void CPickHandler:: unicodeToUTF8(const wstring &src, string& result)
{
	int n = WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0 );
	result.resize(n);
	::WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0 );
}

void CPickHandler::gb2312ToUtf8(const string& src, string& result)
{
	wstring strWideChar;
	gb2312ToUnicode(src, strWideChar);
	unicodeToUTF8(strWideChar, result);
}
void CPickHandler:: gb2312ToUnicode(const string& src, wstring& result)
{
	int n = MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, NULL, 0 );
	result.resize(n);
	::MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}
void CPickHandler::Pick(osgViewer::View* view,float MouseX, float MouseY) 
{ 
	static unsigned char ucMainBodyCount = 0;

	//this->tubeWellNode=NULL;
	//this->lastTubeWellNode=NULL;
	osgUtil::LineSegmentIntersector::Intersections Intersections;
	std::string strOutput="";

	static unsigned char ucSelectDeviceNo = 0;

	osgUtil::LineSegmentIntersector::Intersections intersections; 

	std::string gdlist="";
	float x = MouseX;
	float y = MouseY;
	osg::ref_ptr<osg::Material> material = new osg::Material;
	//设置材质的光照颜色
	material->setAmbient(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0,1.0,0.0,1.0));
	//设置材质的混合光颜色
	//material->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0,1.0,1.0,0.5));
	//设置镜面反射光颜色
	//material->setSpecular(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0,0.0,0.0,1.0));
	////设置影像店的大小
	//material->setShininess(osg::Material::FRONT_AND_BACK,6.0);

	view->computeIntersections(MouseX,MouseY,intersections);
//	 this->tubeWellNode=NULL;
	if (intersections.size()>0)
	{
		osgUtil::LineSegmentIntersector::Intersections::iterator itr = intersections.begin();
		if (itr->nodePath.empty())
			return;
		//if(count==0)
		//	this->tubeWellNode=NULL;==this->tubeWellNode
		this->lastTubeWellNode= this->tubeWellNode;
		if (this->lastTubeWellNode&&this->pickcount>0)//
		{
			if(this->lastTubeWellNode==NULL) return;
			
			this->lastTubeWellNode->getOrCreateStateSet()->removeAttribute(osg::StateAttribute::MATERIAL);
		}
		//得到管井Node节点

		if(itr->nodePath.size()<1) return;
		this->tubeWellNode = (itr->nodePath)[itr->nodePath.size()-1];
		if(this->tubeWellNode->getName().compare("Earth") == 0) 
		{
			//this->tubeWellNode=NULL;
			return;
		}
		if (this->tubeWellNode)
		{
			this->tubeWellNode->getOrCreateStateSet()->setAttributeAndModes(material,osg::StateAttribute::ON);
		}

		this->pickcount=this->pickcount+1;
	}	
	if (view->computeIntersections(x,y,intersections))
	{
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			std::ostringstream os;

			if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))//
			{
				string str= hitr->nodePath.back()->getName();
				if (str !="Earth"&&str !="The text label")
				{
					os<<""<<hitr->nodePath.back()->getName()<<""<<std::endl;//\"\"
				}
			}
			gdlist = os.str();
			break;
		}
	   setLabel(gdlist);
	}
}
	/**管道参数显示*/
	