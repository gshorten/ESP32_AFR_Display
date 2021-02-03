// functions to get AFR data from the speeduino

void getAFR() {
  // read the afr data from the Speeduino.
  const long readFreq = 50;
  static long lastRead = millis();
  if (millis() - lastRead > readFreq) {
    // don't read more frequently than we have to.
    o2Actual = getSpeeduinoData(19, 1, Serial2);
    o2Target = getSpeeduinoData(10, 1, Serial2);
    Serial.print("Actual 02 data: "); Serial.println(o2Actual);
    Serial.print("Target O2 data: "); Serial.println(o2Target);
    lastRead = millis();
  }
}

byte getSpeeduinoData(int dataStart, int noBytes, Stream &port)
{
  /*  gets data from the speeduino, if it is a two byte word then it joins the bytes into an integer.
      DataStart is the location (offset from 0) of the data in the list, see http://wiki.speeduino.com/en/Secondary_Serial_IO_interface
      noBytes can only be 1 or 2 (ie, a single byte or a two byte word)
      the initiating sequence is sent as a series of 7 bytes
      port is the serial port: Serial2 on the mega, Serial2 on the ESP32 */

  int speedValue = 0;                 // return value
  const byte requestData = 0x72;      // the letter "r" in hex ,could send the integer or char but being consistent :-)
  const byte canID = 0x100;           // speeduino canbus ID; i don't know what this is so I picked one at random
  const byte rCommand = 0x30;         // "r" type command
  byte startLSB = lowByte(dataStart); // high and low bytes for start and length of data sequence
  byte startMSB = highByte(dataStart);
  byte lengthLSB = lowByte(noBytes);
  byte lengthMSB = highByte(noBytes);
  // make an array of the bytes to send to start transmission of the data
  byte sendSequence[] = {requestData, canID, rCommand, startLSB, startMSB, lengthLSB, lengthMSB};
  port.write(sendSequence, 7); // send the sequence to the arduino
  //delay(10);
  byte firstByte = port.read();
  if (firstByte == 0x72) // we apparently have data back, so process it
  {
    port.read();      //next byte in buffer should be the data type confirmation. ignore for now
    if (noBytes == 1) // if there is supposed to only be one byte then do another read and save the value
    {
      speedValue = port.read();
    }
    else if (noBytes == 2)
    { // there are two bytes of data so have to do two reads and join the bytes into an integer
      byte firstByte = port.read();
      byte secondByte = port.read();
      speedValue = (secondByte << 8) | firstByte; //join high and low bytes into integer value
    }
  }
  return speedValue;
}
