#ifndef SMU_H
#define SMU_H

#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

#define MUXA0 3
#define MUXA1 4
#define MUXA2 5
#define MUXEN 6

extern Adafruit_MCP4725 DAC;
extern Adafruit_ADS1115 ads;

class SMU
{
public:
    String CurrentPackage;
    //double CurrentPackage[50];
    float multiplier = 0.000125;
    float LimitVoltage = 4.096;
    float TransimpedanceResistor[4] = {100, 1000, 10000, 100000};
    int resistorIndex = 1;
    float lastReadVoltage;
    double lastCurrent;
    int count;

    int LSVStepTime;
    float LSVVoltageStep;
    float LSVInitialVoltage;
    float LSVFinalVoltage;
    
    int CVStepTime;
    float CVVoltageStep;
    float CVInitialVoltage;
    float CVFinalVoltage;
    float CVPeakVoltage;
    int CVCycles;
    
    int DPVPulseTime;
    float DPVVoltageStep;
    float DPVInitialVoltage;
    float DPVFinalVoltage;
    float DPVPulseVoltage;
    int DPVLowTime;
    
    int NPVPulseTime;
    float NPVVoltageStep;
    float NPVInitialVoltage;
    float NPVFinalVoltage;
    int NPVLowTime;

    void SerialInputInterpretation();
    void LinearSweepVoltammetry();
    void CyclicSweepVoltammetry();
    void DifferentialPulseVoltammetry();
    void NormalPulseVoltammetry();
    void SendCurrent();
    void ReadCurrent();
    void ADSSetGain();
    void TransimpedanceAutoScale();
    void SetScale(int resistor);
};

#endif