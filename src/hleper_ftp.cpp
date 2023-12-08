/********************************************************************** FTP Server ***************************************************/

#include <helper_ftp.h>

FtpServer ftpSrv(21);           //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial


void _callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace){
  switch (ftpOperation) {
    case FTP_CONNECT:
      Serial.println(F("FTP: Connected!"));
      break;
    case FTP_DISCONNECT:
      Serial.println(F("FTP: Disconnected!"));
      break;
    case FTP_FREE_SPACE_CHANGE:
      Serial.printf("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
      break;
    default:
      break;
  }
};

void _transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize){
  switch (ftpOperation) {
    case FTP_UPLOAD_START:
      Serial.println(F("FTP: Upload start!"));
      break;
    case FTP_UPLOAD:
      Serial.printf("FTP: Upload of file %s byte %u\n", name, transferredSize);
      break;
    case FTP_TRANSFER_STOP:
      Serial.println(F("FTP: Finish transfer!"));
      break;
    case FTP_TRANSFER_ERROR:
      Serial.println(F("FTP: Transfer error!"));
      break;
    default:
      break;
  }

  /* FTP_UPLOAD_START = 0,
   * FTP_UPLOAD = 1,
   *
   * FTP_DOWNLOAD_START = 2,
   * FTP_DOWNLOAD = 3,
   *
   * FTP_TRANSFER_STOP = 4,
   * FTP_DOWNLOAD_STOP = 4,
   * FTP_UPLOAD_STOP = 4,
   *
   * FTP_TRANSFER_ERROR = 5,
   * FTP_DOWNLOAD_ERROR = 5,
   * FTP_UPLOAD_ERROR = 5
   */
}; 


void listAllFiles()
{
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while(file)
    {
    Serial.print("FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
    }
}

void ftp_init()
{   
#ifdef ESP32       //esp32 we send true to format spiffs if cannot mount
if (SPIFFS.begin(true, "/spiffs", 5, "spiffs" ))
  {
#elif defined ESP8266
if (SPIFFS.begin()) {
#endif
  /*
  bool formatted = SPIFFS.format();
    if(formatted)
      {
      Serial.println("\n\nSuccess formatting");
      }
      else
      {
      Serial.println("\n\nError formatting");
      }
*/
  ftpSrv.setCallback(_callback);
  ftpSrv.setTransferCallback(_transferCallback);
  Serial.println("SPIFFS opened!");
  ftpSrv.begin("esp32","esp32","ROM Wizard FTP");    //username, password for ftp.   (default 21, 50009 for PASV)
  } else {
		  Serial.println(F("Problem to discover and configure device, check wiring also!"));
		  while(1) yield();
	  } 
        
Serial.println("");
Serial.print("Total space:      ");
Serial.print(SPIFFS.totalBytes()/1024);
Serial.println(" Kbyte");
 
Serial.print("Total space used: ");
Serial.print(SPIFFS.usedBytes()/1024);
Serial.println(" Kbyte");
Serial.println();

listAllFiles();
}

void ftp_run(){
  //SPIFFS.begin(true);
  ftpSrv.handleFTP();
}