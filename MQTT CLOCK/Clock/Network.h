#ifndef _Network_H
#define _Network_H

class Network
{
  public:
    Network(const char* ssidPrm, const char* pwPrm, IPAddress ipPrm, IPAddress gatewayPrm);
    void networkConnect(void);
    void upDate(void);
     
  private:
    const char* ssid;
    const char* pw;
    IPAddress ip;
    IPAddress gateway;

    void networkConnectSuccess(void);
    void networkConnectFail(int error);
};
#endif
