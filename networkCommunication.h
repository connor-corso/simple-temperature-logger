// Pre-Developed Includes
#include "lwip/altcp.h"

// Local function declarations
ip_addr_t set_ip(void);
void transmit_data(int, float, ip_addr_t);
void print_ip_address(ip_addr_t);

// Other function (redeclarations?)
extern int connect(void);