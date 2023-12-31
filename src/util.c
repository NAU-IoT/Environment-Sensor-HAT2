/**
 * utility file for sensor-agnostic operations
 */
#include "../lib/util.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void i2c_scan(const char *bus,
              unsigned char found_addrs[],
              size_t *num_found_addrs) {
    int file;

    // initialize number of found addresses
    *num_found_addrs = 0;

    // iterate through possible I2C address 0-127
    for (int address = 0x00; address <= 0x7F; address++) {
        // oattempt to open bus
        file = open(bus, O_RDWR);
        if (file < 0) {
            perror("[!] Failed to open I2C bus");
            return;
        }

        // nothing at this address
        if (ioctl(file, I2C_SLAVE, address) < 0) {
            close(file);
            continue;
        }

        // attempt to read one byte from each address to determine if available
        unsigned char buffer;
        if (read(file, &buffer, 1) == 1) {
            // device found and responded, store the address
            found_addrs[*num_found_addrs] = address;
            // increment number of found addresses
            (*num_found_addrs)++;
        }
        // close file
        close(file);
    }
}

int i2c_init(const char *bus, unsigned char addresses[], size_t num_addrs) {
    printf("[+] Initializing I2C device: %s\n", bus);
    int file;

    // create an array to store found addresses, range is 0-127 so max=128
    unsigned char found_addrs[128];
    size_t num_found_addrs = 0;

    // scan for valid addresses
    i2c_scan(bus, found_addrs, &num_found_addrs);

    // check if values in found_addrs are present in the passed in addresses[]
    for (size_t i = 0; i < num_addrs; i++) {
        unsigned char addr = addresses[i];
        int valid = 0;
        // cmp current address against current in found_addrs
        for (size_t j = 0; j < num_found_addrs; j++) {
            if (addr == found_addrs[j]) {
                valid = 1;
                break;
            }
        }

        // mismatch of available and specified addresses!!
        if (!valid) {
            printf("[!] Invalid address specified: 0x%02X\n", addr);
            exit(EXIT_FAILURE);
        }
    }

    // Now, iterate over the valid addresses and initialize them
    for (size_t i = 0; i < num_found_addrs; i++) {
        file = open(bus, O_RDWR);
        if (file < 0) {
            perror("Failed to open I2C device");
            exit(EXIT_FAILURE);
        }

        if (ioctl(file, I2C_SLAVE, found_addrs[i]) < 0) {
            printf("[!] Failed to initialize address: 0x%02X\n",
                   found_addrs[i]);
            exit(EXIT_FAILURE);
        } else {
            printf("[+] Initializing address: 0x%02X\n", found_addrs[i]);
        }

        // close after initialization
        close(file);
    }

    // return the dev file
    return file;
}
