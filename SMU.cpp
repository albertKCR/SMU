#include "SMU.h"

void SMU::SerialInputInterpretation()
{
    int commaIndex, commaIndexAux, startIndex, numFloats, lastCommaIndex, index;
    String input = "";
    input = Serial.readString();
    String restOfString = "";
    int Technique;

    index = input.indexOf(',');

    if (index != -1)
    {
        Technique = input.substring(0, index).toInt();

        restOfString = input.substring(index + 1);
    }
    else
    {
        Technique = input.toInt();
        restOfString = "";
    }

    switch (Technique)
    {
    case 0:
        commaIndex = restOfString.indexOf(',');
        LSVStepTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        LSVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        LSVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        LSVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        //Serial.println(LSVStepTime);
        //Serial.println(LSVVoltageStep);
        //Serial.println(LSVInitialVoltage);
        //Serial.println(LSVFinalVoltage);
        LinearSweepVoltammetry();
        break;
    case 1:
        commaIndex = restOfString.indexOf(',');
        CVStepTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVPeakVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVCycles = restOfString.substring(commaIndex + 1, commaIndexAux).toInt();

        Serial.println(CVStepTime);
        Serial.println(CVVoltageStep);
        Serial.println(CVInitialVoltage);
        Serial.println(CVFinalVoltage);
        Serial.println(CVPeakVoltage);
        Serial.println(CVCycles);
        CyclicSweepVoltammetry();
        break;
    case 2:
        commaIndex = restOfString.indexOf(',');
        DPVPulseTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        DPVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        DPVPulseVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() * 0.001;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVLowTime = restOfString.substring(commaIndex + 1, commaIndexAux).toInt();

        Serial.println(DPVPulseTime);
        Serial.println(DPVVoltageStep);
        Serial.println(DPVInitialVoltage);
        Serial.println(DPVFinalVoltage);
        Serial.println(DPVPulseVoltage);
        Serial.println(DPVLowTime);
        DifferentialPulseVoltammetry();
        break;
    case 3:
        commaIndex = restOfString.indexOf(',');
        NPVPulseTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        NPVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        NPVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        NPVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        NPVLowTime = restOfString.substring(commaIndexAux + 1, commaIndex).toInt();

        Serial.println(NPVPulseTime);
        Serial.println(NPVVoltageStep);
        Serial.println(NPVInitialVoltage);
        Serial.println(NPVFinalVoltage);
        Serial.println(NPVLowTime);
        NormalPulseVoltammetry();
        break;
    default:
        break;
    }
}

void SMU::LinearSweepVoltammetry()
{
    CurrentPackage = "";
    int timer = 0;
    count = 0;
    int LSVVoltageStepbit = (LSVVoltageStep * 4095)/5;
    int LSVInitialVoltagebit = (LSVInitialVoltage * 4095)/5;
    int LSVFinalVoltagebit = (LSVFinalVoltage * 4095)/5;

    //Serial.println(LSVVoltageStepbit);
    //Serial.println(LSVInitialVoltagebit);
    //Serial.println(LSVFinalVoltagebit);
    //Serial.println(CurrentPackage);
    for (float i = LSVInitialVoltagebit; i <= LSVFinalVoltagebit; i = i + LSVVoltageStepbit)
    {
        DAC.setVoltage(i, false);
        ReadCurrent();
        timer = millis();
        while ((millis() - timer) < LSVStepTime)
        {
        }
        if (count == 20)
        {
            SendCurrent();
            CurrentPackage = "";
            count = 0;
        }
        count++;
    }
    if (count =! 0) SendCurrent();
    DAC.setVoltage(0x000, false);
}

void SMU::CyclicSweepVoltammetry()
{
    CurrentPackage = "";
    int timer = 0;
    count = 0;
    for (int cycle = 0; cycle < CVCycles; cycle++)
    {
        for (float v = CVInitialVoltage; v <= CVPeakVoltage; v += CVVoltageStep)
        {
            Serial.print(v, 3);
            Serial.print(",");
            DAC.setVoltage(v, false);
            ReadCurrent();
            timer = millis();
            while ((millis() - timer) < CVStepTime)
            {
            }
            if (count == 50)
            {
                SendCurrent();
                count = 0;
            }
            count++;
        }
        for (float v = CVPeakVoltage; v >= CVFinalVoltage; v -= CVVoltageStep)
        {
            Serial.print(v, 3);
            Serial.print(",");
            DAC.setVoltage(v, false);
            ReadCurrent();
            timer = millis();
            while ((millis() - timer) < CVStepTime)
            {
            }
            if (count == 50)
            {
                SendCurrent();
                count = 0;
            }
            count++;
        }
    }
    if (count =! 0) SendCurrent();
}

void SMU::DifferentialPulseVoltammetry()
{
    CurrentPackage = "";
    int timer = 0;
    count = 0;

    float lastVoltage = DPVInitialVoltage;

    while (lastVoltage < DPVFinalVoltage)
    {
        lastVoltage = lastVoltage + DPVPulseVoltage;
        timer = millis();
        while ((millis() - timer) < DPVLowTime)
        {
        }
        // DAC.setVoltage(lastVoltage, false);
        // ReadCurrent();
        Serial.print(lastVoltage);
        Serial.print(",");

        lastVoltage = lastVoltage - DPVVoltageStep;
        timer = millis();
        while ((millis() - timer) < DPVPulseTime)
        {
        }
        // DAC.setVoltage(lastVoltage, false);
        // ReadCurrent();
        Serial.print(lastVoltage);
        Serial.print(",");

        if (count == 50)
        {
            SendCurrent();
            count = 0;
        }
        count++;
    }
    if (count =! 0) SendCurrent();
}

