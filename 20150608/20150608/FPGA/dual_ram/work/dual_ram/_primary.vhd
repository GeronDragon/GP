library verilog;
use verilog.vl_types.all;
entity dual_ram is
    port(
        clk             : in     vl_logic;
        xzcs6_a         : in     vl_logic;
        xzcs6_b         : in     vl_logic;
        addr_a          : in     vl_logic_vector(9 downto 0);
        data_a          : inout  vl_logic_vector(15 downto 0);
        addr_b          : in     vl_logic_vector(9 downto 0);
        data_b          : inout  vl_logic_vector(15 downto 0);
        we_a            : in     vl_logic;
        we_b            : in     vl_logic;
        rd_a            : in     vl_logic;
        rd_b            : in     vl_logic;
        oled1           : out    vl_logic;
        oled2           : out    vl_logic;
        oled3           : out    vl_logic;
        squarewave_a    : in     vl_logic;
        state_a         : out    vl_logic;
        xint2_b         : out    vl_logic
    );
end dual_ram;
