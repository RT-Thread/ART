#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif
  extern int wlan_system_init(void);
  extern int wlan_begin(const char* ssid, const char* password);
  extern int wlan_auto(void);
#ifdef __cplusplus
}
#endif

static char ssid[] = "yourssid";
static char password[] = "yourpassword";

int main(void)
{
  int status;

  /* initialize wlan sub system */
  wlan_system_init();

  status = wlan_begin(ssid, password);
  rt_kprintf("WiFi status=%d.\n", status);

  return 0;
}
