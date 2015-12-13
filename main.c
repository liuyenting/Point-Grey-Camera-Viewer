#include "camera.h"
#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h> // PRIx64
#include <opencv/highgui.h>

#define IMG_WIDTH   1280
#define IMG_HEIGHT  960 + 2 // 2 is the magic number.
#define FRAME_RATE  DC1394_FRAMERATE_30

int capture = 0;
void sig_handler(int signo) {
	if(signo == SIGINT)
		capture = 0; // Stop the acuiqisiton loop.
}

int main(int argc, char *argv[]) {
	if(signal(SIGINT, sig_handler) == SIG_ERR)
		fprintf(stderr, "Unable to catch SIGINT\n");

	dc1394_t *obj;
	dc1394camera_t *camera;
	dc1394video_frame_t *frame;

	open_device(&obj, &camera);
	printf("Using camera with GUID %" PRIx64 "\n", camera->guid);
	set_acq_param(&camera, IMG_WIDTH, IMG_HEIGHT, FRAME_RATE);

	IplImage *img;
	open_display("Live Preview");

	capture = 1;
	start_capture(&camera);

	while(capture) {
		img = grab_frame(&camera, &frame);
		cvShowImage("Live Preview", img);
		cvWaitKey(1);
	}

	close_display();

	stop_capture(&camera);
	close_device(&obj, &camera);

	exit(EXIT_SUCCESS);
}
