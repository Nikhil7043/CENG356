library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ALU is
    generic (
        n : integer := 6
    );
    port (
        A, B : in  std_logic_vector(n-1 downto 0);
        S    : in  std_logic_vector(2 downto 0);
        RSLT : out std_logic_vector(n-1 downto 0);
        OVF, CF, NF, ZF : out std_logic
    );
end entity ALU;

architecture behavioral of ALU is

    component CLA
        generic (n : integer);
        port (
            A, B : in  std_logic_vector(n-1 downto 0);
            CIN  : in  std_logic;
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
    signal R_int : std_logic_vector(n-1 downto 0); -- INTERNAL RESULT

begin

    -- Logic operations
    R_And <= A and B;
    R_Or  <= A or B;
    R_Xor <= A xor B;
    NotA  <= not A;

    -- Shift operations (logical)
    R_Shr <= '0' & A(n-1 downto 1);
    R_Shl <= A(n-2 downto 0) & '0';

    -- Subtraction control
    Sub <= '1' when S = "001" else '0';
    B_l <= not B when Sub = '1' else B;

    -- Adder/Subtractor
    AddSub : CLA
        generic map (n => n)
        port map (
            A    => A,
            B    => B_l,
            CIN  => Sub,
            SUM  => Sum,
            COUT => C_l
        );

    -- Output MUX
    DataSelect : MUX
        generic map (n => n)
        port map (
            D0  => Sum,     -- 000 Add
            D1  => Sum,     -- 001 Sub
            D2  => R_And,   -- 010
            D3  => R_Or,    -- 011
            D4  => R_Xor,   -- 100
            D5  => NotA,    -- 101
            D6  => R_Shr,   -- 110
            D7  => R_Shl,   -- 111
            SEL => S,
            Y   => R_int
        );

    
    RSLT <= R_int;

    NF <= R_int(n-1);

    ZF <= '1' when unsigned(R_int) = 0 else '0';

    CF <= C_l when (S = "000" or S = "001") else '0';

    OVF <= (
            (A(n-1) and B_l(n-1) and not Sum(n-1)) or
            (not A(n-1) and not B_l(n-1) and Sum(n-1))
           ) when (S = "000" or S = "001")
           else '0';

end architecture behavioral;
