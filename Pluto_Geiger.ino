/*
  Pluto Geiger
 
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
0x06	-	Probe Preset Position

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
0.9		-	Ottimizzazione del codice per liberare RAM
			Settings Gestione display
			Gestione del tasto menù nei settaggi per non scrivere tutte le volte nella EEPROM
			Modificato il comportamento del tasto SET durante il conteggio. Geiger Torna al riepilogo, One Count e Loop Salva e torna al riepilogo
			Ulteriori ottimizzazioni al dipslay liberata altra RAM
			Ottimizzato ulteriormente il codice e liberata ancora un po' di RAM
			Inserita la grafica per la gestione della batteria. Manca la logica
			Versione iniziale della logica della batteria
			Sistemato il controllo batteria da verificare la formula
			Sistemato il display Settings per visualizzare lo stato della batteria
0.10	-	Gestione display completa dei timeout, rimane solo da fare la schermata di scrittura
			Impostazioni Timeout Retroilluminazione a 10,20,30,60,120 secondi
			Aggiunti 2 livelli di warning alla batteria al 5% e al 2%
			Corretto il simbolo di batteria oltre il 100%
			Aggiunte maggiori sensibilità sul settaggio della sonda
			Aggiunto il segno della corrente quando la batteria è a zero
0.11	-	Aggiunto il settaggio e la gestione del Pre-Set della sensibilità delle sonde
			Aggiunte le sonde LND e quella dall'SV500
			Ottimizzato il codice eliminando stringhe per libebrare RAM
0.12	-	Spostate alcune stringhe nella FLASH per liberare RAM
0.13	-	Iniziato lo Sviluppo della Scala Automatica
			Scala automatica implementata prima versione da testare
			Spostate le stringhe della SD in Progmem
0.14	-	Corretti i bug sulla base tempi
			Corrette alcuni layout del dipslay
0.15	-	Contatore Geiger con 3 tempi di campionamento
			Spostata la scritta a string_1 fruori da PROGMEM direttamente in RAM, per fare posto al firmware
			Reinserita string1 vuota, altrimenti spostava tutte le scritte del display
			Aumentato il refresh del display a 300ms
0.16	-	Rimossi i Röntgen come misura


*/

//Assegnazione PIN
#define PULSE_IN 7		//Ingresso dei segnali
#define KEY_MENU 14		//Tasto Menù
#define KEY_SET 15		//Tasto SET
#define KEY_UP 8		//Tasto +
#define KEY_DW 9		//Tasto -
#define BEEPER 6		//Buzzer
#define AUDIO_OUT 5		//Uscita Audio verso il PC
#define lcd_addr 0x20	//Indirizzo I2C del Display con la versione SMD è 20 con la versione pdip è 38

//Librerie
#include <EEPROM.h>				//libreria EEPROM
#include <SD.h>					//Libreria SD Ladyada - http://www.ladyada.net/products/microsd/
#include <avr/pgmspace.h>		//Libreria di Supporto SD Card e per salvare le variabili stringa nella FLASH
#include <Wire.h>				//Libreria I2C
#include <RTClib.h>				//Libreria DS1307 RTC
#include <LiquidCrystal_I2C.h>	//Libreria LCD I2C con PCF8574AP - http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/
#include <Voltmetro.h>			//Libreria che calcola il voltaggio


//Versione Firmware
const String fw_version = "0.16";

//Inizializzo l'LCD via I2C
LiquidCrystal_I2C lcd(lcd_addr,16,2);	//inizializzo il display 16 col 2 righe

unsigned long TotImp=0;			// Totale degli impulsi
uint16_t BaseTempi=10;			// * * * Base tempi in secondi
unsigned long TempoMax=0;		// Termine conteggio
uint8_t VarServInt=0;			// Variabile di servizio
uint8_t VarServInt1=0;			// Variabile di servizio uno
float CPM=0;					// CPM
float Molt=6;					// * * * Moltiplicatore fra CP e CPM (dipende da BaseTempi)
float Rad=0;					// Radioattività espressa nella scala calcolata
float RadRaw=0;					// Radioattività senza moltiplicatore
bool beep_flag=0;
uint8_t mode = 0;				//0 = One Count 1 = Loop Count 2 = infinite 3 = Geiger1 4 = Geiger2 5 = Geiger3

uint8_t geiger_calc_time=0;		//Variabile che contiene il numero di volte che devono passare 100ms prima di fare il conteggio del geiger

//Unità di Misura
//uint8_t c_unit = 0;														//Unità di misura 0=Sievert, 1=Röntgen
//prog_char unit_0[] PROGMEM = "Siev.";
//prog_char unit_1[] PROGMEM = "Ront.";
//const char *unit_set_desc[] PROGMEM = {unit_0,unit_1};					//Nomi delle misure per i settings
//const char *unit_set_desc[] PROGMEM = {unit_0};					//Nomi delle misure per i settings
prog_char unit_sv_0[] PROGMEM =	"Sv/h";
prog_char unit_sv_1[] PROGMEM = "mSv/h";
prog_char unit_sv_2[] PROGMEM = "uSv/h";
const char *unit_sv_desc[] PROGMEM = {unit_sv_0,unit_sv_1,unit_sv_2};	//Misure in Sievert
//prog_char unit_rt_0[] PROGMEM =	"R/h";
//prog_char unit_rt_1[] PROGMEM = "mR/h";
//prog_char unit_rt_2[] PROGMEM = "uR/h";
//const char *unit_rt_desc[] PROGMEM = {unit_rt_0,unit_rt_1,unit_rt_2};	//Misure in Röntgen

