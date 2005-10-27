////////////////////////////////////////////
//
// fixes compilation isues for Windows CE
//


// fix for badly ported ACE under VS2005...
//
#if defined(_WIN32_WCE) && (_MSC_VER>=1400)
#  include <map>								// ACE needs some defitions from STL before it is included
#endif //_WIN32_WCE
