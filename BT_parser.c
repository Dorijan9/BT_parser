#include <stdio.h>
#include <stdint.h>

void parse_ble_data(const char *ble_data) {
    // Convert the hex string to bytes
    size_t ble_data_len = strlen(ble_data);
    uint8_t ble_bytes[ble_data_len / 2];
    for (size_t i = 0; i < ble_data_len; i += 2) {
        sscanf(ble_data + i, "%2hhx", &ble_bytes[i / 2]);
    }

    // Initialize variables to store parsed data
    uint8_t *product_id = NULL;
    uint16_t food_temperature = 0;
    uint16_t furnace_temperature = 0;
    uint16_t battery_voltage = 0;

    // Start parsing the data
    size_t index = 0;
    while (index < ble_data_len) {
        uint8_t field_length = ble_bytes[index];
        uint8_t field_type = ble_bytes[index + 1];
        uint8_t *field_data = &ble_bytes[index + 2];

        if (field_type == 0xFF) {  // Vendor Information
            uint8_t *vendor_data = field_data;

            // Parse the vendor information parameters
            product_id = vendor_data;
            food_temperature = vendor_data[6] + (vendor_data[7] << 8);
            furnace_temperature = vendor_data[8] + (vendor_data[9] << 8);
            battery_voltage = vendor_data[10] + (vendor_data[11] << 8);

            break;  // No need to continue parsing
        }

        index += field_length + 1;
    }

    // Print the parsed data
    printf("Product ID: %02X %02X %02X %02X %02X %02X\n",
           product_id[0], product_id[1], product_id[2],
           product_id[3], product_id[4], product_id[5]);
    printf("Food Temperature: %.1f °C\n", food_temperature / 10.0);
    printf("Furnace Temperature: %.1f °C\n", furnace_temperature / 10.0);
    printf("Battery Voltage: %.2f V\n", battery_voltage / 100.0);
}

int main() {
    const char *ble_data = "02010607094654303030310DFF060070CAEA80FD02AA0B1301";
    parse_ble_data(ble_data);
    return 0;
}
