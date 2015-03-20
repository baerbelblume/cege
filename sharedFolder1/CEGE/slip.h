void send_packet(char *p, int len);
int recv_packet(char *p, int len);
void set_fletcher_chk(uint8_t *buffer, uint8_t len, uint8_t *chk_a, uint8_t *chk_b);
uint8_t check_fletcher_chk(uint8_t *buffer, uint8_t len);

