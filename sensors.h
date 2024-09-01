//int setup_dht20(); // Not needed as the setup_all_sensors() function should be used instead
uint32_t setup_all_sensors();


#include "lib/DHT20.h"
extern DHT20 sens;
extern DHT20 *sens_ptr;