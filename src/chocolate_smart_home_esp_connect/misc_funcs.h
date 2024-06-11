#ifndef MISC_FUNCS_H_
#define MISC_FUNCS_H_

void flash() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
}

#endif
