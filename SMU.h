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
extern Adafruit_ADS1115 ADS;

class SMU
{
public:
    float VoltagePoints[100];
    double CurrentPackage[50];
    float multiplier = 0.0001875;
    float LimitVoltage = 4.096;
    float TransimpedanceResistor[4] = {100, 1000, 10000, 100000};
    int resistorIndex = 3;
    float lastVoltage;
    double lastCurrent;
    int count;

    int LSVStepTime;
    int LSVVoltageStep;
    int LSVInitialVoltage;
    int LSVFinalVoltage;
    
    int CVStepTime;
    int CVVoltageStep;
    int CVInitialVoltage;
    int CVFinalVoltage;
    int CVPeakVoltage;
    int CVCycles;
    
    int DPVPulseTime;
    int DPVVoltageStep;
    int DPVInitialVoltage;
    int DPVFinalVoltage;
    int DPVPulseVoltage;
    int DPVLowTime;
    
    int NPVPulseTime;
    int NPVVoltageStep;
    int NPVInitialVoltage;
    int NPVFinalVoltage;
    int NPVLowTime;

    void SerialInputInterpretation();
    void LinearSweepVoltammetry(int StepTime);
    void CyclicSweepVoltammetry(int StepTime);
    void DifferentialPulseVoltammetry(int PulseTime, int LowTime);
    void NormalPulseVoltammetry(int PulseTime, int LowTime);
    void SendCurrent();
    void ReadCurrent();
    void ADSSetGain();
    void TransimpedanceAutoScale();
    void SetScale(int resistor);
};

#endif