int T2count=0;					//Counter che viene incrementato dal timer 2
boolean lampeggio=0;			//variabile che si inverte ogni 500ms e serve per far lampeggiare le cose

uint8_t geiger_status = 3;	//Stato dell'apparecchio per gestire i loop
							//0 Setup Iniziale Parametri
							//1 Conteggio
							//2 Visualizzazione Risultati
							//3 Riassunto dei settaggi - Schermata iniziale

uint8_t display_refresh=0;			//Variabile che si incrementa ogni 100ms per effettuare il refresh del display
unsigned long lcd_millis = 0;		//Contiene i millis() a cui si è acceso il display
boolean lcd_state = 0;				//Contiene lo stato della illuminazione del display
const char* lcd_desc[] = {"Off","On","10","20","30","60","120"};
uint16_t	lcd_mode_values[] = {0,0,10000,20000,30000,60000,120000};
uint8_t lcd_mode = 0;				//0=Off 
									//1=On 
									//2=10 Sec 
									//3=20 Sec 
									//4=30 Sec

//Sonda
uint16_t	probe_preset_list[] = {0,3600,500};		//Array con i Preset
uint8_t		probe_preset = 0;						//Preset selezionato, 0=nessun preset
unsigned int SensCustom=0;							//Sensibilità della sonda custom impostata nei parametri
unsigned int Sens=1000;								// Sensibilità della sonda in CPM in mR/h usata nei conteggi


//Carattere con il simbolo del livello della batteria
byte batt0[8] = {0b00100,0b11111,0b10001,0b10001,0b10001,0b10001,0b10001,0b11111};
byte batt20[8] = {0b00100,0b11111,0b10001,0b10001,0b10001,0b10001,0b11111,0b11111};
byte batt40[8] = {0b00100,0b11111,0b10001,0b10001,0b10001,0b11111,0b11111,0b11111};
byte batt60[8] = {0b00100,0b11111,0b10001,0b10001,0b11111,0b11111,0b11111,0b11111};
byte batt80[8] = {0b00100,0b11111,0b10001,0b11111,0b11111,0b11111,0b11111,0b11111};
byte batt100[8] = {0b00100,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
byte battPWR[8] = {0b01010,0b01010,0b11111,0b10001,0b10001,0b01110,0b00100,0b00100};
byte battEmpty[8] = {0b00100,0b11111,0b10001,0b11011,0b10101,0b11011,0b10001,0b11111};
byte battAlert[8] = {0b10001,0b10001,0b01010,0b00100,0b00100,0b01010,0b10001,0b10001};

//Conteggio in tempo reale/geiger
//float CPM	lo eredito
unsigned long sec_totali = 0;
unsigned long min_totali = 0;
boolean b500ms = false;
boolean b100ms = false;
long inizio = 0;	//contiene la variabile millis() al momento dell'inizio del conteggio

//File su SD
File sd_file;
const int chipSelect = 10;
boolean sd_card_ok = false;

//Real Time Clock
RTC_DS1307 RTC;			//Dichiaro l'oggetto RTC
//Array di cifre con lo zero davanti, per gestire la visualizzazione sul display
const char* CifreConZero[] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09"};
volatile uint8_t hour, minute, second, month, day;
volatile uint16_t year;
uint8_t timeedit_hh=0,timeedit_mm=0,timeedit_ss=0;	//Variabile che servono a modificare il display in edit

// Base Tempi ---------------
uint8_t		SetTemp=0;				// Indice della Base Tempi
uint16_t	Tempo[6]={10,30,60,180,600,1800};
float		K[6]={6,2,1,.333,.1,.033};

//Variabili che Gestiscono il Voltmetro con la mia libreria
// Voltmetro(pin,R1,R2.VRef)
Voltmetro voltmt1(2,330000.0,100000.0,1.1);
uint8_t batt_perc = 0;

//Variabili per il display salvate nella flash
prog_char string_0[] PROGMEM = "Pluto Geiger";   
prog_char string_1[] PROGMEM = "";	//Prb CPM x mR/h
prog_char string_2[] PROGMEM = "";  //Prb Preset
prog_char string_3[] PROGMEM = "";   //BackLight
prog_char string_4[] PROGMEM = "";	//Battery		
prog_char string_5[] PROGMEM = "Siev.";	//Count Unit //Siev
prog_char string_6[] PROGMEM = "";	//Counter Mode
prog_char string_7[] PROGMEM = "One Count";
prog_char string_8[] PROGMEM = "Loop Count";
prog_char string_9[] PROGMEM = "Infinite";
prog_char string_10[] PROGMEM = "sec";
prog_char string_11[] PROGMEM = "SD Fail";
prog_char string_12[] PROGMEM = "SD OK";
prog_char string_13[] PROGMEM = "Time sec";
prog_char string_14[] PROGMEM = "Time";
prog_char string_15[] PROGMEM = "Pulse";
prog_char string_16[] PROGMEM = "CPM";
prog_char string_17[] PROGMEM = ":";
prog_char string_18[] PROGMEM = "Prb:";
prog_char string_19[] PROGMEM = "Sec:";
prog_char string_20[] PROGMEM = "Time";
prog_char string_21[] PROGMEM = "Date";
prog_char string_22[] PROGMEM = "Mn:";
prog_char string_23[] PROGMEM = "/";
prog_char string_24[] PROGMEM = ",";
prog_char string_25[] PROGMEM = "Geiger";

// Then set up a table to refer to your strings.
const char *string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9,
										string_10, string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19,
										string_20, string_21, string_22, string_23, string_24, string_25};
