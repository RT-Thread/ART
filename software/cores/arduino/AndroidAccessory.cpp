/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <AndroidAccessory.h>

AndroidAccessory::AndroidAccessory(const char *manufacturer,
                                   const char *model,
                                   const char *description,
                                   const char *version,
                                   const char *uri,
                                   const char *serial) : manufacturer(manufacturer),
                                                         model(model),
                                                         description(description),
                                                         version(version),
                                                         uri(uri),
                                                         serial(serial)
{

}

void AndroidAccessory::powerOn(void)
{
	rt_usb_adk_set_string(manufacturer, model, description, 
		version, uri, serial);		
}

bool AndroidAccessory::isConnected(void)
{   
    device = rt_device_find("adkdev");
    if(device == RT_NULL) return false;
	else return true;
}

int AndroidAccessory::read(void *buff, int len)
{
	if(device != RT_NULL)
		return rt_device_read(device, 0, buff, len);
	return 0;
}

int AndroidAccessory::write(void *buff, int len)
{
	if(device != RT_NULL)
		return rt_device_write(device, 0, buff, len);
	return 0;
}
