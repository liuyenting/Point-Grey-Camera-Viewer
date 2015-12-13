/*
 * List dc1394 cameras currently connected and their capabilities
 *
 * Written by John Stowers <john.stowers@gmail.com>
 * Modified by Liu, Yen-ting <windows.linux.mac@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#include <dc1394/dc1394.h>

#define print_case(A) case A: printf(#A ""); break;
void print_video_mode(uint32_t format) {
	switch(format) {
		print_case(DC1394_VIDEO_MODE_160x120_YUV444);
		print_case(DC1394_VIDEO_MODE_320x240_YUV422);
		print_case(DC1394_VIDEO_MODE_640x480_YUV411);
		print_case(DC1394_VIDEO_MODE_640x480_YUV422);
		print_case(DC1394_VIDEO_MODE_640x480_RGB8);
		print_case(DC1394_VIDEO_MODE_640x480_MONO8);
		print_case(DC1394_VIDEO_MODE_640x480_MONO16);
		print_case(DC1394_VIDEO_MODE_800x600_YUV422);
		print_case(DC1394_VIDEO_MODE_800x600_RGB8);
		print_case(DC1394_VIDEO_MODE_800x600_MONO8);
		print_case(DC1394_VIDEO_MODE_1024x768_YUV422);
		print_case(DC1394_VIDEO_MODE_1024x768_RGB8);
		print_case(DC1394_VIDEO_MODE_1024x768_MONO8);
		print_case(DC1394_VIDEO_MODE_800x600_MONO16);
		print_case(DC1394_VIDEO_MODE_1024x768_MONO16);
		print_case(DC1394_VIDEO_MODE_1280x960_YUV422);
		print_case(DC1394_VIDEO_MODE_1280x960_RGB8);
		print_case(DC1394_VIDEO_MODE_1280x960_MONO8);
		print_case(DC1394_VIDEO_MODE_1600x1200_YUV422);
		print_case(DC1394_VIDEO_MODE_1600x1200_RGB8);
		print_case(DC1394_VIDEO_MODE_1600x1200_MONO8);
		print_case(DC1394_VIDEO_MODE_1280x960_MONO16);
		print_case(DC1394_VIDEO_MODE_1600x1200_MONO16);
		print_case(DC1394_VIDEO_MODE_EXIF);
		print_case(DC1394_VIDEO_MODE_FORMAT7_0);
		print_case(DC1394_VIDEO_MODE_FORMAT7_1);
		print_case(DC1394_VIDEO_MODE_FORMAT7_2);
		print_case(DC1394_VIDEO_MODE_FORMAT7_3);
		print_case(DC1394_VIDEO_MODE_FORMAT7_4);
		print_case(DC1394_VIDEO_MODE_FORMAT7_5);
		print_case(DC1394_VIDEO_MODE_FORMAT7_6);
		print_case(DC1394_VIDEO_MODE_FORMAT7_7);
	default:
		printf("Unknown (%i)\n", format);
		break;
	}
}

void print_color_coding(dc1394color_coding_t color_id) {
	switch(color_id) {
		print_case(DC1394_COLOR_CODING_MONO8);
		print_case(DC1394_COLOR_CODING_YUV411);
		print_case(DC1394_COLOR_CODING_YUV422);
		print_case(DC1394_COLOR_CODING_YUV444);
		print_case(DC1394_COLOR_CODING_RGB8);
		print_case(DC1394_COLOR_CODING_MONO16);
		print_case(DC1394_COLOR_CODING_RGB16);
		print_case(DC1394_COLOR_CODING_MONO16S);
		print_case(DC1394_COLOR_CODING_RGB16S);
		print_case(DC1394_COLOR_CODING_RAW8);
		print_case(DC1394_COLOR_CODING_RAW16);
	default:
		printf("Unknown (%d)\n",color_id);
		break;
	}
}

void print_color_filter(dc1394color_filter_t color) {
	switch(color) {
		print_case(DC1394_COLOR_FILTER_RGGB);
		print_case(DC1394_COLOR_FILTER_GBRG);
		print_case(DC1394_COLOR_FILTER_GRBG);
		print_case(DC1394_COLOR_FILTER_BGGR);
	default:
		printf("Unknown (%d)\n",color);
		break;
	}
}

void print_frame_info(dc1394video_frame_t *frame) {
	dc1394video_mode_t mode = frame->video_mode;

	printf("-------- Frame ---------\n");
	printf("size:   %dw x %dh\n", frame->size[0], frame->size[1]);
	printf("roi:    %d,%d\n", frame->position[0], frame->position[1]);
	printf("bpp     %d\n", frame->data_depth);
	printf("stride: %d\n", frame->stride);
	printf("bytes:  %lld\n", frame->total_bytes);
	printf("time:   %lld\n", frame->timestamp);

	printf("video mode:\n        "); print_video_mode(mode); printf("\n");
	printf("color coding:\n        "); print_color_coding(frame->color_coding); printf("\n");
	if((mode == DC1394_VIDEO_MODE_FORMAT7_0) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_1) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_2) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_3) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_4) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_5) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_6) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_7))
	{
		printf("color filter:\n        "); print_color_filter(frame->color_filter); printf("\n");
	} else {
		printf("color filter:\n        N/A\n");
	}

//    dc1394color_coding_t     color_coding;          /* the color coding used. This field is valid for all video modes. */
//    dc1394color_filter_t     color_filter;          /* the color filter used. This field is valid only for RAW modes and IIDC 1.31 */
//    uint32_t                 yuv_byte_order;        /* the order of the fields for 422 formats: YUYV or UYVY */
//    uint32_t                 data_depth;            /* the number of bits per pixel. The number of grayscale levels is 2^(this_number).
//                                                       This is independent from the colour coding */
//    uint32_t                 stride;                /* the number of bytes per image line */
//    dc1394video_mode_t       video_mode;            /* the video mode used for capturing this frame */
//    uint64_t                 total_bytes;           /* the total size of the frame buffer in bytes. May include packet-
//                                                       multiple padding and intentional padding (vendor specific) */
//    uint32_t                 image_bytes;           /* the number of bytes used for the image (image data only, no padding) */
//    uint32_t                 padding_bytes;         /* the number of extra bytes, i.e. total_bytes-image_bytes.  */
//    uint32_t                 packet_size;           /* the size of a packet in bytes. (IIDC data) */
//    uint32_t                 packets_per_frame;     /* the number of packets per frame. (IIDC data) */
//    uint64_t                 timestamp;             /* the unix time [microseconds] at which the frame was captured in
//                                                       the video1394 ringbuffer */
//    uint32_t                 frames_behind;         /* the number of frames in the ring buffer that are yet to be accessed by the user */
//    dc1394camera_t           *camera;               /* the parent camera of this frame */
//    uint32_t                 id;                    /* the frame position in the ring buffer */
//    uint64_t                 allocated_image_bytes; /* amount of memory allocated in for the *image field. */
//    dc1394bool_t             little_endian;         /* DC1394_TRUE if little endian (16bpp modes only),
//                                                       DC1394_FALSE otherwise */
//    dc1394bool_t             data_in_padding;       /* DC1394_TRUE if data is present in the padding bytes in IIDC 1.32 format,
//                                                       DC1394_FALSE otherwise */
}

