# Aegis Antivirus

Aegis is a simple antivirus scanner written in C that checks files for malware by comparing their SHA-256 hashes against a known malware database.

## Features

- File scanning with SHA-256 hash checking
- Recursive directory scanning
- Background monitoring of Downloads directory
- System-wide installation support

## Dependencies

- OpenSSL (for SHA-256 hashing)
- Linux/Unix system
- C compiler (GCC recommended)

## Building

1. Clone the repository:
```bash
git clone <repository-url>
cd aegis
```

2. Install the csv file from the link in Install_csv_here.md and place it right inside the folder

3. Build the project:
```bash
make
```

4. Install (optional):
```bash
sudo make install
```


## Usage

```bash
aegis [options]

Options:
  -h      Print help menu
  -f      Scan all files in a folder
          Usage: -f [full path to folder]
  -d      Background mode and monitors Downloads directory
  -i      Install Aegis system-wide
```


To scan a directory:
```bash
aegis -f /path/to/directory
```

To run in background monitoring mode:
```bash
aegis -d
```
