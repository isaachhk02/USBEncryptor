build:
	gcc usbencryptor.c -o usbencryptor
install:
	sudo cp -v usbencryptor /usr/bin
uninstall:
	sudo rm -v /usr/bin/usbencryptor