void SMU::NormalPulseVoltammetry()
{
    CurrentPackage = "";
    int timer = 0;
    count = 0;

    float lastVoltage = NPVInitialVoltage;

    while (lastVoltage < NPVFinalVoltage)
    {
        lastVoltage = lastVoltage + NPVVoltageStep;
        Serial.print(lastVoltage);
        Serial.print(",");
        // DAC.setVoltage(lastVoltage, false);
        // ReadCurrent();
        timer = millis();
        while ((millis() - timer) < NPVLowTime)
        {
        }

        Serial.print(NPVInitialVoltage);
        Serial.print(",");
        // DAC.setVoltage(NPVInitialVoltage, false);
        // ReadCurrent();
        timer = millis();
        while ((millis() - timer) < NPVLowTime)
        {
        }

        if (count == 50)
        {
            SendCurrent();
            count = 0;
        }
        count++;
    }
    if (count =! 0) SendCurrent();
}

void SMU::SendCurrent()
{
    Serial.print(CurrentPackage);
}

// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V    0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V    0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V    0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V    0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V    0.0078125mV
void SMU::ReadCurrent()
{
    int16_t result;
    result = ads.readADC_Differential_0_1();
    lastReadVoltage = result * multiplier;
    lastCurrent = lastReadVoltage / TransimpedanceResistor[resistorIndex];
    CurrentPackage = CurrentPackage + String(lastReadVoltage, 5) + ",";
    ADSSetGain();
    // TransimpedanceAutoScale();
}

void SMU::ADSSetGain()
{
    if (abs(lastReadVoltage) > 3.9)
    {
        ads.setGain(GAIN_TWOTHIRDS);
        multiplier = 0.0001875;
        LimitVoltage = 6.144;
    }
    else if (abs(lastReadVoltage) < 3.9 && abs(lastReadVoltage) > 1.9)
    {
        ads.setGain(GAIN_ONE);
        multiplier = 0.000125;
        LimitVoltage = 4.096;
    }
    else if (abs(lastReadVoltage) < 1.9 && abs(lastReadVoltage) > 0.85)
    {
        ads.setGain(GAIN_TWO);
        multiplier = 0.0000625;
        LimitVoltage = 2.048;
    }
    else if (abs(lastReadVoltage) < 0.85 && abs(lastReadVoltage) > 0.38)
    {
        ads.setGain(GAIN_FOUR);
        multiplier = 0.00003125;
        LimitVoltage = 1.024;
    }
    else if (abs(lastReadVoltage) < 0.38 && abs(lastReadVoltage) > 0.19)
    {
        ads.setGain(GAIN_EIGHT);
        multiplier = 0.000015625;
        LimitVoltage = 0.512;
    }
    else if (abs(lastReadVoltage) < 0.19)
    {
        ads.setGain(GAIN_SIXTEEN);
        multiplier = 0.0000078125;
        LimitVoltage = 0.256;
    }
}

void SMU::TransimpedanceAutoScale()
{
    if (lastCurrent < 0.00005)
    {
        SetScale(TransimpedanceResistor[3]);
        resistorIndex = 3;
    }
    else if (lastCurrent > 0.00005 && lastCurrent < 0.0005)
    {
        SetScale(TransimpedanceResistor[2]);
        resistorIndex = 2;
    }
    else if (lastCurrent > 0.0005 && lastCurrent < 0.005)
    {
        SetScale(TransimpedanceResistor[1]);
        resistorIndex = 1;
    }
    else if (lastCurrent > 0.005)
    {
        SetScale(TransimpedanceResistor[0]);
        resistorIndex = 0;
    }
}

void SMU::SetScale(int resistor)
{
    switch (resistor)
    {
    case 100:
        digitalWrite(MUXEN, HIGH);
        digitalWrite(MUXA0, LOW);
        digitalWrite(MUXA1, LOW);
        digitalWrite(MUXA2, LOW);
        digitalWrite(MUXEN, LOW);
        break;
    case 1000:
        digitalWrite(MUXEN, HIGH);
        digitalWrite(MUXA0, HIGH);
        digitalWrite(MUXA1, LOW);
        digitalWrite(MUXA2, LOW);
        digitalWrite(MUXEN, LOW);
        break;
    case 10000:
        digitalWrite(MUXEN, HIGH);
        digitalWrite(MUXA0, LOW);
        digitalWrite(MUXA1, HIGH);
        digitalWrite(MUXA2, LOW);
        digitalWrite(MUXEN, LOW);
        break;
    case 100000:
        digitalWrite(MUXEN, HIGH);
        digitalWrite(MUXA0, HIGH);
        digitalWrite(MUXA1, HIGH);
        digitalWrite(MUXA2, LOW);
        digitalWrite(MUXEN, LOW);
        break;
    default:
        break;
    }
}