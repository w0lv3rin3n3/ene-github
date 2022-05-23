/*
 * SmartHome.c
 *
 * Author : Claudiu-Daniel
 */ 
#define F_CPU 16000000

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "network.h"
#include "enc28j60.h"
#include "usart.h"
#include "dht.h"
#include "adc.h"

/* MOTION PIN */
#define PIR_PIN PINC
#define PIR_PORT PORTC
#define PIR_DDR DDRC
#define PIR_PINBLOCK PC1

/* RELAY PIN */
#define RL_PIN PINC
#define RL_PORT PORTC
#define RL_DDR DDRC
#define RL_PINBLOCK1 PC2



/* ================================== GAS SENSOR =============================== */
#define         RL_VALUE                     (10)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet

#define         LPG                      (0)         // Gas identity no.
#define         SMOKE                    (1)


float           LPGCurve[3]  = {2.3,0.20,-0.45};   //two points from LPG curve are taken point1:(200,1.6) point2(10000,0.26)
//take log of each point (lg200, lg 1.6)=(2.3,0.20)  (lg10000,lg0.26)=(4,-0.58)
//find the slope using these points. take point1 as reference
//data format:{ x, y, slope};

float           SmokeCurve[3] ={2.3,0.53,-0.43};    //two points from smoke curve are taken point1:(200,3.4) point2(10000,0.62)
//take log of each point (lg200, lg3.4)=(2.3,0.53)  (lg10000,lg0.63)=(4,-0.20)
//find the slope using these points. take point1 as reference
//data format:{ x, y, slope};

float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms

#define AREF 5

void ADC_init2(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (7 << ADPS0);
}

int ADC_get2(uint8_t channel)
{
	ADMUX = (ADMUX & ~(0x1f << MUX0)) | channel;

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}

int  GetPercentage(float rs_ro_ratio, float *pcurve);
int GetGasPercentage(float rs_ro_ratio, int gas_id);
float ReadSensor();
float ResistanceCalculation(int raw_adc);
float SensorCalibration();
/* ================================== END GAS SENSOR =============================== */



static uint8_t mymac[6] =  {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4] = {192,168,1,155};
#define MYWWWPORT 80
#define MYUDPPORT 1200
static char password[]="secret";
#define BUFFER_SIZE 800
static uint8_t buf[BUFFER_SIZE+1];

int time;

void lightLED() {
	PORTD ^= (1 << PD7);
}

int8_t readTemp() {
	int8_t temperature = 0;
	int8_t result;
	char printbuff[100], nr[10];
	
	result = dht_gettemperature(&temperature);
	
	if (result < 0) {
		USART0_print("error\r\n"); 
		return -1;
	}

	itoa(temperature, printbuff, 10);
	itoa(time++, nr, 10);
	
	if (time > 100)
		time = 0;

	USART0_print(nr);
	USART0_print(" -> Temperature: ");
	USART0_print(printbuff);
	USART0_print("*C\r\n");
	
	return temperature;
}

/* motion with interrupts */
void activatePIR()
{
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT17);
}

ISR(PCINT2_vect)
{
	cli();
	if (PIR_PINBLOCK & (1 << PIR_PIN)) {
		USART0_print("MOTION DETECTED!");
		_delay_ms(300);
	}
	sei();
}

void initMotion() {
	PIR_DDR &= ~(1 << PIR_PINBLOCK);
	PIR_PORT |= (1 << PIR_PINBLOCK);
}

int8_t readMotion() {
	if (PIR_PIN & (1 << PIR_PINBLOCK)) {
		USART0_print("MOTION DETECTED!\r\n");
		return 1;
	} else {
		USART0_print("NO MOTION DETECTED!\r\n");
		return 0;
	}
}

int8_t readGas(int type) {
	char buff[50];
	int8_t value = -1;
	
	if (type == LPG) {
		value = GetGasPercentage(ReadSensor()/Ro, LPG);
		itoa(value, buff, 10);
		USART0_print("LPG: ");
		USART0_print(buff);
		USART0_print("ppm\n\r");
	} else if (type == SMOKE) {
		value = GetGasPercentage(ReadSensor()/Ro, SMOKE);
		itoa(value, buff, 10);
		USART0_print("SMOKE: ");
		USART0_print(buff);
		USART0_print("ppm\n\r");
	}
	
	return value;
}

void initRelay() {
	RL_DDR |= (1 << RL_PINBLOCK1);
	RL_PORT |= (1 << RL_PINBLOCK1);
	//RL_PORT &= ~(1 << RL_PINBLOCK2);
}

