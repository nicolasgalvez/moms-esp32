#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <vector>

class Temperature {
public:
    // Constructor
    Temperature(int GPIO_Pin);
    
    // Destructor
    ~Temperature();
    
    // Member functions
    void setTemperature();
    float getTemperatureF();
    float getTemperatureC();
    std::vector<float> getTemperatures();
private:
    // Member variables
    int GPIO_Pin;
    OneWire oneWire;
    DallasTemperature dallasTemperature;
    float temperatureF;
    float temperatureC;
};

#endif // TEMPERATURE_H