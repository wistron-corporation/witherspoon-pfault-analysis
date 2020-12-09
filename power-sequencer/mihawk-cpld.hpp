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
     * Confirm the Mowgli's HDD status(fault or rebuild).
     *
     * @param[in] statusReg - I2C's statusReg, slaveAddr
     * offset.
     */
    int checkFault(int statusReg);

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
    bool errorcodeMask;

    /**
     * The flag of Mowgli's HDD-fault status
     */
    bool faultcodeMask = false;

    /**
     * The flag of Mowgli's HDD-rebuild status
     */
    bool rebuildcodeMask = false;

    enum class ErrorCode : int
    {
        /**
         * All of powerOnErrorcode are the definition of error-code
         * which are read on CPLD-error-code-register.
         */
        /**
         * The definition of error-code:
         * Read CPLD-error-code-register fail.
         */
        _0 = 0,

        /**
         * The definition of error-code:
         * PSU1_PGOOD fail.
         */
        _1 = 1,

        /**
         * The definition of error-code:
         * PSU0_PGOOD fail.
         */
        _2 = 2,

        /**
         * The definition of error-code:
         * 240Va_Fault_A fail.
         */
        _3 = 3,

        /**
         * The definition of error-code:
         * 240Va_Fault_B fail.
         */
        _4 = 4,

        /**
         * The definition of error-code:
         * 240Va_Fault_C fail.
         */
        _5 = 5,

        /**
         * The definition of error-code:
         * 240Va_Fault_D fail.
         */
        _6 = 6,

        /**
         * The definition of error-code:
         * 240Va_Fault_E fail.
         */
        _7 = 7,

        /**
         * The definition of error-code:
         * 240Va_Fault_F fail.
         */
        _8 = 8,

        /**
         * The definition of error-code:
         * 240Va_Fault_G fail.
         */
        _9 = 9,

        /**
         * The definition of error-code:
         * 240Va_Fault_H fail.
         */
        _10 = 10,

        /**
         * The definition of error-code:
         * 240Va_Fault_J fail.
         */
        _11 = 11,

        /**
         * The definition of error-code:
         * 240Va_Fault_K fail.
         */
        _12 = 12,

        /**
         * The definition of error-code:
         * 240Va_Fault_L fail.
         */
        _13 = 13,

        /**
         * The definition of error-code:
         * P5V_PGOOD fail.
         */
        _14 = 14,

        /**
         * The definition of error-code:
         * P3V3_PGOOD fail.
         */
        _15 = 15,

        /**
         * The definition of error-code:
         * P1V8_PGOOD fail.
         */
        _16 = 16,

        /**
         * The definition of error-code:
         * P1V1_PGOOD fail.
         */
        _17 = 17,

        /**
         * The definition of error-code:
         * P0V9_PGOOD fail.
         */
        _18 = 18,

        /**
         * The definition of error-code:
         * P2V5A_PGOOD fail.
         */
        _19 = 19,

        /**
         * The definition of error-code:
         * P2V5B_PGOOD fail.
         */
        _20 = 20,

        /**
         * The definition of error-code:
         * Vdn0_PGOOD fail.
         */
        _21 = 21,

        /**
         * The definition of error-code:
         * Vdn1_PGOOD fail.
         */
        _22 = 22,

        /**
         * The definition of error-code:
         * P1V5_PGOOD fail.
         */
        _23 = 23,

        /**
         * The definition of error-code:
         * Vio0_PGOOD fail.
         */
        _24 = 24,

        /**
         * The definition of error-code:
         * Vio1_PGOOD fail.
         */
        _25 = 25,

        /**
         * The definition of error-code:
         * Vdd0_PGOOD fail.
         */
        _26 = 26,

        /**
         * The definition of error-code:
         * Vcs0_PGOOD fail.
         */
        _27 = 27,

        /**
         * The definition of error-code:
         * Vdd1_PGOOD fail.
         */
        _28 = 28,

        /**
         * The definition of error-code:
         * Vcs1_PGOOD fail.
         */
        _29 = 29,

        /**
         * The definition of error-code:
         * Vddr0_PGOOD fail.
         */
        _30 = 30,

        /**
         * The definition of error-code:
         * Vtt0_PGOOD fail.
         */
        _31 = 31,

        /**
         * The definition of error-code:
         * Vddr1_PGOOD fail.
         */
        _32 = 32,

        /**
         * The definition of error-code:
         * Vtt1_PGOOD fail.
         */
        _33 = 33,

        /**
         * The definition of error-code:
         * GPU0_PGOOD fail.
         */
        _34 = 34,

        /**
         * The definition of error-code:
         * GPU1_PGOOD fail.
         */
        _35 = 35,

        /**
         * The definition of error-code:
         * PSU0PSU1_PGOOD fail.
         */
        _36 = 170,

        /**
         * The definition of error-code:
         * P1V35_PGOOD fail.
         */
        _37 = 36,

        /**
         * The definition of error-code:
         * P1V2_PGOOD fail.
         */
        _38 = 37,

        /**
         * The definition of error-code:
         * P0V92_PGOOD fail.
         */
        _39 = 38

    };

    enum class HDDErrorCode : int
    {
        /**
         * The definition of CPLD-HDD-Fault-code:
         * HDD_0 fail.
         */
        _0 = 1,

        /**
         * The definition of CPLD-HDD-Fault-code:
         * HDD_1 fail.
         */
        _1 = 2
    };

    enum class HDDRebuildCode : int
    {
        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_0 or HDD_1 ends to rebuild.
         */
        _0 = 0,

        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_0 starts to rebuild.
         */
        _1 = 1,

        /**
         * The definition of CPLD-HDD-Rebuild-code:
         * HDD_1 starts to rebuild.
         */
        _2 = 2
    };
};

} // namespace power
} // namespace witherspoon
