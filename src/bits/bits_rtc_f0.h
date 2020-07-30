#pragma once

#include <cstdint>

namespace mcu::RTC_bits {
   enum Hour_format {_24 = 0b0, _12};
   enum Units {_0 = 0b0000, _1_unit, _2_unit, _3_unit, _4_unit, _5_unit, _6_unit, _7_unit, _8_unit, _9_unit};
   enum Tens_min  {_0_min = 0b000, _1_min, _2_min, _3_min, _4_min, _5_min};
   enum Tens_hour  {_0_hour = 0b00, _1_hour, _2_hour};
   struct TR {
      uint8_t     SU  :4; // Bits 3:0 SU[3:0]: Second units in BCD format
      uint8_t     ST  :3; // Bits 6:4 ST[2:0]: Second tens in BCD format
      uint32_t        :1; // Bit 7 Reserved, must be kept at reset value.
      uint8_t     MNU :4; // Bits 11:8 MNU[3:0]: Minute units in BCD format
      uint8_t     MNT :3; // Bits 14:12 MNT[2:0]: Minute tens in BCD format
      uint32_t        :1; // Bit 15 Reserved, must be kept at reset value.
      uint8_t     HU  :4; // Bits 19:16 HU[3:0]: Hour units in BCD format
      uint8_t     HT  :2; // Bits 21:20 HT[1:0]: Hour tens in BCD format
      Hour_format PM  :1; // Bit 22 PM: AM/PM notation
      uint32_t        :9; // Bits 31-23 Reserved, must be kept at reset value
   }__attribute__((packed));

   struct DR {
      enum Day { forbidden = 0b000, Monday = 0b001, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
      uint32_t DU  :4; // Bits 3:0 DU[3:0]: Date units in BCD format
      uint32_t DT  :2; // Bits 5:4 DT[1:0]: Date tens in BCD format
      uint32_t     :2; // Bits 7:6 Reserved, must be kept at reset value.
      uint32_t MU  :4; // Bits 11:8 MU: Month units in BCD format
      uint32_t MT  :1; // Bit 12 MT: Month tens in BCD format
      uint32_t WDU :3; // Bits 15:13 WDU[2:0]: Week day units
      uint32_t YU  :4; // Bits 19:16 YU[3:0]: Year units in BCD format
      uint32_t YT  :4; // Bits 23:20 YT[3:0]: Year tens in BCD format
      uint32_t     :8; // Bits 31:24 Reserved, must be kept at reset value
   }__attribute__((packed));

   struct CR {
      enum Wakeup_clock {div_16 = 0b000, div_8 = 0b001, div_4 = 0b010, div_2 = 0b11};
      enum Edge {rising = 0b0, falling};
      enum Calibration {_512_Hz = 0b0, _1_Hz};
      enum Polarity {high = 0b0, low};
      enum Output {disabled = 0b00, alarm = 0b01,reserved, wakeup};
      Wakeup_clock WUCKSEL :3; //Bits 2:0 WUCKSEL[2:0]: Wakeup clock selection
      Edge         TSEDGE  :1; // Bit 3 TSEDGE: Time-stamp event active edge
      bool         REFCKON :1; // RTC_REFIN reference clock detection enable (50 or 60 Hz)
      bool         BYPSHAD :1; // Bit 5 BYPSHAD: Bypass the shadow registers
      Hour_format  FMT     :1; // Bit 6 FMT: Hour format
      uint32_t             :1; // Bit 7 Reserved, must be kept at reset value.
      bool         ALRAE   :1; // Bit 8 ALRAE: Alarm A enable
      uint32_t             :1; // Bit 9 Reserved, must be kept at reset value
      bool         WUTE    :1; // Bit 10 WUTE: Wakeup timer enable
      bool         TSE     :1; // Bit 11 TSE: timestamp enable
      bool         ALRAIE  :1; // Bit 12 ALRAIE: Alarm A interrupt enable
      uint32_t             :1; // Bit 13 Reserved, must be kept at reset value
      bool         WUTIE   :1; // Bit 14 WUTIE: Wakeup timer interrupt enable
      bool         TSIE    :1; // Bit 15 TSIE: Time-stamp interrupt enable
      bool         ADD1H   :1; // Bit 16 ADD1H: Add 1 hour (summer time change)
      bool         SUB1H   :1; // Bit 17 SUB1H: Subtract 1 hour (winter time change)
      bool         BKP     :1; // Bit 18 BKP: Backup
      Calibration  COSEL   :1; // Bit 19 COSEL: Calibration output selection
      Polarity     POL     :1; // Bit 20 POL: Output polarity
      Output       OSEL    :2; // Bits 22:21 OSEL[1:0]: Output selection
      bool         COE     :1; // Bit 23 COE: Calibration output enable
      uint32_t             :8; // Bits 31:24 Reserved, must be kept at reset value.
   }__attribute__((packed));

   struct ISR {
      bool ALRAWF :1; // Bit 0 ALRAWF: Alarm A write flag
      uint32_t    :1; // Bit 1 Reserved, must be kept at reset value.
      bool WUTWF  :1; // Bit 2 WUTWF: Wakeup timer write flag
      bool SHPF   :1; // Bit 3 SHPF: Shift operation pending
      bool INITS  :1; // Bit 4 INITS: Initialization status flag
      bool RSF    :1; // Bit 5 RSF: Registers synchronization flag
      bool INITF  :1; // Bit 6 INITF: Initialization flag
      bool INIT   :1; // Bit 7 INIT: Initialization mode
      bool ALRAF  :1; // Bit 8 ALRAF: Alarm A flag
      uint32_t    :1; // Bit 9 Reserved, must be kept at reset value.
      bool WUTF   :1; // Bit 10 WUTF: Wakeup timer flag
      bool TSF    :1; // Bit 11 TSF: Time-stamp flag
      bool TSOVF  :1; // Bit 12 TSOVF: Time-stamp overflow flag
      bool TAMP1F :1; // Bit 13 TAMP1F: RTC_TAMP1 detection flag
      bool TAMP2F :1; // Bit 14 TAMP2F: RTC_TAMP2 detection flag
      uint32_t    :1; // Bit 15 Reserved, must be kept at reset value
      bool RECALPF:1; // Bit 16 RECALPF: Recalibration pending Flag
      uint32_t    :15; // Bits 31:17 Reserved, must be kept at reset value
   }__attribute__((packed));

   struct PRER {
      uint32_t PREDIV_S :15; // Bits 14:0 PREDIV_S[14:0]: Synchronous prescaler factor
      uint32_t          :1; // Bit 15 Reserved, must be kept at reset value.
      uint32_t PREDIV_A :7; // Bits 22:16 PREDIV_A[6:0]: Asynchronous prescaler factor
      uint32_t          :9; // Bits 31:23 Reserved, must be kept at reset value
   }__attribute__((packed));

   struct WPR {
      uint32_t KEY :8; // Bits 7:0 KEY: Write protection key
      uint32_t     :24; // Bits 31:8 Reserved, must be kept at reset value.
   }__attribute__((packed));

}