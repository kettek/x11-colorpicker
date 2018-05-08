ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

default: x11-colorpicker

install: x11-colorpicker
	install -d $(DESTIR)$(PREFIX)/bin/
	install -m 557 x11-colorpicker $(DESTDIR)$(PREFIX)/bin/

x11-colorpicker.o: x11-colorpicker.c
	gcc -c x11-colorpicker.c -o x11-colorpicker.o

x11-colorpicker: x11-colorpicker.o
	gcc x11-colorpicker.o -o x11-colorpicker -lX11

clean:
	-rm -f x11-colorpicker.o
	-rm -f x11-colorpicker