char buffer[20];    // make sure this is large enough for the largest string it must hold


void setup() {
	lcd.init();					// Avvio il display
	lcd.backlight();			// Accendo la retroilluminazione del display
	lcd_state = 1;
	pinMode(BEEPER, OUTPUT);	// Uscita Buzzer
	pinMode(AUDIO_OUT, OUTPUT);	// Uscita Audio
	pinMode(PULSE_IN,INPUT);	// Ingresso impulso della sonda
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
	delay(500); //100
	digitalWrite(BEEPER, LOW);

	//Inizializzo la SDCard
	pinMode(SS, OUTPUT);
	lcd.clear();
	lcd.setCursor(0, 1); 
	if (!SD.begin(chipSelect)) {
		sd_card_ok = false;
	}else{
		sd_card_ok = true;
	}
	display_handle(4);
	delay(1500);

	//Inizializzo il bus I2C
	Wire.begin();

	//Inizializzo l'RTC
	RTC_Handle();

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

	lcdBacklightHandle();	//Gestisco la retroilluminazione

	setProbeSens();			//Imposto la sensibilità della sonda
}


void setProbeSens() {
	//Routine che viene chiamata per impostare la sensibilità della sonda
	if (probe_preset == 0) {
		Sens = SensCustom;
	} else {
		Sens = probe_preset_list[probe_preset];
	}
}


void batteryLevelHandle() {

	if (voltmt1.getVoltage() == 0.00) {
		 lcd.createChar(0,battPWR);
	} else {
		//batt_perc=((x-VMin)/(VMax-VMin))*100
		batt_perc = ((voltmt1.getVoltage()-2.8)/1.4)*100;
	
		if (batt_perc < 2) lcd.createChar(0,battAlert);
		if ((batt_perc >= 2) && (batt_perc < 5)) lcd.createChar(0,battEmpty);
		if ((batt_perc >= 5) && (batt_perc < 20)) lcd.createChar(0,batt0);
		if ((batt_perc >= 20) && (batt_perc < 40)) lcd.createChar(0,batt20);
		if ((batt_perc >= 40) && (batt_perc < 60)) lcd.createChar(0,batt60);
		if ((batt_perc >= 60) && (batt_perc < 80)) lcd.createChar(0,batt80);
		if (batt_perc >= 80) lcd.createChar(0,batt100);
		//if ((batt_perc >= 80) && (batt_perc <= 100)) lcd.createChar(0,batt100);
		//if (batt_perc > 101) lcd.createChar(0,battChg);
	}
	lcd.setCursor(15, 1);
	lcd.write((uint8_t)0);

}

void lcdBacklightHandle(){
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
			//Se viene premuto uno dei 4 tasti e il display non è Off	
			if ((digitalRead(KEY_UP)==LOW) || (digitalRead(KEY_DW)==LOW) || (digitalRead(KEY_SET)==LOW) || (digitalRead(KEY_MENU)==LOW)) {
				lcd.backlight();		//Accendo l'illuminazione
				lcd_state = 1;			//Display acceso
				lcd_millis = millis();	//Mi segno l'istante in cui ho acceso il display
			}
						
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


void RTC_Handle(){
			RTC.begin();
			RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);	//Avvio l'uscita a 1Hz
			if (! RTC.isrunning()) {
				//Se l'RTC non e' inizializzato imposto l'ora di default e lo avvio
				//Succede solo alla prima accensione o quando si scarica la batteria
				//RTC.adjust(DateTime(__DATE__, __TIME__));
				RTC.adjust(DateTime(2000,01,01,00,00,00));
				RTC.setSqwOutSignal(RTC_DS1307::Frequency_1Hz);
			}
}

float getDoseMultiplier(float dose) {
	// In base alla dose calcolo la scala
	if (dose < 10) return 1;				// micro
	else if (dose < 10000) return 0.001;	// milli
	else return 0.000001;					// unità intera
 }

