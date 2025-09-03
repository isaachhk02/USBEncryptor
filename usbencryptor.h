#ifndef MBR_ENCRYPTOR_H
#define MBR_ENCRYPTOR_H

#include <crypt.h>
#include <stddef.h>

// Function declarations
char* generateRandomKey(size_t length);
char* loadKey(size_t length);
void xorEncryptDecrypt(char *data, size_t len, const char *key);
void displayProgress(size_t bytesProcessed, size_t totalSize);
int encryptDrive(const char *path);
int decryptDrive(const char *path);
int showHelp();

#endif
