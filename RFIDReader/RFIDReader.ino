/*Loren Olsen 
 * ECET 230
 * Dec 2021
 * This code was taken from the original author below
 * Modified a bit for my ECEt 230 project
 * Its to read an RFID tag and send that via serial to my pc. 
 * THe pc needs to be able to tell the arduino that it got 
 * the correct tag and then turn on a green led. Turn on a red LED if it didnt work. 
 * 
 * Reading a packet doesnt work
 * LEDs dont work
 * CHecksum doesnt work
 * 
 * 
 * 
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String printbuffer;
int packetnumber;

void setup() {
	//Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	//mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	//Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {

  
  
  //Reset the printbuffer
  printbuffer ="";
  
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));
    
// This is my version of mfrc522.PICC_DumpDetailsToSerial to get just the UID of the card    
    //mfrc522.PICC_DumpUIDToSerial(&(mfrc522.uid));


//Start getting the packet ready to send
    packetnumber ++;
    if (packetnumber > 999)
    {
      packetnumber = 1;
    }
    
    printbuffer += "####";
    
    //Calculate the packet number and put it in the string
    printbuffer += packetnumbercalc(packetnumber);
    
    //get the UID of the tag as a string
    String uidoftag = mfrc522.PICC_DumpUIDToString(&(mfrc522.uid));
    printbuffer += uidoftag;
    
    //printbuffer += mfrc522.PICC_DumpUIDToString(&(mfrc522.uid));

    //Calcualte the checksum and put it in the string
    printbuffer += calcualtechecksum(uidoftag);

    
    //Print out the buffer as a string to serial
    Serial.println(printbuffer);
}


String packetnumbercalc(int packetnumber)
{
  String returnpacketnum;
  if (packetnumber < 10)
    {
      returnpacketnum = "00";
      returnpacketnum += packetnumber;
    }

  if (packetnumber >= 10 && packetnumber < 100)
    {
      returnpacketnum = "0";
      returnpacketnum += packetnumber;
    }
  if (packetnumber >= 100)
  {
    returnpacketnum = packetnumber;
  }
    return returnpacketnum; 
}



String calcualtechecksum(String uidString)
{
  int x;
  String checksum;


  for (int i = 0; i < 9; i++)
  {
    x += int(uidString[i]); 
  }

  x = x % 1000;

  checksum = String(x);


  checksum = "UUUU";
  

  
  return checksum;
}
