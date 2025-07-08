#include <stdio.h>
#include <stdint.h>
static int linear11(int raw)
{
	int8_t e;
	int16_t m;
	int64_t tmp;
	e = (raw >> 11) & 0x1F; // 取高 5 bit
	m = raw & 0x07FF; // 取低 11 bit
    if (e & 0x10) e |= ~0x1F;  // e 為負數則補 1  0x10 = 0001 0000  0x1F = 0001 1111
	if (m & 0x0400) m |= ~0x07FF; //  m 為負數則補 1

	printf("mpc42013 e: %d, m: %d\n", e, m);
	tmp = (int64_t)m * 1000;

	if (e >= 0) {
		return (int)(tmp << e); // 左移：代表乘以 2^e
	} else {
		return (int)(tmp >> -e); // 右移：代表除以 2^-e
	}
	return -1;
}
int main()
{
    int raw = 0x01a5;
    printf("%d\n", linear11(raw));
}