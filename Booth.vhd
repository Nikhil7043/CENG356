library ieee;  -- Include standard IEEE library for logic
use ieee.std_logic_1164.all;  -- Provides std_logic and std_logic_vector types

-- Entity: Booth Multiplier
-- Implements multiplication of two N-bit signed numbers.

entity Booth is
  generic (NofB : integer := 6);  -- Number of bits in multiplier/multiplicand
  port (
    MULD    : in  std_logic_vector((NofB-1) downto 0);  -- Multiplicand input
    MULR    : in  std_logic_vector((NofB-1) downto 0);  -- Multiplier input
    RST     : in  std_logic;                             -- Active-low system reset
    CLK     : in  std_logic;                             -- Main system clock
    ClkB    : in  std_logic;                             -- Clock for shift registers (can be inverted)
    MULT    : in  std_logic;                             -- Start multiplication signal
    RSLT    : out std_logic_vector((2*NofB-1) downto 0) -- 2*N-bit multiplication result
  );
end Booth;

-- Architecture: RTL
-- Declare components and interconnections 

architecture RTL of Booth is

  -- ALU for add/subtract
  -- Sub = '0' → perform A + B; Sub = '1' → perform A - B.
  -- Used to add/subtract multiplicand based on Booth code (Q0 Q-1).

  component AddSub
    generic (n : integer);
    port (
      A    : in  std_logic_vector((n-1) downto 0); -- Operand A (accumulator)
      B    : in  std_logic_vector((n-1) downto 0); -- Operand B (multiplicand)
      Sub  : in  std_logic;                         -- 0=add, 1=subtract
      COUT : out std_logic;                         -- Carry output
      SUM  : out std_logic_vector((n-1) downto 0)  -- Result
    );
  end component;

  -- Shift register component
  -- Operates on positive edge of clock, loads DATA when LOAD=1, shifts right otherwise.
  -- MSB is filled with serial input SIN.

  component SHIFT
    generic (n : integer);
    port (
      CLK    : in  std_logic;                        -- Clock input
      ENABLE : in  std_logic;                        -- Enable shifting/loading
      LOAD   : in  std_logic;                        -- Parallel load signal
      SIN    : in  std_logic;                        -- Serial input to MSB
      DATA   : in  std_logic_vector((n-1) downto 0);-- Parallel load input
      SHFT_O : out std_logic_vector((n-1) downto 0) -- Shift register output
    );
  end component;

  -- Counter for Booth iterations
  -- 3-bit counter, counts number of add/shift operations, completes when CNT_O="110".

  component CNT
    generic (n : integer);
    port (
      CLK   : in  std_logic;
      RST   : in  std_logic;                           -- Active-low reset input
      En    : in  std_logic;                           -- Enable counting
      CNT_O : out std_logic_vector((n-1) downto 0)     -- Counter output
    );
  end component;

  -- State Machine (SM)
  -- Indicates whether multiplier is in Reset, Load, or Shift state.
  -- Controls the flow of multiplication.

  component SM
    port (
      CLK      : in std_logic;
      RSTB     : in std_logic;  -- Active-high reset
      MULT     : in std_logic;  -- Start multiplication
      DONE     : in std_logic;  -- Multiplication complete
      RST_ST   : out std_logic; -- Reset state output
      LOAD_ST  : out std_logic; -- Load state output
      SHIFT_ST : out std_logic  -- Shift state output
    );
  end component;

  -- Internal signals

  signal RstB, D0, ShiftEn, LdSum, Load, Q0, Done, SinL, RstSt, LdSt, ShtSt, Sub : std_logic; -- Control signals
  signal Cf : std_logic;  -- ALU carry
  signal Cnt_O : std_logic_vector(2 downto 0);       -- 3-bit counter
  signal BoothCode : std_logic_vector(1 downto 0);   -- [Q0, Q-1] for Booth logic
  signal A, M, Sum : std_logic_vector((NofB-1) downto 0); -- Accumulator, multiplicand, ALU result
  signal Shft_O : std_logic_vector((2*NofB-1) downto 0);  -- Combined shift registers

