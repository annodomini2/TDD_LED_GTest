#include <gtest/gtest.h>
#include "LedDriver.h"
#include "stdint.h"
#include "RuntimeErrorStub.h"
#include "stdio.h"
#include "string.h"

/***********************************************************************
 * LED Test
 * 
 * Requirements:
 * 1. All LEDs are off after the driver is initialised
 * 2. A Single LED can be turned on
 * 3. A Single LED can be turned off
 * 4. Multiple LEDs can be turned on
 * 5. Multiple LEDs can be turned off
 * 6. Turn on all LEDs
 * 7. Ensure LED memory is not readable
 * 8. Turn off all LEDs
 * 9. Query LED state on
 * 10. Check boundary values
 * 11. Check out-of-bounds values - LED On
 * 12. Check out-of-bounds values - LED Off
 * 13. Check out-of-bounds - Runtime error
 * 14. Read out-of-bounds - LEDs are always off
 * 15. Query LED state off
 * 16. Read Off out-of-bounds - LEDs are always off
 * 17. Invert LED Output.
 * 18. Null Initialise protection
 * 
************************************************************************/

static uint16_t VirtualLEDs;

TEST( LedDriver_Initialisation, Normal ) 
{
    VirtualLEDs = 0xFFFF;

    LedDriver_Init(&VirtualLEDs, false, false);

    ASSERT_EQ( VirtualLEDs, 0x0000 );
}

TEST( LedDriver_Initialisation, Inverted_Output )
{
    VirtualLEDs = 0x0000;

    LedDriver_Init(&VirtualLEDs, true, false);

    ASSERT_EQ( VirtualLEDs, 0xFFFF );
}

TEST( LedDriver_Initialisation, Inverted_Input )
{
    VirtualLEDs = 0xFFFF;

    LedDriver_Init(&VirtualLEDs, false, true);

    ASSERT_EQ( VirtualLEDs, 0x0000 );
}

TEST( LedDriver_Initialisation, Inverted_Input_and_Output )
{
    VirtualLEDs = 0x0000;

    LedDriver_Init(&VirtualLEDs, true, true);

    ASSERT_EQ( VirtualLEDs, 0xFFFF );
}

TEST( LedDriver_Initialisation, Address_Null )
{
    ASSERT_EQ( LedDriver_Init(NULL, false, false), -1 );
}

class LedDriver_Operation_Normal : public ::testing::Test 
{
    protected:
        virtual void SetUp() 
        {
            VirtualLEDs = 0xFFFF;
            LedDriver_Init(&VirtualLEDs, false, false);
        }

        virtual void TearDown()
        {
            // tear down code
        }
};

//TEST_F(LedDriver_Operation_Normal, "2. A Single Led Can Be Turned On") 
TEST_F(LedDriver_Operation_Normal, 2ASingleLedCanBeTurnedOn) 
{
    LedDriver_TurnOn(1);

    ASSERT_EQ( VirtualLEDs, 1 );
}

//TEST_F(LedDriver_Operation_Normal,  "3. A Single LED can be turned off" )
TEST_F(LedDriver_Operation_Normal, 3ASingleLedCanBeTurnedOff) 
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0 );
}

//TEST_F(LedDriver_Operation_Normal, "4. Multiple LEDs can be turned on" ) 
TEST_F(LedDriver_Operation_Normal, 4MultipleLEDsCanBeTurnedOn ) 
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0x180 );
}

//TEST_F(LedDriver_Operation_Normal, "5. Multiple LEDs can be turned off" )
TEST_F(LedDriver_Operation_Normal, 5MultipleLEDsCanBeTurnedOff ) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(8);

    ASSERT_TRUE( VirtualLEDs == 0xFF7F );
}

//TEST_F(LedDriver_Operation_Normal, "6. Turn on all LEDs" ) 
TEST_F(LedDriver_Operation_Normal, 6TurnOnAllLEDs) 
{
    LedDriver_TurnOnAll();

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_Normal, "7. Ensure LED memory is not readable" ) 
TEST_F(LedDriver_Operation_Normal, 7EnsureLedMemoryIsNotReadable ) 
{
    VirtualLEDs = 0xFFFF;
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0x0080 );
}

//TEST_F(LedDriver_Operation_Normal, "8. Turn off all LEDs" ) 
TEST_F(LedDriver_Operation_Normal, 8TurnOffAllLEDs ) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOffAll();

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_Normal, "9. Query LED state" ) 
TEST_F(LedDriver_Operation_Normal, 9QueryLEDState ) 
{
    ASSERT_TRUE(false == LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    ASSERT_TRUE(true == LedDriver_IsOn(11));
}

//TEST_F(LedDriver_Operation_Normal, "10. Check boundary values" ) 
TEST_F(LedDriver_Operation_Normal, 10CheckBoundaryValues ) 
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);

    ASSERT_TRUE( VirtualLEDs == 0x8001 );
}

