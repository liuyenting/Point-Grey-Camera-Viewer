#include "display.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

void open_display(const char *name) {
	cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
}

void close_display() {
	cvDestroyAllWindows();
}
