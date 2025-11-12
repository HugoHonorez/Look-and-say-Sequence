#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "led.h"

int main(int argc, char *argv[]) {

        if (initLed() != 0) {
                return 1;
        }

        printf("LED ON!\n");

        for (int i = 0; i < 100; i++) {

                setRedLed();
                sleep(1);
                setGreenLed();
                sleep(1);
                setBlueLed();
                sleep(1);
                setCyanLed();
                sleep(1);
                setOffLed();
                sleep(1);

        }

        clearLed();
        return 0;

}
