/*
USB ENCRYPTOR
Created by isaachhk02
*/


#include "usbencryptor.h"
#include <stdio.h>
#include <crypt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to generate a random key
char* generateRandomKey(size_t length) {
    char* key = (char*)malloc(length + 1);
    if (key == NULL) {
        printf("ERROR: Memory allocation failed for key!\n");
        return NULL;
    }
    srand(time(NULL));
    for (size_t i = 0; i < length; ++i) {
        key[i] = (char)(rand() % 256);
    }
    key[length] = '\0';
    return key;
}

// Function to load the key from file
char* loadKey(size_t length) {
    FILE *keyFile = fopen("encryption_key.bin", "rb");
    if (keyFile == NULL) {
        printf("ERROR: Could not open key file!\n");
        return NULL;
    }
    char* key = (char*)malloc(length + 1);
    if (key == NULL) {
        printf("ERROR: Memory allocation failed!\n");
        fclose(keyFile);
        return NULL;
    }
    size_t bytesRead = fread(key, 1, length, keyFile);
    if (bytesRead != length) {
        printf("ERROR: Could not read key from file!\n");
        free(key);
        fclose(keyFile);
        return NULL;
    }
    key[length] = '\0';
    fclose(keyFile);
    return key;
}

// Simple XOR encryption function
void xorEncryptDecrypt(char *data, size_t len, const char *key) {
    size_t keyLen = strlen(key);
    for (size_t i = 0; i < len; ++i) {
        data[i] ^= key[i % keyLen];
    }
}

int encryptDrive(const char *path) {
    FILE *dev = fopen(path, "r+b");  // Use "r+b" for binary read/write
    if (dev == NULL) {
        printf("ERROR: Invalid path!\n");
        return -1;
    }

    // Generate a random key
    size_t keyLength = 16;
    char* key = generateRandomKey(keyLength);
    if (key == NULL) {
        fclose(dev);
        return -1;
    }

    // Encrypt the entire drive in chunks
    size_t chunkSize = 4096;
    char *buffer = (char*)malloc(chunkSize);
    if (buffer == NULL) {
        printf("ERROR: Memory allocation failed for buffer!\n");
        free(key);
        fclose(dev);
        return -1;
    }

    fseek(dev, 0, SEEK_SET);
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, chunkSize, dev)) > 0) {
        xorEncryptDecrypt(buffer, bytesRead, key);
        fseek(dev, -((long)bytesRead), SEEK_CUR);
        fwrite(buffer, 1, bytesRead, dev);
        fflush(dev);
    }

    // Save the key to a file for decryption
    FILE *keyFile = fopen("encryption_key.bin", "wb");
    if (keyFile) {
        fwrite(key, 1, keyLength, keyFile);
        fclose(keyFile);
        printf("Key saved to encryption_key.bin\n");
    } else {
        printf("ERROR: Could not save key!\n");
        free(key);
        free(buffer);
        fclose(dev);
        return -1;
    }
    free(key);
    free(buffer);

    fclose(dev);
    printf("Encryption completed for %s\n", path);
    return 0;
}

int decryptDrive(const char *path) {
    FILE *dev = fopen(path, "r+b");  // Use "r+b" for binary read/write
    if (dev == NULL) {
        printf("ERROR: Invalid path!\n");
        return -1;
    }

    // Load the key
    size_t keyLength = 16;
    char* key = loadKey(keyLength);
    if (key == NULL) {
        fclose(dev);
        return -1;
    }

    // Decrypt the entire drive in chunks
    size_t chunkSize = 4096;
    char *buffer = (char*)malloc(chunkSize);
    if (buffer == NULL) {
        printf("ERROR: Memory allocation failed for buffer!\n");
        free(key);
        fclose(dev);
        return -1;
    }

    fseek(dev, 0, SEEK_SET);
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, chunkSize, dev)) > 0) {
        xorEncryptDecrypt(buffer, bytesRead, key);
        fseek(dev, -((long)bytesRead), SEEK_CUR);
        fwrite(buffer, 1, bytesRead, dev);
        fflush(dev);
    }

    free(key);
    free(buffer);
    fclose(dev);
    printf("Decryption completed for %s\n", path);
    return 0;
}

int showHelp() {
    printf("Author: isaachhk02\n");
    printf("Usage: usbencryptor --encrypt <path> or --decrypt <path>\n");
    printf("Example: usbencryptor --encrypt /dev/sda\n");
    printf("Example: usbencryptor --decrypt /dev/sda\n");
    printf("WARNING: Encrypting a full drive can make it unusable. Backup first!\n");
    return 0;
}

int main(int argc, char *argv[]) {  // Changed to char *argv[] for consistency
    if (argc < 2) {
        showHelp();
        return 1;
    } else {
        if (strcmp(argv[1], "--encrypt") == 0) {
            if (argc < 3) {
                printf("ERROR: Please provide a path to encrypt.\n");
                showHelp();
                return 1;
            }
            char select;
            printf("WARNING: This operation is irreversible! Make sure to make a backup!\n");
            printf("ARE YOU SURE TO ENCRYPT? (Y/N)\n");
            scanf(" %c", &select);  // Read user input
            switch (select) {
                case 'Y':
                case 'y':
                    printf("Encrypting %s\n", argv[2]);
                    encryptDrive(argv[2]);
                    break;
                case 'N':
                case 'n':
                    printf("Operation canceled!\n");
                    break;
                default:
                    printf("Invalid input. Operation canceled!\n");
                    break;
            }
        } else if (strcmp(argv[1], "--decrypt") == 0) {
            if (argc < 3) {
                printf("ERROR: Please provide a path to decrypt.\n");
                showHelp();
                return 1;
            }
            printf("Decrypting %s\n", argv[2]);
            decryptDrive(argv[2]);
        } else {
            showHelp();
        }
    }
    return 0;
}
