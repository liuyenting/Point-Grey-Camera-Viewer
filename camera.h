#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <dc1394/dc1394.h>
#include <opencv/cv.h>

dc1394error_t open_device(dc1394_t **obj, dc1394camera_t **camera);
dc1394error_t set_acq_param(dc1394camera_t **camera, int width, int height, dc1394framerate_t frame_rate);
dc1394error_t start_capture(dc1394camera_t **camera);
IplImage * grab_frame(dc1394camera_t **camera, dc1394video_frame_t **frame);
dc1394error_t stop_capture(dc1394camera_t **camera);
dc1394error_t close_device(dc1394_t **obj, dc1394camera_t **camera);

#define STOP_AND_FREE(cam) \
	do { stop_capture(cam); dc1394_camera_free(*cam); } while(0);

#endif
