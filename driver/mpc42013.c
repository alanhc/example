// SPDX-License-Identifier: GPL-2.0+
/*
 * Hardware monitoring driver for mps mpc42013
 */
#include <linux/bits.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include "pmbus.h"
#ifndef PMBUS_CAPABILITY
#define PMBUS_CAPABILITY 0x19
#endif
enum chips {
	mpc42013,
};

#define MPC42013_REL_MODE BIT(7) /* VOUT_MODE relative mode bit */
#define MPC42013_LINEAR_MASK 0x60 /* VOUT_MODE linear format mask */
#define MPC42013_PAGE_NUM (2)

#define IOUT_RESOLUTION (0.5)
#define POUT_RESOLUTION (1)
#define VOUT_RESOLUTION (1)



static const struct i2c_device_id mpc42013_id[];

static int linear11(int raw)
{
	int8_t e;
	int16_t m;
	int64_t tmp;
	e = (raw >> 11) & 0x1F;
	m = raw & 0x07FF;

	if (e & 0x10)
		e |= ~0x1F;
	if (m & 0x0400)
		m |= ~0x07FF;

	pr_alert_once("mpc42013 e: %d, m: %d\n", e, m);
	tmp = (int64_t)m * 1000;

	if (e >= 0) {
		return (int)(tmp << e);
	} else {
		return (int)(tmp >> -e);
	}
	return -1;
}

static int caculate_vdiff(int vout_scale_loop)
{
	int scale_loop;
	scale_loop = scale_reg & 0xff;
	return scale_loop / 8;
}
/*
 * The mpc42013 defaults to the relative format for the following registers:
 * VOUT_MARGIN_HIGH, VOUT_MARGIN_LOW, VOUT_OV_FAULT_LIMIT, VOUT_OV_WARN_LIMIT,
 * VOUT_UV_WARN_LIMIT, and VOUT_UV_FAULT_LIMIT.
 * Intercept these register read to convert the relative format to the absolute
 * format, that is to multiply the ULINEAR16 value by the VOUT_COMMAND value
 * and then shift the value by the exponent in VOUT_MODE. Return -EINVAL when
 * unsupported registers are read.
 */
static int mpc42013_read_word_data(struct i2c_client *client, int page,
				   int phase, int reg)
{
	int raw = pmbus_read_word_data(client, page, phase, reg);
	long masked_raw;
	if (raw < 0) {
		pr_alert_once("mpc42013: reg=0x%x, read error=%d\n", reg, raw);
		return raw;
	}
	int scale_reg = pmbus_read_word_data(client, page, phase,
						 PMBUS_VOUT_SCALE_LOOP); // 0x29
	int vdiff_gain = caculate_vdiff(scale_reg);
	switch (reg) {
	case PMBUS_READ_VIN:
		int16_t val = (int16_t)raw;
		int mantissa = val & 0x7FF;
		int res = (int)mantissa;
		pr_alert_once("PMBUS_READ_VIN m:%d, res: %d", mantissa, res * 125);
		return res;
	case PMBUS_READ_VOUT:
		int step_sel;
		long vout_mv;
		step_sel = (scale_reg >> 10) & 0x7;
		static const int res_num[] = {
			625, 500, 250, 200, 100, 1, 1, 1
		};
		static const int res_den[] = { 100, 100, 100, 100,
					       100, 256, 512, 1024 };
		vout_mv = (int)((long)raw * vdiff_gain * res_num[step_sel] /
				res_den[step_sel]) * VOUT_RESOLUTION;
		pr_alert_once("mpc42013: %ld * %d * %d = %d\n", (long)raw, res_den[step_sel], res_num[step_sel] / res_den[step_sel], (int)vout_mv);
		return vout_mv;
	case PMBUS_READ_POUT:
		    masked_raw = raw & 0x7ff;
			pr_alert("mpc42013: raw %x %d, mask_raw: %x %d, ans:%d", raw, raw, (int)masked_raw, (int)masked_raw, (int)(masked_raw*vdiff_gain));
            return (int)(masked_raw*vdiff_gain*POUT_RESOLUTION);
	case PMBUS_READ_IOUT:
		    masked_raw = raw & 0x7ff;
            pr_alert("mpc42013: masked_raw %x %d, return: %x %d, ans: %d", raw, raw,  (int)masked_raw,  (int)masked_raw, (int)(masked_raw*IOUT_RESOLUTION));
			return (int)(masked_raw*IOUT_RESOLUTION);
	case PMBUS_READ_TEMPERATURE_1:
			return linear11(raw);
	case PMBUS_READ_IIN:
		return linear11(raw);
	case PMBUS_READ_PIN:
		return linear11(raw);
	default:
		return 0;
	}

}

static struct pmbus_driver_info mpc42013_info = {
	.pages = MPC42013_PAGE_NUM,
	.format[PSC_VOLTAGE_IN] = direct,
	.format[PSC_VOLTAGE_OUT] = linear,
	.format[PSC_TEMPERATURE] = linear,
	.format[PSC_CURRENT_OUT] = linear,
	.format[PSC_CURRENT_IN]  = linear,
	.format[PSC_POWER] = linear,
	.func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN | PMBUS_HAVE_STATUS_INPUT |
		   PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_IOUT |
		   PMBUS_HAVE_STATUS_IOUT | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT |
		   PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP,
	.read_word_data = mpc42013_read_word_data,

};

static const struct i2c_device_id mpc42013_id[] = { { "mpc42013", mpc42013 },
						    {} };
MODULE_DEVICE_TABLE(i2c, mpc42013_id);

static int mpc42013_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct pmbus_driver_info *info;
	int ret;

	info = devm_kmemdup(dev, &mpc42013_info,
			    sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	info->m[PSC_VOLTAGE_IN] = 8;
	info->b[PSC_VOLTAGE_IN] = 0;
	info->R[PSC_VOLTAGE_IN] = 0;

	ret = i2c_smbus_write_byte_data(client, PMBUS_PAGE, 0x00);
	if (ret)
		dev_warn(dev, "Unable to set PAGE=0 (%d), ignore\n", ret);
	
	
	ret  = pmbus_do_probe(client, info);
	if (ret) return ret;

	dev_info(dev, "%s probed\n", client->name);   
	
	return 0;
}

static const struct of_device_id __maybe_unused mpc42013_of_match[] = {
	{ .compatible = "mps,mpc42013", .data = (void *)mpc42013 },
	{}
};

MODULE_DEVICE_TABLE(of, mpc42013_of_match);

static struct i2c_driver mpc42013_driver = {
	.driver = {
		.name = "mpc42013",
		.of_match_table = of_match_ptr(mpc42013_of_match),
	},
	.probe_new = mpc42013_probe,
	.remove = pmbus_do_remove,
	.id_table = mpc42013_id,
};

module_i2c_driver(mpc42013_driver);

MODULE_AUTHOR("Alan Tseng <alan_tseng@wiwynn.com>");
MODULE_DESCRIPTION("PMBus driver for MPC42013");
MODULE_LICENSE("GPL");
