library verilog;
use verilog.vl_types.all;
entity SquareWave is
    generic(
        CRAZY_TIME      : integer := 45000;
        START_TIME      : integer := 30000
    );
    port(
        iClk            : in     vl_logic;
        isquareWave     : in     vl_logic;
        iRst_n          : in     vl_logic;
        oState_n        : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of CRAZY_TIME : constant is 1;
    attribute mti_svvh_generic_type of START_TIME : constant is 1;
end SquareWave;
