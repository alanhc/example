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

#define MPC42013_REL_MODE BIT(7)
#define MPC42013_LINEAR_MASK 0x60
#define MPC42013_PAGE_NUM (1)

static const struct i2c_device_id mpc42013_id[];

static int mpc42013_read_byte_data(struct i2c_client *client, int page, int reg)
{
    int reg_value = pmbus_read_byte_data(client, page, reg);

    if (reg_value < 0)
        return reg_value;

    if (reg == PMBUS_VOUT_MODE)
    {
        if (reg_value & MPC42013_LINEAR_MASK)
        {
            dev_err(&client->dev,
                    "VOUT_MODE is not in linear mode\n");
            return -EINVAL;
        }
        reg_value &= ~MPC42013_REL_MODE;
    }
    return reg_value;
}
static int linear11(int raw)
{
    int8_t e;
    int16_t m;
    s64 tmp;
    e = (raw >> 11) & 0x1F;
    m = raw & 0x07FF;

    if (e & 0x10)
        e |= ~0x1F;
    if (m & 0x0400)
        m |= ~0x07FF;

    pr_alert("mpc42013 e: %d, m: %d\n", e, m);
    tmp = (s64)m * 1000;

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

static int mpc42013_read_write_word(struct i2c_client *client, int page,
                                    int phase, int reg, int word,
                                    bool is_reading)
{
    int data_format, vout_command, vout_mode, exponent;

    data_format = pmbus_read_byte_data(client, page, PMBUS_VOUT_MODE);
    if (data_format < 0)
        return data_format;
    int raw;

    if (is_reading)
    {
        switch (reg)
        {
        case PMBUS_READ_VIN:
            int16_t m;
            raw = pmbus_read_word_data(client, page, phase, reg);
            if (raw < 0)
                return raw;
            m = raw & 0x7FF;
            int res;
            res = (int)m;
            pr_alert("mpc42013 m: %x, %d, vin: %d, raw: %x %d\n", m,
                     m, res, raw, raw);
            return res;
        }
    }
    if (data_format & MPC42013_REL_MODE)
    {
        switch (reg)
        {
        case PMBUS_READ_IIN:
            raw = pmbus_read_word_data(client, page, phase, reg);
            return linear11(raw);
        case PMBUS_READ_VOUT:
            int scale_reg, vdiff_gain_num, vdiff_gain_den, step_sel,
                scale_loop;
            long vout_mv;
            raw = pmbus_read_word_data(client, page, phase,
                                       PMBUS_READ_VOUT); // 0x8B
            scale_reg = pmbus_read_word_data(
                client, page, phase,
                PMBUS_VOUT_SCALE_LOOP); // 0x29
            step_sel = (scale_reg >> 10) & 0x7;
            scale_loop = scale_reg & 0xff;

            static const int res_num[] = {625, 500, 250, 200,
                                          100, 1, 1, 1};
            static const int res_den[] = { 100, 100, 100, 100,

                                           static const int res_den[] = {100, 100, 100, 100,
                                                                         100, 256, 512, 1024};
            vdiff_gain_num = scale_loop;
            vdiff_gain_den = 8;
            vout_mv = (long)raw * vdiff_gain_num *
                      res_num[step_sel] / res_den[step_sel] / 8;
            pr_alert("mpc42013 vout_mv: %d, raw: %x %d\n", vout_mv,
                     raw, raw);
            return vout_mv;
        case PMBUS_READ_IOUT:
            raw = pmbus_read_word_data(client, page, phase, reg);
            return linear11(raw);
        case PMBUS_READ_TEMPERATURE_1:
            raw = pmbus_read_word_data(client, page, phase, reg);
            return linear11(raw);
        case PMBUS_READ_POUT:
            raw = pmbus_read_word_data(client, page, phase, reg);
            return linear11(raw);
        case PMBUS_READ_PIN:
            raw = pmbus_read_word_data(client, page, phase, reg);
            return linear11(raw);

        case PMBUS_VOUT_MARGIN_HIGH:
        case PMBUS_VOUT_MARGIN_LOW:
        case PMBUS_VOUT_OV_FAULT_LIMIT:
        case PMBUS_VOUT_OV_WARN_LIMIT:
        case PMBUS_VOUT_UV_WARN_LIMIT:
        case PMBUS_VOUT_UV_FAULT_LIMIT:

            vout_command = pmbus_read_word_data(client, page, phase,
                                                PMBUS_VOUT_COMMAND);
            if (vout_command < 0)

                return vout_mode;

            exponent = ((s8)(vout_mode << 3)) >> 3;

            if (is_reading)
            {
                word *= vout_command;
                if (exponent >= 0)
                    word <<= exponent;
                else
                    word >>= -exponent;
            }
            else
            {
                if (exponent >= 0)
                    word >>= exponent * 2;
                else
                    word <<= -exponent * 2;
                word = DIV_ROUND_CLOSEST(word,
                                         vout_command * 1000);
            }
            return word;
        case PMBUS_VIN_OV_WARN_LIMIT:
        case PMBUS_VIN_UV_FAULT_LIMIT:
        case PMBUS_VIRT_READ_VIN_MIN:
        case PMBUS_VIRT_READ_VIN_MAX:
        case PMBUS_VIRT_READ_IOUT_MAX:
            return -EINVAL;

        default:
            return word;
        }
    }

    return word;
}
}

static int mpc42013_read_word_data(struct i2c_client *client, int page,
                                   int phase, int reg)
{
    int reg_value = pmbus_read_word_data(client, page, phase, reg);
    if (reg_value < 0)
        return reg_value;
    int word;
    word = reg_value;
    switch (reg)
    {
    default:
        return word;
    }

    return reg_value;
}
static struct pmbus_driver_info mpc42013_info = {
    .pages = MPC42013_PAGE_NUM,
    .format[PSC_VOLTAGE_IN] = direct,
    .format[PSC_VOLTAGE_OUT] = vid,
    .format[PSC_TEMPERATURE] = linear,
    .format[PSC_CURRENT_OUT] = linear,
    .format[PSC_POWER] = linear,
    .func[0] = PMBUS_HAVE_VIN | PMBUS_HAVE_IIN | PMBUS_HAVE_STATUS_INPUT |
               PMBUS_HAVE_VOUT | PMBUS_HAVE_STATUS_VOUT | PMBUS_HAVE_IOUT |
               PMBUS_HAVE_STATUS_IOUT | PMBUS_HAVE_PIN | PMBUS_HAVE_POUT |
               PMBUS_HAVE_TEMP | PMBUS_HAVE_STATUS_TEMP,
    .read_word_data = mpc42013_read_word_data,
    .read_byte_data = mpc42013_read_byte_data,
};
static const struct i2c_device_id mpc42013_id[] = {
    {"mpc42013", mpc42013},
};
MODULE_DEVICE_TABLE(i2c, mpc42013_id);

static int mpc42013_probe(struct i2c_client *client)
{
    struct device *dev = &client->dev;
    struct pmbus_driver_info *info;
    int ret;

    /* 1. 先 clone 一份 capability table */
    info = devm_kmemdup(dev, &mpc42013_info,
                        sizeof(struct pmbus_driver_info), GFP_KERNEL);
    info->m[PSC_VOLTAGE_IN] = 125;
    info->b[PSC_VOLTAGE_IN] = 0;
    info->R[PSC_VOLTAGE_IN] = 0;
    if (!info)
        return -ENOMEM;

    /* 3A. 若「一定要」保證 PAGE=0 ➜ 改用 i2c_smbus_* (簡單) */
    ret = i2c_smbus_write_byte_data(client, PMBUS_PAGE, 0x00);
    if (ret)
        dev_warn(dev, "Unable to set PAGE=0 (%d), ignore\n", ret);

    /* ---- 或 ---- */
    /* 3B. 仍想用 pmbus API，就放到這裡呼叫:
     *     pmbus_write_byte_data(client, 0, PMBUS_PAGE, 0);
     *   (此時 driver_info 已存在，可安全存取)
     */
    dev_info(dev, "%s probed (pages=%d, func0=0x%08x)\n", client->name,
             info->pages, info->func[0]);
    ret = pmbus_do_probe(client, info);
    if (ret)
        return ret;
    dev_info(dev, "%s: pages=%d, func0=0x%08x\n", client->name, info->pages,
             info->func[0]);
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

MODULE_AUTHOR("Your Name <>");
MODULE_DESCRIPTION("PMBus driver for MPC42013");
MODULE_LICENSE("GPL");
