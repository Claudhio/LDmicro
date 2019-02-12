#include <avr/io.h>

// Librairie AtMega pour afficheur LCD 16x2 + IO-Expander sur bus I2C 

#include "../ladder.h"
#include "UsrLib.h"
#include "I2cLib.h"
#include "LcdI2clib.h"			 // Fichier header librairie LCD

static unsigned char port= 0;		// adaptation pour IO-Expander I2C
static int lcd_i2c_adr= 0;			// adaptation pour IO-Expander I2C

void LCD_I2C_SendCommand(char);
void LCD_I2C_SendChar(char);
void LCD_I2C_Send(char,int);
void LCD_I2C_Send4msb(char);
void LCD_I2C_Enable(void);

// Envoi commande
void LCD_I2C_SendCommand(char commande)
	{
   	LCD_I2C_Send(commande,0);
	}

// Envoi caractere
void LCD_I2C_SendChar(char caractere)
	{
   	LCD_I2C_Send(caractere,1);
	}

// G�re l'�criture d'un octet vers le LCD
void LCD_I2C_Send(char donnee,int type)
	{
   	// Activation de RS si type = 1 (donnee)
   	PORT_LCD (|= (type << BIT_LCD_RS))
   	// Ecriture des 4 MSB
   	LCD_I2C_Send4msb(donnee);   
   	// Ecriture des 4 LSB
   	LCD_I2C_Send4msb(donnee << 4);   
   	// Desactivation RS
   	PORT_LCD  (&= ~(1<<BIT_LCD_RS))
   	// Attente execution
   	delay_ms(6);
	}

// G�re l'enable du LCD
void LCD_I2C_Enable(void)
	{
   	PORT_LCD (|= (1 << BIT_LCD_E))      // E � 1
//   	asm("NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;");
	delay_us1(10);
   	PORT_LCD (&= ~(1 << BIT_LCD_E))     // E � 0
	}

// G�re l'�criture des 4 MSB vesr le LCD
void LCD_I2C_Send4msb(char donnee)
	{
   	// Reset bit de donn�es
   	PORT_LCD (&= ~((1 << BIT_LCD_D4) | (1 << BIT_LCD_D5) | (1 << BIT_LCD_D6) | (1 << BIT_LCD_D7)))
   
   	// Ecriture des 4 MSB
   	if (donnee & (1 << 4))
      	PORT_LCD (|= (1 << BIT_LCD_D4)) // Bit D4 � 1
   
   	if (donnee & (1 << 5))
      	PORT_LCD (|= (1 << BIT_LCD_D5)) // Bit D5 � 1
   
   	if (donnee & (1 << 6))
      	PORT_LCD (|= (1 << BIT_LCD_D6)) // Bit D6 � 1
   
   	if (donnee & (1 << 7))
      	PORT_LCD (|= (1 << BIT_LCD_D7)) // Bit D7 � 1
    
   	// Validation donnee par basculement de l'enable
   	LCD_I2C_Enable();
	}

// Initialisation du LCD
void LCD_I2C_Init(int i2c_adr)
	{
    lcd_i2c_adr= i2c_adr;

   	// Attente de l'initialisation interne du circuit LCD
   	delay_ms(20);
   	PORT_LCD (&= ~((1 << BIT_LCD_D4) | (1 << BIT_LCD_D5) | (1 << BIT_LCD_D6) | (1 << BIT_LCD_D7)))

   	// RS � 0
   	PORT_LCD (&= ~(1 << BIT_LCD_RS))
   	delay_ms(10);
   
   	// Configuration initiale : etape 1
   	LCD_I2C_Send4msb(0x30);
   	delay_ms(5);

   	// Configuration initiale : etape 2
   	LCD_I2C_Send4msb(0x30);
   	delay_us2(160);

   	// Configuration initiale : etape 3
   	LCD_I2C_Send4msb(0x30);
   	delay_us2(160);

   	// Passage en mode 4 bits
   	LCD_I2C_Send4msb(0x20);
   	delay_ms(5);
   
   	// Envoi des commandes de param�trage
   	LCD_I2C_SendCommand(0x28);		// 4 bits - 2 lignes - Police 5x7
   	LCD_I2C_Config(0,0,0);   	// Display off - Cursor off - Blinking off
   	LCD_I2C_Erase();
   	LCD_I2C_InsertMode(1,0);   	// Cursor increase - Display not shifted
   	LCD_I2C_Config(1,0,0);   	//  Display on - Cursor off - Blinking off
	}      

// Efface le contenu de l'ecran LCD
void LCD_I2C_Erase(void)
	{   
   	LCD_I2C_SendCommand(LCD_CMD_EFF);
	}

// Renvoie le curseur � la position initiale
void LCD_I2C_Home(void)
	{   
   	LCD_I2C_SendCommand(LCD_CMD_HOME);
	}

// S�lection du mode d'insertion des caract�res sur le LCD
void LCD_I2C_InsertMode(int direction_curseur,int inversion_affichage)
	{   
   	LCD_I2C_SendCommand(LCD_CMD_ENTRY_MODE | (inversion_affichage << LCD_BIT_ENTRY_SHIFT) | (direction_curseur << LCD_BIT_ENTRY_INC));
	}

// Configuration affichage LCD
void LCD_I2C_Config(int affichage_actif,int curseur_actif,int clignotement_actif)
	{
   	LCD_I2C_SendCommand(
      		LCD_CMD_DISPLAY
      		| (affichage_actif << LCD_BIT_DISP_DISP)
      		| (curseur_actif << LCD_BIT_DISP_CURS)
      		| (clignotement_actif << LCD_BIT_DISP_BLINK)
   		);
	}