char *getDoseScaleSymbol() {
	uint8_t i = 0;
	if (RadRaw < 10) i=2;			// micro
	else if (RadRaw < 10000) i=1;	// milli
	else i=0;					// unità Intera
	
	strcpy_P(buffer, (char*)pgm_read_word(&(unit_sv_desc[i]))); //Sievert

	/*switch (c_unit) {
		case 0:
			strcpy_P(buffer, (char*)pgm_read_word(&(unit_sv_desc[i]))); //Sievert
			break;
		case 1:
			//strcpy_P(buffer, (char*)pgm_read_word(&(unit_rt_desc[i]))); //Röntgen
			break;
	}*/
	return buffer;
}


void display_handle(uint8_t func) {
	//Gestione delle varie visualizzazioni fisse del display
	switch (func) {
		case 0: {	//Splash Screen
			lcd.clear();
			lcd.setCursor(2, 0); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[0]))); //Pluto Geiger
			lcd.print(buffer);
			//lcd.print("Pluto Geiger");
			lcd.setCursor(6, 1); 
			lcd.print("v"+fw_version);
			break;
		}
		
		case 1: {	//Setup CPM Sonda
			lcd.clear();
			lcd.setCursor(2, 0); 
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[1]))); //Prb CPM x mR/h
			//lcd.print(buffer);
			lcd.print("Prb CPM x mR/h");
			lcd.setCursor(6, 1);
			lcd.print(SensCustom);
			break;
		}

		case 2: {	//Setup Base tempi
			lcd.clear();
			lcd.setCursor(4, 0); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[13]))); //Time sec
			lcd.print(buffer);
			//lcd.print("TIME sec");

			lcd.setCursor(4, 1);
			lcd.print(Tempo[SetTemp]);
			break;
		}

		case 3:{
			// Aspetto del display durante il conteggio
			// Display presente durante il conteggio
			lcd.clear();
			lcd.setCursor(0, 0); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[14]))); //Time
			lcd.print(buffer);
			//lcd.print("Time");
			lcd.setCursor(12, 0);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[10]))); //sec
			lcd.print(buffer);
			//lcd.print("sec");
			lcd.setCursor(0, 1); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[15]))); //Pulse
			lcd.print(buffer);
			//lcd.print("Pulse");
			batteryLevelHandle();

			lcd.setCursor(6,0);
			lcd.print((TempoMax-millis())/1000);
			//lcd.print(TempoMax);
			//lcd.setCursor(10, 1); 
			//lcd.print(millis());
			


			lcd.setCursor(6, 1); 
			lcd.print(TotImp);
			break;
		}

		case 4: {
			//SD CARD Check
			lcd.clear();
			lcd.setCursor(2, 0); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[0]))); //Pluto Geiger
			lcd.print(buffer);
			//lcd.print("Pluto Geiger");
			if (sd_card_ok) {
				lcd.setCursor(5, 1); 
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[12]))); //SD OK
			}else{
				lcd.setCursor(5, 1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[11]))); //SD Fail
			}
			lcd.print(buffer);
			break;
	    }

		case 5: {	
			// Aspetto del display a fine conteggio
			// Display presente alla fine del conteggio
			lcd.clear();
			lcd.setCursor(0, 0); 
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[16]))); //CPM
			lcd.print(buffer);
			//lcd.print("CPM");

			lcd.setCursor(0, 1);
			lcd.print(getDoseScaleSymbol());
			batteryLevelHandle();
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
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[6]))); //Count Mode
			//lcd.print(buffer);
			lcd.print("Counter Mode");
			if (mode == 0) {
				lcd.setCursor(4,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[7]))); //One Count
				lcd.print(buffer);
				//lcd.print("One Count");
			}
			if (mode == 1) {
				lcd.setCursor(3,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[8]))); //Loop Count
				lcd.print(buffer);
				//lcd.print("Loop Count");
			}
			if (mode == 2) {
				lcd.setCursor(5,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[9]))); //Infinite
				lcd.print(buffer);
				//lcd.print("Infinite");
			}
			if (mode >= 3) {
				lcd.setCursor(2,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[25]))); //Geiger
				lcd.print(buffer);
				lcd.setCursor(9,1);
				lcd.print(mode-2);
				lcd.setCursor(11,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[10]))); //Geiger
				lcd.print(buffer);
			}
			break;
	   }
		case 8:{
			//PORTATA LA GESTIONE AL /
			break;
	   }
		case 9:{
			/*
			//Aspetto del display durante il setup delle Unità di misura
			lcd.clear();
			lcd.setCursor(3,0);
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[5]))); //Count Unit
			//lcd.print(buffer);
			lcd.print("Count Unit");
			
			//Unità di misura 0=Sievert 1=Röntgen
			lcd.setCursor(6,1);
			strcpy_P(buffer, (char*)pgm_read_word(&(unit_set_desc[c_unit])));
			lcd.print(buffer);*/
			break;
	   }
		case 10:{
			//SPOSTATO NEL 9
			break;
	   }
		case 11:{
			//Schermo di riepilogo iniziale
			lcd.clear();
			lcd.setCursor(0,0);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[18]))); //Prb:
			lcd.print(buffer);
			//lcd.print("Prb:");
			lcd.setCursor(4,0);
			lcd.print(Sens);
			lcd.setCursor(10,0);
			if (mode == 0) lcd.print("One");
			if (mode == 1) lcd.print("Loop");
			if (mode == 2) lcd.print("Inf");
			if (mode == 3) lcd.print("Geig 1");
			if (mode == 4) lcd.print("Geig 2");
			if (mode == 5) lcd.print("Geig 3");
			//Seconda Riga
			lcd.setCursor(0,1);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[19]))); //Sec:
			lcd.print(buffer);
			//lcd.print("Sec:");
			lcd.setCursor(4,1);
			lcd.print(BaseTempi,DEC);
			lcd.setCursor(10,1);
			//strcpy_P(buffer, (char*)pgm_read_word(&(unit_set_desc[c_unit])));
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[5])));
			lcd.print(buffer);
			//lcd.print(getDoseScaleSymbol());
			batteryLevelHandle();
			
			break;
	   }
		case 12:{
			//Valore del settaggio durante il setup dell'Ora
			lcd.clear();
			lcd.setCursor(7,0);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[20]))); //Time
			lcd.print(buffer);
			//lcd.print("Time");
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
			lcd.setCursor(7,0);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[21]))); //Date
			lcd.print(buffer);
			//lcd.print("Date");
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
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[16]))); //CPM
			lcd.print(buffer);
			//lcd.print("CPM:");
			lcd.setCursor(3,0);
			strcpy_P(buffer, (char*)pgm_read_word(&(string_table[17]))); //:
			lcd.print(buffer);
			if (mode==2) {	//Se sono in modalità Geiger non scrivo i minuti
				lcd.setCursor(9,0);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[22]))); //Mn:
				lcd.print(buffer);
				//lcd.print("Mn:");
			}
			lcd.setCursor(9,1);
			//recupero la scala in maniera automatica			
			lcd.print(getDoseScaleSymbol());
			
			batteryLevelHandle();

			lcd.setCursor(4,0);
			lcd.print(CPM,0);	//Scrivo i CPM
			if (mode==2) {	//Se sono in modalità Geiger non scrivo i minuti
				lcd.setCursor(12,0);
				lcd.print(min_totali);	//Scrivo i minuti
			}
			lcd.setCursor(2, 1);
			lcd.print(Rad,3);		//Scrivo il valore calcolato
			break;
	   }
		case 17:{
			//PORTATO NEL 16
			break;
	   }
		case 18:{
			//Visualizzazione del display dello stato della batteria
			lcd.clear();
			lcd.setCursor(4,0);
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[4]))); //Battery
			//lcd.print(buffer);
			lcd.print("Battery");

			lcd.setCursor(0,1);
			lcd.setCursor(1,1);
			lcd.print(voltmt1.getVoltage());
			lcd.setCursor(6,1);
			lcd.print("V");
			lcd.setCursor(9,1);
			lcd.print(batt_perc);
			lcd.setCursor(12,1);
			lcd.print("%");
			batteryLevelHandle();
			break;
	   }
		case 19:{
			//Probe Preset
			lcd.clear();
			lcd.setCursor(4, 0); 
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[2])));	//Prb Preset
			//lcd.print(buffer);
			lcd.print("Prb Preset");
			lcd.setCursor(7, 1);
			lcd.print(probe_preset_list[probe_preset]);
			break;
	   }
		case 20:{
			//Impostazioni del Display Schermo Statico
			lcd.clear();
			lcd.setCursor(4,0);
			//strcpy_P(buffer, (char*)pgm_read_word(&(string_table[3])));	//BackLight
			//lcd.print(buffer);
			lcd.print("BackLight");	
			if (lcd_mode < 2) {
				//On o OFF
				lcd.setCursor(4, 1);
				lcd.print(lcd_desc[lcd_mode]);
			} else {
				lcd.setCursor(4, 1); 
				lcd.print(lcd_desc[lcd_mode]);
				lcd.setCursor (8,1);
				strcpy_P(buffer, (char*)pgm_read_word(&(string_table[10]))); //sec
				lcd.print(buffer);
				//lcd.print("sec");
			}
			break;
	   }
	}
}



