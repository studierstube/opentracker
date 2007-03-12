#include <OpenTracker/core/Module.h>
#include <Opentracker/core/OtLogger.h>
namespace ot {

  Context*  Module::contextx = NULL;

  Module::~Module() {
    logPrintW("Deleting Module\n");
  };

  OT_IMPLEMENT_IREFCOUNTED(Module);
};
