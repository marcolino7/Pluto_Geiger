/*
  Pluto Geiger
  -- #BUILD:8 --

 
  Basato sul progetto originale di Andrea Bosi alias Enotria
  e modifiche di Dario Tortato - Marconmeteo di radioactivity forum

Assegnazione PIN
0		- Seriale RX
1		- Seriale TX
2		- 
3		- 
4		- 
5		- Audio Out
6		- Buzzer
7		- Impulso Geiger
8		- Tasto +
9		- Tasto -
10		- SD CS
11		- SD DI
12		- SD DO
13		- SD Ck
A0 (14) - Tasto MENU
A1 (15) - Tasto SET
A2 (16) - VBatt IN
A3 (17) - 
A4 (18) - 
A5 (19) - 

Posizioni EEPROM
0x00	-	Base Tempi
0x01	-	Sensibilità Sonda int
0x02	-	Sensibilità Sonda int
0x03	-	mode 0=One Count 1=Loop Counts 2=Geiger
0x04	-	Unità di misura 0=mR/h 1=uR/h 2=uSv/h
0x05	-	Display Mode

Tensione Batteria
Cella Litio:	da 4,20 a 2,80 con un partitore formato da 2 reistenze all'1% da 330kk e 100k
				Tensione Max: 4,20V
				Tensione Min: 2,80V


0.1		-	Eliminato il doppio Setup
			Spostato il Beep anche quando il contatore è fermo
			Riordinati e Nominati tutti i PIN
			Collegata la SD
			Splash Screen
			Inizializzazione SD con messaggio di OK o Errore
			Primi Test di Lettura, Scrittura dei rilevamenti su SD
0.2		-	Aggiunto il Buzzer anche a fine lettura
			Sistemato il formato del codice
			Indicatore di scrittura su SD
			Beep All'accensione
			Log su SD completo
0.3		-	Ottimizzata la scrittura su File effettuando trim delle stringhe, ancora da finire
0.4		-	Sostituita la libresia SD con quella di Ladyada
			Scrittura del log con anche dtata e ora
0.5		-	Interfacciato il display con un PCF8574AP e recuperate 6 Porte
			L'indirizzo I2C del controller è 0x38
0.6		-	Corretto un Bug sul Beeper allo startup
			Spostata l'uscita audio sulla porta 5 del Micro in modo da pilotarla con lo stesso impulso del buzzer
			------------------------ PLUTO GEIGER ---------------------------------------------------------------
0.7		-	Reimpostato il programma a stati
			Eliminato il doppio setup all'avvio
			Diminuito lo sfarfallio del display
			Resi Modulari i setup in modo da poter essere riutilizzati
			Aggiunto il pulsante menù sul pin 14 (A0) e impostato per reiniziare il conteggio
			A fine conteggio se premo menù torno al menù rapido, se premo SET reinizio il conteggio
			Aggiunta la modalità per usufruire del conteggio One Count per eseguire un conteggio su tutte le basi tempi e fermarsi
				oppure Loops Count, per effettuare conteggi continui su tutte le basi tempi
			Aggiunta la modalità Geiger Count non ancora implementata
			Aggiunta la gestione del tasto Menù che interrompe il conteggio, portando al menù rapido
			Ottimizzata la scrittura su SD liberati 2K di flash
			Completata la gestione delle unità di misura
			Aggiunta la schermata di riepilogo iniziale. Se si preme SET, inizia il conteggio, se si preme menù si va nei settaggi
			Aggiunti i menù e il display per gestire l'impostazione di data e ora
			Aggiunta la gestione del menù avanzato dalla schremata di riepilogo premendo a lungo il tasto menù
			Aggiunta la modalità Geiger, con conteggio in tempo reale, con il fondo funziona
			Corretto un bug che non resettava i conteggi, all'avvio di una nuova misurazione
			Corretto un bug che caricava male la base tempi
			Modificato l'array delle unità di misura da string in char
			Aggiormo il display ogni 500ms a seguito dell'interrupt del timer2 su tutti i modi di conteggio
			Diversificati i delay a fine conteggio a seconda se si scrive o no su SD
			Modificati gli aggiornamenti del display in conteggio. Geiger ogni 500ms faccio i calcoli, ogni 10ms aggiorno
				Loop e Count, aggiorno ogni 100 ms durante il conteggio
			Aggiunta la scrittura del log durante il conteggio Geiger mediante la pressione del tasto menù, nella colonna secondi
				vengono scritti i secondi totali trascorsi
			Aggiunto nel log la scrittura della modalità di funzionamento
			Aggiunta una bozza della visualizzazione dello stato della batteria
0.8		-	Inserita la libreria per leggere i volt della batteria
			Impostata la resistenza di PullUp interna sui pulsanti
			Inserit la gestione dei settings della retroilluminazione
			Gestione Preliminare della retroilluminazione del display

*/

//Assegnazione PIN
#define PULSE_IN 7		//Ingresso dei segnali
#define KEY_MENU 14		//Tasto Menù
#define KEY_SET 15		//Tasto SET
#define KEY_UP 8		//Tasto +
#define KEY_DW 9		//Tasto -
#define BEEPER 6		//Buzzer
#define AUDIO_OUT 5		//Uscita Audio verso il PC
//#define mem_addr 0x51	//Indirizzo I2C della EEPROM
#define lcd_addr 0x20	//Indirizzo I2C del Display con la versione SMD è 20 con la versione pdip è 38

