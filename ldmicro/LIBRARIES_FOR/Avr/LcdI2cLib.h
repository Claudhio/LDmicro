#include <string.h>

#include "UsrLib.h"

#define BIT_LCD_RS   	0     // bit du port sur lequel est connect�e la pin RS du LCD		// adapte pour IO-Expander facon Arduino
#define BIT_LCD_E   	2     // bit du port sur lequel est connect�e la pin E du LCD		//
#define BIT_LCD_D4   	4     // bit du port sur lequel est connect�e la pin D4 du LCD		//
#define BIT_LCD_D5   	5     // bit du port sur lequel est connect�e la pin D5 du LCD		//
#define BIT_LCD_D6   	6     // bit du port sur lequel est connect�e la pin D6 du LCD		//
#define BIT_LCD_D7   	7     // bit du port sur lequel est connect�e la pin D7 du LCD		//

#define PORT_LCD(x)		port x; I2C_MasterSetReg(lcd_i2c_adr, 0, port);		// adaptation pour IO-Expander I2C

#define LCD_CMD_EFF        	  0x01   // Commande d'effacement LCD
#define LCD_CMD_HOME      	  0x02   // Commande de renvoi du curseur � la position initiale
#define LCD_CMD_ENTRY_MODE    0x04   // Commande de s�lection du mode d'insertion
#define LCD_BIT_ENTRY_SHIFT   0      // Position bit de s�lection inversion/non inversion
#define LCD_BIT_ENTRY_INC     1      // Position bit de s�lection 									
							  		 // incr�mentation/d�crementation curseur
#define LCD_CMD_DISPLAY 	  0x08   // Commande de gestion de l'affichage LCD
#define LCD_BIT_DISP_DISP     2      // Position bit d'activation/d�sactivation de l'affichage
#define LCD_BIT_DISP_CURS     1      // Position bit d'activation/d�sactivation curseur
#define LCD_BIT_DISP_BLINK    0      // Position bit d'activation/d�sactivation clignotement curseur
#define LCD_CMD_SHIFT      	  0x10   // Commande de d�placement affichage/curseur
#define LCD_BIT_SHIFT_TYPE    3      // Position bit de type de d�placement
#define LCD_BIT_SHIFT_SENS    2      // Position bit de sens de d�placement
#define LCD_CMD_SET_DDRAM     0x80   // Commande d'affectation de l'adresse DDRAM
#define LCD_LIGNE_1           0x00   // Adresse ligne 1
#define LCD_LIGNE_2           0x40   // Adresse ligne 2
#define LCD_CMD_SET_CGRAM     0x40   // Commande d'affectation de l'adresse CGRAM

// Envoi au LCD du caract�re pass� en param�tre
void LCD_I2C_SendChar(char caractere);
   
// Envoi au LCD de la commande pass�e en param�tre
void LCD_I2C_SendCommand(char commande);

// Initialisation du LCD
void LCD_I2C_Init(int i2c_adr);

// Effacement du LCD
void LCD_I2C_Erase(void);

// Renvoi du curseur en haut � gauche
void LCD_I2C_Home(void);

// S�lection du mode d'insertion pour le LCD
// direction_curseur : 1 = incr�mentation - 0 = d�cr�mentation
// inversion_affichage : 1 = inversion - 0 = non inversion
void LCD_I2C_InsertMode(int direction_curseur,int inversion_affichage);

// Configuration affichage LCD
// affichage_actif : 1 = actif - 0 = inactif
// curseur_actif : 1 = actif - 0 = inactif
// clignotement_actif : 1 = actif - 0 = inactif
void LCD_I2C_Config(int affichage_actif,int curseur_actif,int clignotement_actif);

// D�placement affichage ou curseur
// type : 1 = affichage - 0 = curseur
// sens : 1 = droite - 0 = gauche
void LCD_I2C_Move(int type,int sens);

// D�placement du curseur de n caract�res vers la droite
// nombre_caracteres : nombre de caracteres de d�placement vers la droite
void LCD_I2C_MoveRight(int nombre_caracteres);

// D�placement du curseur de n caract�res vers la gauche
// nombre_caracteres : nombre de caracteres de d�placement vers la gauche
void LCD_I2C_MoveLeft(int nombre_caracteres);

// D�placement de n caract�res vers la droite des donn�es � partir du curseur
// nombre_caracteres : nombre de caracteres de d�placement vers la droite
void LCD_I2C_ShiftRight(int nombre_caracteres);

// D�placement de n caract�res vers la gauche des donn�es � partir du curseur
// nombre_caracteres : nombre de caracteres de d�placement vers la gauche
void LCD_I2C_ShiftLeft(int nombre_caracteres);

// Ecriture d'une cha�ne de caract�re sur le LCD
// chaine : le pointeur sur la cha�ne de caract�res � envoyer
void LCD_I2C_Write(char *chaine);

// D�placement du curseur
// y : num�ro de la ligne (1 ou 2)
// x : num�ro du caract�re (1 � 16)
void LCD_I2C_MoveCursor(int y,int x);

// Affiche une variale de type entier sur l'�cran LCD
// entier : l'entier � afficher sur l'�cran
void LCD_I2C_ShowLong(long entier);

// Affiche une variable de type double � l'�cran avec un nombre de d�cimales d�fini
// vdouble : la valeur d�cimale � afficher
// nb_decimales : le nombre de d�cimales � afficher apr�s la virgule
void LCD_I2C_ShowDouble(double vdouble, int nb_decimales);


