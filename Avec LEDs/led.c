#include <stdio.h>
#include <stdlib.h>
#include <gpiod.h>
#include "led.h"

#define CHIPNAME "/dev/gpiochip0"
#define RED_PIN 12
#define GREEN_PIN 13
#define BLUE_PIN 19

static struct gpiod_chip *chip = NULL;
static struct gpiod_line *red = NULL;
static struct gpiod_line *green = NULL;
static struct gpiod_line *blue = NULL;

int initLed(void) {

        chip = gpiod_chip_open(CHIPNAME);

        if (!chip) {
                printf("Error: unable to open the chip.");
                return 1;
        }

        red = gpiod_chip_get_line(chip, RED_PIN);
        green = gpiod_chip_get_line(chip, GREEN_PIN);
        blue = gpiod_chip_get_line(chip, BLUE_PIN);

        if (!red || !green || !blue) {
                printf("Error: unable to access GPIO lines.");
                gpiod_chip_close(chip);
                return 1;
        }

        gpiod_line_request_output(red, "rgb_led", 1);
        gpiod_line_request_output(green, "rgb_led", 1);
        gpiod_line_request_output(blue, "rgb_led", 1);

        return 0;
}

static void setColor(int r, int g, int b) {
        gpiod_line_set_value(red, r);
        gpiod_line_set_value(green, g);
        gpiod_line_set_value(blue, b);
}

void setRedLed(void) {
        setColor(0, 1, 1);
}

void setGreenLed(void) {
        setColor(1, 0, 1);
}

void setBlueLed(void) {
        setColor(1, 1, 0);
}

void setCyanLed(void) {
        setColor(1, 0, 0);
}

void setOffLed(void) {
        setColor(1, 1, 1);
}

void clearLed(void) {
        gpiod_line_release(red);
        gpiod_line_release(green);
        gpiod_line_release(blue);
        gpiod_chip_close(chip);
}
