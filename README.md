# Supermini-radio-receiver
Will add arduino code when I have time and also some tutorial for those, who are interested. Right now, you can atleast go to external references, to see, what websites I used for inspiration.
# How to
You must change this part of the MxL5007T.h to work with 10.7MHz IF frequency for FM, as suggested on onetransistor website, to work with my code:
- Take the IF from tuner, consider it as any RF signal and perform an extra conversion. Frequency conversions are performed by mixing two signals (the incoming RF with a local oscillator). The mixing result must be an IF at a common frequency, such as the 10.7 MHz for FM detectors. Therefore local oscillator (L.O.) should work at a frequency that's either 10.7 MHz higher or lower than input RF.

That means changing this part of the header file to this, so the tv tuners if output is correctly configured to 9.1915MHz (closest value possible):
```
void init(unsigned char xtal_freq = MxL_XTAL_24_MHz, unsigned char mode = 1, unsigned char ifFreq = 7, unsigned char inverted = 0, unsigned char loopThrough = 0);
```
Crystal is very often 24MHz, but you can also change this. Mode doesnt really matter, I chose DVB-T. Important is IF frequency output, so set it to 7. Other values are not really needed for this purpose.
# Block diagram
Will add soon...
# External references
- https://www.onetransistor.eu/2014/08/mxl5007t-based-radio.html - Original idea to make this device, also contains library for the tv tuner itself
- https://earmark.net/gesr/cf.htm - Ceramic filter page, choosing right crystal filter for fm demodulator
- https://github.com/Chris--A/Keypad - Keypad library (can also be found on Arduino Playground)
- https://github.com/DeanIsMe/SevSeg - Library for 4-Digit 7-Segment Display
# TO DO
- Upgrade the code to work with menu button
- Fix displaying frequency when going under 100MHz

