#include "screen.h"

void print_Menu_Interface(void)
{
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

    // Première fonctionnalité
    BSP_LCD_DrawRect(START_X, START_Y, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 1, START_Y + 1, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 2, START_Y + 2, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 3, START_Y + 3, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DisplayStringAt(START_X - 15, START_Y + BUTTON_WIDTH_HEIGHT + 20, (uint8_t *)"Play/Pause", LEFT_MODE);
    Draw_Play_Button();

    // Deuxième fonctionnalité
    BSP_LCD_DrawRect(START_X + SPACE_BETWEEN_BUTTONS, START_Y, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 1 + SPACE_BETWEEN_BUTTONS, START_Y + 1, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 2 + SPACE_BETWEEN_BUTTONS, START_Y + 2, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 3 + SPACE_BETWEEN_BUTTONS, START_Y + 3, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DisplayStringAt(START_X + SPACE_BETWEEN_BUTTONS - 2, START_Y + BUTTON_WIDTH_HEIGHT + 20, (uint8_t *)"Audio Out", LEFT_MODE);
    Draw_Output_Button(LCD_COLOR_WHITE);

    // Troisième fonctionnalité
    BSP_LCD_DrawRect(START_X + SPACE_BETWEEN_BUTTONS * 2, START_Y, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 1 + SPACE_BETWEEN_BUTTONS * 2, START_Y + 1, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 2 + SPACE_BETWEEN_BUTTONS * 2, START_Y + 2, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 3 + SPACE_BETWEEN_BUTTONS * 2, START_Y + 3, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DisplayStringAt(START_X + SPACE_BETWEEN_BUTTONS * 2 + 20, START_Y + BUTTON_WIDTH_HEIGHT + 20, (uint8_t *)"Record", LEFT_MODE);
    Draw_SD_Button(LCD_COLOR_WHITE);

    // Quatrième fonctionnalité
    BSP_LCD_DrawRect(START_X + SPACE_BETWEEN_BUTTONS * 3, START_Y, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 1 + SPACE_BETWEEN_BUTTONS * 3, START_Y + 1, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 2 + SPACE_BETWEEN_BUTTONS * 3, START_Y + 2, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DrawRect(START_X + 3 + SPACE_BETWEEN_BUTTONS * 3, START_Y + 3, BUTTON_WIDTH_HEIGHT, BUTTON_WIDTH_HEIGHT);
    BSP_LCD_DisplayStringAt(START_X + SPACE_BETWEEN_BUTTONS * 3 + 60, START_Y + BUTTON_WIDTH_HEIGHT + 20, (uint8_t *)"AI", LEFT_MODE);
    Draw_AI_Button(LCD_COLOR_WHITE);

    // Ligne pour Luminosité
    BSP_LCD_DrawRect(20, BSP_LCD_GetYSize() - 40, BSP_LCD_GetXSize() / 2 - 40, 20);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize() - 60, (uint8_t *)"Luminosite", LEFT_MODE);

    // Ligne pour Volume
    BSP_LCD_DrawRect(BSP_LCD_GetXSize() / 2 + 20, BSP_LCD_GetYSize() - 40, BSP_LCD_GetXSize() / 2 - 40, 20);
    BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() / 2 + 20, BSP_LCD_GetYSize() - 60, (uint8_t *)"Volume", LEFT_MODE);
    BSP_LCD_SetFont(&Font24);

    // Rectangle central des logs
    BSP_LCD_DrawRect(20, START_Y + BUTTON_WIDTH_HEIGHT + 50, BSP_LCD_GetXSize() - 40, BSP_LCD_GetYSize() / 2 - 60);
}

void Draw_Play_Button(void)
{
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(START_X + 20, START_Y + 20, BUTTON_WIDTH_HEIGHT - 40, BUTTON_WIDTH_HEIGHT - 40);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    FillTriangle(START_X + 43, START_X + 43, START_X + BUTTON_WIDTH_HEIGHT - 40 - 3, START_Y + 43, START_Y + BUTTON_WIDTH_HEIGHT - 43, START_Y + BUTTON_WIDTH_HEIGHT / 2);
}

void Draw_Pause_Button(void)
{
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(START_X + 20, START_Y + 20, BUTTON_WIDTH_HEIGHT - 40, BUTTON_WIDTH_HEIGHT - 40);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(START_X + 43, START_Y + 43, 20, BUTTON_WIDTH_HEIGHT - 86);
    BSP_LCD_FillRect(START_X + 43 + 20 + 13, START_Y + 43, 20, BUTTON_WIDTH_HEIGHT - 86);
}
#define OFFSET_LEFT -3 // Définir un décalage vers la gauche

void Draw_Output_Button(uint32_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillRect(START_X + 4 + SPACE_BETWEEN_BUTTONS, START_Y + 4, BUTTON_WIDTH_HEIGHT - 4, BUTTON_WIDTH_HEIGHT - 4);

    // Dessiner le cercle principal plus petit et plus épais
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillCircle(START_X + SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH_HEIGHT / 2 - OFFSET_LEFT, START_Y + BUTTON_WIDTH_HEIGHT / 2, BUTTON_WIDTH_HEIGHT / 2 - 25);

    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillCircle(START_X + SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH_HEIGHT / 2 - OFFSET_LEFT, START_Y + BUTTON_WIDTH_HEIGHT / 2, BUTTON_WIDTH_HEIGHT / 2 - 35);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Masquer la moitié inférieure du cercle avec un rectangle blanc
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillRect(START_X + SPACE_BETWEEN_BUTTONS - OFFSET_LEFT + 5, START_Y + BUTTON_WIDTH_HEIGHT / 2, BUTTON_WIDTH_HEIGHT - 20, BUTTON_WIDTH_HEIGHT / 2);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Dessiner les coussinets avec des rectangles
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    BSP_LCD_FillRect(START_X + SPACE_BETWEEN_BUTTONS + 20 - OFFSET_LEFT, START_Y + BUTTON_WIDTH_HEIGHT / 2 - 12, 15, 30);
    BSP_LCD_FillRect(START_X + SPACE_BETWEEN_BUTTONS + BUTTON_WIDTH_HEIGHT - 20 - OFFSET_LEFT - 15, START_Y + BUTTON_WIDTH_HEIGHT / 2 - 12, 15, 30);
}

void Draw_SD_Button(uint32_t color)
{
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillRect(START_X + 4 + SPACE_BETWEEN_BUTTONS * 2, START_Y + 4, BUTTON_WIDTH_HEIGHT - 4, BUTTON_WIDTH_HEIGHT - 4);

    // Définir la couleur de remplissage
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Dessiner un rectangle plein pour le troisième bouton
    BSP_LCD_FillRect(START_X + SPACE_BETWEEN_BUTTONS * 2 + 40, START_Y + 28, 65, 90);

    // Dessiner un rectangle plein pour le troisième bouton
    BSP_LCD_SetTextColor(color);
    FillTriangle(START_X + SPACE_BETWEEN_BUTTONS * 2 + 110, START_X + SPACE_BETWEEN_BUTTONS * 2 + 110, START_X + SPACE_BETWEEN_BUTTONS * 2 + 80, START_Y + 20, START_Y + 50, START_Y + 20);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

void Draw_AI_Button(uint32_t color)
{
    // Définir les coordonnées de base pour le troisième bouton
    int x = START_X + SPACE_BETWEEN_BUTTONS * 3;
    int y = START_Y;

    // Ajuster les coordonnées pour centrer le logo
    int center_x = x + BUTTON_WIDTH_HEIGHT / 2;
    int center_y = y + BUTTON_WIDTH_HEIGHT / 2;

    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillRect(START_X + 4 + SPACE_BETWEEN_BUTTONS * 3, START_Y + 4, BUTTON_WIDTH_HEIGHT - 4, BUTTON_WIDTH_HEIGHT - 4);

    // Définir la couleur de remplissage
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    // Agrandir les dimensions des rectangles
    int rect_width = 10;   // Largeur des bras de la croix
    int rect_height = 100; // Hauteur des bras de la croix

    // Dessiner la croix avec des rectangles pleins
    BSP_LCD_FillRect(center_x - rect_width / 2 - 10, center_y - rect_height / 2, rect_width, rect_height); // Bras vertical
    BSP_LCD_FillRect(center_x - rect_width / 2 + 10, center_y - rect_height / 2, rect_width, rect_height); // Bras vertical
    BSP_LCD_FillRect(center_x - rect_height / 2, center_y - rect_width / 2 - 10, rect_height, rect_width); // Bras horizontal
    BSP_LCD_FillRect(center_x - rect_height / 2, center_y - rect_width / 2 + 10, rect_height, rect_width);

    BSP_LCD_FillRect(center_x - rect_height / 2, center_y - rect_width / 2 + 10, rect_width, 25);                    // Bras horizontal
    BSP_LCD_FillRect(center_x - rect_height / 2, center_y - rect_width / 2 - 25, rect_width, 25);                    // Bras horizontal
    BSP_LCD_FillRect(center_x - rect_height / 2 + rect_height - 10, center_y - rect_width / 2 - 25, rect_width, 25); // Bras horizontal
    BSP_LCD_FillRect(center_x - rect_height / 2 + rect_height - 10, center_y - rect_width / 2 + 10, rect_width, 25);

    BSP_LCD_FillRect(center_x - rect_width / 2 + 10, center_y - rect_height / 2, 25, rect_width);                    // Bras vertical
    BSP_LCD_FillRect(center_x - rect_width / 2 - 25, center_y - rect_height / 2, 25, rect_width);                    // Bras vertical
    BSP_LCD_FillRect(center_x - rect_width / 2 - 25, center_y - rect_height / 2 + rect_height - 10, 25, rect_width); // Bras vertical
    BSP_LCD_FillRect(center_x - rect_width / 2 + 10, center_y - rect_height / 2 + rect_height - 10, 25, rect_width);

    // Ajouter un carré plein au milieu de la croix
    int square_size = 60; // Taille du carré central
    BSP_LCD_FillRect(center_x - square_size / 2, center_y - square_size / 2, square_size, square_size);
}

void Clear_Log(void)
{
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(21, START_Y + BUTTON_WIDTH_HEIGHT + 51, BSP_LCD_GetXSize() - 40 - 2, BSP_LCD_GetYSize() / 2 - 60 - 2);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}
