#pragma once
const int RULE_LEN = 100;
const int RULES_TOTAL = 7;

static const char Rules[RULES_TOTAL][RULE_LEN] = {
	"S->[A]",
	"A->[Double]",
	"Double->-[UnsignedDouble]|[UnsignedDouble]",
	"UnsignedDouble->[Int]|[Int].[UInt]",
	"Int->-[UInt]|[UInt]",
	"UInt->[Digit]|[Digit][UInt]",
	"Digit->0|1|2|3|4|5|6|7|8|9"	
};
