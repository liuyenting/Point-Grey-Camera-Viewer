#include "camera.h"

#include <stdlib.h> // EXIT_FAILURE
#include <inttypes.h> // PRIx64

dc1394error_t error;

dc1394error_t open_device(dc1394_t **obj, dc1394camera_t **camera) {
	dc1394camera_list_t *list;
	dc1394video_frame_t *frame;

	// Initiate the IIDC library.
	*obj = dc1394_new();
	if(!(*obj))
		exit(EXIT_FAILURE);

	// Search for devices.
	error = dc1394_camera_enumerate(*obj, &list);
	DC1394_ERR_RTN(error, "Failed to enumerate cameras");

	if(list->num == 0) {
		dc1394_log_error("No cameras found");
		exit(EXIT_FAILURE);
	}

	// Initiate the camera handle, using the first enlisted device.
	*camera = dc1394_camera_new(*obj, list->ids[0].guid);
	if(!(*camera)) {
		dc1394_log_error("Failed to initialize camera with guid %" PRIx64, list->ids[0].guid);
		exit(EXIT_FAILURE);
	}
	dc1394_camera_free_list(list);

	// Make sure the transmission between this device is stopped.
	dc1394_video_set_transmission(*camera, DC1394_OFF);
	dc1394_capture_stop(*camera);

	// Preset the acquisition speed.
	dc1394_video_set_iso_speed(*camera, DC1394_ISO_SPEED_400);
}

dc1394error_t set_acq_param(dc1394camera_t **camera, int width, int height, dc1394framerate_t frame_rate) {
	error = dc1394_format7_set_roi(*camera,
	                               DC1394_VIDEO_MODE_FORMAT7_4, // Custom format, mode 4.
	                               DC1394_COLOR_CODING_RGB8, // 24-bit RGB output.
	                               DC1394_USE_MAX_AVAIL, // Use maximum transfer packet size.
	                               0, 0, // left, top (default to (0, 0) now).
	                               width, height); // width, height
	DC1394_ERR_CLN_RTN(error, dc1394_camera_free(*camera), "Unable to set custom format");

	error = dc1394_video_set_framerate(*camera, frame_rate);
	DC1394_ERR_CLN_RTN(error, dc1394_camera_free(*camera), "Unable to set frame rate");
}

dc1394error_t start_transmission(dc1394camera_t **camera) {
	error = dc1394_video_set_transmission(*camera, DC1394_ON);
	DC1394_ERR_CLN_RTN(error, STOP_AND_FREE(camera), "Failed to start isochronous transmission");
}

dc1394error_t stop_transmission(dc1394camera_t **camera) {
	error = dc1394_video_set_transmission(*camera, DC1394_OFF);
	DC1394_ERR_RTN(error, "Failed to stop the transmission");
}

dc1394error_t start_capture(dc1394camera_t **camera) {
	error = dc1394_capture_setup(*camera, 4, DC1394_CAPTURE_FLAGS_DEFAULT); // Set to 4 DMA buffers.
	DC1394_ERR_CLN_RTN(error, dc1394_camera_free(*camera), "Error capturing");

	start_transmission(camera);
}

dc1394error_t grab_frame(dc1394camera_t **camera, dc1394video_frame_t **frame) {
	error = dc1394_capture_dequeue(*camera, DC1394_CAPTURE_POLICY_WAIT, frame);
	DC1394_ERR_CLN_RTN(error, STOP_AND_FREE(camera), "Unable to capture");
}

dc1394error_t stop_capture(dc1394camera_t **camera) {
	dc1394_capture_stop(*camera);

	stop_transmission(camera);
}

dc1394error_t close_device(dc1394_t **obj, dc1394camera_t **camera) {
	dc1394_capture_stop(*camera);
	dc1394_video_set_transmission(*camera, DC1394_OFF);

	dc1394_camera_free(*camera);
	dc1394_free(*obj);
}
