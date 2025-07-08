#include <stdio.h>
int main()
{
    int raw;
    raw = 0x042e;
    
    int scale_reg, vdiff_gain_num, vdiff_gain_den, step_sel,
        scale_loop;
    long vout_mv;
    scale_reg = 0x105b;

    // raw = pmbus_read_word_data(client, page, phase,
    // 			   PMBUS_READ_VOUT); // 0x8B
    // scale_reg = pmbus_read_word_data(
    // 	client, page, phase,
    // 	PMBUS_VOUT_SCALE_LOOP); // 0x29
    step_sel = (scale_reg >> 10) & 0x7;
    scale_loop = scale_reg & 0xff;
    printf("%x, %d\n", scale_reg, scale_loop);
    static const int res_num[] = { 625, 500, 250, 200,
                    100, 1,	 1,   1 };
    static const int res_den[] = { 100, 100, 100, 100,
                    100, 256, 512, 1024 };
    vdiff_gain_num = scale_loop;
    vdiff_gain_den = 8;
    printf("%d\n", vdiff_gain_num/vdiff_gain_den);
    printf("%d\n", res_num[step_sel] / res_den[step_sel]);
    vout_mv = (long)raw * vdiff_gain_num *
        res_num[step_sel] / res_den[step_sel] / 8;
    printf("mpc42013: %ld * %d * %d = %d\n", (long)raw,  res_den[step_sel] / 8, res_num[step_sel] / res_den[step_sel], (int)vout_mv);
    

    return 0;
}