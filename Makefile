X = .exe
VERSION = 0.1

all: gpsdecode$(X) gpsencode$(X)

gpsdecode$(X): gpsdecode.c
	gcc $< -o $@

gpsencode$(X): gpsencode.c
	gcc $< -o $@

clean:
	rm -f gpsdecode$(X) gpsencode$(X)

dist:
	mkdir gpsdecode-$(VERSION)
	cp -p README.md UNLICENSE gpsdecode.c gpsencode.c bool.h Makefile gpsdecode-$(VERSION)/
	zip -9rq gpsdecode-$(VERSION).zip gpsdecode-$(VERSION)
	rm -rf gpsdecode-$(VERSION)