//Librerie
#include <EEPROM.h>				//libreria EEPROM
#include <SD.h>					//Libreria SD Ladyada - http://www.ladyada.net/products/microsd/
#include <avr/pgmspace.h>		//Libreria di Supporto SD Card
#include <stdio.h>				//C++ Standart I/O
#include <Wire.h>				//Libreria I2C
#include <RTClib.h>				//Libreria DS1307 RTC
#include <LiquidCrystal_I2C.h>	//Libreria LCD I2C con PCF8574AP - http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/
#include <Voltmetro.h>			//Libreria che calcola il voltaggio

//Versione Firmware
const String fw_version = "0.8";

//Inizializzo l'LCD via I2C
LiquidCrystal_I2C lcd(lcd_addr,16,2);	//inizializzo il display 16 col 2 righe


uint8_t SetTemp=0;       // Indice della Base Tempi
unsigned long TotImp=0;        // Totale degli impulsi
uint16_t BaseTempi=10;    // * * * Base tempi in secondi
unsigned long TempoMax=0;      // Termine conteggio
uint8_t VarServInt=0;     // Variabile di servizio
uint8_t VarServInt1=0;     // Variabile di servizio uno
unsigned int Sens=1000;      // Sensibilità in mR/h
float CPM=0;          // CPM
float Molt=6;         // * * * Moltiplicatore fra CP e CPM (dipende da BaseTempi)
float Rad=0;		  // Radioattività espressa in mR/h
int beep_flag=0;
uint8_t mode = 0;			//0 = One Count 1 = Loop Count 2 = Geiger
uint8_t count_units = 0;			//Unità di misura 0=mR/h 1=uR/h 2=uSv/h
char* units_desc[] = {"mR/h","uR/h","uSv/h"};
int T2count=0;			//Counter che viene incrementato dal timer 2
boolean lampeggio=0;			//variabile che si inverte ogni 500ms e serve per far lampeggiare le cose

uint8_t geiger_status = 3;	//Stato dell'apparecchio per gestire i loop
						//0 Setup Iniziale Parametri
						//1 Conteggio
						//2 Visualizzazione Risultati
						//3 Riassunto dei settaggi - Schermata iniziale

unsigned long lcd_millis = 0;	//Contiene i millis() a cui si è acceso il display
boolean lcd_state = 0;				//Contiene lo stato della illuminazione del display
//const char* lcd_desc[] = {"Off","On","30 Sec"};
uint16_t	lcd_mode_values[] = {0,0,10000,20000,30000};
uint8_t lcd_mode = 0;		//0=Off 
						//1=On 
						//2=10 Sec 
						//3=20 Sec 
						//4=30 Sec

//Conteggio in tempo reale/geiger
//float CPM	lo eredito
unsigned long sec_totali = 0;
unsigned long min_totali = 0;
boolean b500ms = false;
boolean b100ms = false;
long inizio = 0;	//contiene la variabile millis() al momento dell'inizio del conteggio
//long TotImp=0; lo eredito

//File su SD
File sd_file;
const int chipSelect = 10;
boolean sd_card_ok = false;

//Real Time Clock
	RTC_DS1307 RTC; //Dichiaro l'oggetto RTC
    //Array di cifre con lo zero davanti, per gestire la visualizzazione sul display
    const char* CifreConZero[] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};
	volatile uint8_t hour, minute, second, month, day;
	volatile uint16_t year;
	uint8_t timeedit_hh=0,timeedit_mm=0,timeedit_ss=0;	//Variabile che servono a modificare il display in edit


// Base Tempi ---------------
uint8_t Tempo[6]={
  10,30,60,180,600,1800};
float K[6]={
  6,2,1,.333,.1,.033};

//Variabili che Gestiscono il Voltmetro con la mia libreria
// Voltmetro(pin,R1,R2.VRef)
Voltmetro voltmt1(2,330000.0,100000.0,1.1); //4.80
//float v_voltmt1;