//TEST_F(LedDriver_Operation_Normal, "11. Check out-of-bounds values - LED On" ) 
TEST_F(LedDriver_Operation_Normal, 11CheckOutOfBoundsValuesLEDOn) 
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_Normal, "12. Check out-of-bounds values - LED Off" ) 
TEST_F(LedDriver_Operation_Normal, 12CheckOutOfBoundsValuesLEDOff) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_Normal, "14. Read On out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_Normal, 14ReadOnOutOfBoundsLEDsAreAlwaysOff) 
{
    ASSERT_TRUE( false == LedDriver_IsOn(0) );
    ASSERT_TRUE( false == LedDriver_IsOn(17) );
}

//TEST_F(LedDriver_Operation_Normal, "15. Query LED state off" ) 
TEST_F(LedDriver_Operation_Normal, 15QueryLEDStateOff) 
{
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
    LedDriver_TurnOn(11);
    ASSERT_TRUE( false == LedDriver_IsOff(11) );
}

//TEST_F(LedDriver_Operation_Normal, "16. Read Off out-of-bounds - LEDs are always off" )
TEST_F(LedDriver_Operation_Normal, 16ReadOffOutOfBoundsLEDsAreAlwaysOff) 
{
    ASSERT_TRUE( true == LedDriver_IsOff(0) );
    ASSERT_TRUE( true == LedDriver_IsOff(17) );
}

TEST( LedDriver_Runtime_Error, OutOfBounds ) 
{
    LedDriver_Init(&VirtualLEDs, false, false);

    LedDriver_TurnOff(-1);

    ASSERT_EQ( 0, strcmp("LED Driver: out-of-bounds LED", RuntimeErrorStub_GetLastError()) );
    ASSERT_EQ( -1, RuntimeErrorStub_GetLastParameter() );
}

class LedDriver_Operation_InvertedOutput : public ::testing::Test 
{
    protected:
        virtual void SetUp() 
        {
            VirtualLEDs = 0x0000;
            LedDriver_Init(&VirtualLEDs, true, false);
            ASSERT_EQ( VirtualLEDs, 0xFFFF );
        }

        virtual void TearDown()
        {
            // tear down code
        }
};

//TEST_F(LedDriver_Operation_InvertedOutput, "2. Single LED On")
TEST_F(LedDriver_Operation_InvertedOutput, 2SingleLEDOn)
{
    LedDriver_TurnOn(8);

    ASSERT_EQ( VirtualLEDs, 0xFF7F );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "3. Single LED Off")
TEST_F(LedDriver_Operation_InvertedOutput, 3SingleLEDOff)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}
    
//TEST_F(LedDriver_Operation_InvertedOutput, "4. Multiple LEDs can be turned on") 
TEST_F(LedDriver_Operation_InvertedOutput, 4MultipleLEDsCanBeTurnedOn) 
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0xFE7F );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "5. Multiple LEDs can be turned off" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 5MultipleLEDsCanBeTurnedOff) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(8);

    ASSERT_TRUE( VirtualLEDs == 0x80 );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "6. Turn on all LEDs" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 6TurnOnAllLEDs) 
{
    LedDriver_TurnOnAll();

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "7. Ensure LED memory is not readable" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 7EnsureLEDMemoryIsNotReadable) 
{
    VirtualLEDs = 0x0000;
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0xFF7F );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "8. Turn off all LEDs" )
TEST_F(LedDriver_Operation_InvertedOutput, 8TurnOffAllLEDs)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOffAll();

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedOutput, "9. Query LED state" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 9QueryLEDState) 
{
    ASSERT_TRUE(false == LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    ASSERT_TRUE(true == LedDriver_IsOn(11));
}

//TEST_F(LedDriver_Operation_InvertedOutput, "10. Check boundary values" )
TEST_F(LedDriver_Operation_InvertedOutput, 10CheckBoundaryValues)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);

    ASSERT_TRUE( VirtualLEDs == 0x7FFE );
}

//TEST_F(LedDriver_Operation_InvertedOutput, 11. Check out-of-bounds values - LED On" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 11CheckOutOfBoundsValuesLEDOn)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedOutput, 12. Check out-of-bounds values - LED Off" )
TEST_F(LedDriver_Operation_InvertedOutput, 12CheckOutOfBoundsValuesLEDOff) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedOutput, 14. Read On out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_InvertedOutput, 14ReadOnOutOfBoundsLEDsAreAlwaysOff)
{
    ASSERT_TRUE( false == LedDriver_IsOn(0) );
    ASSERT_TRUE( false == LedDriver_IsOn(17) );
}

