build:
	gcc mbrencryptor.c -o mbrencryptor
install:
	sudo cp -v mbrencryptor /usr/bin
uninstall:
	sudo rm -v /usr/bin/mbrencryptor