void setup() {
	lcd.init();					// Avvio L'RTC
	lcd.backlight();			// Accendo la retroilluminazione del display
	lcd_state = 1;
	pinMode(BEEPER, OUTPUT);	// Uscita Buzzer
	pinMode(AUDIO_OUT, OUTPUT);	// Uscita Audio
	pinMode(PULSE_IN,INPUT);			// Ingresso impulso della sonda
	//Inizializzo i pulsanti
	pinMode(KEY_SET,INPUT);		// Pulsante di selezione
	pinMode(KEY_UP,INPUT);		// Tasto +
	pinMode(KEY_DW,INPUT);		// Tasto -
	pinMode(KEY_MENU,INPUT);	// Tasto Menù
	//Attivo la resistenza di PullUp
	digitalWrite(KEY_SET,HIGH);
	digitalWrite(KEY_UP,HIGH);
	digitalWrite(KEY_DW,HIGH);
	digitalWrite(KEY_MENU,HIGH);

	//Imposto il riferimento dell'ADC a 1,1 volt
	analogReference(INTERNAL);

	//Spalsh Screen
	display_handle(0);	//Splash Screen
	delay(1500);

	digitalWrite(BEEPER, HIGH);
	delay(250); //100
	digitalWrite(BEEPER, LOW);

	//Inizializzo la SDCard
	pinMode(SS, OUTPUT);
	lcd.setCursor(0, 1); 
	if (!SD.begin(chipSelect)) {
		lcd.print("    SD Fail     ");
		sd_card_ok = false;
	}else{
		lcd.print("     SD OK      ");
		sd_card_ok = true;
	}
	delay(1500);

	//SD.remove("LETTURE.TXT");

	//Inizializzo il bus I2C
	Wire.begin();

	//Inizializzo l'RTC
	RTC_Handle(0);

	//Imposto gli interrupt sul Pin 7
	PCICR |= (1 << PCIE2);
	PCMSK2 = (1 << PCINT23) ; // pin7

	// ######---- Setup Timer2 to fire every 1ms ----######
		TCCR2B = 0x00;        //Disbale Timer2 while we set it up
		TCNT2  = 130;         //Reset Timer Count to 130 out of 255
		TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
		TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
		TCCR2A = 0x00;        //Timer2 Control Reg A: Normal port operation, Wave Gen Mode normal
		TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
	// ######---- Setup Timer2 to fire every 1ms ----######

	//Leggo i set dalla EEPROM
	EEPROM_Init_Read();

	//lcdBacklightHandle(1);	//Accendo la retro illuminazione

}

/*void lcdBacklightHandle(byte func){
	//func 0=Spegni 1=Accendi 2=Gestisce
	switch (func) {
		case 0: { //Spegni
			lcd.noBacklight();		//Spengo il display
			lcd_millis = 0;			//Vuoto la variabile
			lcd_state = 0;			//Display Spento
			break;
		}

		case 1: { //Accendi
			lcd.backlight();		//Accendo l'illuminazione
			lcd_millis = millis();  //Salvo l'istante dell'accensione
			lcd_state = 1;			//Display acceso
			break;
		}

		case 2: { //Gestisce
			switch (lcd_mode) {
				case 0: {	//Retroilluminazione spenta
					if (lcd_state == 1) {
						lcd.noBacklight();		//Spengo il display
						lcd_state = 0;			//Display Spento
					}
					break;
				}
				case 1: {	//Retroilluminazione sempre accesa
					if (lcd_state == 0) {
						lcd.backlight();		//Accendo l'illuminazione
						lcd_state = 1;			//Display acceso
					}
					break;
				}
				default: {	//Gestisco la retroilluminazione
					if (lcd_state == 1 ) {	//Se il display è acceso verifico il tempo
						if ( (millis() - lcd_millis) > lcd_mode_values[lcd_mode] ) {
							//se la differenza ta quando si è acceso il backligt e il momento attuale
							//è superiore al parametro impostato in lcd_mode_values
							//Spengo il display
							lcd.noBacklight();		//Spengo il display
							lcd_millis = 0;			//Vuoto la variabile
							lcd_state = 0;			//Display Spento
						}
					}
					break;
				}
			}
		}

	}
}*/


void RTC_Handle(int func){
	//Gestione dell'RTC
	switch (func) {
		case 0: {	//RTC Init
			RTC.begin();
			RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);	//Avvio l'uscita a 1Hz
			if (! RTC.isrunning()) {
				//Se l'RTC non � inizializzato imposto l'ora di default e lo avvio
				//Succede solo alla prima accensione o quando si scarica la batteria
				//RTC.adjust(DateTime(__DATE__, __TIME__));
				RTC.adjust(DateTime(2000,01,01,00,00,00));
				RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);
			}
			break;
		}
		
		case 1: {
			break;
		}
	}
}



