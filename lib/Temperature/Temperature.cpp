#include "Temperature.h"
#include <vector>

Temperature::Temperature(int GPIO_Pin)
{
    this->GPIO_Pin = GPIO_Pin;
    this->oneWire = OneWire(GPIO_Pin);
    this->dallasTemperature(&this->oneWire);
    this->dallasTemperature.begin();
}
void Temperature::setTemperature()
{
    // Implement the logic to set the temperature here
    this->dallasTemperature.requestTemperatures();
    this->temperatureC = this->dallasTemperature.getTempCByIndex(0);
    this->temperatureF = this->dallasTemperature.getTempFByIndex(0);
}

float Temperature::getTemperatureF()
{
    this->setTemperature();
    return this->temperatureF;
}
float Temperature::getTemperatureC()
{
    this->setTemperature();
    return this->temperatureC;
}
std::vector<float> Temperature::getTemperatures()
{
    std::vector<float> temperatures;
    temperatures.push_back(this->temperatureC);
    temperatures.push_back(this->temperatureF);
    return temperatures;
}
