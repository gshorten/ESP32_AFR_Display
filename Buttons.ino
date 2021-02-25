/*
 * Functions to change the display
 */

void handleTopButton(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  // not using this button right now, below is for diagnostics
  Serial.print(F("handleEvent(): pin: "));
  Serial.print(button->getPin());
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);
}

void handleBottomButton(AceButton* button, uint8_t eventType, uint8_t buttonState) {
 // cycle through modes when down button is pressed.  This changes the display shown
 Serial.print("Bottom Button pressed");
 
 g_Mode ++;       // button push increments mode variable.
 if (g_Mode > NUM_MODES){
  g_Mode = 0;   // wrap around
 }
 // blank screen, gets rid of old display when switching
 tft.fillScreen(TFT_BLACK);
}
