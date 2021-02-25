#include "config.h"

#include "mihawk-cpld.hpp"

#include "gpio.hpp"
#include "utility.hpp"

#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <elog-errors.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <org/open_power/Witherspoon/Fault/error.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>
#include <string>
#include <xyz/openbmc_project/Common/Device/error.hpp>

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
}

// i2c bus & i2c slave address of Mihawk/Mowgli's CPLD
#ifdef MOWGLICPLD_DEVICE_ACCESS
#define busId 3
#else
#define busId 11
#endif
#define slaveAddr 0x40

// SMLink Status Register(PSU status Register)
const static constexpr size_t StatusReg_0 = 0x05;

// SMLink Status Register(Interrupt-control-bit Register)
const static constexpr size_t StatusReg_1 = 0x20;

// SMLink Status Register(Power-on error code Register)
const static constexpr size_t StatusReg_2 = 0x21;

// SMLink Status Register(Power-ready error code Register)
const static constexpr size_t StatusReg_3 = 0x22;

// SMLink Status Register(Mowgli's HDD 0~7 fault status Register)
const static constexpr size_t StatusReg_4 = 0x40;

// SMLink Status Register(Mowgli's HDD 0~7 rebuild status Register)
const static constexpr size_t StatusReg_5 = 0x43;

using namespace std;
namespace witherspoon
{
namespace power
{
const auto DEVICE_NAME = "MIHAWKCPLD"s;

namespace fs = std::filesystem;
using namespace phosphor::logging;

using namespace sdbusplus::org::open_power::Witherspoon::Fault::Error;

MIHAWKCPLD::MIHAWKCPLD(size_t instance, sdbusplus::bus::bus& bus) :
    Device(DEVICE_NAME, instance), bus(bus)
{
}

void MIHAWKCPLD::onFailure()
{
    bool poweronError = checkPoweronFault();

    // If the interrupt of power_on_error is switch on,
    // read CPLD_register error code to analyze
    // and report the error log event.
    if (poweronError)
    {
        ErrorCode code;
        code = static_cast<ErrorCode>(readFromCPLDErrorCode(StatusReg_2));

        switch (code)
        {
            case ErrorCode::_1:
                report<ErrorCode1>();
                break;
            case ErrorCode::_2:
                report<ErrorCode2>();
                break;
            case ErrorCode::_3:
                report<ErrorCode3>();
                break;
            case ErrorCode::_4:
                report<ErrorCode4>();
                break;
            case ErrorCode::_5:
                report<ErrorCode5>();
                break;
            case ErrorCode::_6:
                report<ErrorCode6>();
                break;
            case ErrorCode::_7:
                report<ErrorCode7>();
                break;
            case ErrorCode::_8:
                report<ErrorCode8>();
                break;
            case ErrorCode::_9:
                report<ErrorCode9>();
                break;
            case ErrorCode::_10:
                report<ErrorCode10>();
                break;
            case ErrorCode::_11:
                report<ErrorCode11>();
                break;
            case ErrorCode::_12:
                report<ErrorCode12>();
                break;
            case ErrorCode::_13:
                report<ErrorCode13>();
                break;
            case ErrorCode::_14:
                report<ErrorCode14>();
                break;
            case ErrorCode::_15:
                report<ErrorCode15>();
                break;
            case ErrorCode::_16:
                report<ErrorCode16>();
                break;
            case ErrorCode::_17:
                report<ErrorCode17>();
                break;
            case ErrorCode::_18:
                report<ErrorCode18>();
                break;
            case ErrorCode::_19:
                report<ErrorCode19>();
                break;
            case ErrorCode::_20:
                report<ErrorCode20>();
                break;
            case ErrorCode::_21:
                report<ErrorCode21>();
                break;
            case ErrorCode::_22:
                report<ErrorCode22>();
                break;
            case ErrorCode::_23:
                report<ErrorCode23>();
                break;
            case ErrorCode::_24:
                report<ErrorCode24>();
                break;
            case ErrorCode::_25:
                report<ErrorCode25>();
                break;
            case ErrorCode::_26:
                report<ErrorCode26>();
                break;
            case ErrorCode::_27:
                report<ErrorCode27>();
                break;
            case ErrorCode::_28:
                report<ErrorCode28>();
                break;
            case ErrorCode::_29:
                report<ErrorCode29>();
                break;
            case ErrorCode::_30:
                report<ErrorCode30>();
                break;
            case ErrorCode::_31:
                report<ErrorCode31>();
                break;
            case ErrorCode::_32:
                report<ErrorCode32>();
                break;
            case ErrorCode::_33:
                report<ErrorCode33>();
                break;
            case ErrorCode::_34:
                report<ErrorCode34>();
                break;
            case ErrorCode::_35:
                report<ErrorCode35>();
                break;
            case ErrorCode::_36:
                report<ErrorCode36>();
                break;
#ifdef MOWGLICPLD_DEVICE_ACCESS
            case ErrorCode::_37:
                report<ErrorCode37>();
                break;
            case ErrorCode::_38:
                report<ErrorCode38>();
                break;
            case ErrorCode::_39:
                report<ErrorCode39>();
                break;
            case ErrorCode::_40:
                report<ErrorCode40>();
                break;
#endif
            default:
                // If the errorcode isn't 1~39(Mihawk:1~36),
                // it indicates that the CPLD register
                // has a reading issue,
                // so the errorcode0 error is reported.
                report<ErrorCode0>();
                break;
        }
        clearCPLDregister();
    }
}

void MIHAWKCPLD::analyze()
{
    bool powerreadyError = checkPowerreadyFault();

    // Use the function of GPIO class to check
    // GPIOF0(CPLD uses).
    using namespace witherspoon::gpio;
    GPIO gpio{"/dev/gpiochip0", static_cast<gpioNum_t>(40), Direction::input};

    // Check GPIOFO pin whether is switched off.
    // if GPIOF0 has been switched off,
    // check CPLD's errorcode & report error.
    if (gpio.read() == Value::low)
    {
        // If the interrupt of power_ready_error is switch on,
        // read CPLD_register error code to analyze and
        // report the error event.
        if (powerreadyError)
        {
            ErrorCode code;
            code = static_cast<ErrorCode>(readFromCPLDErrorCode(StatusReg_3));

            if (!errorcodeMask)
            {
                // Check the errorcodeMask & errorcode whether
                // are the same value to avoid to report the
                // same error again.
                switch (code)
                {
                    case ErrorCode::_1:
                        report<ErrorCode1>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_2:
                        report<ErrorCode2>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_3:
                        report<ErrorCode3>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_4:
                        report<ErrorCode4>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_5:
                        report<ErrorCode5>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_6:
                        report<ErrorCode6>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_7:
                        report<ErrorCode7>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_8:
                        report<ErrorCode8>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_9:
                        report<ErrorCode9>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_10:
                        report<ErrorCode10>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_11:
                        report<ErrorCode11>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_12:
                        report<ErrorCode12>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_13:
                        report<ErrorCode13>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_14:
                        report<ErrorCode14>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_15:
                        report<ErrorCode15>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_16:
                        report<ErrorCode16>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_17:
                        report<ErrorCode17>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_18:
                        report<ErrorCode18>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_19:
                        report<ErrorCode19>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_20:
                        report<ErrorCode20>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_21:
                        report<ErrorCode21>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_22:
                        report<ErrorCode22>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_23:
                        report<ErrorCode23>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_24:
                        report<ErrorCode24>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_25:
                        report<ErrorCode25>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_26:
                        report<ErrorCode26>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_27:
                        report<ErrorCode27>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_28:
                        report<ErrorCode28>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_29:
                        report<ErrorCode29>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_30:
                        report<ErrorCode30>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_31:
                        report<ErrorCode31>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_32:
                        report<ErrorCode32>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_33:
                        report<ErrorCode33>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_34:
                        report<ErrorCode34>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_35:
                        report<ErrorCode35>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_36:
                        report<ErrorCode36>();
                        errorcodeMask = 1;
                        break;
#ifdef MOWGLICPLD_DEVICE_ACCESS
                    case ErrorCode::_37:
                        report<ErrorCode37>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_38:
                        report<ErrorCode38>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_39:
                        report<ErrorCode39>();
                        errorcodeMask = 1;
                        break;
                    case ErrorCode::_40:
                        report<ErrorCode40>();
                        errorcodeMask = 1;
                        break;
#endif
                    default:
                        // If the errorcode is not 1~39(Mihawk:1~36),
                        // it indicates that the CPLD register
                        // has a reading issue, so the
                        // errorcode0 error is reported.
                        report<ErrorCode0>();
                        errorcodeMask = 1;
                        break;
                }
            }
            clearCPLDregister();
        }
    }

    if (gpio.read() == Value::high)
    {
        // If there isn't an error(GPIOF0
        // which CPLD uses is switched on),
        // we clear errorcodeMask.
        errorcodeMask = 0;
    }

#ifdef MOWGLICPLD_DEVICE_ACCESS
    if (!faultcodeMask)
    {
        HDDErrorCode code = checkHDDError(StatusReg_4);
        switch (code)
        {
            case HDDErrorCode::_0:
                report<HDDErrorCode0>();
                faultcodeMask = true;
                break;
            case HDDErrorCode::_1:
                report<HDDErrorCode1>();
                faultcodeMask = true;
                break;
            default:
                faultcodeMask = false;
                break;
        }
    }

    HDDRebuildCode code1 = checkHDDRebuild(StatusReg_5);
    switch (code1)
    {
        case HDDRebuildCode::_1:
            if (!rebuildcodeMask)
            {
                report<HDDRebuildCode1>();
                rebuildcodeMask = true;
            }
            break;
        case HDDRebuildCode::_2:
            if (!rebuildcodeMask)
            {
                report<HDDRebuildCode2>();
                rebuildcodeMask = true;
            }
            break;
        default:
            if (rebuildcodeMask)
            {
                report<HDDRebuildCode0>();
            }
            rebuildcodeMask = false;
            break;
    }
#endif
}

// Check for PoweronFault
bool MIHAWKCPLD::checkPoweronFault()
{
    bool result = 0;
    std::string i2cBus = "/dev/i2c-" + std::to_string(busId);

    // open i2c device(CPLD-PSU-register table)
    int fd = open(i2cBus.c_str(), O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        std::cerr << "Unable to open i2c device \n";
    }

    // set i2c slave address
    if (ioctl(fd, I2C_SLAVE_FORCE, slaveAddr) < 0)
    {
        std::cerr << "Unable to set device address \n";
        close(fd);
    }

    // check whether support i2c function
    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) < 0)
    {
        std::cerr << "Not support I2C_FUNCS \n";
        close(fd);
    }

