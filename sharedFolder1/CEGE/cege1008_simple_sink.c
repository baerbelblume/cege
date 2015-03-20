#include <stdio.h>
#include <mc1322x.h>
#include <board.h>
#include "contiki.h"
#include "contiki-uart.h"
#include "net/rime.h"
#include "lib/list.h"
#include "lib/memb.h"
#include "dev/leds.h"
#include "string.h"
#include "slip.h"

#define FLASH_LED(l) {      \
    leds_on(l);             \
    clock_delay_msec(50);   \
    leds_off(l);            \
    clock_delay_msec(50); }


/*---------------------------------------------------------------------------*/
PROCESS(cege1008_simple_sink_process, "cege1008_simple_sink");
AUTOSTART_PROCESSES(&cege1008_simple_sink_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(cege1008_simple_sink_process, ev, data)
{ 
  static clock_time_t sendTime;
  char pkt[129];

  PROCESS_BEGIN();

  // Set UART2 to 115200 baud
  //
	uart2_init(INC, MOD, SAMP);

  // Listen on the serial port, and print what we see
  //
  sendTime = clock_time();

  while(1) {
    if (uart2_can_get()) {
      int l = recv_packet(pkt, 128);

		  if (l > 0) {
        pkt[l] = '\0';
   			printf("pkt[%d] %s\n", l, pkt);
        
        int t, y, m, d, H, M, S;
        sscanf(pkt, "%d %d %d %d %d %d %d", &t, &y, &m, &d, &H, &M, &S);

        // Better have a type 1 message
        //
        if (t != 1)
          continue;

        printf("Time is: %02d:%02d:%02d %02d/%02d/%02d\n", H, M, S, d, m, y);
        FLASH_LED(LEDS_BLUE);

      }
		  else
			  printf("Erroneous packet received\n");
    }

    // Fake up a the sending of data to the R-Pi. This would normally be in response to
    // an incoming radio message. Do this every second for now.
    //
    if (clock_time() > sendTime) {
      static char *str    = "Hello World";
      static int   strlen = 11;

      printf("Sending %s to R-Pi\n", str);
		  send_packet(str, strlen);
      sendTime = clock_time() + CLOCK_SECOND;
    }

    PROCESS_PAUSE();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