// D�placement affichage ou curseur
void LCD_I2C_Move(int type,int sens)
	{
   	LCD_I2C_SendCommand(
      		LCD_CMD_SHIFT
      		| (type << LCD_BIT_SHIFT_TYPE)
      		| (sens << LCD_BIT_SHIFT_SENS)
   		);
	}

// D�placement du curseur de n caract�res vers la droite
void LCD_I2C_MoveRight(int nombre_caracteres)
	{
	int i;

	for(i=0;i<nombre_caracteres;i++)
  		LCD_I2C_Move(0,1);
	}

// D�placement du curseur de n caract�res vers la gauche
void LCD_I2C_MoveLeft(int nombre_caracteres)
	{
   	int i;

   	for(i=0;i<nombre_caracteres;i++)
  		LCD_I2C_Move(0,0);
	}

// D�placement de n caract�res vers la droite des donn�es � partir du curseur
void LCD_I2C_ShiftRight(int nombre_caracteres)
	{
   	int i;

   	for(i=0;i<nombre_caracteres;i++)
      	LCD_I2C_Move(1,1);
	}

// D�placement de n caract�res vers la gauche des donn�es � partir du curseur
void LCD_I2C_ShiftLeft(int nombre_caracteres)
	{
   	int i;
	
   	for(i=0;i<nombre_caracteres;i++)
      	LCD_I2C_Move(1,0);
	}

// Ecriture d'une cha�ne de caract�re sur le LCD
void LCD_I2C_Write(char *chaine)
	{
   	int i;
   
   	if (chaine == 0)		// Si le pointeur pass� en param�tre n'est pas valide
      	return;      		// => sortie de la fonction 
					    
   	for (i=0;i< strlen(chaine);i++)
      	LCD_I2C_SendChar(chaine[i]); 
	}

// D�placement du curseur
void LCD_I2C_MoveCursor(int y,int x)
	{
   	int adresse;
   
   	if ((y < 1) || (y > 2) || (x < 1) || (x > 16))
   		{
      	return;      
		// Si le num�ro de caract�re/ligne n'est pas bon => sortie de la fonction
   		}   

   	// Ligne 1
   	if (y == 1)
   		{
      	adresse = LCD_LIGNE_1 + (x - 1);
   		}
   	// Ligne 2
   	else
   		{
      	adresse = LCD_LIGNE_2 + (x - 1);
   		}
   
   	LCD_I2C_SendCommand(LCD_CMD_SET_DDRAM | adresse);
	}

/*
// Affiche une variale de type entier sur l'�cran LCD
void LCD_I2C_ShowLong(long entier)
	{
   	int compteur = 0;
   	char caracteres[10];  // Un entier long comporte un maximum de 10 digits  
   
   	// Si l'entier vaut 0, on l'affiche tout de suite
   	if(entier == 0)
   		{
      	LCD_I2C_SendChar('0');
   		}   
   	else
   		{
      	// Gestion de l'affichage du signe moins dans le cas d'un nombre n�gatif
      	if(entier < 0)
      		{
        	LCD_I2C_SendChar('-');
        	entier = -entier;   // Repassage du nombre en positif
      		}
      
      	// Construction de la cha�ne de caract�res correspondante
      	// Principe: divisions successives par 10 pour r�cup�rer chaque digit
      	// jusqu'� arriver � 0.
      	while((entier>0) && (compteur <= 10))
      		{
        	caracteres[compteur] = entier % 10;  // R�cup�ration du digit actuel
        	entier /= 10;   // Passage au digit suivant
        	compteur++;
      		}
      
      	// Affichage de la cha�ne (on commence par les derniers digit
      	// puisque la construction s'est faites dans le sens inverse)
      	for(;compteur>0; compteur--)
      		{
        	LCD_I2C_SendChar('0' + caracteres[compteur-1]);
      		}
   		}
	}

// Affiche une variable de type double � l'�cran avec un nombre de d�cimales d�fini
void LCD_I2C_ShowDouble(double vdouble, int nb_decimales)
	{
   	unsigned long fraction;
   	unsigned long fraction_successives;
   	unsigned long multiple = 1;
   	int nb_decimales_affichees = nb_decimales - 1;

   	// Etape 1 : Affichage de la valeur enti�re
   	LCD_I2C_ShowLong((long) vdouble);

   	// Etape 2 : Affichage de la partie d�cimale
   	// Principe : Multiplication successive par 10 pour construire le nombre entier
   	// correspondant aux d�cimales

   	if (nb_decimales > 0)
   		{
      	// Affichage de la virgule
      	LCD_I2C_SendChar(',');
      
      	// R�cup�ration du premier diviseur
      	while(nb_decimales--)
      		{
        	multiple *= 10;
      		}
      
      	// Toute la partie d�cimale est transform�e en partie enti�re      
      	// Nombre positif
      	if (vdouble >= 0)
      		{
        	fraction = (vdouble - (int) vdouble) * multiple;
      		}
      	// Nombre n�gatif
      	else
      		{
        	fraction = ((int) vdouble - vdouble) * multiple;      
      		}
      
      	fraction_successives = fraction;
      
      	// V�rification si besoin de rajouter des z�ros apr�s la virgule
      	// avant d'afficher la partie d�cimale
      	while(fraction_successives /= 10)
      		{
        	nb_decimales_affichees--;
      		}
      
      	// Affichage des z�ros si besoin
      	while(nb_decimales_affichees--)
      		{
        	LCD_I2C_SendChar('0');
      		}
      
      	// Affichage de la partie d�cimale
      	LCD_I2C_ShowLong((long) fraction);
   		}
	}
*/