void display_handle(int func) {
	//Gestione delle varie visualizzazioni fisse del display
	switch (func) {
		case 0: {	//Splash Screen
			lcd.clear();
			lcd.setCursor(2, 0); 
			lcd.print("Pluto Geiger");
			lcd.setCursor(4, 1); 
			lcd.print("Ver "+fw_version);
			break;
		}
		
		case 1: {	//Setup CPM Sonda
			lcd.clear();
			lcd.setCursor(0, 0); 
			lcd.print("Probe CPM x mR/h");
			//lcd.setCursor(0, 1); 
			//lcd.print("                ");
			break;
		}

		case 2: {	//Setup Base tempi
			lcd.clear();
			lcd.setCursor(4, 0); 
			lcd.print("TIME secs");
			//lcd.setCursor(0, 1); 
			//lcd.print("                ");
			break;
		}

		case 3:{
			// Aspetto del display durante il conteggio
			// Display presente durante il conteggio
			lcd.clear();
			lcd.setCursor(0, 0); 
			lcd.print("Time");
			lcd.setCursor(12, 0);
			lcd.print("sec.");
			lcd.setCursor(0, 1); 
			lcd.print("Pulse");
			break;
		}

		case 4: {
			// Visualizza il conteggio sul display
			lcd.setCursor(5,0);
			lcd.print("     ");  
			lcd.setCursor(6,0);
			lcd.print(int((TempoMax-millis())/1000));
			lcd.setCursor(6, 1); 
			lcd.print(TotImp);
			break;
	    }

		case 5: {	
			// Aspetto del display a fine conteggio
			// Display presente alla fine del conteggio
			lcd.clear();
			lcd.setCursor(0, 0); 
			lcd.print("CPM");
			//lcd.setCursor(0, 1);
			//lcd.print("                ");
			lcd.setCursor(0, 1);
			lcd.print(units_desc[count_units]);
		}

		case 6: {	
			//Scrive i risultati del conteggio sul display
			lcd.setCursor(5, 0); 
			lcd.print(CPM,0);
			lcd.setCursor(6, 1); 
			lcd.print(Rad,3);
			lcd.setCursor(0, 0);
			break;
		}
		case 7:{
			//Aspetto del display durante il setup della modalità operativa
			lcd.clear();
			lcd.setCursor(3,0);
			lcd.print("Count Mode");
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			break;
	   }
		case 8:{
			//Valore del settaggio durante il setup della modalità operativa
			lcd.setCursor(3,1);
			if (mode == 0) lcd.print(" One Count");
			if (mode == 1) lcd.print("Loop Counts");
			if (mode == 2) lcd.print(" Geiger");
			break;
	   }
		case 9:{
			//Aspetto del display durante il setup delle Unità di misura
			lcd.clear();
			lcd.setCursor(3,0);
			lcd.print("Count Unit");
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			break;
	   }
		case 10:{
			//Valore del settaggio durante il setup della
			//Unità di misura 0=mR/h 1=uR/h 2=uSv/h
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			lcd.setCursor(5,1);
			lcd.print(units_desc[count_units]);
			break;
	   }
		case 11:{
			//Schermo di riepilogo iniziale
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Prb:");
			lcd.setCursor(4,0);
			lcd.print(Sens);
			lcd.setCursor(10,0);
			lcd.print(" ");
			if (mode == 0) lcd.print("One");
			if (mode == 1) lcd.print("Loop");
			if (mode == 2) lcd.print("Geig");
			//Seconda Riga
			lcd.setCursor(0,1);
			lcd.print("Sec:");
			lcd.setCursor(4,1);
			lcd.print(BaseTempi,DEC);
			lcd.setCursor(11,1);
			lcd.print(units_desc[count_units]);
			
			break;
	   }
		case 12:{
			//Valore del settaggio durante il setup dell'Ora
			lcd.clear();
			lcd.setCursor(3,0);
			lcd.print("System Time");
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			break;
	   }
		case 13:{
			//Visualizzazione dell'ora e modifica delle ore
			lcd.setCursor(4,1);
			gestione_cifre(hour,timeedit_hh);
			lcd.setCursor(6,1);
			lcd.print(":");
			lcd.setCursor(7,1);
			gestione_cifre(minute,timeedit_mm);
			lcd.setCursor(9,1);
			lcd.print(":");
			lcd.setCursor(10,1);
			gestione_cifre(second,timeedit_ss);
			break;
	   }
		case 14:{
			//Valore del settaggio durante il setup della data
			lcd.clear();
			lcd.setCursor(3,0);
			lcd.print("System Date");
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			break;
	   }
		case 15:{
			//Visualizzazione della data e modifica della data
			lcd.setCursor(3,1);
			gestione_cifre(day,timeedit_hh);
			lcd.setCursor(5,1);
			lcd.print("/");
			lcd.setCursor(6,1);
			gestione_cifre(month,timeedit_mm);
			lcd.setCursor(8,1);
			lcd.print("/");
			lcd.setCursor(9,1);
			gestione_cifre(year,timeedit_ss);
			break;
	   }
		case 16:{
			//Visualizzazione del display per il conteggio in tempo reale Geiger
			lcd.clear();
			lcd.setCursor(0,0);
			//lcd.print("CPM:     Mn:    ");
			lcd.print("CPM:");
			lcd.setCursor(9,0);
			lcd.print("Mn:");
			//lcd.setCursor(0,1);
			//lcd.print("                ");
			lcd.setCursor(9,1);
			lcd.print(units_desc[count_units]);
			break;
	   }
		case 17:{
			//Visualizzazione del conteggio in tempo reale Geiger
			lcd.setCursor(4,0);
			lcd.print("     ");	//Scrivo del bianco
			lcd.setCursor(4,0);
			lcd.print(CPM,0);	//Scrivo i CPM
			lcd.setCursor(12,0);
			lcd.print(min_totali);	//Scrivo i minuti
			lcd.setCursor(2, 1); 
			lcd.print("       ");	//Scrivo del bianco
			lcd.setCursor(2, 1);
			lcd.print(Rad,3);
			break;
	   }
		case 18:{
			//Visualizzazione del display dello stato della batteria
			lcd.clear();
			lcd.setCursor(5,0);
			lcd.print("Battery");	//Scrivo del bianco
			//lcd.setCursor(0,1);
			//lcd.print("                ");	//Scrivo del bianco
			break;
	   }
		case 19:{
			//Visualizzazione dello stato della batteria
			lcd.setCursor(0,1);
			//lcd.print(analogRead(2));	//Scrivo del bianco
			lcd.setCursor(6,1);
			//lcd.print((1.1*analogRead(2))/1023);
			lcd.print(voltmt1.getVoltage());
			break;
	   }
		case 20:{
			//Impostazioni del Display Schermo Statico
			lcd.clear();
			lcd.setCursor(4,0);
			lcd.print("BackLight");	//Scrivo del bianco
			lcd.setCursor(0,1);
			break;
	   }
		case 21:{
			//Impostazioni del Display Schermo Statico
			//lcd.setCursor(0,1);
			lcd.setCursor(4, 1); 
				lcd.print("      ");
				lcd.setCursor(4, 1);
				switch (lcd_mode)  {
					case 0:
						lcd.print("Off");
						break;
					case 1:
						lcd.print("On");
						break;
					case 2:
						lcd.print("30 sec");
						break;
				}
			break;
	   }

	}
}

