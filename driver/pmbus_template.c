// SPDX-License-Identifier: GPL-2.0+
#include <linux/bits.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include "pmbus.h"

enum chips
{
    mpc42013,
};

#define MPC42013_PAGE_NUM (1)

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
    tmp = (int64_t)m * 1000;
    if (e >= 0)
    {
        return (int)(tmp << e);
    }
    else
    {
        return (int)(tmp >> -e);
    }
    return -1;
}

static int mpc42013_read_word_data(struct i2c_client *client, int page,
                                   int phase, int reg)
{
    int raw = pmbus_read_word_data(client, page, phase, reg);
    long masked_raw;
    if (raw < 0)
    {
        pr_alert_once("mpc42013: reg=0x%x, read error=%d\n", reg, raw);
        return raw;
    }

    switch (reg)
    {
    case PMBUS_READ_VIN:
        // TODO
        return 0;
    case PMBUS_READ_VOUT:
        // TODO
        return vout_mv;
    case PMBUS_READ_POUT:
        // TODO
        return 0;
    case PMBUS_READ_IOUT:
        // TODO
        return 0;
    case PMBUS_READ_TEMPERATURE_1:
        raw = pmbus_read_word_data(client, page, phase, reg);
        return linear11(raw);
    case PMBUS_READ_IIN:
        raw = pmbus_read_word_data(client, page, phase, reg);
        return linear11(raw);
    case PMBUS_READ_PIN:
        raw = pmbus_read_word_data(client, page, phase, reg);
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
    .format[PSC_CURRENT_IN] = linear,
    .format[PSC_POWER] = linear,
    .func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN | PMBUS_HAVE_STATUS_INPUT |
               PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_IOUT |
               PMBUS_HAVE_STATUS_IOUT | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT |
               PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP,
    .read_word_data = mpc42013_read_word_data,

};

static const struct i2c_device_id mpc42013_id[] = {{"mpc42013", mpc42013},
                                                   {}};
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

    ret = pmbus_do_probe(client, info);
    if (ret)
        return ret;

    dev_info(dev, "%s probed\n", client->name);

    return 0;
}

static const struct of_device_id __maybe_unused mpc42013_of_match[] = {
    {.compatible = "mps,mpc42013", .data = (void *)mpc42013},
    {}};

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

MODULE_AUTHOR("who <email>");
MODULE_DESCRIPTION("PMBus driver for MPC42013");
MODULE_LICENSE("GPL");