void setting_handle(uint8_t func) {
	//Gestice le impostazioni
	switch (func) {
		case 0:{	//Sensibilità Sonda
			//display_handle(1);
			delay(500);
			do {
				Buzzer();
				lcdBacklightHandle();
				display_handle(1);
				if (digitalRead(KEY_UP)== HIGH && Sens<64000) {
					if (SensCustom <= 10) SensCustom=SensCustom+1;
					if ((SensCustom > 10) && (SensCustom <= 1000)) SensCustom=SensCustom+10;
					if ((SensCustom > 1000) && (SensCustom <= 10000)) SensCustom=SensCustom+50;
					if (SensCustom > 10000) SensCustom=SensCustom+1000;      
				}
				if (digitalRead(KEY_DW)== HIGH && Sens >= 0) { 
					if (SensCustom <= 10) SensCustom=SensCustom-1;
					if ((SensCustom > 10) && (SensCustom <= 1000)) SensCustom=SensCustom-10;
					if ((SensCustom > 1000) && (SensCustom <= 10000)) SensCustom=SensCustom-50;
					if (SensCustom > 10000) SensCustom=SensCustom-1000;      
				}
				if (digitalRead(KEY_MENU)== LOW) break;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						EEPROMWriteInt(0x01,SensCustom);     // Scrive Sensibilità Sonda nella EEPROM
					}
				setProbeSens();			//Imposto la sensibilità della sonda
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
				lcdBacklightHandle();
				BaseTempi=Tempo[SetTemp];
				Molt= K[SetTemp];
				display_handle(2);
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && SetTemp < 6) SetTemp++;
				if (digitalRead(KEY_DW)== HIGH && SetTemp > 0) SetTemp--;
				if (digitalRead(KEY_MENU)== LOW) break;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						EEPROM.write(0x00,SetTemp);    // Scrive Set della Base Tempi
						//BaseTempi = Tempo[SetTemp];
						//Molt= K[SetTemp];
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 2: { //Modalità Geiger, Scaler o Ratemeter
			display_handle(7);
			delay(500);
			do {
				Buzzer();
				lcdBacklightHandle();
				display_handle(7);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && mode < 6) mode++;
				if (digitalRead(KEY_DW)== HIGH && mode > 0) mode--;
				if (digitalRead(KEY_MENU)== LOW) break;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						EEPROM.write(0x03,mode);     
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 3: { //Unità di misura
/*			display_handle(9);
			delay(500);
			do {
				Buzzer();
				lcdBacklightHandle();
				display_handle(9);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && c_unit < 2) c_unit++;
				if (digitalRead(KEY_DW)== HIGH && c_unit > 0) c_unit--;
				if (digitalRead(KEY_MENU)== LOW) break;
				delay(50);
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						//lcd.setCursor(6, 1); 
						//lcd.print("    ");
						EEPROM.write(0x04,c_unit);    // Scrive Set della Base Tempi       
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);*/
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
				lcdBacklightHandle();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && hour < 25) hour++;
				if (digitalRead(KEY_DW)== HIGH && hour > 0) hour--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && minute < 60) minute++;
				if (digitalRead(KEY_DW)== HIGH && minute > 0) minute--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(13);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && second < 60) second++;
				if (digitalRead(KEY_DW)== HIGH && second > 0) second--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && day < 31) day++;
				if (digitalRead(KEY_DW)== HIGH && day > 1) day--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && month < 12) month++;
				if (digitalRead(KEY_DW)== HIGH && month > 1) month--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(15);	// Visualizzo il valore dell'RTC
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && year < 2500) year++;
				if (digitalRead(KEY_DW)== HIGH && year > 2000) year--;
				if (digitalRead(KEY_MENU)== LOW) break;
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
				lcdBacklightHandle();
				display_handle(18);	
				delay(50);
				if (digitalRead(KEY_MENU)== LOW) break;
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
		case 7: { //Impostazioni del Display
			display_handle(20);
			delay(500);
			do {
				Buzzer();
				lcdBacklightHandle();
				display_handle(20);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && lcd_mode < 7) lcd_mode++;
				if (digitalRead(KEY_DW)== HIGH && lcd_mode > 0) lcd_mode--;
				if (digitalRead(KEY_MENU)== LOW) break;
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						EEPROM.write(0x05,lcd_mode);    // Scrive Set della Base Tempi       
					}
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}

		case 8: { //Preset Sonda
			display_handle(19);
			delay(500);
			do {
				Buzzer();
				lcdBacklightHandle();
				display_handle(19);	// Visualizzo il valore salvato EEPROM
				delay(50);
				if (digitalRead(KEY_UP)== HIGH && probe_preset < 3) probe_preset++;
				if (digitalRead(KEY_DW)== HIGH && probe_preset > 0) probe_preset--;
				if (digitalRead(KEY_MENU)== LOW) break;
				if (digitalRead(KEY_SET)== LOW) {
					delay(50);
					if (digitalRead(KEY_SET)== LOW) {
						EEPROM.write(0x06,probe_preset);    // Scrive Set della Base Tempi       
					}
					setProbeSens();			//Imposto la sensibilità della sonda
				}
			}
			while (digitalRead(KEY_SET)== HIGH);
			break;
		}
	}
}

