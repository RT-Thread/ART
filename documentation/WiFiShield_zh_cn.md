# ART + WiFi shield使用说明 #

## 1. 使用前的准备 ##
在使用ART + WiFi shield前，请预先获得ART板及WiFi shield（shield意为盾，即可以插在ART上的扩展板）。

同时也需要一张TF卡，把ART_work-0093\hardware\firmware目录完整复制到TF卡根目录，这步可以在Windows下进行。

使用时先保证ART中已经烧写了支持WiFi shield的固件：rtthread_wifi.bin，可以使用如下步骤下载新的固件：

1. 在ART_work软件中选择*ART board(with WiFi shield)*板卡
2. 按下ART的DFU按钮，然后是Reset按钮，松开Reset按钮，松开DFU按钮，让ART板进入DFU模式
3. 在ART_work软件中选择Burn Firmware烧写固件

如果烧写成功，会在ART_work的提示信息中显示下载固件成功。

## 2. 使用WiFi shield ##
当使用ART+WiFi shield时，有两种烧写模式，一种是类似原来的方式把Arduino应用程序烧写到芯片ROM中，一种则是通过WiFi无线网络把Arduino应用程序推送给ART板。这两种方式在ART_work软件界面上都是点工具栏的下载按钮，所以这两种的区别在于当前使用了哪种板子：

- 如果是*ART board*，下载按钮是下载到芯片的ROM中；
- 如果是*ART board(with WiFi shield)*，下载按钮是通过网络方式推送给ART板子。

但是在使用WiFi网络前，需要关联到AP，这个时候还没有WiFi网络，所以只能选择ART board烧写到ROM的方式（另外一种是基于命令行方式，见后面描述）。

可以在ART_work中选择WiFi的例程，例如选择*ConnectWithWPA*例程，打开它，并修改SSID和password分别为关联的AP和密码。编译然后下载：
** 请选择板卡为ART board，然后使用DFU烧写这个程序 **

下载完毕，重启动ART板，系统会自动运行ConnectWithWPA程序，寻找到WiFi网络并关联上去。这部分在ART的控制台中有输出信息，如果打印了“You're connected to the network”说明已经关联到网络中，另外当关联成功时，WiFi shield的红色led灯会常亮。

rtthread_wifi.bin固件默认采用DHCP方式从网络中获得IP地址，获得的IP地址也可以在控制台中输入:
>`finsh /> list_if()`

来查看获得的IP地址，网关，DNS等信息。可以在PC上通过ping命令来看WiFi网络是否已经通了。

> 注：第一次关联WiFi AP会比较慢，这个过程中WiFi shield上的红色led灯会闪烁。如果最终led灭了，说明关联AP失败；如果led常亮说明关联AP成功。

在第二次时重新关联相同的AP时，关联的速度会大幅提快，通常在5 - 10秒以内。

## 3. 命令行方式开启WiFi网络 ##
ART + WiFi shield固件已经默认把一些Wlan相关命令输出到命令行，所以ART启动后可以使用Wlan相关命令手动开启WiFi网络。可以使用下面两条命令来开启：

>`finsh /> wlan()`

>`finsh /> wlan_begin("ssid", "password")`

第一条命令是初始化RT-Thread WiFi子系统，第二条命令是关联到WiFi网络的AP（ssid、password请分别使用需要关联的AP名称及密码）。

另外一条命令是：

>`finsh /> wlan_auto()`

这条命令可以替换wlan_beign()命令用于自动关联上一次关联过的AP（这个AP必须是上一次已经关联过，并保存了相关信息的AP（至少包括SSID和password））。

同时在ART中也包括一个系统启动初始化脚本（位置在ART_work-0093\hardware\ART\root\init.rc），所以也可以在初始化脚本中把WiFi网络打开的命令放在其中，这样ART+WiFi shield启动起来立刻会关联到网络。

## 4. 通过WiFi方式推送Arduino程序并运行它 ##
ART_work 0093版本增加了网络方式下载Arduino程序的功能，在使用前需要预先配置下目标地址，可以编辑ART_work-0093\hardware\ART\boards.txt文件，把其中的：

>`art_wifi.upload.addr=192.168.0.35`

更改成ART+WiFi shield实际的IP地址。

然后可以选择个Blink程序，编译，下载到ART板子里（记着板子类型选择*ART board(with WiFi shield)*）。在程序运行起来后，你可以修改Blink程序，例如修改闪烁的周期，重新编译下载看看是否现象已经不同了。

## 5. 编译ART + WiFi shield固件 ##
编译固件的方式与以前的版本类似，需要安装Python 2.7以及scons。比较关键的是，ART + WiFi shield固件因为包括TCP/IP协议栈、更多的文件系统功能，所以编译固件用的配置文件不一样了，这个配置在ART\_work中文件名叫做rtconfig\_wifi.h。所以在编译ART + WiFi shield固件前，请把rtconfig_wifi.h改名成rtconfig.h，然后按照原有的办法使用scons命令行编译，编译出来的文件名是rtthread.bin。

ART + WiFi shield固件的代码结构描述：

ART固件代码放在这个目录下：`ART_work-0093\hardware\ART\platform`

其中的子目录情况：

- applications 应用程序代码，包括针对WiFi shield的daemon代码
- drivers 驱动代码（未包括WiFi相关代码）
- libarduino Arduino接口在固件侧实现的代码
- Libraries STM32F4的外设库代码
- wifi WiFi芯片驱动代码

>相类似的：rtthread_lua.bin固件需要使用rtconfig_lua.h配置进行编译
