#include <LeWeiClient.h>

#define malloc      rt_malloc
#define realloc     rt_realloc
#define free        rt_free

//#define DEBUG_PRINTF    rt_kprintf
#define DEBUG_PRINTF(...)

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char server[] = "open.lewei50.com";

LeWeiClient::LeWeiClient(const char * user_key, const char * gateway)
{
    char *ptr = head;
    int head_length = 0;
    int tmp;

    user_data = NULL;
    user_str_length = 0;

    begin = false;
    end = false;

    // build head.
    tmp = sprintf(ptr,
                  "POST /api/V1/Gateway/UpdateSensors/%s HTTP/1.1\r\n",
                  gateway);
    head_length += tmp;
    ptr += tmp;

    // build userkey.
    tmp = sprintf(ptr,
                  "userkey: %s\r\n",
                  user_key);
    head_length += tmp;
    ptr += tmp;

    // build Host.
    tmp = sprintf(ptr, "Host: open.lewei50.com \r\n");
    head_length += tmp;
    ptr += tmp;

    // build User-Agent.
    tmp = sprintf(ptr, "User-Agent: RT-Thread ART\r\n");
    head_length += tmp;
    ptr += tmp;

    if (Ethernet.begin(mac) == 0)
    {
        DEBUG_PRINTF("Failed to configure Ethernet using DHCP\r\n");
    }
    else
    {
        DEBUG_PRINTF("Ethernet configuration OK\r\n");
    }
}

int LeWeiClient::append(const char * name, int value)
{
    int length;
    int tmp;
    char * ptr;

    if(begin == false)
    {
        user_data = (char *)malloc(2);
        if(user_data == NULL)
        {
            return -1;
        }

        user_data[0] = '[';
        user_data[1] = 0;
        user_str_length = 1;

        begin = true;
    }

    if(user_data == NULL)
    {
        return -1;
    }

    length  = 23; /* >>{"Name":"","Value":""},<< */
    length += 8;  /* name */
    length += 10; /* value */

    ptr = (char *)realloc(user_data, user_str_length + length + 1);
    if(ptr == NULL)
    {
        return -1;
    }
    user_data = ptr;

    ptr = user_data + user_str_length;

    length = sprintf(ptr,
            "{\"Name\":\"%s\",\"Value\":\"%d\"},",
            name,
            value);

    user_str_length += length;
    DEBUG_PRINTF("append:%s\r\n", ptr);
}

int LeWeiClient::append(const char * name, double value)
{
    int length;
    int tmp;
    char * ptr;

    if(begin == false)
    {
        user_data = (char *)malloc(2);
        if(user_data == NULL)
        {
            DEBUG_PRINTF("malloc(2) ERROR!\r\n");
            return -1;
        }

        user_data[0] = '[';
        user_data[1] = 0;
        user_str_length = 1;

        begin = true;
    }

    if(user_data == NULL)
    {
        DEBUG_PRINTF("user_data == NULL!\r\n");
        return -1;
    }

    length  = 23; /* >>{"Name":"","Value":""},<< */
    length += 8;  /* name */
    length += 13; /* value: ab.cd */

    ptr = (char *)realloc(user_data, user_str_length + length + 1);
    if(ptr == NULL)
    {
        DEBUG_PRINTF("realloc == NULL!\r\n");
        return -1;
    }
    user_data = ptr;

    ptr = user_data + user_str_length;

    length = sprintf(ptr,
            "{\"Name\":\"%s\",\"Value\":\"%.2f\"},",
            name,
            value);

    user_str_length += length;
    DEBUG_PRINTF("append:%s\r\n", ptr);
}

int LeWeiClient::send()
{
    int result = 0;
    char value_str[6] = "00.00";
    int total_len = 0;
    int tmp_len;


    if (client.connect(server, 80))
    {
        // send the HTTP PUT request:
        client.print(head);

        client.print("Content-Length: ");
        client.println(strlen(user_data) + 1); // ']'

        // last pieces of the HTTP PUT request:
        client.println("Connection: close");
        client.println();

        // post data
        client.print(user_data);
        client.println("]");

        DEBUG_PRINTF("data:%s]\r\n", user_data);

        result = 0;
        goto send_exit;
    }
    else
    {
        DEBUG_PRINTF("connect failed!\r\n");
        result = -1;
        goto send_exit;
    }

send_exit:
    client.stop();

    begin = false;
    end = false;

    if(user_data != NULL)
    {
        free(user_data);
    }

    return result;
}
