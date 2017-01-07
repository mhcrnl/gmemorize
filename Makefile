gmem: gui.c main.c
	gcc -o gmem `pkg-config --libs --cflags gtk+-2.0` `xml2-config --libs` `xml2-config --cflags` -Wall gui.c main.c
