#include "config.h"

#include "mihawk-cpld.hpp"

#include "utility.hpp"
#include "gpio.hpp"
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

// i2c bus & i2c slave address of Mihawk's CPLD
#define busId 11
#define slaveAddr 0x40

// SMLink Status Register(Interrupt-control-bit Register)
const static constexpr size_t StatusReg_1 = 0x20;

// SMLink Status Register(Power-on error code Register)
const static constexpr size_t StatusReg_2 = 0x21;

// SMLink Status Register(Power-ready error code Register)
const static constexpr size_t StatusReg_3 = 0x22;

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
    // read CPLD_register error code to analyze and report the error event.
    if (poweronError)
    {
        int errorcode;
        errorcode = readFromCPLDErrorCode(StatusReg_2);
        switch (errorcode)
        {
            case ErrorCode_0:
                report<ErrorCode0>();
                break;
            case ErrorCode_1:
                report<ErrorCode1>();
                break;
            case ErrorCode_2:
                report<ErrorCode2>();
                break;
            case ErrorCode_3:
                report<ErrorCode3>();
                break;
            case ErrorCode_4:
                report<ErrorCode4>();
                break;
            case ErrorCode_5:
                report<ErrorCode5>();
                break;
            case ErrorCode_6:
                report<ErrorCode6>();
                break;
            case ErrorCode_7:
                report<ErrorCode7>();
                break;
            case ErrorCode_8:
                report<ErrorCode8>();
                break;
            case ErrorCode_9:
                report<ErrorCode9>();
                break;
            case ErrorCode_10:
                report<ErrorCode10>();
                break;
            case ErrorCode_11:
                report<ErrorCode11>();
                break;
            case ErrorCode_12:
                report<ErrorCode12>();
                break;
            case ErrorCode_13:
                report<ErrorCode13>();
                break;
            case ErrorCode_14:
                report<ErrorCode14>();
                break;
            case ErrorCode_15:
                report<ErrorCode15>();
                break;
            case ErrorCode_16:
                report<ErrorCode16>();
                break;
            case ErrorCode_17:
                report<ErrorCode17>();
                break;
            case ErrorCode_18:
                report<ErrorCode18>();
                break;
            case ErrorCode_19:
                report<ErrorCode19>();
                break;
            case ErrorCode_20:
                report<ErrorCode20>();
                break;
            case ErrorCode_21:
                report<ErrorCode21>();
                break;
            case ErrorCode_22:
                report<ErrorCode22>();
                break;
            case ErrorCode_23:
                report<ErrorCode23>();
                break;
            case ErrorCode_24:
                report<ErrorCode24>();
                break;
            case ErrorCode_25:
                report<ErrorCode25>();
                break;
            case ErrorCode_26:
                report<ErrorCode26>();
                break;
            case ErrorCode_27:
                report<ErrorCode27>();
                break;
            case ErrorCode_28:
                report<ErrorCode28>();
                break;
            case ErrorCode_29:
                report<ErrorCode29>();
                break;
            case ErrorCode_30:
                report<ErrorCode30>();
                break;
            case ErrorCode_31:
                report<ErrorCode31>();
                break;
            case ErrorCode_32:
                report<ErrorCode32>();
                break;
            case ErrorCode_33:
                report<ErrorCode33>();
                break;
            case ErrorCode_34:
                report<ErrorCode34>();
                break;
            case ErrorCode_35:
                report<ErrorCode35>();
                break;
            case ErrorCode_36:
                report<ErrorCode36>();
                break;
            default:
                break;
        }
        clearCPLDregister();
    }
}

