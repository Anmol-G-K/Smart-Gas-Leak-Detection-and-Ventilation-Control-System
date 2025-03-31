#include <xc.h>
#include <stdint.h>
#include "C:\Users\GNKRISHNAN\Documents\CODING\23EEE214_MCA\HEADERS\lcd.h"
#include "C:\Users\GNKRISHNAN\Documents\CODING\23EEE214_MCA\HEADERS\adc.h"

// Definitions
#define RL_VALUE 10
#define _XTAL_FREQ 20000000

// Fixed point scaling (Q8.8 format)
#define FP_SHIFT 8
#define FP_SCALE (1 << FP_SHIFT)
#define TO_FP(x) ((int16_t)((x) * FP_SCALE))
#define FROM_FP(x) ((x) >> FP_SHIFT)
#define PWM_FREQUENCY 1000  // 1kHz
#define PWM_PERIOD (_XTAL_FREQ/(PWM_FREQUENCY*4)) // For 1kHz @ 20MHz

// Add these gas concentration thresholds
#define GAS_THRESHOLD_1 100   // PPM
#define GAS_THRESHOLD_2 200
#define GAS_THRESHOLD_3 300
#define GAS_THRESHOLD_4 400
#define GAS_THRESHOLD_5 500

#define AVERAGE_SAMPLES 10  // Number of samples for averaging
#define AVERAGE_DELAY 100   // Delay between samples in ms

#define BUZZER PORTBbits.RB0
#define BUZZER_TRIS TRISBbits.TRISB0

// System and initialization functions
void system_init(void);
void PWM_Initialize(void);

// Gas sensor reading and calculation functions
uint16_t read_mq(void);
uint16_t calculate_resistance(uint16_t adc_value);
uint16_t get_gas_concentration(uint16_t rs_value);

// PWM function prototypes
void PWM_Initialize(void);
void PWM_SetDutyCycle(uint8_t duty);
void update_pwm_from_gas(uint16_t ppm);

// Utility functions
void int_to_str(uint16_t value, char* buffer);



// Lookup table for gas concentration (stored in program memory)
const int16_t concentration_table[] = {
    TO_FP(0.1), TO_FP(100),   // RS/R0 ratio, PPM pairs
    TO_FP(0.2), TO_FP(500),
    TO_FP(0.3), TO_FP(1000),
    TO_FP(0.4), TO_FP(2000),
    TO_FP(0.5), TO_FP(3000)
};
#define TABLE_SIZE (sizeof(concentration_table) / sizeof(concentration_table[0]) / 2)

// Function declarations
void system_init(void);
uint16_t read_mq(void);
void int_to_str(uint16_t value, char* buffer);

