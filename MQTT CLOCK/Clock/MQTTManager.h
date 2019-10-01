#ifndef _MQTTMAN_H
#define _MQTTMAN_H

class MQTTManager
{
  public:
    MQTTManager(ClockData* clkDtrPtrPrm);
    void MQTTManagerStart(void);
    void MQTT_connect(void);
    void upDate(void);
    
    
  private:
    ClockData* clkDtrPtr;
    
};
#endif
