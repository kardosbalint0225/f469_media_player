/*
 * Copyright (C) 2017 HAW-Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @ingroup     drivers_mtd_sdcard
 * @{
 *
 * @file
 * @brief       Driver for using sdcard_spi via mtd interface
 *
 * @author      Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * @}
 */
//#define ENABLE_DEBUG 0
#include "debug.h"
//#include "kernel_defines.h"
#include "macros/utils.h"
#include "mtd.h"
#include "mtd_sdcard.h"
#include "sdcard.h"
#include "sdcard_config.h"

#include <inttypes.h>
#include <errno.h>
#include <string.h>

static int mtd_sdcard_init(mtd_dev_t *dev)
{
    debug("mtd_sdcard_init\r\n");

    //TODO: dev->work_area = pvPortMalloc(SDCARD_SDHC_BLOCK_SIZE); ?

    if (0 == sdcard_init())
    {
        /* erasing whole sectors is handled internally by the card so you can
           delete single blocks (i.e. pages) */
        dev->pages_per_sector = 1;
        dev->sector_count     = (uint32_t)(sdcard_get_capacity() / SDCARD_SDHC_BLOCK_SIZE);

        /* sdcard always uses the fixed block size of SD-HC cards */
        dev->page_size        = SDCARD_SDHC_BLOCK_SIZE;
        dev->write_size       = SDCARD_SDHC_BLOCK_SIZE;
        return 0;
    }
    return -EIO;
}

static int mtd_sdcard_read_page(mtd_dev_t *dev, void *buff, uint32_t page, uint32_t offset, uint32_t size)
{
    int err;

    debug("mtd_sdcard_read_page: page:%" PRIu32 " offset:%" PRIu32 " size:%" PRIu32 "\r\n", page, offset, size);

    if (offset || size % SDCARD_SDHC_BLOCK_SIZE)
    {
        if (NULL == dev->work_area)
        {
            debug("mtd_sdcard_read_page: no work area\r\n");
            return -ENOTSUP;
        }

        err = sdcard_read_blocks(page, 1, dev->work_area);
        if (0 != err)
        {
            return -EIO;
        }

        size = MIN(size, SDCARD_SDHC_BLOCK_SIZE - offset);
        debug("mtd_sdcard_read_page: read %" PRIu32 " bytes at offset %" PRIu32 "\r\n", size, offset);
        memcpy(buff, (uint8_t *)dev->work_area + offset, size);
        return size;
    }

    err = sdcard_read_blocks(page, size / SDCARD_SDHC_BLOCK_SIZE, buff);
    if (0 != err)
    {
        return -EIO;
    }

    return size;
}

static int mtd_sdcard_write_page(mtd_dev_t *dev, const void *buff, uint32_t page, uint32_t offset, uint32_t size)
{
    int err;

    debug("mtd_sdcard_write_page: page:%" PRIu32 " offset:%" PRIu32 " size:%" PRIu32 "\r\n", page, offset, size);

    if (offset || size % SDCARD_SDHC_BLOCK_SIZE)
    {
        if (NULL == dev->work_area)
        {
            debug("mtd_sdcard_write_page: no work area\r\n");
            return -ENOTSUP;
        }

        err = sdcard_read_blocks(page, 1, dev->work_area);
        if (0 != err)
        {
            return -EIO;
        }

        size = MIN(size, SDCARD_SDHC_BLOCK_SIZE - offset);
        debug("mtd_sdcard_write_page: write %" PRIu32 " bytes at offset %" PRIu32 "\r\n", size, offset);
        memcpy((uint8_t *)dev->work_area + offset, buff, size);
        err = sdcard_write_blocks(page, 1, dev->work_area);
    }
    else
    {
        err = sdcard_write_blocks(page, size / SDCARD_SDHC_BLOCK_SIZE, buff);
    }

    if (0 != err)
    {
        debug("mtd_sdcard_write_page: error %d\r\n", err);
        return -EIO;
    }
    return size;
}

static int mtd_sdcard_erase_sector(mtd_dev_t *dev, uint32_t sector, uint32_t count)
{
    debug("mtd_sdcard_erase_sector: sector: %" PRIu32 " count: %" PRIu32 "\r\n", sector, count);

    if (NULL == dev->work_area)
    {
        debug("mtd_sdcard_erase_sector: no work area\r\n");
        return -ENOTSUP;
    }

    memset(dev->work_area, 0, SDCARD_SDHC_BLOCK_SIZE);

    while (count)
    {
        int err;
        err = sdcard_write_blocks(sector, 1, dev->work_area);
        if (0 != err)
        {
            debug("mtd_sdcard_erase_sector: error %d\r\n", err);
            return -EIO;
        }
        --count;
        ++sector;
    }

    return 0;
}

static int mtd_sdcard_power(mtd_dev_t *dev, enum mtd_power_state power)
{
    (void)dev;
    (void)power;

    /* TODO: implement power down of sdcard in sdcard_spi
    (make use of sdcard_spi_params_t.power pin) */
    return -ENOTSUP; /* currently not supported */
}

static int mtd_sdcard_read(mtd_dev_t *dev, void *buff, uint32_t addr,
                           uint32_t size)
{
    int res = mtd_sdcard_read_page(dev, buff, addr / SDCARD_SDHC_BLOCK_SIZE,
                                   addr % SDCARD_SDHC_BLOCK_SIZE, size);
    if (res < 0) {
        return res;
    }
    if (res == (int)size) {
        return 0;
    }
    return -EOVERFLOW;
}

static int mtd_sdcard_write(mtd_dev_t *dev, const void *buff, uint32_t addr,
                            uint32_t size)
{
    int res =  mtd_sdcard_write_page(dev, buff, addr / SDCARD_SDHC_BLOCK_SIZE,
                                     addr % SDCARD_SDHC_BLOCK_SIZE, size);
    if (res < 0) {
        return res;
    }
    if (res == (int)size) {
        return 0;
    }
    return -EOVERFLOW;
}

const mtd_desc_t mtd_sdcard_driver = {
    .init = mtd_sdcard_init,
    .read = mtd_sdcard_read,
    .read_page = mtd_sdcard_read_page,
    .write = mtd_sdcard_write,
    .write_page = mtd_sdcard_write_page,
    .erase_sector = mtd_sdcard_erase_sector,
    .power = mtd_sdcard_power,
};
