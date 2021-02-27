/*
   Display Related Functions
*/

/*!
   @brief Updates the display with Speeduino data based on the mode, which is changed with
   the bottom pushbutton

   @param g_Mode
   Global variable for display mode
*/
void updateDisplay() {
  // update the right display based on the current mode
  switch (g_Mode) {
    case MODE_AFR:
      showAFR(afrFreq);
      break;
    case MODE_EGO:
      showEGO(egoFreq);
      break;
    case MODE_LOOPS:
      showLoops(loopsFreq);
      break;
    case MODE_WARMUP:
      showWarmup(warmupFreq);
      break;
    case MODE_GAMMA:
      showGammaE(gammaFreq);
      break;
    default:
      showAFR();
      break;
  }
}

void showWarmup(int freq) {
  // displays warmup enrichment
  dispNum.fillSprite(TFT_BLACK);
  int warmup = SData.getWarmup(freq);     // Get latest Loops reading
  //warmup = warmup -100;              // 0 will be no warmup enrichment, 100 will b 100%
  dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  dispNum.setTextDatum(MC_DATUM);
  dispNum.drawNumber(warmup, 100, 24);
  dispNum.pushSprite(0, 0);
  // draw label on bottom
  descText.setTextColor(TFT_WHITE, TFT_BLACK);
  descText.drawString("Warmup", 40, 20);
  descText.pushSprite(0, 90);
  showFreq(freq);       // display frequency at bottom right
}

void showAFR(int freq) {
  // displays the current AFR and a triangle on the bottom for AFR variance
  float afrVar;
  static byte oldPos;
  static float avgAfrVar;
  float actualAFR;
  float targetAFR;

  // get actual and target afr, use SpeedData library (instance is SData)
  actualAFR = SData.getActualAFR(freq);
  targetAFR = SData.getTargetAFR(freq);

  // Set colours for Actual AFR depending on value.
  if (actualAFR > 16.0 || actualAFR < 12.0) {
    // Dangerous AFR; AFR is outside normal safe range
    dispNum.setTextColor(TFT_WHITE, TFT_RED);
  }
  else if (actualAFR < 15 && actualAFR > 13.5) {
    // normal range
    dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  else {
    // warning; outside normal but not in danger
    dispNum.setTextColor(TFT_ORANGE, TFT_BLACK);
  }
  dispNum.drawFloat(actualAFR, 1, 0, 40);
  dispNum.pushSprite(20, 0);

  // calculate AFR variance
  afrVar = actualAFR - targetAFR;
  //Serial.print("AFR Variance :"); Serial.println(afrVar);

  // limit variance displayed to +- 1.5
  if ( afrVar < -1.5) {
    afrVar = -1.5;
  }
  else if (afrVar > 1.5) {
    afrVar = 1.5;
  }
  // calulate position of variance indicator
  int indPosition = (afrVar * 70) + 105;

  // only draw indicator if it's moved by more than 10 pixels, this stops it from hopping around
  if (abs(indPosition - oldPos) > 10) {
    // draw bar at bottom
    tft.fillRect(0, 90, 120, 45, TFT_RED);
    tft.fillRect(120, 90, 120, 45, TFT_BLUE);
    // draw indicator
    afrVarInd.pushSprite(indPosition, 85, TFT_TRANSPARENT);
    oldPos = indPosition;
  }
  // display frequency at bottom right
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextFont(4);
  tft.drawNumber(afrFreq, 210, 110);
}

void showEGO(int freq) {
  // displays the EGO Correction value on the display

  long EGO = SData.getEGO(freq);
  //dispNum.fillSprite(TFT_BLACK);

  if (EGO > 115) {
    EGO = 115;
  }
  else if (EGO < 85) {
    EGO = 85;
  }
  // set display colors
  if (EGO > 105) {
    dispNum.fillSprite(TFT_BLUE);
    dispNum.setTextColor(TFT_WHITE, TFT_BLUE);
  }
  else if (EGO < 95) {
    dispNum.fillSprite(TFT_RED);
    dispNum.setTextColor(TFT_WHITE, TFT_RED);
  }
  else {
    dispNum.fillSprite(TFT_BLACK);
    dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  // display EGO value
  String dispEGO = String(EGO - 100);
  dispNum.setTextDatum(MC_DATUM);
  dispNum.drawString(dispEGO, 100, 24);
  dispNum.pushSprite(0, 0);
  // draw label on bottom
  descText.setTextDatum(MC_DATUM);
  descText.setTextColor(TFT_WHITE, TFT_BLACK);
  descText.drawString("EGO Corr", 40, 20);
  descText.pushSprite(0, 90);
  showFreq(freq);       // display frequency at bottom right
}

void showLoops(int freq) {
  // show the loops per second display
  dispNum.fillSprite(TFT_BLACK);
  int loopsPS = SData.getLoops(freq);     // Get latest Loops reading

  dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  dispNum.setTextDatum(MC_DATUM);
  dispNum.drawNumber(loopsPS, 100, 24);
  dispNum.pushSprite(0, 0);
  // draw label on bottom
  descText.setTextColor(TFT_WHITE, TFT_BLACK);
  descText.drawString("Loops", 40, 20);
  descText.pushSprite(0, 90);
  showFreq(freq);       // display frequency at bottom right
}

void showGammaE(int freq) {
  // show the total fuel enrichment (GammaE);
  dispNum.fillSprite(TFT_BLACK);
  int gammaE = SData.getGammaE(freq);     // Get latest gammaE reading

  dispNum.setTextColor(TFT_WHITE, TFT_BLACK);
  dispNum.setTextDatum(MC_DATUM);
  dispNum.drawNumber(gammaE, 100, 24);
  dispNum.pushSprite(0, 0);
  // draw label on bottom
  descText.setTextColor(TFT_WHITE, TFT_BLACK);
  descText.drawString("GammaE", 40, 20);
  descText.pushSprite(0, 90);

  showFreq(freq);       // display frequency at bottom right

}

void showFreq(int freq) {
  // display frequency at bottom right
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(4);
  tft.drawNumber(freq, 210, 110);
}