    // check whether support i2c-read function
    if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA))
    {
        std::cerr << "Not support I2C_FUNC_SMBUS_READ_BYTE_DATA \n";
        close(fd);
    }

    int statusValue_1;

    statusValue_1 = i2c_smbus_read_byte_data(fd, StatusReg_1);
    close(fd);

    if (statusValue_1 < 0)
    {
        std::cerr << "i2c_smbus_read_byte_data failed \n";
        result = 0;
    }

    if ((statusValue_1 >> 5) & 1)
    {
        // If power_on-interrupt-bit is read as 1,
        // switch on the flag.
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}

// Read CPLD_register error code and return the result to analyze.
int MIHAWKCPLD::readFromCPLDErrorCode(int statusReg)
{
    std::string i2cBus = "/dev/i2c-" + std::to_string(busId);

    // open i2c device(CPLD-PSU-register table)
    int fd = open(i2cBus.c_str(), O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        std::cerr << "Unable to open i2c device(CPLD_register) \n";
    }

    // set i2c slave address
    if (ioctl(fd, I2C_SLAVE_FORCE, slaveAddr) < 0)
    {
        std::cerr << "Unable to set device address \n";
        return 0;
    }

    // check whether support i2c function
    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) < 0)
    {
        std::cerr << "Not support I2C_FUNCS \n";
        return 0;
    }

    // check whether support i2c-read function
    if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA))
    {
        std::cerr << "Not support I2C_FUNC_SMBUS_READ_BYTE_DATA \n";
        return 0;
    }

    int statusValue_2;
    statusValue_2 = i2c_smbus_read_byte_data(fd, statusReg);
    close(fd);

    if (statusValue_2 < 0)
    {
        statusValue_2 = 0;
    }

    // return the i2c-read data
    return statusValue_2;
}

