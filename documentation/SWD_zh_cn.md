# ART SWD连接指南 #

在ART中包括一个SWD的调试接口，支持四线的SWD仿真器连接（例如ULink2、JLink等）。通过SWD，可以配合PC上的Keil MDK、IAR、Eclipse GDB等方式进行与ART板联机调试。

## SWD与20 pin JTAG连接 ##
4线SWD接口与20 pin的JTAG连接示意图如下图所示：

![SWD](./swd.png)

请注意下图中的20 pin JTAG缺口是朝上的。