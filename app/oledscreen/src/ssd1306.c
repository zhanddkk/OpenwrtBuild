#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "log.h"
#include "ssd1306.h"

#define I2C_DEV_ADDR        0x3c
#define I2C_DEV             "/dev/i2c-0"

#define SSD1306_DATA        0x40
#define SSD1306_COMMAND     0x80

#define SSD1306_MEM_DEBUG   0

struct ssd1306_array
{
    uint8_t type;
    uint8_t data[0];
};

struct ctrl {
    int fd;
};

static struct ctrl _ctrl;
#if SSD1306_MEM_DEBUG == 1
static int _mem_cnt = 0;
#endif
static struct ssd1306_array *_ssd1306_alloc_array(uint32_t len, uint8_t type)
{
    struct ssd1306_array *array;

    array = (struct ssd1306_array *)malloc(sizeof(struct ssd1306_array) + len);
    if (!array) {
        LOG_ERROR("malloc memory for ssd1306 i2c failed that type = %02X", type);
        return NULL;
    }

    array->type = type;
#if SSD1306_MEM_DEBUG == 1
    _mem_cnt++;
    LOG_INFO("==malloc== [%08X]@%p", _mem_cnt, array);
#endif
    return array;
}

static void _ssd1306_free_array(struct ssd1306_array *array)
{
#if SSD1306_MEM_DEBUG == 1
    _mem_cnt--;
    LOG_INFO("== free == [%08X]@%p", _mem_cnt, array);
#endif
    free(array);
}

static inline int _ssd1306_write_cmd(uint8_t cmd)
{
    struct ssd1306_array *array;
    struct i2c_rdwr_ioctl_data work_queue;
    struct i2c_msg msg; /* addr, flag(w: 0, r: 1), len, buf */
    int ret;

    array = _ssd1306_alloc_array(1, SSD1306_COMMAND);
    if (!array) {
        return -1;
    }

    array->data[0] = cmd;

    msg.addr = I2C_DEV_ADDR;
    msg.buf = (unsigned char *)array;
    msg.flags = 0;
    msg.len = sizeof(struct ssd1306_array) + 1; // type(1 byte) + cmd(1 byte)

    work_queue.nmsgs = 1;
    work_queue.msgs = &msg;

    ret = ioctl(_ctrl.fd, I2C_RDWR, &work_queue);
    if (ret < 0) {
        LOG_ERROR("write cmd(%02X) to i2c(%02X) failed", cmd, I2C_DEV_ADDR);
    } else {
        ret = 0;
    }
    _ssd1306_free_array(array);

    return ret;
}

static int _ssd1306_write_array(struct ssd1306_array *array, uint32_t len)
{
    int ret;

    len += sizeof(struct ssd1306_array);
    struct i2c_rdwr_ioctl_data work_queue;
    struct i2c_msg msg[] = {
        /* addr, flag(w: 0, r: 1), len, buf */
        {I2C_DEV_ADDR, 0, len, (unsigned char *)array},
    };

    work_queue.nmsgs = sizeof(msg) / sizeof(struct i2c_msg);
    work_queue.msgs = msg;

    ret = ioctl(_ctrl.fd, I2C_RDWR, &work_queue);
    if (ret < 0) {
        LOG_ERROR("write array to i2c(%02X) failed", I2C_DEV_ADDR);
    } else {
        ret = 0;
    }

    return ret;
}

static int _i2c_init(void)
{
    int ret;
    _ctrl.fd = open(I2C_DEV, O_RDWR);
    if (_ctrl.fd < 0) {
        LOG_ERROR("open %s failed", I2C_DEV);
        ret = -1;
    } else {
        ret = 0;
    }
    return ret;
}

int ssd1306_init(void)
{
    int ret = _i2c_init();
    if (ret == 0) {
        const uint8_t _init_code[] = {
            0xae,  // --turn off oled panel
            0xc8,  // --set COM output scan direction that is from COM[N-1] to COM0
            0x00,  // --set low column address
            0x10,  // --set high column address
            0x40,  // --set start line address
            0x81,  // --set contrast control register
            0xcf,
            0xa1,  // --set segment re-map 95 to 0
            0xa6,  // --set normal display
            0xa8,  // --set multiplex ratio(1 to 64)
            0x3f,  // --1/64 duty
            0xd3,  // --set display offset
            0x00,  // --not offset
            0xd5,  // --set display clock divide ratio/oscillator frequency
            0x80,  // --set divide ratio
            0xd9,  // --set pre-charge period
            0xf1,
            0xda,  // --set com pins hardware configuration
            0x12,
            0xdb,  // --set vcomh
            0x40,
            0x8d,  // --set Charge Pump enable/disable
            0x14,  // --set(0x10) disable
            0xaf,  // --turn on oled panel
        };
        const int count = sizeof(_init_code);

        for (int i = 0; i < count; i++) {
            ret = _ssd1306_write_cmd(_init_code[i]);
            if (ret < 0) {
                break;
            }
        }
    }
    
    if (ret < 0) {
        ssd1306_dinit();
        LOG_ERROR("init for ssd1306 failed");
    }
    
    return ret;
}

void ssd1306_dinit(void)
{
    if (_ctrl.fd > 0) {
        close(_ctrl.fd);
        _ctrl.fd = -1;
    }
}

void ssd1306_clear(int mode)
{
    int len = SSD1306_WIDTH;
    struct ssd1306_array *array = _ssd1306_alloc_array(len, SSD1306_DATA);
    if (array) {
        memset(array->data, mode? 0xff : 0, len);
        for (uint8_t row = 0; row < SSD1306_HEIGHT; row++) {
            _ssd1306_write_cmd(0xb0 | row);
            _ssd1306_write_cmd(0x00);
            _ssd1306_write_cmd(0x10);
            _ssd1306_write_array(array, len);
        }
        _ssd1306_free_array(array);
    } else {
        LOG_ERROR("clear failed");
    }
}

void ssd1306_draw_bitmap(uint8_t column, uint8_t row, uint8_t w, uint8_t h, const uint8_t *data, int mode)
{
    int len = w;
    struct ssd1306_array *array;
    if (column >= SSD1306_WIDTH) return;
    if (row >= SSD1306_HEIGHT) return;
    if (column + len > SSD1306_WIDTH) {
        len = SSD1306_WIDTH - column;
    }
    array = _ssd1306_alloc_array(len, SSD1306_DATA);
    if (array) {
        for (uint8_t y = 0; y < h; y++) {
            uint8_t vy = row + y;
            if (vy < 8) {
                uint8_t vx = column;
                int offset = y * w;
                _ssd1306_write_cmd(0xb0 | vy);
                _ssd1306_write_cmd(vx & 0x0f);
                _ssd1306_write_cmd(0x10 | vx >> 4);
                if (mode) {
                    const uint8_t *pd = &data[offset];
                    for (int i = 0; i < len; i++) {
                        array->data[i] = ~pd[i];
                    }
                } else {
                    memcpy(array->data, &data[offset], len);
                }
                _ssd1306_write_array(array, len);
            } else {
                LOG_INFO("out of view vy=%d", vy);
                break;
            }
        }
        _ssd1306_free_array(array);
    }
}