// Check PSU_DC_PGOOD state form PSU register via CPLD
bool MIHAWKCPLD::checkPowerreadyFault()
{
    bool result;
    std::string i2cBus = "/dev/i2c-" + std::to_string(busId);

    // open i2c device(CPLD-PSU-register table)
    int fd = open(i2cBus.c_str(), O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        std::cerr << "Unable to open i2c device \n";
    }

    // set i2c slave address
    if (ioctl(fd, I2C_SLAVE_FORCE, slaveAddr) < 0)
    {
        std::cerr << "Unable to set device address \n";
        return 0;
    }

    // check whether support i2c function
    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) < 0)
    {
        std::cerr << "Not support I2C_FUNCS \n";
        return 0;
    }

    // check whether support i2c-read function
    if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA))
    {
        std::cerr << "Not support I2C_FUNC_SMBUS_READ_BYTE_DATA \n";
        return 0;
    }

    int statusValue_3;
    statusValue_3 = i2c_smbus_read_byte_data(fd, StatusReg_1);
    close(fd);

    if (statusValue_3 < 0)
    {
        std::cerr << "i2c_smbus_read_byte_data failed \n";
        result = 0;
    }

    if ((statusValue_3 >> 6) & 1)
    {
        // If power_ready-interrupt-bit is read as 1,
        // switch on the flag.
        result = 1;
    }
    else
    {
        result = 0;
    }

    // return the i2c-read data
    return result;
}