void print_video_mode_info( dc1394camera_t *camera, dc1394video_mode_t mode) {
	int j;
	dc1394error_t err;

	printf("Mode: ");
	print_video_mode(mode);
	printf("\n");

	if((mode == DC1394_VIDEO_MODE_FORMAT7_0) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_1) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_2) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_3) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_4) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_5) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_6) ||
	   (mode == DC1394_VIDEO_MODE_FORMAT7_7))
	{
		dc1394format7mode_t f7mode;

		err=dc1394_format7_get_mode_info(camera, mode, &f7mode);
		if(err) {
			DC1394_ERR(err,"Could not format 7 information");
		} else {
			printf( "Image Sizes:\n"
			        "  size = %ix%i\n"
			        "  max = %ix%i\n"
			        "  pixels = %i\n",
			        f7mode.size_x, f7mode.size_y,
			        f7mode.max_size_x, f7mode.max_size_y,
			        f7mode.pixnum);

			printf( "Color:\n");
			for(j=0; j<f7mode.color_codings.num; j++) {
				printf("  [%d] coding = ", j);
				print_color_coding(f7mode.color_codings.codings[j]);
				printf("\n");
			}
			printf("  filter = ");
			print_color_filter(f7mode.color_filter);
			printf("\n");
		}

	} else {
		dc1394framerates_t framerates;
		err=dc1394_video_get_supported_framerates(camera,mode,&framerates);
		if(err) {
			DC1394_ERR(err,"Could not get frame rates");
		} else {
			printf("Frame Rates:\n");
			for(j = 0; j < framerates.num; j++) {
				uint32_t rate = framerates.framerates[j];
				float f_rate;
				dc1394_framerate_as_float(rate,&f_rate);
				printf("  [%d] rate = %f\n",j,f_rate );
			}
		}
	}

}

int main(int argc, char *argv[]) {
	unsigned int i;
	dc1394_t * d;
	dc1394camera_list_t * list;
	dc1394error_t err;
	dc1394featureset_t features;

	d = dc1394_new();
	if(!d)
		return 1;
	err = dc1394_camera_enumerate(d, &list);
	DC1394_ERR_RTN(err,"Failed to enumerate cameras");

	if(list->num == 0) {
		dc1394_log_error("No cameras found");
		return 1;
	}

	for(i = 0; i < list->num; i++) {
		dc1394camera_t *camera = dc1394_camera_new(d, list->ids[i].guid);

		if(camera) {
			unsigned int j;
			dc1394video_modes_t modes;

			// Print hardware informations.
			dc1394_camera_print_info(camera, stdout);

			// Print supported camera features.
			err = dc1394_feature_get_all(camera,&features);
			if(err != DC1394_SUCCESS) {
				dc1394_log_warning("Could not get feature set");
			} else {
				dc1394_feature_print_all(&features, stdout);
			}

			// Print a list of supported modes.
			printf("------ Supported Video Modes ------\n");

			err = dc1394_video_get_supported_modes(camera, &modes);
			DC1394_ERR_RTN(err,"Could not get list of modes");

			for(j = 0; j < modes.num; j++) {
				print_video_mode_info(camera, modes.modes[j]);
			}

			dc1394_camera_free(camera);
		}
	}
	dc1394_camera_free_list (list);
	dc1394_free (d);

	return 0;
}
