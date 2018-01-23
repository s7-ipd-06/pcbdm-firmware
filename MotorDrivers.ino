void initDrivers(){
  SPI.begin();
  
  stepper_X.init(SS_X);
  stepper_Y.init(SS_Y);
  stepper_Z.init(SS_Z);
  delay(1);
  
  stepper_X.resetSettings();
  stepper_X.setCurrentMilliamps(1260);
  stepper_X.setStepMode(8);
  stepper_X.enableDriver();
  
  stepper_Y.resetSettings();
  stepper_Y.setCurrentMilliamps(1260);
  stepper_Y.setStepMode(8);
  stepper_Y.enableDriver();
  
  stepper_Z.resetSettings();
  stepper_Z.setCurrentMilliamps(300);
  stepper_Z.setStepMode(8);
  stepper_Z.enableDriver();
  
  
}