void toggleSwitch() {
	RL_PORT ^= (1 << RL_PINBLOCK1);
}





/* ===================================== ETHERNET =================================== */

uint8_t verify_password(char *str)
{
	// the first characters of the received string are
	// a simple password/cookie:
	if (strncmp(password,str,5)==0){
		return(1);
	}
	return(0);
}

// takes a string of the form password/commandNumber and analyse it
// return values: -1 invalid password, otherwise command number
//                -2 no command given but password valid
//                -3 valid password, no command and no trailing "/"
int8_t analyse_get_url(char *str)
{
	uint8_t loop=1;
	uint8_t i=0;
	while(loop){
		if(password[i]){
			if(*str==password[i]){
				str++;
				i++;
				}else{
				return(-1);
			}
			}else{
			// end of password
			loop=0;
		}
	}
	// is is now one char after the password
	if (*str == '/'){
		str++;
		}else{
		return(-3);
	}
	// check the first char, garbage after this is ignored (including a slash)
	if (*str < 0x3a && *str > 0x2f){
		// is a ASCII number, return it
		return(*str-0x30);
	}
	return(-2);
}

// answer HTTP/1.0 301 Moved Permanently\r\nLocation: password/\r\n\r\n
// to redirect to the url ending in a slash
uint16_t moved_perm(uint8_t *buf)
{
	uint16_t plen;
	plen=make_tcp_data_pos(buf,0,PSTR("HTTP/1.0 301 Moved Permanently\r\nLocation: "));
	plen=make_tcp_data(buf,plen,password);
	plen=make_tcp_data_pos(buf,plen,PSTR("/\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	plen=make_tcp_data_pos(buf,plen,PSTR("<h1>301 Moved Permanently</h1>\n"));
	plen=make_tcp_data_pos(buf,plen,PSTR("add a trailing slash to the url\n"));
	return(plen);
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf, uint8_t on_off)
{
	/* Create the content with the parameter*/
	int8_t temperature, gas, smoke, motion;
	char temp_str[50], gas_str[50], smoke_str[50];
	
	temperature = readTemp();
	gas = readGas(LPG);
	smoke = readGas(SMOKE);
	motion = readMotion();
	itoa(temperature, temp_str, 10);
	itoa(gas, gas_str, 10);
	itoa(smoke, smoke_str, 10);

	uint16_t plen;
	plen = make_tcp_data_pos(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	
	plen = make_tcp_data_pos(buf, plen, PSTR("\r\nThe parameters from your room: \r\n"));
	plen = make_tcp_data_pos(buf, plen, PSTR("\tTemperature: "));
	plen = make_tcp_data(buf, plen, temp_str);
	plen = make_tcp_data_pos(buf, plen, PSTR(" *C\r\n"));
	
	plen = make_tcp_data_pos(buf, plen, PSTR("\tGas: "));
	plen = make_tcp_data(buf, plen, gas_str);
	plen = make_tcp_data_pos(buf, plen, PSTR(" ppm\r\n"));
	
	plen = make_tcp_data_pos(buf, plen, PSTR("\tSmoke: "));
	plen = make_tcp_data(buf, plen, smoke_str);
	plen = make_tcp_data_pos(buf, plen, PSTR(" ppm\r\n"));
	
	if (!motion) {
		plen = make_tcp_data_pos(buf, plen, PSTR("\tSAFE! There is NO motion in your room\r\n"));
		if (!(RL_PIN & (1 << RL_PINBLOCK1)))
			RL_PORT |= (1 << RL_PINBLOCK1);
	} else {
		plen = make_tcp_data_pos(buf, plen, PSTR("\tALERT! There is motion in your room\r\n"));
		if (RL_PIN & (1 << RL_PINBLOCK1))
			RL_PORT &= ~(1 << RL_PINBLOCK1);
	}

	plen = make_tcp_data_pos(buf, plen, PSTR("\r\n"));
	return(plen);
}

/* ===================================== END ETHERNET =================================== */







int main()
{
	uint16_t plen;
	uint16_t dat_p;
	
	/* init INs and OUTs */
	USART0_init();
	initRelay();
	ADC_init2();
	initMotion();
	DDRD |= (1 << PD7);		// LED
	USART0_print("Sensors initialised");
	
	CLKPR = (1<<CLKPCE);
	CLKPR = 0;
	_delay_loop_1(50);
	ENC28J60_Init(mymac);
	
	USART0_print("MAC initialised\r\n");
	ENC28J60_ClkOut(2); // change clkout from 6.25MHz to 12.5MHz
	_delay_loop_1(50); // 12ms
	USART0_print("CLK initialised\r\n");
	
	ENC28J60_PhyWrite(PHLCON,0x476);
	_delay_loop_1(50); // 12ms
	init_network(mymac,myip,MYWWWPORT);
	USART0_print("Network initialised\r\n");
	
	
	USART0_print("Calibrating...\n\r");
	Ro = SensorCalibration();
	USART0_print("Calibration is done...\n\r");

	
	while(1) {
		readTemp();
		readMotion();
		readGas(LPG);
		readGas(SMOKE);
		toggleSwitch();
		
		USART0_print("\r\n");
		lightLED();
		_delay_ms(2000);
/*

		// get the next new packet:
        plen = ENC28J60_PacketReceive(BUFFER_SIZE, buf);

        // plen will ne unequal to zero if there is a valid 
        // packet (without crc error)
        if (plen == 0)
			continue;

  
        // arp is broadcast if unknown but a host may also
        // verify the mac address by sending it to 
        // a unicast address.
        if(eth_is_arp(buf,plen)) {
	        arp_reply(buf);
			USART0_print("Received and sent ARP\r\n");
	        continue;
        }

        // check if ip packets are for us:
        if(eth_is_ip(buf, plen) == 0) {
			continue;
		}
		
		if(buf[IP_PROTO] == IP_ICMP && buf[ICMP_TYPE] == ICMP_REQUEST){
			// a ping packet, let's send pong
			icmp_reply(buf, plen);
			USART0_print("Received ping, sent pong\r\n");
			continue;
		}
		
		//USART0_print("www\r\n");
		// tcp port www start, compare only the lower byte
		if (buf[IP_PROTO] == IP_TCP 
				&& buf[TCP_DST_PORT] == 0 
				&&buf[TCP_DST_PORT+1] == MYWWWPORT) {

			if (buf[TCP_FLAGS] & TCP_SYN){
				tcp_synack(buf);
				continue;
			}
			
			if (buf[TCP_FLAGS] & TCP_ACK){
				init_len_info(buf);
				dat_p = get_tcp_data_ptr();
				if (dat_p == 0){
					if (buf[TCP_FLAGS] & TCP_FIN){
						tcp_ack(buf);
					}
					continue;
				}
				if (strncmp("GET ", (char *)&(buf[dat_p]), 4) != 0) {
					plen = make_tcp_data_pos(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
					goto SENDTCP;
				}
				if (strncmp("/ ", (char *)&(buf[dat_p+4]), 2) == 0) {
					plen = make_tcp_data_pos(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
					plen = make_tcp_data_pos(buf, plen, PSTR("\r\nUsage: http://host_or_ip/password \r\n\r\n"));
					goto SENDTCP;
				}
				plen = print_webpage(buf, 0);

SENDTCP:
				USART0_print("Sending page back \r\n");
				tcp_ack(buf);
				tcp_ack_with_data(buf, plen);
				continue;
			}
		}*/
	}
	
	
	
	return 0;
}







/* ============================== GAS ================================== */

float ResistanceCalculation(int raw_adc)
{                                                         // sensor and load resistor forms a voltage divider. so using analog value and load value
	return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));     // we will find sensor resistor.
}

float SensorCalibration()
{
	int i;                                   // This function assumes that sensor is in clean air.
	float val=0;
	
	for (i=0;i<10;i++) {                   //take multiple samples and calculate the average value
		
		//val += ResistanceCalculation(ADC_GetAdcValue(0));
		val += ResistanceCalculation(ADC_get2(0));
		_delay_ms(500);
	}
	val = val/10;
	val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro
	//according to the chart in the datasheet
	
	return val;
}

float ReadSensor()
{
	int i;
	float rs=0;
	
	for (i=0;i<5;i++) {                                 // take multiple readings and average it.
		rs += ResistanceCalculation(ADC_get2(0));   // rs changes according to gas concentration.
		_delay_ms(50);
	}
	
	rs = rs/5;
	
	return rs;
}

int GetGasPercentage(float rs_ro_ratio, int gas_id)
{
	if ( gas_id == LPG ) {
		//USART0_print("ok");
		return GetPercentage(rs_ro_ratio,LPGCurve);
		
		} else if ( gas_id == SMOKE ) {
		return GetPercentage(rs_ro_ratio,SmokeCurve);
	}
	
	return 0;
}

int GetPercentage(float rs_ro_ratio, float *curve)
{
	return (pow(10,( ((log(rs_ro_ratio)-curve[1])/curve[2]) + curve[0])));
}

/* ============================== GAS ================================== */