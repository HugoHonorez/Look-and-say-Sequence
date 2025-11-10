#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define CHIPNAME "/dev/gpiochip0"

#define RED_PIN   12
#define GREEN_PIN 13
#define BLUE_PIN  19

void setColor(struct gpiod_line *r, struct gpiod_line *g, struct gpiod_line *b,
              int red, int green, int blue)
{
    gpiod_line_set_value(r, red);
    gpiod_line_set_value(g, green);
    gpiod_line_set_value(b, blue);
}

int main(void)
{
    struct gpiod_chip *chip;
    struct gpiod_line *red, *green, *blue;

    chip = gpiod_chip_open(CHIPNAME);
    if (!chip) {
        perror("Erreur: impossible d'ouvrir le chip GPIO");
        return 1;
    }
    // Récupérer les lignes
    red = gpiod_chip_get_line(chip, RED_PIN);
    green = gpiod_chip_get_line(chip, GREEN_PIN);
    blue = gpiod_chip_get_line(chip, BLUE_PIN);

    if (!red || !green || !blue) {
        perror("Erreur: impossible d'accéder aux lignes GPIO");
        gpiod_chip_close(chip);
        return 1;
    }

    // Configurer les lignes en sortie
    gpiod_line_request_output(red, "rgb_led", 0);
    gpiod_line_request_output(green, "rgb_led", 0);
    gpiod_line_request_output(blue, "rgb_led", 0);

    printf("Contrôle LED RGB avec libgpiod\n");

    while (1) {
        setColor(red, green, blue, 0, 1, 1); // Rouge //Fonctionne pas
        sleep(1);
        setColor(red, green, blue, 1, 0, 1); // Vert
        sleep(1);
        setColor(red, green, blue, 1, 1, 0); // Bleu
        sleep(1);
        setColor(red, green, blue, 1, 0, 0); // IDK
        sleep(1);
        setColor(red, green, blue, 1, 1, 1); // Éteint //Inverse
        sleep(1);
    }

    // Libérer les ressources
    gpiod_line_release(red);
    gpiod_line_release(green);
    gpiod_line_release(blue);
    gpiod_chip_close(chip);

    return 0;
}