void gestione_cifre(uint16_t dato, uint8_t status){
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
	SetTemp=EEPROM.read(0x00);				// Legge Set della Base Tempi
	if (SetTemp==255) SetTemp = 0;			// Se la EEPROM è vuota, imposto 10 secondi
	BaseTempi= Tempo[SetTemp];				// Imposto la base tempi recuperandola dall'array
	
	SensCustom=EEPROMReadInt(0x01);			// Legge Sensibilità Sonda
	if (SensCustom==0xFF00) Sens = 3500;	// Se la EEPROM è vuota a 32767 (Integer), imposto la sensibilità a 3500
	if (SensCustom==0xFF) Sens = 200;		// Se la EEPROM è vuota a 255 (Byte), imposto la sensibilità a 200

	mode=EEPROM.read(0x03);					// Modalità Scaler, Ratemeter o Geiger
	if (mode > 6) mode = 0;					// Se la EEPROM è vuota, imposto a One Count

	//c_unit=EEPROM.read(0x04);				//Unità di misura 0=mR/h 1=uR/h 2=uSv/h
	//if (c_unit==255) c_unit=2;				//Se la EEPROM è vuota, imposto i uSv/h come default

	lcd_mode=EEPROM.read(0x05);				//Modalita dell'LCD
	if (lcd_mode==255) lcd_mode=1;			//Se la EEPROM è vuota, imposto il display sempre acceso

	probe_preset=EEPROM.read(0x06);				//Modalita dell'LCD
	if (probe_preset==255) probe_preset=0;	//Se la EEPROM è vuota, imposto il display sempre acceso

}

