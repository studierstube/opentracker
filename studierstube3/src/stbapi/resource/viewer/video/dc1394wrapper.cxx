#include "dc1394wrapper.h"

#ifdef USE_DC1394

using namespace dc1394;
using namespace std;


#define DROP_FRAMES 1
#define NUM_BUFFERS 8
//------------------------------------------------------------------------------
void Error::
print(ostream &s) const
{
    s << '(' << file << ':' << line << ") " << message << endl;
}

//------------------------------------------------------------------------------
Camera::
Camera(int port, int n):
    node(n)
{
    handle = dc1394_create_handle(port);
    if(handle == 0) DC1394_ERROR("Couldn't create handle");
    cameraOn();
}

Camera::
~Camera()
{
    cameraOff();
    if(dc1394_destroy_handle(handle) != DC1394_SUCCESS) DC1394_ERROR("Couldn't destroy handle");
}

void Camera::
getIsoChannelAndSpeed(unsigned int &channel, unsigned int &speed) const
{
    if(dc1394_get_iso_channel_and_speed(handle, node, &channel, &speed) != DC1394_SUCCESS) DC1394_ERROR("");
}

void Camera::
setIsoChannelAndSpeed(unsigned int channel, unsigned int speed)
{
    if(dc1394_set_iso_channel_and_speed(handle, node, channel, speed) != DC1394_SUCCESS) DC1394_ERROR("");
}

//------------------------------------------------------------------------------
Capture::DmaParams::
DmaParams():
    channel(1),
    format(FORMAT_VGA_NONCOMPRESSED),
    mode(MODE_320x240_YUV422),
    speed(SPEED_400),
    frame_rate(FRAMERATE_15),
    num_dma_buffers(NUM_BUFFERS),
    drop_frames(DROP_FRAMES)
{
}

//------------------------------------------------------------------------------
Capture::
Capture(Camera &c, const DmaParams &p):
    camera(c)
{
    if(dc1394_dma_setup_capture(camera.handle, camera.node, p.channel, p.format, p.mode, p.speed, p.frame_rate,
                                p.num_dma_buffers, p.drop_frames, p.dma_device_file.c_str(), &capture) != DC1394_SUCCESS)
        DC1394_ERROR("Couldn't setup camera for DMA capture");
}

Capture::
~Capture()
{
    if(dc1394_dma_release_camera(camera.handle, &capture) != DC1394_SUCCESS) DC1394_ERROR("Couldn't release camera");
}

//------------------------------------------------------------------------------
FeatureSet::
FeatureSet(const Camera &camera)
{
    if(dc1394_get_camera_feature_set(camera.handle, camera.node, &features) != DC1394_SUCCESS) DC1394_ERROR("Couldn't get feature set");
}

void FeatureSet::
print() const
{
    dc1394_print_feature_set(const_cast<dc1394_feature_set *>(&features));
}

#endif // USE_DC1394

/* ===========================================================================
   End of dc1394wrapper.cxx
   ===========================================================================
   Automatic Emacs configuration follows.
   Local Variables:
   mode:c++
   c-basic-offset: 4
   eval: (c-set-offset 'substatement-open 0)
   eval: (c-set-offset 'case-label '+)
   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
   eval: (setq indent-tabs-mode nil)
   End:
   =========================================================================== */
