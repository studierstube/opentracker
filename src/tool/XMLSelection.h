
#ifndef __XML_SELECTION__
#define __XML_SELECTION__

#ifdef USE_TINYXML
  #include <tinyxml/tinyxml.h>
  #define OT_DOMDOCUMENT TiXmlDocument
  #define OT_DOMELEMENT TiXmlElement
#else
  #include <xercesc/dom/DOM.hpp>
  #define OT_DOMDOCUMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument
  #define OT_DOMELEMENT XERCES_CPP_NAMESPACE_QUALIFIER DOMElement
  #ifndef USE_XERCES
    #define USE_XERCES 1
  #endif
#endif //#include <tinyxml.h>


#endif //__XML_SELECTION__