void MIHAWKCPLD::analyze()
{
    bool powerreadyError = checkPowerreadyFault();
    
    //Use the function of GPIO class to check
    //GPIOF0(CPLD uses).
    using namespace witherspoon::gpio;
    GPIO gpio{"/dev/gpiochip0", static_cast<gpioNum_t>(40),
              Direction::input};

    //Check GPIOFO pin whther is switched off.
    //if GPIOF0 has been switched off, 
    //check CPLD's errorcode & report error.
    if (gpio.read() == Value::low )
    {
        //Check CPLD's powerready_errorcode register
        //whther is updated.
        if (powerreadyError)
        {
            int errorcode;
            errorcode = readFromCPLDErrorCode(StatusReg_3);

            //If CPLD's error is detected to 0,
            //report the error of reading CPLD register
            //fail.
            if (errorcode == ErrorCode_0)
            {
                report<ErrorCode0>();
            }
            else if (errorcodeMask != errorcode)
            {
                //Check the errorcodeMask & errorcode whether
                //are the same value to avoid to report the 
                //same error again.
                switch (errorcode)
                {
                    case ErrorCode_1:
                        report<ErrorCode1>();
                        errorcodeMask = ErrorCode_1;
                        break;
                    case ErrorCode_2:
                        report<ErrorCode2>();
                        errorcodeMask = ErrorCode_2;
                        break;
                    case ErrorCode_3:
                        report<ErrorCode3>();
                        errorcodeMask = ErrorCode_3;
                        break;
                    case ErrorCode_4:
                        report<ErrorCode4>();
                        errorcodeMask = ErrorCode_4;
                        break;
                    case ErrorCode_5:
                        report<ErrorCode5>();
                        errorcodeMask = ErrorCode_5;
                        break;
                    case ErrorCode_6:
                        report<ErrorCode6>();
                        errorcodeMask = ErrorCode_6;
                        break;
                    case ErrorCode_7:
                        report<ErrorCode7>();
                        errorcodeMask = ErrorCode_7;
                        break;
                    case ErrorCode_8:
                        report<ErrorCode8>();
                        errorcodeMask = ErrorCode_8;
                        break;
                    case ErrorCode_9:
                        report<ErrorCode9>();
                        errorcodeMask = ErrorCode_9;
                        break;
                    case ErrorCode_10:
                        report<ErrorCode10>();
                        errorcodeMask = ErrorCode_10;
                        break;
                    case ErrorCode_11:
                        report<ErrorCode11>();
                        errorcodeMask = ErrorCode_11;
                        break;
                    case ErrorCode_12:
                        report<ErrorCode12>();
                        errorcodeMask = ErrorCode_12;
                        break;
                    case ErrorCode_13:
                        report<ErrorCode13>();
                        errorcodeMask = ErrorCode_13;
                        break;
                    case ErrorCode_14:
                        report<ErrorCode14>();
                        errorcodeMask = ErrorCode_14;
                        break;
                    case ErrorCode_15:
                        report<ErrorCode15>();
                        errorcodeMask = ErrorCode_15;
                        break;
                    case ErrorCode_16:
                        report<ErrorCode16>();
                        errorcodeMask = ErrorCode_16;
                        break;
                    case ErrorCode_17:
                        report<ErrorCode17>();
                        errorcodeMask = ErrorCode_17;
                        break;
                    case ErrorCode_18:
                        report<ErrorCode18>();
                        errorcodeMask = ErrorCode_18;
                        break;
                    case ErrorCode_19:
                        report<ErrorCode19>();
                        errorcodeMask = ErrorCode_19;
                        break;
                    case ErrorCode_20:
                        report<ErrorCode20>();
                        errorcodeMask = ErrorCode_20;
                        break;
                    case ErrorCode_21:
                        report<ErrorCode21>();
                        errorcodeMask = ErrorCode_21;
                        break;
                    case ErrorCode_22:
                        report<ErrorCode22>();
                        errorcodeMask = ErrorCode_22;
                        break;
                    case ErrorCode_23:
                        report<ErrorCode23>();
                        errorcodeMask = ErrorCode_23;
                        break;
                    case ErrorCode_24:
                        report<ErrorCode24>();
                        errorcodeMask = ErrorCode_24;
                        break;
                    case ErrorCode_25:
                        report<ErrorCode25>();
                        errorcodeMask = ErrorCode_25;
                        break;
                    case ErrorCode_26:
                        report<ErrorCode26>();
                        errorcodeMask = ErrorCode_26;
                        break;
                    case ErrorCode_27:
                        report<ErrorCode27>();
                        errorcodeMask = ErrorCode_27;
                        break;
                    case ErrorCode_28:
                        report<ErrorCode28>();
                        errorcodeMask = ErrorCode_28;
                        break;
                    case ErrorCode_29:
                        report<ErrorCode29>();
                        errorcodeMask = ErrorCode_29;
                        break;
                    case ErrorCode_30:
                        report<ErrorCode30>();
                        errorcodeMask = ErrorCode_30;
                        break;
                    case ErrorCode_31:
                        report<ErrorCode31>();
                        errorcodeMask = ErrorCode_31;
                        break;
                    case ErrorCode_32:
                        report<ErrorCode32>();
                        errorcodeMask = ErrorCode_32;
                        break;
                    case ErrorCode_33:
                        report<ErrorCode33>();
                        errorcodeMask = ErrorCode_33;
                        break;
                    case ErrorCode_34:
                        report<ErrorCode34>();
                        errorcodeMask = ErrorCode_34;
                        break;
                    case ErrorCode_35:
                        report<ErrorCode35>();
                        errorcodeMask = ErrorCode_35;
                        break;
                    case ErrorCode_36:
                        report<ErrorCode36>();
                        errorcodeMask = ErrorCode_36;
                        break;
                    default:
                        break;
                }
            clearCPLDregister();
            }
        }
    }

    if (gpio.read() == Value::high)
    {
            //If there isn't an error(GPIOF0
            //which CPLD uses is switched on),
            //we clear errorcodeMask.
            errorcodeMask = 0;
    }
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

    // clear CPLD_register by writing 0x01 to it.
    if ((i2c_smbus_write_byte_data(fd, StatusReg_1, 0x01)) < 0)
    {
        std::cerr << "i2c_smbus_write_byte_data failed \n";
    }
    close(fd);
}

} // namespace power
} // namespace witherspoon
