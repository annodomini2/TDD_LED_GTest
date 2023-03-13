#include "LedDriver.h"
#include "RuntimeError.h"

#define ALL_LEDS_ON 0xFFFF
#define ALL_LEDS_OFF 0x0000
#define MIN_LED 1
#define MAX_LED 16
#define TRUE 1
#define FALSE 0

static inline uint16_t convertLedNumberToBit(uint16_t ledNumber);
static inline void updateHardware(void);
static inline uint8_t validateRequestedLed(int16_t LedIndex);
static inline void setLedBit(uint16_t LedIndex);
static inline void clearLedBit(uint16_t LedIndex);
static inline void setBit(uint16_t LedIndex);
static inline void clearBit(uint16_t LedIndex);
static bool isInitialised(void);

static uint16_t* ledaddress;
static uint16_t ledstatus;
static bool inverted_output;
static bool inverted_input;

int LedDriver_Init(uint16_t* Address, bool InvertOutput, bool InvertInput)
{
    int result;

    result = -1;

    ledaddress = Address;

    if (TRUE == isInitialised())
    {    
        inverted_output = InvertOutput;
        inverted_input = InvertInput;

        if (TRUE == inverted_output)
        {
            ledstatus = ALL_LEDS_ON;
        }
        else
        {
            ledstatus = ALL_LEDS_OFF;
        }
        
        updateHardware();

        result = 0;
    }

    return result;
}

int LedDriver_TurnOn(int16_t LedIndex)
{
    int result; 
    
    result = -1;

    if (TRUE == isInitialised())
    {
        if (TRUE == validateRequestedLed(LedIndex))
        {
            setLedBit(LedIndex);
            updateHardware();
            result = 0;
        }
    }

    return result;
}

int LedDriver_TurnOff(int16_t LedIndex)
{
    int result; 
    
    result = -1;

    if (TRUE == isInitialised())
    {
        if (TRUE == validateRequestedLed(LedIndex))
        {
            clearLedBit(LedIndex);
            updateHardware();

            result = 0;
        }
    }

    return result;
}

int LedDriver_TurnOnAll(void)
{
    int result;

    result = -1;

    if (TRUE == isInitialised())
    {
        result = 0;

        if (TRUE == inverted_output)
        {
            ledstatus = ALL_LEDS_OFF;
        }
        else
        {
            ledstatus = ALL_LEDS_ON;
        }

        updateHardware();
    }

    return result;
}

int LedDriver_TurnOffAll(void)
{
    int result;

    result = -1;

    if (TRUE == isInitialised())
    {
        result = 0;
        
        if (TRUE == inverted_output)
        {
            ledstatus = ALL_LEDS_ON;
        }
        else
        {
            ledstatus = ALL_LEDS_OFF;
        }

        updateHardware();
    }

    return result;
}

bool LedDriver_IsOn(int16_t LedIndex)
{
    bool status;

    status = FALSE;

    if (TRUE == validateRequestedLed(LedIndex))
    {
        if (TRUE == inverted_output)
        {
            status = (FALSE == (ledstatus & convertLedNumberToBit(LedIndex)));
        }
        else
        {
            status = (ledstatus & convertLedNumberToBit(LedIndex));
        }
    }

    return status;
}

bool LedDriver_IsOff(int16_t LedIndex)
{
    return (FALSE == LedDriver_IsOn(LedIndex));
}

static inline uint16_t convertLedNumberToBit(uint16_t ledNumber)
{
    if (TRUE == inverted_input)
    {
        return (1 << (16 - ledNumber));
    }
    else
    {
        return (1 << (ledNumber - 1));
    }
}

static inline void updateHardware(void)
{
    *ledaddress = ledstatus;
}

static inline uint8_t validateRequestedLed(int16_t LedIndex)
{
    uint8_t result = FALSE;

    if ((MIN_LED <= LedIndex) && (MAX_LED >= LedIndex))
    {
        result = TRUE;
    }
    else
    {
        RUNTIME_ERROR("LED Driver: out-of-bounds LED", LedIndex);
    }

    return result;
}

static inline void setLedBit(uint16_t LedIndex)
{
    if (TRUE == inverted_output)
    {
        clearBit(LedIndex);
    }
    else
    {
        setBit(LedIndex);
    }
}

static inline void clearLedBit(uint16_t LedIndex)
{
    if (TRUE == inverted_output)
    {
        setBit(LedIndex);
    }
    else
    {
        clearBit(LedIndex);
    }
}

static inline void setBit(uint16_t LedIndex)
{
    ledstatus |= convertLedNumberToBit(LedIndex);
}

static inline void clearBit(uint16_t LedIndex)
{
    ledstatus &= ~(convertLedNumberToBit(LedIndex));
}

static bool isInitialised(void)
{
    return (NULL != ledaddress);
}