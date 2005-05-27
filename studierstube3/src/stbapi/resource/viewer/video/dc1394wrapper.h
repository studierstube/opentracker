#ifndef DC1394_CAMERA_H
#define DC1394_CAMERA_H

#include <config.h>

#ifdef USE_DC1394

#include <iostream>
#include <string>

#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>


#define DC1394_ERROR(msg) throw Error(__FILE__, __LINE__, msg)

#define DC1394_CALL_GENERIC(name_c, name_cpp)                           \
    inline void name_cpp {                                              \
        if(dc1394_ ## name_c != DC1394_SUCCESS) DC1394_ERROR("Couldn't execute " #name_c); \
    }

#define DC1394_CALL_CAMERA(name_c, name_cpp) DC1394_CALL_GENERIC(name_c(handle, node), name_cpp())
  
#define DC1394_GET_VALUE(type_c, name_c, type_cpp, name_cpp)            \
    inline type_cpp get ## name_cpp() const {                           \
        type_c value;                                                   \
        if(dc1394_get_ ## name_c(handle, node, &value) != DC1394_SUCCESS) DC1394_ERROR("Couldn't get " #name_c); \
        return (type_cpp)value;                                         \
    }

#define DC1394_SET_VALUE(type_c, name_c, type_cpp, name_cpp)            \
    inline void set ## name_cpp(type_cpp value) const {                 \
        if(dc1394_set_ ## name_c(handle, node, (type_c)value) != DC1394_SUCCESS) DC1394_ERROR("Couldn't set " #name_c); \
    }

#define DC1394_GET_SET_VALUE(type_c, name_c, type_cpp, name_cpp)        \
    DC1394_GET_VALUE(type_c, name_c, type_cpp, name_cpp)                \
        DC1394_SET_VALUE(type_c, name_c, type_cpp, name_cpp)


namespace dc1394 {

    class Error {
        std::string file;
        int line;
        std::string message;

    public:
        Error(const char *f, int l, const char *m): file(f), line(l), message(m) {}

        void print(std::ostream &s = std::cerr) const;
    };

    class Camera {
        raw1394handle_t handle;
        nodeid_t node;

    public:
        /**
           Constructor.
           @param port port number
           @param n node id
        */
        Camera(int port, int n);

        /**
           Destructor.
        */
        ~Camera();

        // SetCameraControlRegister
        // GetCameraControlRegister
        // dc1394_get_camera_feature_set
        // dc1394_get_camera_feature
        // dc1394_get_camera_misc_info
        // dc1394_print_feature
        // dc1394_print_feature_set
        // dc1394_get_camera_nodes
        // dc1394_get_sorted_camera_nodes
        DC1394_CALL_CAMERA(init_camera, initCamera);
        // dc1394_is_camera
        // dc1394_get_sw_version
        // dc1394_print_camera_info
        // dc1394_get_camera_info

        // Functions for querying camera attributes
        // dc1394_query_supported_formats
        // dc1394_query_supported_modes
        // dc1394_query_supported_framerates
        // dc1394_query_revision
        // dc1394_query_basic_functionality
        // dc1394_query_advanced_feature_offset
        // dc1394_query_feature_characteristics

        // Get/Set the framerate, mode, format, iso channel/speed for the video
        DC1394_GET_SET_VALUE(unsigned int, video_framerate, unsigned int, VideoFramerate);
        DC1394_GET_SET_VALUE(unsigned int, video_mode, unsigned int, VideoMode);
        DC1394_GET_SET_VALUE(unsigned int, video_format, unsigned int, VideoFormat);
        void getIsoChannelAndSpeed(unsigned int &channel, unsigned int &speed) const;
        void setIsoChannelAndSpeed(unsigned int channel, unsigned int speed);

        // Turn camera on or off
        DC1394_CALL_CAMERA(camera_on, cameraOn);
        DC1394_CALL_CAMERA(camera_off, cameraOff);

        // Start/stop isochronous data transmission
        DC1394_CALL_CAMERA(start_iso_transmission, startIsoTransmission);
        DC1394_CALL_CAMERA(stop_iso_transmission, stopIsoTransmission);
        DC1394_GET_VALUE(dc1394bool_t, iso_status, bool, IsoStatus);

        // Turn one shot mode on or off
        DC1394_CALL_CAMERA(set_one_shot, setOneShot);
        DC1394_CALL_CAMERA(unset_one_shot, unsetOneShot);

        // Turn multishot mode on or off
        // dc1394_set_multi_shot
        DC1394_CALL_CAMERA(unset_multi_shot, unsetMultiShot);

        DC1394_GET_VALUE(dc1394bool_t, one_shot, bool, OneShot);
        // dc1394_get_multi_shot

        // Get/Set the values of the various features on the camera
        DC1394_GET_SET_VALUE(unsigned int, brightness, unsigned int, Brightness);
        DC1394_GET_SET_VALUE(unsigned int, exposure, unsigned int, Exposure);
        DC1394_GET_SET_VALUE(unsigned int, sharpness, unsigned int, Sharpness);
        // dc1394_get_white_balance
        // dc1394_set_white_balance
        DC1394_GET_SET_VALUE(unsigned int, hue, unsigned int, Hue);
        DC1394_GET_SET_VALUE(unsigned int, saturation, unsigned int, Saturation);
        DC1394_GET_SET_VALUE(unsigned int, gamma, unsigned int, Gamma);
        DC1394_GET_SET_VALUE(unsigned int, shutter, unsigned int, Shutter);
        DC1394_GET_SET_VALUE(unsigned int, gain, unsigned int, Gain);
        DC1394_GET_SET_VALUE(unsigned int, iris, unsigned int, Iris);
        DC1394_GET_SET_VALUE(unsigned int, focus, unsigned int, Focus);
        // dc1394_get_temperature
        // dc1394_set_temperature
        DC1394_GET_SET_VALUE(unsigned int, trigger_mode, unsigned int, TriggerMode);
        DC1394_GET_SET_VALUE(unsigned int, zoom, unsigned int, Zoom);
        DC1394_GET_SET_VALUE(unsigned int, pan, unsigned int, Pan);
        DC1394_GET_SET_VALUE(unsigned int, tilt, unsigned int, Tilt);
        DC1394_GET_SET_VALUE(unsigned int, optical_filter, unsigned int, OpticalFilter);
        DC1394_GET_SET_VALUE(unsigned int, capture_size, unsigned int, CaptureSize);
        DC1394_GET_SET_VALUE(unsigned int, capture_quality, unsigned int, CaptureQuality);

        // Convenience functions to query/set based on a variable camera feature
        // dc1394_get_feature_value
        // dc1394_set_feature_value

        // Query/set specific feature characteristics
        // dc1394_is_feature_present
        // dc1394_has_one_push_auto
        // dc1394_is_one_push_in_operation
        // dc1394_start_one_push_operation
        // dc1394_can_read_out
        // dc1394_can_turn_on_off
        // dc1394_is_feature_on
        // dc1394_feature_on_off
        // dc1394_has_auto_mode
        // dc1394_has_manual_mode
        // dc1394_is_feature_auto
        // dc1394_auto_on_off
        // dc1394_get_min_value
        // dc1394_get_max_value

        // Memory setup
        // dc1394_get_memory_load_ch
        // dc1394_get_memory_save_ch
        // dc1394_is_memory_save_in_operation
        // dc1394_set_memory_save_ch
        // dc1394_memory_save
        // dc1394_memory_load

        // Functions to control the trigger feature.
        DC1394_GET_SET_VALUE(dc1394bool_t, trigger_polarity, bool, TriggerPolarity);
        // dc1394_trigger_has_polarity
        DC1394_GET_SET_VALUE(dc1394bool_t, trigger_on_off, bool, TriggerOnOff);

        // FORMAT_7 access functions
        // dc1394_setup_format7_capture
        // dc1394_dma_setup_format7_capture
        // dc1394_query_format7_max_image_size
        // dc1394_query_format7_unit_size
        // dc1394_query_format7_image_position
        // dc1394_query_format7_image_size
        // dc1394_query_format7_color_coding_id
        // dc1394_query_format7_color_coding
        // dc1394_query_format7_pixel_number
        // dc1394_query_format7_total_bytes
        // dc1394_query_format7_packet_para
        // dc1394_query_format7_byte_per_packet
        // dc1394_set_format7_image_position
        // dc1394_set_format7_image_size
        // dc1394_set_format7_color_coding_id
        // dc1394_set_format7_byte_per_packet
        // dc1394_query_format7_value_setting
        // dc1394_set_format7_value_setting
        // dc1394_query_format7_recommended_byte_per_packet
        // dc1394_query_format7_packet_per_frame
        // dc1394_query_format7_packet_per_frame
        // dc1394_query_format7_unit_position

        // Absolute setting functions
        // dc1394_query_absolute_feature_min_max
        // dc1394_query_absolute_feature_value
        // dc1394_set_absolute_feature_value
        // dc1394_query_absolute_control
        // dc1394_absolute_setting_on_off
        // dc1394_has_absolute_control
        // dc1394_get_bandwidth_usage
        // dc1394_get_camera_port

        friend class Capture;
        friend class FeatureSet;
    };

    class Capture {
        Camera &camera;
        dc1394_cameracapture capture;

    public:
        struct DmaParams {
            int channel;
            int format;
            int mode;
            int speed;
            int frame_rate;
            int num_dma_buffers;
            int drop_frames;
            std::string dma_device_file;

            DmaParams();
        };

        Capture(Camera &c, const DmaParams &p);
        ~Capture();

        inline const void *getBuffer() const { return (const void *)capture.capture_buffer; }

        // DMA Capture Functions 
        DC1394_CALL_GENERIC(dma_unlisten(camera.handle, &capture), dmaUnlisten());
        DC1394_CALL_GENERIC(dma_single_capture(&capture), dmaSingleCapture());
        // dc1394_dma_single_capture_poll
        //?dc1394_dma_multi_capture
        //?dc1394_dma_multi_capture_poll
        DC1394_CALL_GENERIC(dma_done_with_buffer(&capture), dmaDoneWithBuffer());

        // Non DMA Capture Functions 
        // dc1394_setup_capture
        // dc1394_release_camera
        // dc1394_single_capture
        //?dc1394_multi_capture
    };

    class FeatureSet {
        dc1394_feature_set features;

    public:
        FeatureSet(const Camera &camera);

        void print() const;
    };
};

#endif //USE_DC1394

#endif

/* ===========================================================================
   End of dc1394wrapper.h
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