//TEST_F(LedDriver_Operation_InvertedOutput, 15. Query LED state off" )
TEST_F(LedDriver_Operation_InvertedOutput, 15QueryLEDStateOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
    LedDriver_TurnOn(11);
    ASSERT_TRUE( false == LedDriver_IsOff(11) );
}

//TEST_F(LedDriver_Operation_InvertedOutput, 16. Read Off out-of-bounds - LEDs are always off" )
TEST_F(LedDriver_Operation_InvertedOutput, 16ReadOffOutOfBoundsLEDsAreAlwaysOff) 
{
    ASSERT_TRUE( true == LedDriver_IsOff(0) );
    ASSERT_TRUE( true == LedDriver_IsOff(17) );
}

class LedDriver_Operation_InvertedInput : public ::testing::Test 
{
    protected:
        virtual void SetUp() 
        {
            LedDriver_Init(&VirtualLEDs, false, true);
        }

        virtual void TearDown()
        {
            // tear down code
        }
};

//TEST_F(LedDriver_Operation_InvertedInput, "2. Single LED On")
TEST_F(LedDriver_Operation_InvertedInput, 2SingleLEDOn)
{
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0x0100 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "3. Single LED Off")
TEST_F(LedDriver_Operation_InvertedInput, 3SingleLEDOff)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}
    
//TEST_F(LedDriver_Operation_InvertedInput, "4. Multiple LEDs can be turned on") 
TEST_F(LedDriver_Operation_InvertedInput, 4MultipleLEDsCanBeTurnedOn)
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0x0180 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "5. Multiple LEDs can be turned off" )
TEST_F(LedDriver_Operation_InvertedInput, 5MultipleLEDsCanBeTurnedOff)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(8);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0x7EFF);
}

//TEST_F(LedDriver_Operation_InvertedInput, "6. Turn on all LEDs" ) 
TEST_F(LedDriver_Operation_InvertedInput, 6TurnOnAllLEDs) 
{
    LedDriver_TurnOnAll();

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedInput, "7. Ensure LED memory is not readable" ) 
TEST_F(LedDriver_Operation_InvertedInput, 7EnsureLEDMemoryIsNotReadable) 
{
    VirtualLEDs = 0x0000;
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0x0100 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "8. Turn off all LEDs" ) 
TEST_F(LedDriver_Operation_InvertedInput, 8TurnOffAllLEDs)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOffAll();

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "9. Query LED state" ) 
TEST_F(LedDriver_Operation_InvertedInput, 9QueryLEDState)
{
    ASSERT_TRUE(false == LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    ASSERT_TRUE(true == LedDriver_IsOn(11));
}

//TEST_F(LedDriver_Operation_InvertedInput, "10. Check boundary values" )
TEST_F(LedDriver_Operation_InvertedInput, 10CheckBoundaryValues) 
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);

    ASSERT_TRUE( VirtualLEDs == 0x8001 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "11. Check out-of-bounds values - LED On" ) 
TEST_F(LedDriver_Operation_InvertedInput, 11CheckOutOfBoundsValuesLEDOn)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedInput, "12. Check out-of-bounds values - LED Off" ) 
TEST_F(LedDriver_Operation_InvertedInput, 12CheckOutOfBoundsValuesLEDOff) 
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedInput, "14. Read On out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_InvertedInput, 14ReadOnOutOfBoundsLEDsAreAlwaysOff)
{
    ASSERT_TRUE( false == LedDriver_IsOn(0) );
    ASSERT_TRUE( false == LedDriver_IsOn(17) );
}

//TEST_F(LedDriver_Operation_InvertedInput, "15. Query LED state off" ) 
TEST_F(LedDriver_Operation_InvertedInput, 15QueryLEDStateOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
    LedDriver_TurnOn(11);
    ASSERT_TRUE( false == LedDriver_IsOff(11) );
}

//TEST_F(LedDriver_Operation_InvertedInput, "16. Read Off out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_InvertedInput, 16ReadOffOutOfBoundsLEDsAreAlwaysOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(0) );
    ASSERT_TRUE( true == LedDriver_IsOff(17) );
}

class LedDriver_Operation_InvertedInputAndOutput : public ::testing::Test 
{
    protected:
        virtual void SetUp() 
        {
            LedDriver_Init(&VirtualLEDs, true, true);
        }

        virtual void TearDown()
        {
            // tear down code
        }
};

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "2. Single LED On")
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 2SingleLEDOn)
{
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0xFEFF );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "3. Single LED Off")
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 3SingleLEDOff)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}
    
