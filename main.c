#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h> // PRIx64

#include "camera.h"

#define IMG_WIDTH   1280
#define IMG_HEIGHT  960 + 2 // 2 is the magic number.
#define FRAME_RATE  DC1394_FRAMERATE_30

int main(int argc, char *argv[]) {
	dc1394_t *obj;
	dc1394camera_t *camera;
	dc1394video_frame_t *frame;

	open_device(&obj, &camera);
	printf("Using camera with GUID %" PRIx64 "\n", camera->guid);

	set_acq_param(&camera, IMG_WIDTH, IMG_HEIGHT, FRAME_RATE);

	start_capture(&camera);

	char buf[32];
	FILE *imgfp = NULL;

	for(int i = 1; i <= 50; ++i) {
		sprintf(buf, "image%d.ppm", i);
		imgfp = fopen(buf, "wb");
		fprintf(imgfp, "P6\n%u %u\n255\n", IMG_WIDTH, IMG_HEIGHT);

		grab_frame(&camera, &frame);
		fwrite(frame->image, 1, frame->image_bytes, imgfp);
		printf("wrote %s\n", buf);
		fclose(imgfp);

		dc1394_capture_enqueue(camera, frame);
	}

	stop_capture(&camera);

	close_device(&obj, &camera);

	exit(EXIT_SUCCESS);
}
