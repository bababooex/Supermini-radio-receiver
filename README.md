# Supermini-radio-receiver
Simple radio receiver, that works with easily obtainable tv tuner chip. Although nowdays, we have RTL-SDR dongles, it is still nice to make radio receiver and test your soldering skills by adding wires to TQFP32 package. That allows the size of the receiver to be very small, I couldnt find any smaller receiver, that doesnt use SDR processing and works on this broad spectrum of frequencies.
# How to
You must change this part of the MxL5007T.h to work with 10.7 MHz IF frequency for FM, as suggested on onetransistor website, to work with my code:
- Take the IF from tuner, consider it as any RF signal and perform an extra conversion. Frequency conversions are performed by mixing two signals (the incoming RF with a local oscillator). The mixing result must be an IF at a common frequency, such as the 10.7 MHz for FM detectors. Therefore local oscillator (L.O.) should work at a frequency that's either 10.7 MHz higher or lower than input RF.

That means changing this part of the header file to this, so the tv tuners if output is correctly configured to 9.1915MHz (closest value possible):
```
void init(unsigned char xtal_freq = MxL_XTAL_24_MHz, unsigned char mode = 1, unsigned char ifFreq = 7, unsigned char inverted = 0, unsigned char loopThrough = 0);
```
Crystal is very often 24 MHz, but you can also change this. Mode doesnt really matter, I chose DVB-T. Important is IF frequency output, so set it to 7. Other values are not really needed for this purpose.
# Block diagram
Will add soon...
# Parameters
- Frequency range --        44 MHz - 999.9 MHz (limited by number of digits, but should be able to go over 1000 MHz)
- Reception problems --     The receiver is less sensitive, almost silent on harmonic frequencies of the tv tuner crystal (in my example 48 MHz, 72 MHz etc.), but this resolves on higher frequencies 
- Supported modulations --  WFM (but can also do NFM and AM in certain circumstances, also depends on the IF filter)
- Sensitivity --            WFM	<0.8 μV (heavily depends on fm demodulator, I used TA2003)
- Receiver bandwidth --     150kHz (I couldnt find lower than this value, but 110kHz would be probably better, but it would maybe destroy WFM audio)
- Frequency step --	        100 kHz
- Intermediate frequency -- 10.7 MHz
- Power consumption	--	    <400mW (not measured, but tuner should consume around 300mW and other parts probably less than 100mW)
- Power supply --           3.7V li-ion battery (with 5V DC-DC converter from powerbank)
# Images
Will add soon...
# Schematics
Will add ater...
# External references
- https://www.onetransistor.eu/2014/08/mxl5007t-based-radio.html - Original idea to make this device, also contains library for the tv tuner itself
- https://earmark.net/gesr/cf.htm - Ceramic filter page, choosing right crystal filter for fm demodulator
- https://github.com/Chris--A/Keypad - Keypad library (can also be found on Arduino Playground)
- https://github.com/DeanIsMe/SevSeg - Library for 4-Digit 7-Segment Display
# TO DO
- Upgrade the code to work with menu button
- Fix displaying frequency when going under 100 MHz

