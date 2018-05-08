#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <stdio.h>


typedef struct RGB {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} RGB;

RGB getColorUnderPointer(Display *display, Window window) {
  RGB rgb;

  Window    root_return;
  int       root_x_return;
  int       root_y_return;
  Window    child_return;
  int       child_x_return;
  int       child_y_return;
  unsigned  int mask_return;
  XImage*   image;
  XColor    colors;

  XQueryPointer(display, window, &root_return, &child_return, &root_x_return, &root_y_return, &child_x_return, &child_y_return, &mask_return);

  if (child_return != None) {
    image         = XGetImage(display, child_return, child_x_return, child_y_return, 1, 1, AllPlanes, ZPixmap);
  } else {
    image         = XGetImage(display, root_return, root_x_return, root_y_return, 1, 1, AllPlanes, ZPixmap);
  }

  colors.pixel  = XGetPixel(image, 0, 0);
  rgb.red       = (colors.pixel & image->red_mask) >> 16;
  rgb.green     = (colors.pixel & image->green_mask) >> 8;
  rgb.blue      = colors.pixel & image->blue_mask;

  XFree(image);

  return rgb;
}

int main(int argc, char *argv[]) {
  Display *display    = XOpenDisplay(NULL);
  Window root_window  = RootWindow(display, DefaultScreen(display));

  Cursor cursor;
  unsigned long event_mask = ButtonPressMask;
  int state;

  cursor = XCreateFontCursor(display, XC_target);
  XGrabPointer(display, root_window, False, event_mask, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
  XGetInputFocus(display, &root_window, &state);

  XEvent event;
  XNextEvent(display, &event);

  RGB rgb = getColorUnderPointer(display, root_window);

  printf("#%02x%02x%02x\n", rgb.red, rgb.green, rgb.blue);

  return 0;
}