//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "4. Multiple LEDs can be turned on") 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 4MultipleLEDsCanBeOurnedOn)
{
    LedDriver_TurnOn(9);
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0xFE7F );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "5. Multiple LEDs can be turned off" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 5MultipleLEDsCanBeTurnedOff)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(8);
    LedDriver_TurnOff(1);

    ASSERT_TRUE( VirtualLEDs == 0x8100);
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "6. Turn on all LEDs" )
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 6TurnOnAllLEDs)
{
    LedDriver_TurnOnAll();

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "7. Ensure LED memory is not readable" )
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 7EnsureLEDMemoryIsNotReadable)
{
    VirtualLEDs = 0x0000;
    LedDriver_TurnOn(8);

    ASSERT_TRUE( VirtualLEDs == 0xFEFF );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "8. Turn off all LEDs" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 8TurnOffAllLEDs)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOffAll();

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "9. Query LED state" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 9QueryLEDState)
{
    ASSERT_TRUE(false == LedDriver_IsOn(11));
    LedDriver_TurnOn(11);
    ASSERT_TRUE(true == LedDriver_IsOn(11));
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "10. Check boundary values" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 10CheckBoundaryValues)
{
    LedDriver_TurnOn(1);
    LedDriver_TurnOn(16);

    ASSERT_TRUE( VirtualLEDs == 0x7FFE );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "11. Check out-of-bounds values - LED On" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 11CheckOutOfBoundsValuesLEDOn)
{
    LedDriver_TurnOn(-1);
    LedDriver_TurnOn(0);
    LedDriver_TurnOn(17);
    LedDriver_TurnOn(3141);

    ASSERT_TRUE( VirtualLEDs == 0xFFFF );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "12. Check out-of-bounds values - LED Off" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 12CheckOutOfBoundsValuesLEDOff)
{
    LedDriver_TurnOnAll();

    LedDriver_TurnOff(-1);
    LedDriver_TurnOff(0);
    LedDriver_TurnOff(17);
    LedDriver_TurnOff(3141);

    ASSERT_TRUE( VirtualLEDs == 0x0000 );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "14. Read On out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 14ReadOnOutOfBoundsLEDsAreAlwaysOff)
{
    ASSERT_TRUE( false == LedDriver_IsOn(0) );
    ASSERT_TRUE( false == LedDriver_IsOn(17) );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "15. Query LED state off" )
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 15QueryLEDStateOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
    LedDriver_TurnOn(11);
    ASSERT_TRUE( false == LedDriver_IsOff(11) );
}

//TEST_F(LedDriver_Operation_InvertedInputAndOutput, "16. Read Off out-of-bounds - LEDs are always off" ) 
TEST_F(LedDriver_Operation_InvertedInputAndOutput, 16ReadOffOutOfBoundsLEDsAreAlwaysOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(0) );
    ASSERT_TRUE( true == LedDriver_IsOff(17) );
}

class LedDriver_Operation_NotInitialised : public ::testing::Test 
{
    protected:
        virtual void SetUp() 
        {
            LedDriver_Init(NULL, true, true);
        }

        virtual void TearDown()
        {
            // tear down code
        }
};

//TEST_F(LedDriver_Operation_NotInitialised, "Turn On")
TEST_F(LedDriver_Operation_NotInitialised, TurnOn)
{
    ASSERT_TRUE( LedDriver_TurnOn(8) == -1 );
}

//TEST_F(LedDriver_Operation_NotInitialised, "Turn Off")
TEST_F(LedDriver_Operation_NotInitialised, TurnOff)
{
    ASSERT_TRUE( LedDriver_TurnOff(1) == -1 );
}
    
//TEST_F(LedDriver_Operation_NotInitialised, "All On" )
TEST_F(LedDriver_Operation_NotInitialised, AllOn)
{
    ASSERT_TRUE( LedDriver_TurnOnAll() == -1 );
}

//TEST_F(LedDriver_Operation_NotInitialised, "All Off" )
TEST_F(LedDriver_Operation_NotInitialised, AllOff)
{
    ASSERT_TRUE( LedDriver_TurnOffAll() == -1 );
}

//TEST_F(LedDriver_Operation_NotInitialised, "Is On" ) 
TEST_F(LedDriver_Operation_NotInitialised, IsOn)
{
    ASSERT_TRUE( false == LedDriver_IsOn(11) );
    ASSERT_TRUE( -1 == LedDriver_TurnOn(11) );
    ASSERT_TRUE( false == LedDriver_IsOn(11) );
}

//TEST_F(LedDriver_Operation_NotInitialised, "Is Off" ) 
TEST_F(LedDriver_Operation_NotInitialised, IsOff)
{
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
    ASSERT_TRUE( -1 == LedDriver_TurnOn(11) );
    ASSERT_TRUE( true == LedDriver_IsOff(11) );
}