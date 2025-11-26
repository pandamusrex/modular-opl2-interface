/**
 * Depends on OPL2 library by DhrBahksteen
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset
 *   Pin  9 - A0
 *   Pin 10 - Latch
 *   Pin 11 - Data
 *   Pin 13 - Shift
 *
 * Modular interface - CV in (0-5V) on
 *   Pin 2 - Bass
 *   Pin 3 - Snare
 *   Pin 4 - HiHat
 *   Pin 5 - Crash
 *   Pin 6 - Cowbell
 *   Pin 7 - Claves
 */

#include <SPI.h>
#include <OPL2.h>
#include <midi_drums.h>

OPL2 opl2;
Instrument bass, snare, hihat, crash, cowbell, claves;
int ticks = 0;

int previous_state[8];
int new_state[8];

void setup() {
	opl2.begin();

	// Load drum instruments and assign them to OPL2 channels.
	bass = opl2.loadInstrument(DRUMINS_BASS_DR1);
	opl2.setInstrument(0, bass);
	snare = opl2.loadInstrument(DRUMINS_SNARE_AC);
	opl2.setInstrument(1, snare);
	hihat = opl2.loadInstrument(DRUMINS_HIHAT_CL);
	opl2.setInstrument(2, hihat);
	crash = opl2.loadInstrument(DRUMINS_CRASH);
	opl2.setInstrument(3, crash);
	cowbell = opl2.loadInstrument(DRUMINS_COWBELL);
	opl2.setInstrument(4, cowbell);
	claves = opl2.loadInstrument(DRUMINS_CLAVES);
	opl2.setInstrument(5, claves);

	//Serial.begin(9600);

	for (int pin=2; pin<8; pin++) {
		pinMode(pin - 2, INPUT);
		previous_state[pin] = LOW;
		new_state[pin] = LOW;
	}
}

void loop() {
	for (int pin=2; pin<8; pin++) {
		new_state[pin] = digitalRead(pin);
		if ( previous_state[pin] == LOW ) {
			if ( new_state[pin] == HIGH ) {
				//Serial.print("Atk ");
				//Serial.println(pin - 2);
				switch(pin) {
					case 2:
						opl2.playNote(0, bass.transpose / 12, bass.transpose % 12);
						break;
					case 3:
						opl2.playNote(1, snare.transpose / 12, snare.transpose % 12);
						break;
					case 4:
						opl2.playNote(2, hihat.transpose / 12, hihat.transpose % 12);
						break;
					case 5:
						opl2.playNote(3, crash.transpose / 12, crash.transpose % 12);
						break;
					case 6:
						opl2.playNote(4, cowbell.transpose / 12, cowbell.transpose % 12);
						break;
					case 7:
						opl2.playNote(5, claves.transpose / 12, claves.transpose % 12);
						break;
				} // end switch
			} // new HIGH
		} // previous LOW

		if ( previous_state[pin] == HIGH ) {
			if ( new_state[pin] == LOW ) {
				//Serial.print("Rel ");
				//Serial.println(pin - 2);
				opl2.setKeyOn(pin - 2, false);
			}
		}

		previous_state[pin] = new_state[pin];
	}
}