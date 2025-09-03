# Full Drive Encryptor

A command-line tool for encrypting and decrypting entire storage devices using XOR encryption.

## ⚠️ WARNING

**This tool can make your drive unusable if used incorrectly. Always backup your data before using this tool. Encrypting the full drive will make all data inaccessible until decrypted.**

## Features

- Encrypt entire storage devices using XOR encryption
- Generate random encryption keys
- Save encryption keys to file for decryption
- Decrypt previously encrypted drives
- Command-line interface with safety prompts

## Requirements

- GCC compiler
- Linux operating system
- Root privileges for accessing device files

## Installation

1. Clone or download the repository
2. Build the project:
   ```bash
   make build
   ```
3. (Optional) Install system-wide:
   ```bash
   sudo make install
   ```

## Usage

### Encrypt a Drive

```bash
usbencryptor --encrypt /dev/sda
```

**Note:** The tool will prompt for confirmation before proceeding with encryption.

### Decrypt a Drive

```bash
usbencryptor --decrypt /dev/sda
```

### Help

```bash
usbencryptor
```

## How It Works

1. **Encryption:**
   - Reads the entire drive in 4KB chunks
   - Generates a random 16-byte key
   - Encrypts each chunk using XOR operation
   - Saves the key to `encryption_key.bin`
   - Writes the encrypted data back to the device

2. **Decryption:**
   - Reads the encrypted drive data in chunks
   - Loads the key from `encryption_key.bin`
   - Decrypts each chunk using XOR operation
   - Writes the decrypted data back to the device

## Safety Precautions

- **Always backup your data** before using this tool
- Test on a virtual machine or non-critical drive first
- Keep the `encryption_key.bin` file safe - it's required for decryption
- Only encrypt drives you understand the consequences of

## Technical Details

- Uses XOR encryption for simplicity and speed
- Encrypts the entire drive in 4KB chunks
- Key length: 16 bytes
- Written in C for performance

## Uninstall

If installed system-wide:
```bash
sudo make uninstall
```

## Author

Created by isaachhk02

## License

This project is provided as-is for educational purposes. Use at your own risk.
