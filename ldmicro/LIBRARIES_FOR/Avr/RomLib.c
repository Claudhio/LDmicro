#include <avr/io.h>

// Librairie AtMega pour EEPROM

#include "../ladder.h"
#include "RomLib.h"			 // Fichier header librairie EEPROM


// Fonction de lecture d'un octet
unsigned char EEPROM_read(int address)
     {
     while (EECR & (1<<EEWE));     // attendre fin d'�criture pr�c�dente
     EEAR = address;
     EECR |= (1<<EERE);            // Data transf�r�e de l'EEPROM dans EEDR

     return EEDR;
     }

// Fonction d'�criture d'un octet
void EEPROM_write(int address, unsigned char data)
     {
     while (EECR & (1<<EEWE));     // attendre fin d'�criture pr�c�dente
     EEAR = address;
     EEDR = data;
     EECR |= (1<<EEMWE);           // Pas d'IT autoris�e entre les 2
     EECR |= (1<<EEWE) ;           // instructions
     }