void int_to_str(uint16_t value, char* buffer) {
    uint8_t i = 0;
    char temp[6];

    do {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while(value && i < 5);

    while(i > 0) {
        *buffer++ = temp[--i];
    }
    *buffer = '\0';
}

uint16_t calculate_resistance(uint16_t adc_value) {
    uint32_t temp;
    if(adc_value < 1) adc_value = 1;
    temp = (uint32_t)RL_VALUE * 1023;
    temp = temp / adc_value;

    return TO_FP(temp) - TO_FP(RL_VALUE);

}


uint16_t get_gas_concentration(uint16_t rs_value) {
    uint8_t i;
    uint16_t ppm;
    
    // Simple lookup and linear interpolation
    for(i = 0; i < TABLE_SIZE - 1; i++) {
        if(rs_value >= concentration_table[i*2]) {
            uint16_t x0 = concentration_table[i*2];
            uint16_t x1 = concentration_table[(i+1)*2];
            uint16_t y0 = concentration_table[i*2+1];
            uint16_t y1 = concentration_table[(i+1)*2+1];
            
            // Linear interpolation
            ppm = y0 + ((uint32_t)(rs_value - x0) * (y1 - y0)) / (x1 - x0);
            return FROM_FP(ppm);
        }
    }
    
    return 5000; // Maximum value
}

void main(void) {
    char buff[6];
    uint16_t rs_value, ppm;
    uint32_t ppm_sum;
    uint16_t ppm_avg;
    uint8_t i;
    
    system_init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("Starting...");
    __delay_ms(1000);
    
    while(1) {
        // Take multiple samples and average
        ppm_sum = 0;
        
        for(i = 0; i < AVERAGE_SAMPLES; i++) {
            rs_value = read_mq();
            ppm = get_gas_concentration(rs_value);
            ppm_sum += ppm;
            
            // Display current reading
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Print_String("Sampling:");
            int_to_str(i+1, buff);
            Lcd_Print_String(buff);
            Lcd_Set_Cursor(2,1);
            int_to_str(ppm, buff);
            Lcd_Print_String(buff);
            Lcd_Print_String(" PPM");
            
            __delay_ms(AVERAGE_DELAY);
        }
        
        // Calculate average
        ppm_avg = (uint16_t)(ppm_sum / AVERAGE_SAMPLES);
        
        // Update PWM based on average
        update_pwm_from_gas(ppm_avg);
        
        // Display final average
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("Gas Concen.: ");
        int_to_str(ppm_avg, buff);
        Lcd_Print_String(buff);
        Lcd_Print_String(" PPM");
        
        // Display PWM duty cycle
        Lcd_Set_Cursor(2,1);
        if (ppm_avg < GAS_THRESHOLD_1) {
            Lcd_Print_String("PWM:0% ");
        } else if (ppm_avg < GAS_THRESHOLD_2) {
            Lcd_Print_String("PWM:25%");
        } else if (ppm_avg < GAS_THRESHOLD_3) {
            Lcd_Print_String("PWM:50%");
        } else if (ppm_avg < GAS_THRESHOLD_4) {
            Lcd_Print_String("PWM:75%");
        } else if (ppm_avg < GAS_THRESHOLD_5) {
            Lcd_Print_String("PWM:80%");
        } else {
            Lcd_Print_String("PWM:100% ALERT!");  // Added alert indication
        }
        
        __delay_ms(2000);  // Display average for 2 seconds
    }
}

uint16_t read_mq(void) {
    uint8_t i;
    uint32_t sum = 0;
    
    // Take average of readings
    for(i = 0; i < 4; i++) {
        sum += calculate_resistance(ADC_Read(0));
        __delay_ms(20);
    }
    
    return (uint16_t)(sum >> 2); // Divide by 4
}

void system_init(void) {
    TRISD = 0x00;     // LCD pins as output
    BUZZER_TRIS = 0;  // Set buzzer pin as output
    BUZZER = 0;       // Initialize buzzer OFF
    Lcd_Start();
    ADC_Initialize();
    PWM_Initialize();
}

void PWM_Initialize(void) {
    TRISC = 0x00;        // Set PORTC as output for PWM
    T2CON = 0x01;        // Timer2 ON, prescaler 1:4
    PR2 = 0xF9;          // Set PWM period for 1kHz
    CCP1CON = 0x0F;      // PWM mode, all bits enabled
    CCPR1L = 0x00;       // Start with 0% duty cycle
    CCP1X = 0;
    CCP1Y = 0;
    TMR2ON = 1;          // Turn on Timer2
}

void PWM_SetDutyCycle(uint8_t duty) {
    uint16_t duty_value;
    
    if(duty > 100) duty = 100;
    
    // Calculate PWM values for the given duty cycle
    duty_value = ((uint32_t)PR2 * duty) / 100;
    
    CCPR1L = (uint8_t)(duty_value);
    CCP1X = (duty_value & 0x02) >> 1;
    CCP1Y = duty_value & 0x01;
}

void update_pwm_from_gas(uint16_t ppm) {
    if (ppm < GAS_THRESHOLD_1) {
        PWM_SetDutyCycle(0);      // 0% duty cycle
        BUZZER = 0;               // Buzzer OFF
    }
    else if (ppm < GAS_THRESHOLD_2) {
        PWM_SetDutyCycle(25);     // 25% duty cycle
        BUZZER = 0;               // Buzzer OFF
    }
    else if (ppm < GAS_THRESHOLD_3) {
        PWM_SetDutyCycle(50);     // 50% duty cycle
        BUZZER = 0;               // Buzzer OFF
    }
    else if (ppm < GAS_THRESHOLD_4) {
        PWM_SetDutyCycle(75);     // 75% duty cycle
        BUZZER = 0;               // Buzzer OFF
    }
    else if (ppm < GAS_THRESHOLD_5) {
        PWM_SetDutyCycle(80);     // 80% duty cycle
        BUZZER = 0;               // Buzzer OFF
    }
    else {
        PWM_SetDutyCycle(100);    // 100% duty cycle
        BUZZER = 1;               // Buzzer ON - High gas concentration alert
        __delay_ms(200);
        BUZZER = 0;               // Buzzer OFF - High gas concentration alert
        __delay_ms(400);
    }
}
    