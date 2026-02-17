library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ALU is
    generic (
        n : integer := 6  -- 6-bit for lab
    );
    port (
        A, B : in std_logic_vector(n-1 downto 0);
        S    : in std_logic_vector(2 downto 0);  -- 3-bit select signal
        RSLT : out std_logic_vector(n-1 downto 0);
        OVF, CF, NF, ZF : out std_logic
    );
end entity ALU;

architecture behavioral of ALU is
    -- Component declarations (match provided CLA and Mux.vhd)
    component CLA
        generic (n : integer);
        port (
            A, B : in std_logic_vector(n-1 downto 0);
            CIN  : in std_logic;
            SUM  : out std_logic_vector(n-1 downto 0);
            COUT : out std_logic
        );
    end component;
    
    component MUX
        generic (n : integer);
        port (
            D0, D1, D2, D3, D4, D5, D6, D7 : in std_logic_vector(n-1 downto 0);
            SEL : in std_logic_vector(2 downto 0);
            Y   : out std_logic_vector(n-1 downto 0)
        );
    end component;
    
    -- Internal signals
    signal R_And, R_Or, R_Xor, NotA : std_logic_vector(n-1 downto 0);
    signal R_Shr, R_Shl : std_logic_vector(n-1 downto 0);
    signal Sub : std_logic;
    signal B_l, Sum : std_logic_vector(n-1 downto 0);
    signal C_l : std_logic;
    signal Zero : std_logic_vector(n-1 downto 0) := (others => '0');  -- For ZF
    
begin
    -- Concurrent logic operations
    R_And <= A and B;                        -- AND (S=010)
    R_Or  <= A or B;                         -- OR  (S=011)
    R_Xor <= A xor B;                        -- XOR (S=100)
    NotA  <= not A;                          -- NOT A (S=101, B ignored)

    -- Shift operations (logical, 1-bit)
    R_Shr <= '0' & A(n-1 downto 1);          -- Right shift (S=110)
    R_Shl <= A(n-2 downto 0) & '0';          -- Left shift  (S=111)

    -- Subtraction control (2's complement via CLA)
    Sub   <= '1' when S = "001" else '0';
    B_l   <= not B when Sub = '1' else B;    -- 1's complement for sub

    -- Adder/Subtractor instantiation
    AddSub : CLA
        generic map (n => n)
        port map (
            A    => A,
            B    => B_l,
            CIN  => Sub,  -- CIN=1 for sub (2's comp)
            COUT => C_l,
            SUM  => Sum
        );

    -- Flags (computed on Sum for add/sub consistency)
    OVF <= (A(n-1) and B_l(n-1) and not Sum(n-1)) or
       (not A(n-1) and not B_l(n-1) and Sum(n-1));
    CF   <= C_l;          -- Carry out
    NF   <= Sum(n-1);     -- Negative (MSB)
    ZF   <= '1' when Sum = Zero else '0';  -- Zero flag

    -- Output multiplexer (matches function table exactly)
    DataSelect : MUX
        generic map (n => n)
        port map (
            D0  => Sum,     -- 000: Add
            D1  => Sum,     -- 001: Sub
            D2  => R_And,   -- 010: AND
            D3  => R_Or,    -- 011: OR
            D4  => R_Xor,   -- 100: XOR
            D5  => NotA,    -- 101: NOT A
            D6  => R_Shr,   -- 110: SHR
            D7  => R_Shl,   -- 111: SHL
            SEL => S,
            Y   => RSLT
        );


end architecture behavioral;
