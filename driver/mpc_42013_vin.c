
#include <stdint.h>

int main()
{
    int raw = 0xe198;
    int16_t val = (int16_t)raw;

    int mantissa = val & 0x7FF;
    int res = (int)mantissa;
    printf("PMBUS_READ_VIN m:%d, res: %d\n", mantissa, res * 125);

    return res;
}