begin

  -- Convert external active-low reset to internal active-high
  RstB <= not RST;  -- Active-high reset for internal use

  -- Multiplicand register (M)
  -- Load multiplicand into internal register during Load state.

  Multiplicand : process(ClkB)
  begin
    if rising_edge(ClkB) then                  -- Trigger on rising edge of ClkB
      if LdSt = '1' then                       -- Load state active
        M <= MULD;                             -- Capture multiplicand
      end if;
    end if;
  end process;

  -- Booth Q-1 logic
  -- D0 = 0 in Load state; else previous LSB from shifter

  D0 <= '0' when LdSt = '1' else Shft_O(0);  -- Q-1 initialization
  ShiftEn <= LdSt or ShtSt;                   -- Enable shift registers in Load or Shift states

  -- Update previous multiplier bit (Q-1)
  process(ClkB)
  begin
    if rising_edge(ClkB) then
      if ShiftEn = '1' then
        Q0 <= D0;                              -- Store D0 into Q0
      end if;
    end if;
  end process;

  -- Booth code and ALU control

  BoothCode <= Shft_O(0) & Q0;               -- Form Booth code: [LSB, Q-1]
  LdSum <= BoothCode(1) xor BoothCode(0);    -- Determine if ALU operation needed
  Load  <= LdSt or LdSum;                    -- Load accumulator either in Load or after ALU

  -- Accumulator update (A)
  -- Arithmetic right shift after ALU or reset to 0 in Load state

  A <= (others => '0') when LdSt = '1'       -- Reset accumulator
       else Sum(NofB-1) & Sum((NofB-1) downto 1);  -- Arithmetic right shift

  -- Serial input for lower shift register

  SinL <= Sum(0) when LdSum = '1'            -- If ALU updated, use LSB of Sum
          else Shft_O(NofB);                 -- Else use current shift register bit

  -- Upper shift register (Accumulator)

  ShifterH : SHIFT
    generic map(n => NofB)                   -- Width of shift register
    port map(
      CLK    => ClkB,                        -- Clock input
      ENABLE => ShiftEn,                     -- Enable shift/load
      LOAD   => Load,                        -- Load accumulator when needed
      SIN    => Shft_O(2*NofB-1),           -- MSB input for arithmetic shift
      DATA   => A,                           -- Data to load into accumulator
      SHFT_O => Shft_O(2*NofB-1 downto NofB) -- Output to upper part of combined shifter
    );

  -- Lower shift register (Multiplier)

  ShifterL : SHIFT
    generic map(n => NofB)                   -- Width of shift register
    port map(
      CLK    => ClkB,                        -- Clock input
      ENABLE => ShiftEn,                     -- Enable shift/load
      LOAD   => LdSt,                        -- Load multiplier in Load state
      SIN    => SinL,                        -- Serial input from accumulator
      DATA   => MULR,                         -- Multiplier data
      SHFT_O => Shft_O(NofB-1 downto 0)      -- Output to lower part of combined shifter
    );

  -- Output assignment
  RSLT <= Shft_O;                             -- Final multiplication result

  -- ALU control
  Sub <= BoothCode(1);                         -- Determines if ALU adds or subtracts

  -- ALU instance
  AdderSubtractor : AddSub
    generic map(n => NofB)                     -- ALU width
    port map(
      A    => Shft_O(2*NofB-1 downto NofB),   -- Accumulator input
      B    => M,                               -- Multiplicand input
      Sub  => Sub,                             -- Operation select
      COUT => Cf,                              -- Carry output
      SUM  => Sum                              -- Result of ALU operation
    );

  -- Counter for Booth iterations
  Counter : CNT
    generic map(n => 3)                        -- 3-bit counter
    port map(
      CLK   => ClkB,                           -- Clock input
      RST   => LdSt,                           -- Reset counter during Load state
      EN    => ShtSt,                           -- Enable counting during Shift state
      CNT_O => Cnt_O                            -- Counter output
    );

  Done <= '1' when Cnt_O = "110" else '0';     -- Set Done after 6 iterations

  -- State Machine instance
  StMachine : SM
    port map(
      CLK      => CLK,                          -- Main clock
      RSTB     => RstB,                         -- Active-high internal reset
      MULT     => MULT,                          -- Start multiplication
      DONE     => Done,                          -- Multiplication complete
      RST_ST   => RstSt,                         -- Reset state output
      LOAD_ST  => LdSt,                          -- Load state output
      SHIFT_ST => ShtSt                          -- Shift state output
    );


end RTL;