void setting_handle(int func) {
	//Gestice le impostazioni
	switch (func) {
		case 0:{	//Sensibilità Sonda
			display_handle(1);
			delay(500);
			do {
				Buzzer();
				lcd.setCursor(6, 1); 
				lcd.print("    ");
				lcd.setCursor(6, 1);
				lcd.print(Sens);
				if (digitalRead(KEY_UP)== HIGH && Sens<64000) {
					if (Sens < 10000) Sens=Sens+50;
					else Sens=Sens+1000;      
				}
				if (digitalRead(KEY_DW)== HIGH && Sens >= 100) { 
					if (Sens < 10000) Sens=Sens-50;
					else Sens=Sens-1000;
				}
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						lcd.setCursor(6, 1); 
						lcd.print("    ");
						EEPROMWriteInt(0x01,Sens);     // Scrive Sensibilità Sonda nella EEPROM
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 1: {	//Base Tempi
			display_handle(2);
			delay(500);
			do {
				Buzzer();
				BaseTempi= Tempo[SetTemp];
				Molt= K[SetTemp];
				lcd.setCursor(6, 1); 
				lcd.print("    ");
				lcd.setCursor(6, 1);
				lcd.print(BaseTempi,DEC);
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && SetTemp < 6) SetTemp++;
				if (digitalRead(KEY_DW)== HIGH && SetTemp > 0) SetTemp--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						lcd.setCursor(6, 1); 
						lcd.print("    ");
						EEPROM.write(0x00,SetTemp);    // Scrive Set della Base Tempi       
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 2: { //Modalità Scaler o Ratemeter
			display_handle(7);
			delay(500);
			do {
				Buzzer();
				display_handle(8);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && mode < 3) mode++;
				if (digitalRead(KEY_DW)== HIGH && mode > 0) mode--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						lcd.setCursor(6, 1); 
						lcd.print("    ");
						EEPROM.write(0x03,mode);    // Scrive Set della Base Tempi       
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 3: { //Unità di misura
			display_handle(9);
			delay(500);
			do {
				Buzzer();
				display_handle(10);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && count_units < 3) count_units++;
				if (digitalRead(KEY_DW)== HIGH && count_units > 0) count_units--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						lcd.setCursor(6, 1); 
						lcd.print("    ");
						EEPROM.write(0x04,count_units);    // Scrive Set della Base Tempi       
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 4: { //Ora
			display_handle(12);
			delay(500);
			DateTime now = RTC.now();	//Leggo data e ora
			hour = now.hour();
			minute = now.minute();
			second = now.second();
			timeedit_hh=1;
			do {
				Buzzer();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && hour < 25) hour++;
				if (digitalRead(KEY_DW)== HIGH && hour > 0) hour--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						goto _minute;	//Vai alla gestione dei minuti
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
_minute:
			timeedit_hh=0;
			timeedit_mm=1;	//modifico il dipslay per editare i minuti
			do {
				Buzzer();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && minute < 60) minute++;
				if (digitalRead(KEY_DW)== HIGH && minute > 0) minute--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						goto _second;	//Vai alla gestione dei minuti
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
_second:
			timeedit_mm=0;
			timeedit_ss=1;	//modifico il dipslay per editare i secondi
			do {
				Buzzer();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && second < 60) second++;
				if (digitalRead(KEY_DW)== HIGH && second > 0) second--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						DateTime now = RTC.now();	//Leggo data e ora
						day = now.day();
						month = now.month();
						year = now.year();
						RTC.adjust(DateTime(year,month,day,hour,minute,second));
						//Imposto L'ora e non cambio la data
						//Eseguo il comando per abilitare l'uscita a 1HZ in caso sia il primo setup
						RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			timeedit_ss=0;	//Resetto tutte le variabili di modifica
			break;
		}

		case 5: { //Data
			display_handle(14);
			delay(500);
			DateTime now = RTC.now();	//Leggo data e ora
			day = now.day();
			month = now.month();
			year = now.year();
			timeedit_hh=1;
			do {
				Buzzer();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && day < 31) day++;
				if (digitalRead(KEY_DW)== HIGH && day > 1) day--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						goto _month;	//Vai alla gestione dei minuti
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
_month:
			timeedit_hh=0;
			timeedit_mm=1;	//modifico il dipslay per editare i minuti
			do {
				Buzzer();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && month < 12) month++;
				if (digitalRead(KEY_DW)== HIGH && month > 1) month--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						goto _year;	//Vai alla gestione dei minuti
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
_year:
			timeedit_mm=0;
			timeedit_ss=1;	//modifico il dipslay per editare i secondi
			do {
				Buzzer();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && year < 2500) year++;
				if (digitalRead(KEY_DW)== HIGH && year > 2000) year--;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						DateTime now = RTC.now();	//Leggo data e ora
						hour = now.hour();
						minute = now.minute();
						second = now.second();
						RTC.adjust(DateTime(year,month,day,hour,minute,second));
						//Imposto L'ora e non cambio la data
						//Eseguo il comando per abilitare l'uscita a 1HZ in caso sia il primo setup
						RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			timeedit_ss=0;	//Resetto tutte le variabili di modifica
			break;
		}
		case 6: { //Visualizza stato della batteria
			display_handle(18);
			delay(500);
			do {
				Buzzer();
				display_handle(19);	// Visualizzo il valore salvato EEPROM
				delay(50);
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 7: { //Impostazioni del Display
			display_handle(20);
			delay(500);
			do {
				Buzzer();
				display_handle(21);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && lcd_mode < 4) lcd_mode++;
				if (digitalRead(KEY_DW)== HIGH && lcd_mode > 0) lcd_mode--;
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						lcd.setCursor(4, 1); 
						lcd.print("      ");
						EEPROM.write(0x05,lcd_mode);    // Scrive Set della Base Tempi       
					}
					//Gestisco il display in base al valore scelto
					//if (lcd_mode == 0) lcdBacklightHandle(0);
					//else lcdBacklightHandle(1);
				}


			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}

	}
}

void gestione_cifre(int dato, int status){
//In questa routine normalizzo le cifre di data e ora e le faccio lampeggiare se sono in modifica
//dato = Dato da normalizzare
//status = Modalità 0=visualizza 1=lampeggia

	String str;
        
        if (dato < 10) {  //Se il numero è minore di 10 lo normalizzo prendendo dall'array, altrimenti lo converto
            str = CifreConZero[dato];
        } else {
            str = String(dato,DEC);  
        }
        
        if (status == 1){  //Se sono in modifica per la cifra
          if (lampeggio == 0){          //E se il lampeggio è 0
            if (dato > 100) lcd.print("    ");  //Se sto gestendo l'anno scrivo 4 spazi
			else lcd.print("  ");				//altriementi ne scrivo 2
          }else{
            lcd.print(str);             //Altrimenti scrivo la cifre 
          }  
        }else{
          lcd.print(str);               //Se non sono in modifica per quella cifra, la scrivo normale 
        }  
}

void EEPROM_Init_Read() {
	SetTemp=EEPROM.read(0x00);		// Legge Set della Base Tempi
	if (SetTemp==255) SetTemp = 0;	// Se la EEPROM è vuota, imposto 10 secondi
	BaseTempi= Tempo[SetTemp];		// Imposto la base tempi recuperandola dall'array
	
	Sens=EEPROMReadInt(0x01);		// Legge Sensibilità Sonda
	if (Sens==0xFF00) Sens = 3500;	// Se la EEPROM è vuota a 32767 (Integer), imposto la sensibilità a 3500
	if (Sens==0xFF) Sens = 200;	// Se la EEPROM è vuota a 255 (Byte), imposto la sensibilità a 200

	mode=EEPROM.read(0x03);					// Modalità Scaler, Ratemeter o Geiger
	if (mode > 2) mode = 0;					// Se la EEPROM è vuota, imposto a One Count

	count_units=EEPROM.read(0x04);			//Unità di misura 0=mR/h 1=uR/h 2=uSv/h
	if (count_units==255) count_units=2;	//Se la EEPROM è vuota, imposto i uSv/h come default

	lcd_mode=EEPROM.read(0x05);				//Modalita dell'LCD
	if (count_units==255) lcd_mode=1;	//Se la EEPROM è vuota, imposto il display sempre acceso

}

void QuickSet_Handle() { 
	// Visualizza e Cambia la modalità di funzionamento
 
	setting_handle(6);	//stato batteria
	setting_handle(0);
	setting_handle(1);
	setting_handle(2);
	setting_handle(3);
	//setting_handle(4);
	//setting_handle(5);


	Sensibilita:    
	/* Sensibilità della sonda in CPM x 1 mR/h
	Il massimo indicabile è 63000, eventualmente indicare 
	il valore togliendo 3 zeri, es. sensibilità di 100000 CPM/mR/h
	impostare 100 e dividere la lettura per 1000
	*/
   	TotImp=0;
	geiger_status = 3; //Dopo il setup Iniziale Torno al riepilogo

}

void FullSet_Handle() { 
	// Visualizza e Cambia la modalità di funzionamento
 
	setting_handle(0);
	setting_handle(1);
	setting_handle(2);
	setting_handle(3);
	setting_handle(4);
	setting_handle(5);
	setting_handle(6);
	setting_handle(7);

   	TotImp=0;
	geiger_status = 3; //Dopo il setup Iniziale Torno al riepilogo

}


void MainSettingsRecap() {
	//routine che visualizza il riepilogo dei settaggi
	//e aspetta un tasto
	display_handle(11);
	key_wait();
	if (VarServInt==LOW) geiger_status = 1;		//E' Stato premuto SET e Comincio il conteggio con i settaggi attuali
	if (VarServInt1==LOW) geiger_status = 0;	//E' Stato premuto MENU e vado nel setup rapido dello strumento
	delay(100);
		//Gestisco il menù avanzato con la pressione prolungata del tasto menù
		if (digitalRead(KEY_MENU) == LOW){
		delay(800);
		if (digitalRead(KEY_MENU) == LOW){
			//Gestire il menù esteso
			geiger_status = 4;
		}
	}
}


void loop(){
	//Loop Principale
	geiger_handle();
	//lcdBacklightHandle(2);	//Gestico la retroilluminazione
}

void geiger_handle() {
		//Gestione Principale del Geiger
	switch (geiger_status){
		case 0: {
			QuickSet_Handle();		//Setup Rapido
			break;
		}
		case 1: {
			pulse_count();			//Conteggio loop, one o geiger
			break;	
		}
		case 2: {
			end_count();			//visualizzazione del conteggio one o loop
			break;
		}
		case 3: {
			MainSettingsRecap();	//Riepilogo delle impostazioni
			break;
		}
		case 4: {
			FullSet_Handle();		//Setup di tutte le opzioni
			break;
		}

	}
}

void pulse_count(){

	//mode == 0 One Count
	//mode == 1 Loop Counts
	//mode == 2 Geiger

		inizio = 0;		//azzero le variabili
		sec_totali = 0;		//azzero le variabili	
		min_totali = 0;		//azzero le variabili
		CPM = 0;
		TotImp = 0;
		Rad = 0;

	if (mode==2){
		//Conteggio Geiger
		delay(500);
		display_handle(16);		//Visualizzo la parte statica del display
		inizio = millis();	//Momento iniziale del conteggio
		do{
			Buzzer();			//Suono il buzzer se necessario
			if (b500ms==true){	//Faccio le operazioni ogni 500ms 
				sec_totali = (millis()-inizio) / 1000;	//Secondi totali dall'inizio del conteggio
				min_totali = sec_totali/60;				//Minuti totali, servono per il display
				CPM = (float(TotImp)/float(sec_totali));
				CPM = CPM*60;							//CPM totali dall'inizio del conteggio
				switch (count_units){					//In base all'unità di misura, conteggio il valore
				  case 0:
						Rad=CPM/Sens;            
						break;
				  case 1:
						Rad=(CPM/Sens)*1000;
						break;
				  case 2:
						Rad=(CPM/Sens)*10;
						break;
				}
				b500ms=false;	//Resetto la variabile dell'interrupt ogni 500ms
			}
			if (b100ms == true){		//aggiorno il display ogni 100ms
				display_handle(17);		//Visualizzo sul display la parte dinamica
				b100ms = false;
			}
			//Se premo il tasto menù dirante il conteggio scrivo il log
			if (digitalRead(KEY_MENU)==LOW) {
				delay(200); //debounc
				if (digitalRead(KEY_MENU)==LOW) Log_Write();
			}
		}while (digitalRead(KEY_SET)==HIGH);	//Attendo la pressione del tasto SET e.....

		geiger_status = 3;	//Vado alla schermata di riepilogo generale

	}else{
		//Conteggio One Count o Loop Count
		TempoMax=millis()+BaseTempi*1000;
		display_handle(3);	//Display per il conteggio
		do {
			Buzzer();
			// Visualizza conteggio in corso 10 volte ogni secondo
			//if (millis()%100 < 1) display_handle(4); //Display in conteggio
			//Visualizzo il conteggio ogni 500 ms
			if (b100ms == true) {
				display_handle(4);
				b100ms = false;
			}
			VarServInt1=digitalRead(KEY_MENU); //Leggo il tasto menù per vedere se interrompere
		} 
		while (TempoMax > millis() && VarServInt1==HIGH);
	
		if (VarServInt1==LOW) geiger_status = 0;	//Se premo menù esco e torno al setup rapido
		else geiger_status = 2;						//Se Fine del conteggio vado alla visualizzazione
	}
}


void end_count() {
	// Finito il conteggio mostra risultato e poi resetta

	digitalWrite(BEEPER, HIGH);
	delay(70);
	digitalWrite(BEEPER, LOW);
 
	cli();
	CPM=TotImp*Molt;
	sei();

	//Unità di misura 0=mR/h 1=uR/h 2=uSv/h
	//Rad=CPM/Sens*10;

	switch (count_units){
      case 0:
            Rad=CPM/Sens;            
            break;
      case 1:
            Rad=(CPM/Sens)*1000;
            break;
      case 2:
            Rad=(CPM/Sens)*10;
            break;
     } 

	display_handle(5);
	display_handle(6);  

	//Provo a Scrivere in un File
	Log_Write();

	if (mode == 0) key_wait();			//Mode = 0 = One Count
	if (mode == 1) {					//Mode = 1 = Loop Count Aspetto 5 secondi e ricomincio
		if (sd_card_ok == true) delay(2500);	//se sto usando una SD in delay deve essere minore in quanto bisogna sommare il tempo di scrittura
		else delay(5000); 
	}
	cli();		//Disabilito gli interrupt
	TotImp=0;	//Resetto il conteggio
	sei();		//Riabilito gli interrupt
	TempoMax=millis()+BaseTempi*1000;  // Nuovo tempo limite

	if (mode == 0) {	//Se One Count, aspetto il tasto
		if (VarServInt==LOW) geiger_status = 1;	//E' Stato premuto SET e Ricomincio il conteggio
		if (VarServInt1==LOW) geiger_status = 0;	//E' Stato premuto MENU e vado nel setup rapido iniziale
	}
	if (mode == 1) geiger_status = 1;	//Se Loop Count reinizio il conteggio
}


void key_wait(){
// Attende la pressione del tasto Set/Start o Menù
  delay(500);
  do{      
      VarServInt=digitalRead(KEY_SET);
	  VarServInt1=digitalRead(KEY_MENU);
	  Buzzer();
  }  
  while (VarServInt==HIGH && VarServInt1==HIGH );
}

void Log_Write(){

	if (sd_card_ok == true) {  //Se la card è ok scrivo, altrimenti desisto
		//Indico la scrittura su SD sul display
		lcd.setCursor(15,1);
		lcd.print("W");
		delay(50);

		//apro il file in scrittura, se non esiste lo crea
		sd_file = SD.open("LOG_1.TXT", FILE_WRITE);
		delay(50);
		DateTime now = RTC.now();	//Leggo data e ora
		delay(100);
		//Compongo la riga da scrivere sul file
		sd_file.print(now.day(),DEC);
		delay(50);
		sd_file.print("/");
		delay(50);
		sd_file.print(now.month(),DEC);
		delay(50);
		sd_file.print("/");
		delay(50);
		sd_file.print(now.year(),DEC);
		delay(50);
		sd_file.print(",");
		delay(50);
		sd_file.print(now.hour(),DEC);
		delay(50);
		sd_file.print(":");
		delay(50);
		sd_file.print(now.minute(),DEC);
		delay(50);
		sd_file.print(":");
		delay(50);
		sd_file.print(now.second(),DEC);
		delay(50);
		sd_file.print(",");
		delay(50);
		//Se sono in Geiger Mode scrivo i seconti totali
		if (mode==2) sd_file.print(sec_totali,DEC);
		else sd_file.print(BaseTempi,DEC);
		delay(50);
		sd_file.print(",");
		delay(50);
		sd_file.print(CPM,2);
		delay(50);
		sd_file.print(",");
		delay(50);
		sd_file.print(Rad,3);
		delay(50);
		sd_file.print(",");
		delay(50);
		sd_file.print(units_desc[count_units]);
		delay(50);
		sd_file.print(",");
		delay(50);
		sd_file.println(mode);
		
		delay(100);
		sd_file.flush();		//Scrive definitivamente su SD
		delay(100);
		sd_file.close();		//Chiude il file
		delay(100);
		//Finita la scrittura su SD
		lcd.setCursor(15,1);
		lcd.print(" ");  

		//Delay totale 1400 mS

	}
}

ISR(PCINT2_vect){ 
	// scatta solo quando il pin 7  cambia stato
	if ( (PIND&(1<<7))!=0) // somma solo quando cambia in su
	TotImp++;
	beep_flag=1;
}

ISR(TIMER2_OVF_vect) {
	//T2 interrupt è lanciato ogni 1ms così da poterlo usare 
	//per temporizzare svariate cose

	T2count++;					//Increments the interrupt counter
	if (T2count == 100 || T2count == 200 || T2count == 300 || T2count == 400 || T2count == 500) {
		//verifico i 100 ms. Faccio così per evitare di portarmi appresso un altro counter
		b100ms = true;
	}
	if (T2count == 500){
		lampeggio = !lampeggio;  //Gestisco il lampeggio delle cifre
		b500ms = true;
		T2count = 0;
	}
	TCNT2 = 130;           //Reset Timer to 130 out of 255
	TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
}  

void Buzzer(){
	if(beep_flag==1){ //il beep_flag � settato, devo far suonare il buzzer!
		digitalWrite(BEEPER, HIGH);
		//digitalWrite(AUDIO_OUT, HIGH);
		delay(1);
		digitalWrite(BEEPER, LOW);
		//digitalWrite(AUDIO_OUT, LOW);
		beep_flag=0; //resetta il beep_flag
	}
}

//This function will write a 2 byte integer to the eeprom at the specified address and address + 1
void EEPROMWriteInt(int p_address, int p_value){
	byte lowByte = ((p_value >> 0) & 0xFF);
	byte highByte = ((p_value >> 8) & 0xFF);
	EEPROM.write(p_address, lowByte);
	EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address){
	byte lowByte = EEPROM.read(p_address);
	byte highByte = EEPROM.read(p_address + 1);
	return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