// Clear CPLD_register after reading.
void MIHAWKCPLD::clearCPLDregister()
{
    std::string i2cBus = "/dev/i2c-" + std::to_string(busId);

    // open i2c device(CPLD-PSU-register table)
    int fd = open(i2cBus.c_str(), O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        std::cerr << "Unable to open i2c device \n";
    }

    // set i2c slave address
    if (ioctl(fd, I2C_SLAVE_FORCE, slaveAddr) < 0)
    {
        std::cerr << "Unable to set device address \n";
        close(fd);
    }

    // check whether support i2c function
    unsigned long funcs = 0;
    if (ioctl(fd, I2C_FUNCS, &funcs) < 0)
    {
        std::cerr << "Not support I2C_FUNCS \n";
        close(fd);
    }

    // check whether support i2c-write function
    if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA))
    {
        std::cerr << "Not support I2C_FUNC_SMBUS_WRITE_BYTE_DATA \n";
        close(fd);
    }

    // check psu pgood status.
    int checkpsu = i2c_smbus_read_byte_data(fd, StatusReg_0);

    // check one of both psus pgood status before clear CPLD_register.
    if(((checkpsu >> 1) & 1) || ((checkpsu >> 2) & 1))
    {
        // clear CPLD_register by writing 0x01 to it.
        if ((i2c_smbus_write_byte_data(fd, StatusReg_1, 0x01)) < 0)
        {
            std::cerr << "i2c_smbus_write_byte_data failed \n";
        }
    }
    close(fd);
}

// Check for Mowgli's HDDFault status
MIHAWKCPLD::HDDErrorCode MIHAWKCPLD::checkHDDError(int statusReg)
{
    if ((readFromCPLDErrorCode(statusReg)) & 1)
    {
        return HDDErrorCode::_0;
    }
    else if (((readFromCPLDErrorCode(statusReg)) >> 1) & 1)
    {
        return HDDErrorCode::_1;
    }
    else
    {
        return static_cast<HDDErrorCode>(0);
    }
}

// Check for Mowgli's HDDRebuild status
MIHAWKCPLD::HDDRebuildCode MIHAWKCPLD::checkHDDRebuild(int statusReg)
{
    if ((readFromCPLDErrorCode(statusReg)) & 1)
    {
        return HDDRebuildCode::_1;
    }
    else if (((readFromCPLDErrorCode(statusReg)) >> 1) & 1)
    {
        return HDDRebuildCode::_2;
    }
    else
    {
        return HDDRebuildCode::_0;
    }
}

} // namespace power
} // namespace witherspoon