void QuickSet_Handle() { 
	// Visualizza e Cambia la modalità di funzionamento
 
	setting_handle(6);	//Batteria
	setting_handle(7);	//display settings
	setting_handle(8);	//Sensibilità Preset della Sonda
	setting_handle(1);	//Base Tempi
	setting_handle(2);	//Modalità
	//setting_handle(3);	//Unità di Misura


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

	setting_handle(8);	//Sensibilità Preset della sonda
	setting_handle(0);	//Sensibilità Custom della sonda
	setting_handle(1);	//Base Tempi
	setting_handle(2);	//Modalità
	//setting_handle(3);	//Unità di Misura
	setting_handle(4);	//Ora
	setting_handle(5);	//Data
	setting_handle(6);	//Batteria
	setting_handle(7);	//Display Settings

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
	lcdBacklightHandle();	//Gestico la retroilluminazione
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
	//mode == 2 Infinite
	//mode == 3 Geiger

		inizio = 0;		//azzero le variabili
		sec_totali = 0;
		min_totali = 0;
		CPM = 0;
		TotImp = 0;
		Rad = 0;

	

	if (mode==2){
		//Conteggio Infinito
		delay(500);
		inizio = millis();	//Momento iniziale del conteggio
		do{
			Buzzer();									//Suono il buzzer se necessario
			lcdBacklightHandle();						//Gestisco la Retro Illuminazione
			if (b500ms==true){							//Faccio le operazioni ogni 500ms 
				sec_totali = (millis()-inizio) / 1000;	//Secondi totali dall'inizio del conteggio
				min_totali = sec_totali/60;				//Minuti totali, servono per il display
				CPM = (float(TotImp)/float(sec_totali));
				CPM = CPM*60;							//CPM totali dall'inizio del conteggio

				RadRaw=(CPM/Sens)*10;		//Calcolo in uSv/h
				//Unità di misura 0=Sievert 1=Röntgen
				/*switch (c_unit){					//In base all'unità di misura, conteggio il valore in scala micro
				case 0:	//Sievert
						RadRaw=(CPM/Sens)*10;
						break;
				  case 1:	//Röntgen
						RadRaw=(CPM/Sens)*1000;
						break;
				}*/
				Rad=RadRaw*getDoseMultiplier(RadRaw); //In base al valore calcolato applico la Scala

				b500ms=false;	//Resetto la variabile dell'interrupt ogni 500ms
			}
			if (b100ms == true){		//aggiorno il display ogni 100ms
				display_refresh++;
				b100ms = false;
			}
			if (display_refresh==3) {
				display_handle(16);		//Visualizzo sul display la parte statica e dinamica
				display_refresh=0;
			}
			//Se premo il tasto menù durante il conteggio scrivo il log
			if (digitalRead(KEY_MENU)==LOW) {
				delay(200); //debounc
				if (digitalRead(KEY_MENU)==LOW) Log_Write();
			}
		}while (digitalRead(KEY_SET)==HIGH);	//Attendo la pressione del tasto SET e.....
		geiger_status = 3;	//Vado alla schermata di riepilogo generale
	}else if ((mode==3)||(mode==4)||(mode==5)){
		//Conteggio Geiger
		delay(500);
		//inizio = millis();	//Momento iniziale del conteggio
		do{
			Buzzer();									//Suono il buzzer se necessario
			lcdBacklightHandle();						//Gestisco la Retro Illuminazione
			if (b100ms==true){
				//Incremento la variabile per il calcolo
				geiger_calc_time++;
			}
			uint8_t secs=0;
			secs = (mode-2);
			if (geiger_calc_time == (secs*10)) {	//Conteggio i timeout di 500 ms per 4 volte 2000 ms = 2 sec
				//Que effettuo il calcoli del Geiger ogni xx secondi, modificabili aumentando il numero di volte
				//Che la variabile geiger_calc_time si incrementoa ogni 500ms
				sec_totali = secs;								//Forzo i secondi totali a 2
				CPM = (float(TotImp)/float(sec_totali));	//Calcolo i CPS
				CPM = CPM*60;								//Converto in CPM

				RadRaw=(CPM/Sens)*10;		//Calcolo in uSv/h
				//Unità di misura 0=Sievert 1=Röntgen
				/*switch (c_unit){					//In base all'unità di misura, conteggio il valore in scala micro
				  case 0:	//Sievert
						RadRaw=(CPM/Sens)*10;
						break;
				  case 1:	//Röntgen
						RadRaw=(CPM/Sens)*1000;
						break;
				}*/
				Rad=RadRaw*getDoseMultiplier(RadRaw); //In base al valore calcolato applico la Scala
				TotImp = 0;			//Azzero il conteggio degli impulsi
				geiger_calc_time=0;	//Resetto la variabile per iniziare un nuovo conteggio
			}
			
			if (b100ms == true){		//aggiorno il display ogni 100ms
				display_refresh++;
				b100ms = false;
			}
			if (display_refresh==3) {
				display_handle(16);		//Visualizzo sul display la parte statica e dinamica
				display_refresh=0;
			}
			//Se premo il tasto menù durante il conteggio scrivo il log
			if (digitalRead(KEY_MENU)==LOW) {
				delay(200); //debounc
				if (digitalRead(KEY_MENU)==LOW) Log_Write();
			}
		}while (digitalRead(KEY_SET)==HIGH);	//Attendo la pressione del tasto SET e.....
		geiger_status = 3;	//Vado alla schermata di riepilogo generale

	}else{
		//Conteggio One Count o Loop Count
		TempoMax=millis()+((unsigned long)BaseTempi)*1000;	//Calcolo il tempo massimo
		do {
			Buzzer();
			lcdBacklightHandle();	//Gestisco la Retro Illuminazione
			// Visualizza conteggio in corso 10 volte ogni secondo
			//if (millis()%100 < 1) display_handle(4); //Display in conteggio
			//Visualizzo il conteggio ogni 500 ms
			if (b100ms == true){		//aggiorno il display ogni 100ms
				display_refresh++;
				b100ms = false;
			}
			if (display_refresh==3) {
				display_handle(3);		//Visualizzo sul display la parte statica e dinamica
				display_refresh=0;
			}
			VarServInt1=digitalRead(KEY_MENU); //Leggo il tasto menù per vedere se interrompere
			VarServInt =digitalRead(KEY_SET); //Leggo il tasto SET per vedere se interrompere
		} 
		while (TempoMax > millis() && (VarServInt1==HIGH && VarServInt==HIGH));
	
		if (VarServInt==LOW) geiger_status = 3;		//Se premo set esco e torno al riepilogo
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

	//Unità di misura 0=Sievert 1=Röntgen
	/*switch (c_unit){					//In base all'unità di misura, conteggio il valore in scala micro
		case 0:	//Sievert
			RadRaw=(CPM/Sens)*10;
			break;
		case 1:	//Röntgen
			RadRaw=(CPM/Sens)*1000;
			break;
	}*/
	RadRaw=(CPM/Sens)*10;						//Calcolo in uSv/h
	Rad=RadRaw*getDoseMultiplier(RadRaw);		//In base al valore calcolato applico la Scala

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
	TempoMax=millis()+((unsigned long)BaseTempi)*1000;	//Calcolo il tempo massimo

	if (mode == 0) {	//Se One Count, aspetto il tasto
		if (VarServInt==LOW) geiger_status = 3;	//E' Stato premuto SET e torno al riepilogo
		if (VarServInt1==LOW) geiger_status = 0;	//E' Stato premuto MENU e vado nel setup rapido iniziale
	}
	if ((mode == 1) && (VarServInt==HIGH)) geiger_status = 1;	//Se Loop Count reinizio il conteggio
	else geiger_status = 3;										//Se Loop Count ma premuto SET riepilogo
}


void key_wait(){
// Attende la pressione del tasto Set/Start o Menù
  delay(500);
  do{      
      VarServInt=digitalRead(KEY_SET);
	  VarServInt1=digitalRead(KEY_MENU);
	  Buzzer();
	  lcdBacklightHandle();
  }  
  while (VarServInt==HIGH && VarServInt1==HIGH );
}

void Log_Write(){

	if (sd_card_ok == true) {  //Se la card è ok scrivo, altrimenti desisto
		//Indico la scrittura su SD sul display
		lcd.setCursor(14,1);
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
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[23])));
		sd_file.print(buffer); // /
		delay(50);
		sd_file.print(now.month(),DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[23])));
		sd_file.print(buffer); // /
		delay(50);
		sd_file.print(now.year(),DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		sd_file.print(now.hour(),DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[17])));
		sd_file.print(buffer); // :
		delay(50);
		sd_file.print(now.minute(),DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[17])));
		sd_file.print(buffer); // :
		delay(50);
		sd_file.print(now.second(),DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		//Se sono in Infinite Mode scrivo i seconti totali
		if (mode==2) sd_file.print(sec_totali,DEC);
		else if (mode==3) sd_file.print(1,DEC);
		else if (mode==4) sd_file.print(2,DEC);
		else if (mode==5) sd_file.print(3,DEC);
		else sd_file.print(BaseTempi,DEC);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		sd_file.print(CPM,2);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		sd_file.print(Rad,3);
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		sd_file.print(getDoseScaleSymbol());
		delay(50);
		strcpy_P(buffer, (char*)pgm_read_word(&(string_table[24])));
		sd_file.print(buffer); // ,
		delay(50);
		sd_file.println(mode);
		
		delay(100);
		sd_file.flush();		//Scrive definitivamente su SD
		delay(100);
		sd_file.close();		//Chiude il file
		delay(100);
		//Finita la scrittura su SD
		lcd.setCursor(14,1);
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
