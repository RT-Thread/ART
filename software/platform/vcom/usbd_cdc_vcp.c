#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#define VCP_RX_BUFSZ	64
static rt_uint8_t vcp_rx_pool[VCP_RX_BUFSZ];
static struct rt_ringbuffer vcp_rx_ringbuffer;
static struct serial_ringbuffer vcom_int_rx;
static rt_bool_t vcom_connected = RT_TRUE;
static struct rt_serial_device vcom_serial;

extern uint8_t APP_Rx_Buffer[APP_RX_DATA_SIZE];
extern uint32_t APP_Rx_ptr_in;

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops = 
{
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};

static uint16_t VCP_Init(void)
{
	return USBD_OK;
}

static uint16_t VCP_DeInit(void)
{
	return USBD_OK;
}

static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    break;

  case GET_ENCAPSULATED_RESPONSE:
    break;

  case SET_COMM_FEATURE:
    break;

  case GET_COMM_FEATURE:
    break;

  case CLEAR_COMM_FEATURE:
    break;

  case SET_LINE_CODING:
    break;

  case GET_LINE_CODING:
    vcom_connected = RT_TRUE;
    break;

  case SET_CONTROL_LINE_STATE:
    /* Not  needed for this driver */
    break;

  case SEND_BREAK:
    /* Not  needed for this driver */
    break;

  default:
    break;
  }

  return USBD_OK;
}

static uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len)
{
	/* send data to USB VCOM */
	return USBD_OK;
}

static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
	rt_uint32_t level;

	/* receive data from USB VCOM */
	level = rt_hw_interrupt_disable();
	rt_ringbuffer_put(&vcp_rx_ringbuffer, Buf, Len);
	rt_hw_interrupt_enable(level);

	/* notify receive data */
	rt_hw_serial_isr(&vcom_serial);

	return USBD_OK;
}

/**
 * UART device in RT-Thread
 */
static rt_err_t usb_vcom_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
	return RT_EOK;
}

static rt_err_t usb_vcom_control(struct rt_serial_device *serial, int cmd, void *arg)
{
	switch (cmd)
	{
	case RT_DEVICE_CTRL_CLR_INT:
		/* disable rx irq */
		break;
	case RT_DEVICE_CTRL_SET_INT:
		/* enable rx irq */
		break;
	}

	return RT_EOK;
}

static int usb_vcom_putc(struct rt_serial_device *serial, char c)
{
	rt_uint32_t level;

	if (vcom_connected != RT_TRUE) return 0;

	level = rt_hw_interrupt_disable();
    APP_Rx_Buffer[APP_Rx_ptr_in] = c;
    APP_Rx_ptr_in++;

    /* To avoid buffer overflow */
    if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
    {
    	APP_Rx_ptr_in = 0;
    }
    rt_hw_interrupt_enable(level);

	return 1;
}

static int usb_vcom_getc(struct rt_serial_device *serial)
{
	int result;
	rt_uint8_t ch;
	rt_uint32_t level;

	result = -1;

	level = rt_hw_interrupt_disable();
	if (RT_RINGBUFFER_SIZE(&vcp_rx_ringbuffer))
	{
		rt_ringbuffer_getchar(&vcp_rx_ringbuffer, &ch);
		result = ch;
	}
	rt_hw_interrupt_enable(level);

	return result;
}

static const struct rt_uart_ops usb_vcom_ops =
{
	usb_vcom_configure,
	usb_vcom_control,
	usb_vcom_putc,
	usb_vcom_getc,
};

void usb_vcom_init(void)
{
	struct serial_configure config;

	/* initialize ring buffer */
	rt_ringbuffer_init(&vcp_rx_ringbuffer, vcp_rx_pool, VCP_RX_BUFSZ);

	config.baud_rate = BAUD_RATE_115200;
	config.bit_order = BIT_ORDER_LSB;
	config.data_bits = DATA_BITS_8;
	config.parity    = PARITY_NONE;
	config.stop_bits = STOP_BITS_1;
	config.invert    = NRZ_NORMAL;

	vcom_serial.ops    = &usb_vcom_ops;
	vcom_serial.int_rx = &vcom_int_rx;
	vcom_serial.config = config;

	/* register UART1 device */
	rt_hw_serial_register(&vcom_serial, "vcom",
		RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
		RT_NULL);
}

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

void usbd_hw_init(void)
{
  usb_vcom_init();

  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ; 
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}

void OTG_FS_IRQHandler(void)
{
	extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
	USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
