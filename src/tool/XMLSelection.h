
#ifndef __XML_SELECTION__
#define __XML_SELECTION__

#ifdef USE_TINYXML
  #include <tinyxml.h>
  #define OT_DOMDOCUMENT TiXmlDocument
  #define OT_DOMELEMENT TiXmlElement
  #ifdef _DEBUG
    #pragma comment( lib, "tinyxmld.lib" )
  #else
    #pragma comment( lib, "tinyxml.lib" )
  #endif 
#else
  #include <xercesc/dom/DOM.hpp>
  #define OT_DOMDOCUMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument
  #define OT_DOMELEMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMElement
  #ifndef USE_XERCES
    #define USE_XERCES 1
  #endif
  #ifdef _DEBUG
    #pragma comment( lib, "xerces-c_2d.lib" )
  #else
    #pragma comment( lib, "xerces-c_2.lib" )
  #endif 
#endif //USE_TINYXML


#endif //__XML_SELECTION__
