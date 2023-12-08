#define ESP32_RTOS                      //Enable RTOS

//#include "FreeRTOS.h"

#ifndef FTP_HLEPER_H
#define FTP_HELPER_H


//#ifndef FS_H
//  #include <FS.h>
//#endif

#ifndef _SPIFFS_H_
  #include <SPIFFS.h>
#endif

#include <SimpleFTPserver.h>

  void _callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace);
  void _transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize);
  void listAllFiles();
  void ftp_init();
  void ftp_run();

#endif

