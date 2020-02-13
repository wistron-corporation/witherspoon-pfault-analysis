#pragma once

#include "device.hpp"
#include "pmbus.hpp"

#include <algorithm>
#include <filesystem>
#include <sdbusplus/bus.hpp>

namespace witherspoon
{
namespace power
{

/**
 * @class MIHAWKCPLD
 *
 * This class implements fault analysis for Mihawk's CPLD
 * power sequencer device.
 *
 */
class MIHAWKCPLD : public Device
{
  public:
    MIHAWKCPLD() = delete;
    ~MIHAWKCPLD() = default;
    MIHAWKCPLD(const MIHAWKCPLD&) = delete;
    MIHAWKCPLD& operator=(const MIHAWKCPLD&) = delete;
    MIHAWKCPLD(MIHAWKCPLD&&) = default;
    MIHAWKCPLD& operator=(MIHAWKCPLD&&) = default;

    /**
     * Constructor
     *
     * @param[in] instance - the device instance number
     * @param[in] bus - D-Bus bus object
     */
    MIHAWKCPLD(size_t instance, sdbusplus::bus::bus& bus);

    /**
     * Analyzes the device for errors when the device is
     * known to be in an error state.  A log will be created.
     */
    void onFailure() override;

    /**
     * Checks the device for errors and only creates a log
     * if one is found.
     */
    void analyze() override;

    /**
     * Clears faults in the device
     */
    void clearFaults() override
    {
    }

  private:
    /**
     * If checkPoweronFault() or checkPowerreadyFault()
     * returns "true", use readFromCPLDErrorCode() 
     * to read CPLD-error-code-register
     * to analyze the fail reason.
     *
     * @param[in] statusReg - I2C's statusReg, slaveAddr
     * offset. 
     * ex.Mihawk's CPLD-register is on slaveAddr ox40 of
     * i2c-11, but poweron_errcode-register is on slaveAddr
     * offset 0x21, power_ready-errorcode-register is on 
     * slaveAddr offset 0x22.
     *
     * @return int - the error-code value which is read on
     * CPLD-error-code-register.
     */
    int readFromCPLDErrorCode(int statusReg);

    /**
     * Checks for PoweronFault on Mihawk's
     * CPLD-power_on-error-interrupt-bit-register
     * whether is transfered to "1".
     *
     * @return bool - true if power_on fail.
     */
    bool checkPoweronFault();

    /**
     * Clear CPLD intrupt record after reading CPLD_register.
     */
    void clearCPLDregister();

    /**
     * Check for PowerreadyFault on Mihawk's
     * CPLD-power_ready-error-interrupt-bit-register
     * whether is transfered to "1".
     *
     * @return bool - true if power_ready fail.
     */
    bool checkPowerreadyFault();

    /**
     * The D-Bus bus object
     */
    sdbusplus::bus::bus& bus;

    /**
     * The parameter which is checked CPLD's the same error 
     * whether is created again.
     */
    int errorcodeMask;

    /**
     * All of powerOnErrorcode are the definition of error-code
     * which are read on CPLD-error-code-register. 
     */
    /**
     * The definition of error-code:
     * Read CPLD's errorcode_register fail.
     */
    static const int ErrorCode_0 = 0;

    /**
     * The definition of error-code:
     * PSU1_PGOOD fail.
     */
    static const int ErrorCode_1 = 1;

    /**
     * The definition of error-code:
     * PSU0_PGOOD fail.
     */
    static const int ErrorCode_2 = 2;

    /**
     * The definition of error-code:
     * 240Va_Fault_A fail.
     */
    static const int ErrorCode_3 = 3;

    /**
     * The definition of error-code:
     * 240Va_Fault_B fail.
     */
    static const int ErrorCode_4 = 4;

    /**
     * The definition of error-code:
     * 240Va_Fault_C fail.
     */
    static const int ErrorCode_5 = 5;

    /**
     * The definition of error-code:
     * 240Va_Fault_D fail.
     */
    static const int ErrorCode_6 = 6;

