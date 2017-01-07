gmem: gui.c main.c
	gcc -o gmem `gtk-config --libs` `gtk-config --cflags` `xml2-config --libs` `xml2-config --cflags` -Wall gui.c main.c
