#include "usart.h"

/*
 * Functie de initializare a controllerului USART
 */
void USART0_init() 
{
    /* seteaza baud rate la 38400 */
    UBRR0H = 0;
    UBRR0L = 25;

    /* porneste transmitatorul */
    UCSR0B = (1<<TXEN0) | (1<<RXEN0);

    /* seteaza formatul frame-ului: 7 biti de date, 2 biti de stop, fara paritate */
    UCSR0C = (1<<USBS0) | (2<<UCSZ00);
}

/*
 * Functie ce transmite un caracter prin USART
 *
 * @param data - caracterul de transmis
 */
void USART0_transmit(char data)
{
    /* asteapta pana bufferul e gol */
    while(!(UCSR0A & (1<<UDRE0)));

    /* pune datele in buffer; transmisia va porni automat in urma scrierii */
    UDR0 = data;
}

/*
 * Functie ce primeste un caracter prin USART
 *
 * @return - caracterul primit
 */
char USART0_receive()
{
    /* asteapta cat timp bufferul e gol */
    while(!(UCSR0A & (1<<RXC0)));

    /* returneaza datele din buffer */
    return UDR0;
}

/*
 * Functie ce transmite un sir de caractere prin USART
 *
 * @param data - sirul (terminat cu '\0') de transmis
 */
void USART0_print(const char *data)
{
    while(*data != '\0')
        USART0_transmit(*data++);
}

/*
 * Functie ce transmite un caracter prin USART
 *
 * @param data - caracterul de transmis
 * @param stream - nefolosit
 *
 * @return - intotdeauna returneaza 0
 */
int USART0_printf(char data, FILE *stream)
{
    /* asteapta pana bufferul e gol */
    while(!(UCSR0A & (1<<UDRE0)));

    /* pune datele in buffer; transmisia va porni automat in urma scrierii */
    UDR0 = data;
    return 0;
}