    /**
     * The definition of error-code:
     * 240Va_Fault_E fail.
     */
    static const int ErrorCode_7 = 7;

    /**
     * The definition of error-code:
     * 240Va_Fault_F fail.
     */
    static const int ErrorCode_8 = 8;

    /**
     * The definition of error-code:
     * 240Va_Fault_G fail.
     */
    static const int ErrorCode_9 = 9;

    /**
     * The definition of error-code:
     * 240Va_Fault_H fail.
     */
    static const int ErrorCode_10 = 10;

    /**
     * The definition of error-code:
     * 240Va_Fault_J fail.
     */
    static const int ErrorCode_11 = 11;

    /**
     * The definition of error-code:
     * 240Va_Fault_K fail.
     */
    static const int ErrorCode_12 = 12;

    /**
     * The definition of error-code:
     * 240Va_Fault_L fail.
     */
    static const int ErrorCode_13 = 13;

    /**
     * The definition of error-code:
     * P5V_PGOOD fail.
     */
    static const int ErrorCode_14 = 14;

    /**
     * The definition of error-code:
     * P3V3_PGOOD fail.
     */
    static const int ErrorCode_15 = 15;

    /**
     * The definition of error-code:
     * P1V8_PGOOD fail.
     */
    static const int ErrorCode_16 = 16;

    /**
     * The definition of error-code:
     * P1V1_PGOOD fail.
     */
    static const int ErrorCode_17 = 17;

    /**
     * The definition of error-code:
     * P0V9_PGOOD fail.
     */
    static const int ErrorCode_18 = 18;

    /**
     * The definition of error-code:
     * P2V5A_PGOOD fail.
     */
    static const int ErrorCode_19 = 19;

    /**
     * The definition of error-code:
     * P2V5B_PGOOD fail.
     */
    static const int ErrorCode_20 = 20;

    /**
     * The definition of error-code:
     * Vdn0_PGOOD fail.
     */
    static const int ErrorCode_21 = 21;

    /**
     * The definition of error-code:
     * Vdn1_PGOOD fail.
     */
    static const int ErrorCode_22 = 22;

    /**
     * The definition of error-code:
     * P1V5_PGOOD fail.
     */
    static const int ErrorCode_23 = 23;

    /**
     * The definition of error-code:
     * Vio0_PGOOD fail.
     */
    static const int ErrorCode_24 = 24;

    /**
     * The definition of error-code:
     * Vio1_PGOOD fail.
     */
    static const int ErrorCode_25 = 25;

    /**
     * The definition of error-code:
     * Vdd0_PGOOD fail.
     */
    static const int ErrorCode_26 = 26;

    /**
     * The definition of error-code:
     * Vcs0_PGOOD fail.
     */
    static const int ErrorCode_27 = 27;

    /**
     * The definition of error-code:
     * Vdd1_PGOOD fail.
     */
    static const int ErrorCode_28 = 28;

    /**
     * The definition of error-code:
     * Vcs1_PGOOD fail.
     */
    static const int ErrorCode_29 = 29;

    /**
     * The definition of error-code:
     * Vddr0_PGOOD fail.
     */
    static const int ErrorCode_30 = 30;

    /**
     * The definition of error-code:
     * Vtt0_PGOOD fail.
     */
    static const int ErrorCode_31 = 31;

    /**
     * The definition of error-code:
     * Vddr1_PGOOD fail.
     */
    static const int ErrorCode_32 = 32;

    /**
     * The definition of error-code:
     * Vtt1_PGOOD fail.
     */
    static const int ErrorCode_33 = 33;

    /**
     * The definition of error-code:
     * GPU0_PGOOD fail.
     */
    static const int ErrorCode_34 = 34;

    /**
     * The definition of error-code:
     * GPU1_PGOOD fail.
     */
    static const int ErrorCode_35 = 35;

    /**
     * The definition of error-code:
     * PSU0PSU1_PGOOD fail.
     */
    static const int ErrorCode_36 = 170;
};

} // namespace power
} // namespace witherspoon
